/*
   This file is part of GNUnet.
   Copyright (C) 2022--2024 GNUnet e.V.

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
 * @file ui/chats.h
 */

#ifndef UI_CHATS_H_
#define UI_CHATS_H_

#include <stdlib.h>
#include <curses.h>

#include <gnunet/gnunet_chat_lib.h>
#include <gnunet/gnunet_util_lib.h>

#include "chat_open_dialog.h"
#include "lobby_create_dialog.h"
#include "lobby_enter_dialog.h"

struct MESSENGER_Application;

/**
 * @struct UI_CHATS_Handle
 */
typedef struct UI_CHATS_Handle
{
  WINDOW *window;

  int line_prev;
  int line_next;

  int line_index;
  int line_offset;
  int line_selected;

  struct GNUNET_CHAT_Context *selected;

  UI_CHAT_OPEN_DIALOG_Handle open_dialog;
  UI_LOBBY_CREATE_DIALOG_Handle create_dialog;
  UI_LOBBY_ENTER_DIALOG_Handle enter_dialog;
} UI_CHATS_Handle;

#define UI_CHATS_ROWS_MIN 8
#define UI_CHATS_COLS_MIN 30

/**
 * Processes the current key event by the view
 * to show the list of available chats.
 *
 * @param[in,out] chats Chats view
 * @param[in,out] app Application handle
 * @param[in] key Key
 */
void
chats_event(UI_CHATS_Handle *chats,
            struct MESSENGER_Application *app,
            int key);

/**
 * Prints the content of the view to show
 * the list of available chats to its selected
 * window view on screen.
 *
 * @param[in,out] chats Chats view
 * @param[in] app Application handle
 */
void
chats_print(UI_CHATS_Handle *chats,
            struct MESSENGER_Application *app);

#endif /* UI_CHATS_H_ */
