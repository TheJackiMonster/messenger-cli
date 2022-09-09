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
 * @file ui/account_create_dialog.h
 */

#ifndef UI_ACCOUNT_CREATE_DIALOG_H_
#define UI_ACCOUNT_CREATE_DIALOG_H_

#include <stdlib.h>
#include <curses.h>

struct MESSENGER_Application;

/**
 * @struct UI_ACCOUNT_CREATE_DIALOG_Handle
 */
typedef struct UI_ACCOUNT_CREATE_DIALOG_Handle
{
  WINDOW **window;

  char name [256];
  int name_len;
  int name_pos;
} UI_ACCOUNT_CREATE_DIALOG_Handle;

/**
 * Processes the current key event by the dialog
 * to create a new account.
 *
 * @param[in,out] create_dialog Account creation dialog
 * @param[in,out] app Application handle
 * @param[in] key Key
 */
void
account_create_dialog_event(UI_ACCOUNT_CREATE_DIALOG_Handle *create_dialog,
			    struct MESSENGER_Application *app,
			    int key);

/**
 * Prints the content of the dialog to create a new
 * account to its selected window view.
 *
 * @param[in] create_dialog Account creation dialog
 */
void
account_create_dialog_print(UI_ACCOUNT_CREATE_DIALOG_Handle *create_dialog);

#endif /* UI_ACCOUNT_CREATE_DIALOG_H_ */
