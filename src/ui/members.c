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
 * @file ui/members.c
 */

#include "members.h"

#include "../application.h"
#include "../util.h"

void
members_event(UI_MEMBERS_Handle *members,
	      struct MESSENGER_Application *app,
	      int key)
{
  members->line_index = 0;
  members->selected = NULL;

  int count = 0;

  UI_MEMBERS_List *element = members->head;
  while (element)
  {
    count++;

    element = element->next;
  }

  switch (key)
  {
    case 27:
    case KEY_EXIT:
    case '\t':
    {
      app->chat.show_members = FALSE;
      break;
    }
    case KEY_UP:
    {
      members->line_selected--;
      break;
    }
    case KEY_DOWN:
    {
      members->line_selected++;
      break;
    }
    case '\n':
    case KEY_ENTER:
    {
      if (members->selected)
      {
	// TODO
      }

      break;
    }
    default:
      break;
  }

  if (members->line_selected < 0)
    members->line_selected = 0;
  else if (members->line_selected >= count)
    members->line_selected = count - 1;

  if (!(members->window))
    return;

  const int height = getmaxy(members->window);
  const int y = members->line_selected - members->line_offset;

  if (y < 0)
    members->line_offset += y;
  else if (y + 1 >= height)
    members->line_offset += y + 1 - height;

  if (members->line_offset < 0)
    members->line_offset = 0;
  else if (members->line_offset >= count)
    members->line_offset = count - 1;
}

static void
_members_iterate_print(UI_MEMBERS_Handle *members,
		       const struct GNUNET_CHAT_Contact *contact)
{
  const bool selected = (members->line_selected == members->line_index);
  const int y = members->line_index - members->line_offset;

  members->line_index++;

  if (y < 0)
    return;

  const int height = getmaxy(members->window);

  if (y >= height)
    return;

  const int attrs_select = A_BOLD;

  if (selected) wattron(members->window, attrs_select);

  wmove(members->window, y, 0);

  const char *name = GNUNET_CHAT_contact_get_name(contact);
  const char *key = GNUNET_CHAT_contact_get_key(contact);

  size_t key_len = key? strlen(key) : 0;

  if (key_len > 4)
    wprintw(members->window, "[%s]: %s", key + (key_len - 4), name);
  else
    wprintw(members->window, "%s", name);

  if (selected) wattroff(members->window, attrs_select);
}

void
members_print(UI_MEMBERS_Handle *members)
{
  if (!(members->window))
    return;

  members->line_index = 0;
  werase(members->window);

  UI_MEMBERS_List *element = members->head;
  while (element)
  {
    _members_iterate_print(members, element->contact);

    element = element->next;
  }
}

void
members_clear(UI_MEMBERS_Handle *members)
{
  UI_MEMBERS_List *element;
  while (members->head)
  {
    element = members->head;

    GNUNET_CONTAINER_DLL_remove(
	members->head,
	members->tail,
	element
    );

    GNUNET_free(element);
  }

  members->line_selected = 0;
}

bool
members_add(UI_MEMBERS_Handle *members,
	    const struct GNUNET_CHAT_Contact *contact)
{
  UI_MEMBERS_List *element = members->head;
  while (element)
  {
    if (element->contact == contact)
      break;

    element = element->next;
  }

  if (element)
    return FALSE;

  element = GNUNET_new(UI_MEMBERS_List);
  element->contact = contact;

  GNUNET_CONTAINER_DLL_insert_tail(
      members->head,
      members->tail,
      element
  );

  return TRUE;
}

void
members_remove(UI_MEMBERS_Handle *members,
	       const struct GNUNET_CHAT_Contact *contact)
{
  UI_MEMBERS_List *element = members->head;
  while (element)
  {
    if (element->contact == contact)
      break;

    element = element->next;
  }

  if (element)
    GNUNET_CONTAINER_DLL_remove(
	members->head,
	members->tail,
	element
    );
}
