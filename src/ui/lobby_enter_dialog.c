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
 * @file ui/lobby_enter_dialog.c
 */

#include "lobby_enter_dialog.h"

#include <gnunet/gnunet_chat_lib.h>
#include <gnunet/gnunet_util_lib.h>

#include "text_input.h"
#include "../application.h"
#include "../util.h"

void
lobby_enter_dialog_event(UI_LOBBY_ENTER_DIALOG_Handle *enter_dialog,
		         struct MESSENGER_Application *app,
		         int key)
{
  struct GNUNET_CHAT_Uri *uri;

  switch (key)
  {
    case 27:
    case KEY_EXIT:
      if (enter_dialog->error)
	GNUNET_free(enter_dialog->error);

      enter_dialog->error = NULL;
      enter_dialog->window = NULL;
      break;
    case '\n':
    case KEY_ENTER:
      if (enter_dialog->uri_len > 0)
      {
	if (enter_dialog->error)
	  GNUNET_free(enter_dialog->error);

	enter_dialog->error = NULL;
	uri = GNUNET_CHAT_uri_parse(enter_dialog->uri, &(enter_dialog->error));

	if (uri)
	{
	  GNUNET_CHAT_lobby_join(app->chat.handle, uri);
	  GNUNET_CHAT_uri_destroy(uri);

	  enter_dialog->uri_len = 0;
	  enter_dialog->window = NULL;
	}
      }

      break;
    default:
      break;
  }

  text_input_event(enter_dialog->uri, key);
}

void
lobby_enter_dialog_print(UI_LOBBY_ENTER_DIALOG_Handle *enter_dialog)
{
  if (!(enter_dialog->window))
    return;

  WINDOW *window = *(enter_dialog->window);

  werase(window);
  wmove(window, 0, 0);

  util_print_prompt(window, "Enter the URI of the lobby:");
  wmove(window, 1, 0);

  wprintw(window, "> ");
  wmove(window, 1, 2);

  wattron(window, A_BOLD);
  wprintw(window, "%s", enter_dialog->uri);
  wattroff(window, A_BOLD);

  if (enter_dialog->error)
  {
    wmove(window, 2, 0);
    wprintw(window, "ERROR: %s", enter_dialog->error);
  }

  wattron(window, A_BOLD);
  wmove(window, 1, 2 + enter_dialog->uri_pos);
  wattroff(window, A_BOLD);

  wcursyncup(window);
  curs_set(1);
}
