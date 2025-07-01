/*
   This file is part of GNUnet.
   Copyright (C) 2022--2025 GNUnet e.V.

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
 * @file ui/account_create_dialog.c
 */

#include "account_create_dialog.h"

#include <gnunet/gnunet_chat_lib.h>
#include <gnunet/gnunet_util_lib.h>

#include "text_input.h"
#include "../application.h"
#include "../util.h"

void
account_create_dialog_event(UI_ACCOUNT_CREATE_DIALOG_Handle *create_dialog,
			    struct MESSENGER_Application *app,
			    int key)
{
  switch (key)
  {
    case 27:
    case KEY_EXIT:
      create_dialog->window = NULL;
      break;
    case '\n':
    case KEY_ENTER:
      if (create_dialog->name_len > 0)
	GNUNET_CHAT_account_create(app->chat.handle, create_dialog->name);

      create_dialog->name_len = 0;
      create_dialog->window = NULL;
      break;
    default:
      break;
  }

  text_input_event(create_dialog->name, key);
}

void
account_create_dialog_print(UI_ACCOUNT_CREATE_DIALOG_Handle *create_dialog)
{
  if (!(create_dialog->window))
    return;

  WINDOW *window = *(create_dialog->window);

  werase(window);
  wmove(window, 0, 0);

  util_print_prompt(window, "Enter name of the new account:");
  wmove(window, 1, 0);

  wprintw(window, "> ");
  wmove(window, 1, 2);

  wattron(window, A_BOLD);

  wprintw(window, "%s", create_dialog->name);
  wmove(window, 1, 2 + create_dialog->name_pos);

  wattroff(window, A_BOLD);

  wcursyncup(window);
  curs_set(1);
}
