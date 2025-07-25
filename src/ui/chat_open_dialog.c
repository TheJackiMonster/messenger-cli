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
 * @file ui/chat_open_dialog.c
 */

#include "chat_open_dialog.h"

#include <gnunet/gnunet_chat_lib.h>
#include <gnunet/gnunet_util_lib.h>

#include "text_input.h"
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
      open_dialog->window = NULL;
      break;
    case '\n':
    case KEY_ENTER:
    {
      if (open_dialog->topic_len > 0)
      {
        struct GNUNET_CHAT_Group *group;
        char *group_name = malloc(open_dialog->topic_len + 2);

        group = GNUNET_CHAT_group_create(app->chat.handle, open_dialog->topic);
        group_name[0] = '#';
        memcpy(group_name + 1, open_dialog->topic, open_dialog->topic_len + 1);

        GNUNET_CHAT_group_set_name(group, group_name);
        free(group_name);
      }

      open_dialog->topic_len = 0;
      open_dialog->window = NULL;
      break;
    }
    default:
      break;
  }

  text_input_event(open_dialog->topic, key);
}

void
chat_open_dialog_print(UI_CHAT_OPEN_DIALOG_Handle *open_dialog)
{
  if (!(open_dialog->window))
    return;

  WINDOW *window = *(open_dialog->window);

  werase(window);
  wmove(window, 0, 0);

  util_print_prompt(window, "Enter the topic of the public chat:");
  wmove(window, 1, 0);

  wprintw(window, "> ");
  wmove(window, 1, 2);

  wattron(window, A_BOLD);

  wprintw(window, "%s", open_dialog->topic);
  wmove(window, 1, 2 + open_dialog->topic_pos);

  wattroff(window, A_BOLD);

  wcursyncup(window);
  curs_set(1);
}
