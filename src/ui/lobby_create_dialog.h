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
 * @file ui/lobby_create_dialog.h
 */

#ifndef UI_LOBBY_CREATE_DIALOG_H_
#define UI_LOBBY_CREATE_DIALOG_H_

#include <stdlib.h>
#include <curses.h>

#include <gnunet/gnunet_chat_lib.h>
#include <gnunet/gnunet_util_lib.h>

struct MESSENGER_Application;

/**
 * @struct UI_LOBBY_CREATE_DIALOG_Handle
 */
typedef struct UI_LOBBY_CREATE_DIALOG_Handle
{
  WINDOW *window;
  WINDOW **win;

  int line_prev;
  int line_next;

  int line_index;
  int line_offset;
  int line_selected;

  uint64_t selected;

  struct GNUNET_CHAT_Lobby *lobby;
  char *uri;
} UI_LOBBY_CREATE_DIALOG_Handle;

/**
 * Processes the current key event by the dialog
 * to create a lobby.
 *
 * @param[in,out] create_dialog Lobby creation dialog
 * @param[in,out] app Application handle
 * @param[in] key Key
 */
void
lobby_create_dialog_event(UI_LOBBY_CREATE_DIALOG_Handle *create_dialog,
		          struct MESSENGER_Application *app,
		          int key);

/**
 * Prints the content of the dialog to create a
 * lobby to its selected window view.
 *
 * @param[in] create_dialog Lobby creation dialog
 */
void
lobby_create_dialog_print(UI_LOBBY_CREATE_DIALOG_Handle *create_dialog);

#endif /* UI_LOBBY_CREATE_DIALOG_H_ */
