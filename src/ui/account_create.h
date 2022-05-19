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
 * @file ui/account_create.h
 */

#ifndef UI_ACCOUNT_CREATE_H_
#define UI_ACCOUNT_CREATE_H_

#include <stdlib.h>
#include <curses.h>

struct MESSENGER_Application;

typedef struct UI_ACCOUNT_CREATE_Handle
{
  WINDOW *window;

  char name [256];
  int name_len;
  int name_pos;
} UI_ACCOUNT_CREATE_Handle;

void
account_create_event(UI_ACCOUNT_CREATE_Handle *create,
		     struct MESSENGER_Application *app,
		     int key);

void
account_create_print(UI_ACCOUNT_CREATE_Handle *create,
		     struct MESSENGER_Application *app);

#endif /* UI_ACCOUNT_CREATE_H_ */
