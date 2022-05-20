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
 * @file ui/chat_open_dialog.c
 */

#include "chat_open_dialog.h"

#include <ctype.h>

#include <gnunet/platform.h>
#include <gnunet/gnunet_chat_lib.h>
#include <gnunet/gnunet_util_lib.h>

#include "../application.h"
#include "../util.h"

void
chat_open_dialog_event(UI_CHAT_OPEN_DIALOG_Handle *open_dialog,
		       struct MESSENGER_Application *app,
		       int key)
{
  switch (key)
  {
    case 27:
    case KEY_EXIT:
    {
      open_dialog->window = NULL;
      break;
    }
    case KEY_LEFT:
    {
      open_dialog->topic_pos--;
      break;
    }
    case KEY_RIGHT:
    {
      open_dialog->topic_pos++;
      break;
    }
    case '\n':
    case KEY_ENTER:
    {
      if (open_dialog->topic_len > 0)
	GNUNET_CHAT_group_create(app->chat.handle, open_dialog->topic);

      open_dialog->topic_len = 0;
      open_dialog->window = NULL;
      break;
    }
    case KEY_BACKSPACE:
    {
      if ((open_dialog->topic_pos < open_dialog->topic_len) &&
	(open_dialog->topic_pos > 0))
      for (int i = open_dialog->topic_pos; i < open_dialog->topic_len; i++)
	open_dialog->topic[i - 1] = open_dialog->topic[i];

      if ((open_dialog->topic_pos > 0) && (open_dialog->topic_len > 0))
      {
	open_dialog->topic_pos--;
	open_dialog->topic_len--;
      }

      break;
    }
    default:
    {
      if (!isalnum(key))
      break;

      for (int i = open_dialog->topic_len - 1; i >= open_dialog->topic_pos; i--)
	open_dialog->topic[i + 1] = open_dialog->topic[i];

      open_dialog->topic[open_dialog->topic_pos++] = (char) key;
      open_dialog->topic_len++;
      break;
    }
  }

  if (open_dialog->topic_len > 255)
    open_dialog->topic_len = 255;

  open_dialog->topic[open_dialog->topic_len] = '\0';

  if (open_dialog->topic_pos < 0)
    open_dialog->topic_pos = 0;

  if (open_dialog->topic_pos > open_dialog->topic_len)
    open_dialog->topic_pos = open_dialog->topic_len;
}

void
chat_open_dialog_print(UI_CHAT_OPEN_DIALOG_Handle *open_dialog,
		       UNUSED struct MESSENGER_Application *app)
{
  if (!(open_dialog->window))
    return;

  werase(open_dialog->window);
  wmove(open_dialog->window, 0, 0);

  wprintw(open_dialog->window, "%s", open_dialog->topic);
  wmove(open_dialog->window, 0, open_dialog->topic_pos);
}
