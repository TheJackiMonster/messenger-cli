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
 * @file ui/accounts.h
 */

#ifndef UI_ACCOUNTS_H_
#define UI_ACCOUNTS_H_

#include <stdlib.h>
#include <curses.h>

#include <gnunet/platform.h>
#include <gnunet/gnunet_chat_lib.h>
#include <gnunet/gnunet_util_lib.h>

#include "account_create_dialog.h"

struct MESSENGER_Application;

/**
 * @struct UI_ACCOUNTS_Handle
 */
typedef struct UI_ACCOUNTS_Handle
{
  WINDOW *window;

  int line_prev;
  int line_next;

  int line_index;
  int line_offset;
  int line_selected;

  struct GNUNET_CHAT_Account *selected;

  UI_ACCOUNT_CREATE_DIALOG_Handle create_dialog;
} UI_ACCOUNTS_Handle;

#define UI_ACCOUNTS_ROWS_MIN 5
#define UI_ACCOUNTS_COLS_MIN 30

/**
 * Processes the current key event by the view
 * to show the list of chat accounts.
 *
 * @param[in,out] accounts Chat accounts view
 * @param[in,out] app Application handle
 * @param[in] key Key
 */
void
accounts_event(UI_ACCOUNTS_Handle *accounts,
	       struct MESSENGER_Application *app,
	       int key);

/**
 * Prints the content of the view to show
 * the list of chat accounts to its selected
 * window view on screen.
 *
 * @param[in,out] accounts Chat accounts view
 * @param[in] app Application handle
 */
void
accounts_print(UI_ACCOUNTS_Handle *accounts,
	       struct MESSENGER_Application *app);

#endif /* UI_ACCOUNTS_H_ */
