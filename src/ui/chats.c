/*
   This file is part of GNUnet.
   Copyright (C) 2022 GNUnet e.V.

   GNUnet is free software: you can redistribute it and/or modify it
   under the terms of the GNU Affero General Public License as published
   by the Free Software Foundation, either version 3 of the License,
   or (at your option) any later version.

   GNUnet is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   SPDX-License-Identifier: AGPL3.0-or-later
 */
/*
 * @author Tobias Frisch
 * @file ui/chats.c
 */

#include "chats.h"

#include "../application.h"
#include "../util.h"

static int
_chats_iterate_group(void *cls,
		     UNUSED struct GNUNET_CHAT_Handle *handle,
		     struct GNUNET_CHAT_Group *group)
{
  UI_CHATS_Handle *chats = cls;

  const bool selected = (chats->line_selected == chats->line_index);

  chats->line_index++;

  if (selected)
    chats->selected = GNUNET_CHAT_group_get_context(group);

  return GNUNET_YES;
}

static int
_chats_iterate_contact(void *cls,
		       UNUSED struct GNUNET_CHAT_Handle *handle,
		       struct GNUNET_CHAT_Contact *contact)
{
  UI_CHATS_Handle *chats = cls;

  const bool selected = (chats->line_selected == chats->line_index);

  chats->line_index++;

  if (selected)
    chats->selected = GNUNET_CHAT_contact_get_context(contact);

  return GNUNET_YES;
}

static int
_chats_iterate_messages(void *cls,
			struct GNUNET_CHAT_Context *context,
			const struct GNUNET_CHAT_Message *message)
{
  MESSENGER_Chat *chat = cls;
  chat_process_message(chat, context, message);
  return GNUNET_YES;
}

void
chats_event(UI_CHATS_Handle *chats,
	    MESSENGER_Application *app,
	    int key)
{
  if (chats->open_dialog.window)
  {
    chat_open_dialog_event(&(chats->open_dialog), app, key);
    return;
  }
  else if (chats->enter_dialog.window)
  {
    lobby_enter_dialog_event(&(chats->enter_dialog), app, key);
    return;
  }

  chats->line_index = 0;
  chats->selected = NULL;

  int count = 3;

  count += GNUNET_CHAT_iterate_groups(
      app->chat.handle,
      &_chats_iterate_group,
      chats
  );

  count += GNUNET_CHAT_iterate_contacts(
      app->chat.handle,
      &_chats_iterate_contact,
      chats
  );

  switch (key)
  {
    case 27:
    case KEY_EXIT:
    {
      GNUNET_CHAT_disconnect(app->chat.handle);
      break;
    }
    case KEY_UP:
    {
      chats->line_selected--;
      break;
    }
    case KEY_DOWN:
    {
      chats->line_selected++;
      break;
    }
    case '\n':
    case KEY_ENTER:
    {
      if (chats->selected)
      {
	GNUNET_CHAT_context_request(chats->selected);

	members_clear(&(app->current.members));
	messages_clear(&(app->current.messages));

	GNUNET_CHAT_context_set_user_pointer(
	    chats->selected,
	    &(app->current)
	);

	GNUNET_CHAT_context_iterate_messages(
	    chats->selected,
	    &_chats_iterate_messages,
	    &(app->chat)
	);

	app->chat.context = chats->selected;
      }
      else if (chats->line_selected == count - 3)
	chats->open_dialog.window = &(chats->window);
      else if (chats->line_selected == count - 1)
	chats->enter_dialog.window = &(chats->window);
      break;
    }
    default:
      break;
  }

  if (chats->line_selected < 0)
    chats->line_selected = 0;
  else if (chats->line_selected >= count)
    chats->line_selected = count - 1;

  if (!(chats->window))
    return;

  const int height = getmaxy(chats->window);
  const int y = chats->line_selected - chats->line_offset;

  if (y < 0)
    chats->line_offset += y;
  else if (y + 1 >= height)
    chats->line_offset += y + 1 - height;

  if (chats->line_offset < 0)
    chats->line_offset = 0;
  else if (chats->line_offset >= count)
    chats->line_offset = count - 1;
}

static int
_chats_print_entry(UI_CHATS_Handle *chats,
		   char type,
		   char chat_type,
		   const char *text)
{
  const bool selected = (chats->line_selected == chats->line_index);
  const int y = chats->line_index - chats->line_offset;

  chats->line_index++;

  if (y < 0)
    return GNUNET_YES;

  const int height = getmaxy(chats->window);

  if (y >= height)
    return GNUNET_NO;

  const int attrs_select = A_BOLD;

  if (selected) wattron(chats->window, attrs_select);

  wmove(chats->window, y, 0);

  if (chat_type)
    wprintw(chats->window, "[%c][%c] %s", type, chat_type, text);
  else
    wprintw(chats->window, "[%c] %s", type, text);

  if (selected) wattroff(chats->window, attrs_select);

  return GNUNET_YES;
}

int
_chats_iterate_print_group(void *cls,
			   UNUSED struct GNUNET_CHAT_Handle *handle,
			   struct GNUNET_CHAT_Group *group)
{
  UI_CHATS_Handle *chats = cls;

  const char *name = GNUNET_CHAT_group_get_name(group);

  return _chats_print_entry(chats, 'x', 'G', name);
}

int
_chats_iterate_print_contact(void *cls,
			     UNUSED struct GNUNET_CHAT_Handle *handle,
			     struct GNUNET_CHAT_Contact *contact)
{
  UI_CHATS_Handle *chats = cls;

  const char *name = GNUNET_CHAT_contact_get_name(contact);

  return _chats_print_entry(chats, 'x', 'C', name);
}

void
chats_print(UI_CHATS_Handle *chats,
	    MESSENGER_Application *app)
{
  if (chats->open_dialog.window)
  {
    chat_open_dialog_print(&(chats->open_dialog), app);
    return;
  }
  else if (chats->enter_dialog.window)
  {
    lobby_enter_dialog_print(&(chats->enter_dialog), app);
    return;
  }

  if (!(chats->window))
    return;

  chats->line_index = 0;
  werase(chats->window);

  GNUNET_CHAT_iterate_groups(
      app->chat.handle,
      &_chats_iterate_print_group,
      chats
  );

  GNUNET_CHAT_iterate_contacts(
      app->chat.handle,
      &_chats_iterate_print_contact,
      chats
  );

  _chats_print_entry(chats, '+', '\0', "Add chat");
  _chats_print_entry(chats, '+', '\0', "Open lobby");
  _chats_print_entry(chats, '+', '\0', "Enter lobby");
}
