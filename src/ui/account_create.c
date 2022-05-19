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
 * @file ui/account_create.c
 */

#include "account_create.h"

#include <ctype.h>

#include <gnunet/platform.h>
#include <gnunet/gnunet_chat_lib.h>
#include <gnunet/gnunet_util_lib.h>

#include "../application.h"
#include "../util.h"

void
account_create_event(UI_ACCOUNT_CREATE_Handle *create,
		     struct MESSENGER_Application *app,
		     int key)
{
  switch (key)
  {
    case 27:
    case KEY_EXIT:
    {
      create->window = NULL;
      break;
    }
    case KEY_LEFT:
    {
      create->name_pos--;
      break;
    }
    case KEY_RIGHT:
    {
      create->name_pos++;
      break;
    }
    case '\n':
    case KEY_ENTER:
    {
      if (create->name_len > 0)
	GNUNET_CHAT_account_create(app->chat.handle, create->name);

      create->name_len = 0;
      create->window = NULL;
      break;
    }
    case KEY_BACKSPACE:
    {
      if ((create->name_pos < create->name_len) &&
	  (create->name_pos > 0))
	for (int i = create->name_pos; i < create->name_len; i++)
	  create->name[i - 1] = create->name[i];

      if ((create->name_pos > 0) && (create->name_len > 0))
      {
	create->name_pos--;
	create->name_len--;
      }

      break;
    }
    default:
    {
      if (!isalnum(key))
	break;

      for (int i = create->name_len - 1; i >= create->name_pos; i--)
	create->name[i + 1] = create->name[i];

      create->name[create->name_pos++] = (char) key;
      create->name_len++;
      break;
    }
  }

  if (create->name_len > 255)
    create->name_len = 255;

  create->name[create->name_len] = '\0';

  if (create->name_pos < 0)
    create->name_pos = 0;

  if (create->name_pos > create->name_len)
    create->name_pos = create->name_len;
}

void
account_create_print(UI_ACCOUNT_CREATE_Handle *create,
		     UNUSED struct MESSENGER_Application *app)
{
  if (!(create->window))
    return;

  werase(create->window);
  wmove(create->window, 0, 0);

  wprintw(create->window, "%s", create->name);
  wmove(create->window, 0, create->name_pos);
}
