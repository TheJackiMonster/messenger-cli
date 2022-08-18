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
 * @file ui/lobby_enter_dialog.h
 */

#ifndef UI_LOBBY_ENTER_DIALOG_H_
#define UI_LOBBY_ENTER_DIALOG_H_

#include <stdlib.h>
#include <curses.h>

struct MESSENGER_Application;

typedef struct UI_LOBBY_ENTER_DIALOG_Handle
{
  WINDOW **window;

  char *error;

  char uri [2048];
  int uri_len;
  int uri_pos;
} UI_LOBBY_ENTER_DIALOG_Handle;

void
lobby_enter_dialog_event(UI_LOBBY_ENTER_DIALOG_Handle *enter_dialog,
		         struct MESSENGER_Application *app,
		         int key);

void
lobby_enter_dialog_print(UI_LOBBY_ENTER_DIALOG_Handle *enter_dialog,
		         struct MESSENGER_Application *app);

#endif /* UI_LOBBY_ENTER_DIALOG_H_ */
