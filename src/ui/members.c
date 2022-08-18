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

#include "list_input.h"
#include "../application.h"
#include "../util.h"

void
members_event(UI_MEMBERS_Handle *members,
	      struct MESSENGER_Application *app,
	      int key)
{
  list_input_reset(members);

  UI_MEMBERS_List *element = members->head;
  while (element)
  {
    list_input_select(members, 1, element->contact);
    element = element->next;
  }

  switch (key)
  {
    case 27:
    case KEY_EXIT:
    case '\t':
      app->chat.show_members = FALSE;
      break;
    case '\n':
    case KEY_ENTER:
      if (members->selected)
      {
	// TODO
      }

      break;
    default:
      break;
  }

  list_input_event(members, key);
}

static void
_members_iterate_print(UI_MEMBERS_Handle *members,
		       const struct GNUNET_CHAT_Contact *contact)
{
  list_input_print(members, 1);

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

  list_input_reset(members);
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
