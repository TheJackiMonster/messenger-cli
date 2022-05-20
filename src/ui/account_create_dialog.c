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
 * @file ui/account_create_dialog.c
 */

#include "account_create_dialog.h"

#include <ctype.h>

#include <gnunet/platform.h>
#include <gnunet/gnunet_chat_lib.h>
#include <gnunet/gnunet_util_lib.h>

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
    {
      create_dialog->window = NULL;
      break;
    }
    case KEY_LEFT:
    {
      create_dialog->name_pos--;
      break;
    }
    case KEY_RIGHT:
    {
      create_dialog->name_pos++;
      break;
    }
    case '\n':
    case KEY_ENTER:
    {
      if (create_dialog->name_len > 0)
	GNUNET_CHAT_account_create(app->chat.handle, create_dialog->name);

      create_dialog->name_len = 0;
      create_dialog->window = NULL;
      break;
    }
    case KEY_BACKSPACE:
    {
      if ((create_dialog->name_pos < create_dialog->name_len) &&
	  (create_dialog->name_pos > 0))
	for (int i = create_dialog->name_pos; i < create_dialog->name_len; i++)
	  create_dialog->name[i - 1] = create_dialog->name[i];

      if ((create_dialog->name_pos > 0) && (create_dialog->name_len > 0))
      {
	create_dialog->name_pos--;
	create_dialog->name_len--;
      }

      break;
    }
    default:
    {
      if (!isalnum(key))
	break;

      for (int i = create_dialog->name_len - 1; i >= create_dialog->name_pos; i--)
	create_dialog->name[i + 1] = create_dialog->name[i];

      create_dialog->name[create_dialog->name_pos++] = (char) key;
      create_dialog->name_len++;
      break;
    }
  }

  if (create_dialog->name_len > 255)
    create_dialog->name_len = 255;

  create_dialog->name[create_dialog->name_len] = '\0';

  if (create_dialog->name_pos < 0)
    create_dialog->name_pos = 0;

  if (create_dialog->name_pos > create_dialog->name_len)
    create_dialog->name_pos = create_dialog->name_len;
}

void
account_create_dialog_print(UI_ACCOUNT_CREATE_DIALOG_Handle *create_dialog,
			    UNUSED struct MESSENGER_Application *app)
{
  if (!(create_dialog->window))
    return;

  werase(create_dialog->window);
  wmove(create_dialog->window, 0, 0);

  wprintw(create_dialog->window, "%s", create_dialog->name);
  wmove(create_dialog->window, 0, create_dialog->name_pos);
}
