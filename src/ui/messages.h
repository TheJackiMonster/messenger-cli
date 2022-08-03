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
 * @file ui/messages.h
 */

#ifndef UI_MESSAGES_H_
#define UI_MESSAGES_H_

#include <stdlib.h>
#include <curses.h>

#include <gnunet/platform.h>
#include <gnunet/gnunet_chat_lib.h>
#include <gnunet/gnunet_util_lib.h>

struct MESSENGER_Application;

typedef struct UI_MESSAGES_List
{
  const struct GNUNET_CHAT_Message *message;

  struct UI_MESSAGES_List *prev;
  struct UI_MESSAGES_List *next;
} UI_MESSAGES_List;

#define TEXT_LEN_MAX 1024

typedef struct UI_MESSAGES_Handle
{
  WINDOW *window;

  UI_MESSAGES_List *head;
  UI_MESSAGES_List *tail;

  int line_index;
  int line_offset;
  int line_selected;

  const struct GNUNET_CHAT_Message *selected;

  char text [1024];
  int text_len;
  int text_pos;
} UI_MESSAGES_Handle;

#define UI_MESSAGES_COLS_MIN 50

void
messages_event(UI_MESSAGES_Handle *messages,
	       struct MESSENGER_Application *app,
	       int key);

void
messages_print(UI_MESSAGES_Handle *messages);

void
messages_clear(UI_MESSAGES_Handle *messages);

void
messages_add(UI_MESSAGES_Handle *messages,
	     const struct GNUNET_CHAT_Message *message);

void
messages_remove(UI_MESSAGES_Handle *messages,
	        const struct GNUNET_CHAT_Message *message);

#endif /* UI_MESSAGES_H_ */
