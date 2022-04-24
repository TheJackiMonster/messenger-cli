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
_chats_iterate(void *cls,
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
_chats_iterate_messages(void *cls,
			UNUSED struct GNUNET_CHAT_Context *context,
			const struct GNUNET_CHAT_Message *message)
{
  UI_MESSAGES_Handle *messages = cls;

  messages_add(messages, message);

  return GNUNET_YES;
}

void
chats_event(UI_CHATS_Handle *chats,
	    MESSENGER_Application *app,
	    int key)
{
  chats->line_index = 0;
  chats->selected = NULL;

  int count = GNUNET_CHAT_iterate_groups(
      app->chat.handle,
      &_chats_iterate,
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
	messages_clear(&(app->messages));

	GNUNET_CHAT_context_iterate_messages(
	    chats->selected,
	    &_chats_iterate_messages,
	    &(app->messages)
	);

	GNUNET_CHAT_context_set_user_pointer(
	    chats->selected,
	    &(app->messages)
	);

	app->chat.context = chats->selected;
      }

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

  const int height = getmaxy(chats->window) - getbegy(chats->window);
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

int
_chats_iterate_print(void *cls,
		     UNUSED struct GNUNET_CHAT_Handle *handle,
		     struct GNUNET_CHAT_Group *group)
{
  UI_CHATS_Handle *chats = cls;

  const bool selected = (chats->line_selected == chats->line_index);
  const int y = chats->line_index - chats->line_offset;

  chats->line_index++;

  if (y < 0)
    return GNUNET_YES;

  const int height = getmaxy(chats->window) - getbegy(chats->window);

  if (y >= height)
    return GNUNET_NO;

  const char *name = GNUNET_CHAT_group_get_name(group);

  const int attrs_select = A_BOLD;

  if (selected) wattron(chats->window, attrs_select);

  wmove(chats->window, y, 0);
  wprintw(chats->window, "%s", name);

  if (selected) wattroff(chats->window, attrs_select);

  return GNUNET_YES;
}

void
chats_print(UI_CHATS_Handle *chats,
	    MESSENGER_Application *app)
{
  if (!(chats->window))
    return;

  chats->line_index = 0;
  werase(chats->window);

  GNUNET_CHAT_iterate_groups(
      app->chat.handle,
      &_chats_iterate_print,
      chats
  );
}
