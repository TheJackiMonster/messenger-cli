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
 * @file ui/members.h
 */

#ifndef UI_MEMBERS_H_
#define UI_MEMBERS_H_

#include <stdlib.h>
#include <curses.h>

#include <gnunet/platform.h>
#include <gnunet/gnunet_chat_lib.h>
#include <gnunet/gnunet_util_lib.h>

struct MESSENGER_Application;

typedef struct UI_MEMBERS_List
{
  const struct GNUNET_CHAT_Contact *contact;

  struct UI_MEMBERS_List *prev;
  struct UI_MEMBERS_List *next;
} UI_MEMBERS_List;

typedef struct UI_MEMBERS_Handle
{
  WINDOW *window;

  UI_MEMBERS_List *head;
  UI_MEMBERS_List *tail;

  int line_index;
  int line_offset;
  int line_selected;

  const struct GNUNET_CHAT_Contact *selected;
} UI_MEMBERS_Handle;

void
members_event(UI_MEMBERS_Handle *members,
	      struct MESSENGER_Application *app,
	      int key);

void
members_print(UI_MEMBERS_Handle *members);

void
members_clear(UI_MEMBERS_Handle *members);

bool
members_add(UI_MEMBERS_Handle *members,
	    const struct GNUNET_CHAT_Contact *contact);

void
members_remove(UI_MEMBERS_Handle *members,
	       const struct GNUNET_CHAT_Contact *contact);

#endif /* UI_MEMBERS_H_ */
