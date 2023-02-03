/*
   This file is part of GNUnet.
   Copyright (C) 2022--2023 GNUnet e.V.

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

#include <stdbool.h>
#include <stdlib.h>
#include <curses.h>

#include <gnunet/gnunet_chat_lib.h>
#include <gnunet/gnunet_util_lib.h>

struct MESSENGER_Application;

/**
 * @struct UI_MEMBERS_List
 */
typedef struct UI_MEMBERS_List
{
  struct GNUNET_CHAT_Contact *contact;

  struct UI_MEMBERS_List *prev;
  struct UI_MEMBERS_List *next;
} UI_MEMBERS_List;

/**
 * @struct UI_MEMBERS_Handle
 */
typedef struct UI_MEMBERS_Handle
{
  WINDOW *window;

  UI_MEMBERS_List *head;
  UI_MEMBERS_List *tail;

  int line_prev;
  int line_next;

  int line_index;
  int line_offset;
  int line_selected;

  struct GNUNET_CHAT_Contact *selected;
} UI_MEMBERS_Handle;

#define UI_MEMBERS_COLS_MIN 30

/**
 * Processes the current key event by the view
 * to show a chats list of members.
 *
 * @param[in,out] members Chat members view
 * @param[in,out] app Application handle
 * @param[in] key Key
 */
void
members_event(UI_MEMBERS_Handle *members,
	      struct MESSENGER_Application *app,
	      int key);

/**
 * Prints the content of the view to show
 * a chats list of members to its selected
 * window view on screen.
 *
 * @param[in,out] members Chat members view
 */
void
members_print(UI_MEMBERS_Handle *members);

/**
 * Clears the list of members the view
 * would print to the screen.
 *
 * @param[out] members Chat members view
 */
void
members_clear(UI_MEMBERS_Handle *members);

/**
 * Adds a new chat contact to the list of
 * members the view will print to the
 * screen.
 *
 * @param[in,out] members Chat members view
 * @param[in] contact Chat contact
 * @return #TRUE if the member is new, otherwise #FALSE
 */
bool
members_add(UI_MEMBERS_Handle *members,
	    struct GNUNET_CHAT_Contact *contact);

/**
 * Removes a chat contact from the list of
 * members the view would print to the
 * screen.
 *
 * @param[in,out] members Chat members view
 * @param[in] contact Chat contact
 */
void
members_remove(UI_MEMBERS_Handle *members,
	       const struct GNUNET_CHAT_Contact *contact);

#endif /* UI_MEMBERS_H_ */
