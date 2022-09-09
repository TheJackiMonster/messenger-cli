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
 * @file ui/lobby_create_dialog.c
 */

#include "lobby_create_dialog.h"

#include "list_input.h"
#include "../application.h"
#include "../util.h"

void
_lobby_open_with_uri(void *cls,
		     const struct GNUNET_CHAT_Uri *uri)
{
  UI_LOBBY_CREATE_DIALOG_Handle *create_dialog = cls;

  if (create_dialog->uri)
    GNUNET_free(create_dialog->uri);

  create_dialog->uri = GNUNET_CHAT_uri_to_string(uri);
}

void
lobby_create_dialog_event(UI_LOBBY_CREATE_DIALOG_Handle *create_dialog,
		          UNUSED struct MESSENGER_Application *app,
		          int key)
{
  create_dialog->window = *(create_dialog->win);

  list_input_reset(create_dialog);

  if (create_dialog->uri)
    list_input_select(create_dialog, 1, 0)
  else
  {
    list_input_select(create_dialog, 1, 30);
    list_input_select(create_dialog, 1, 5 * 60);
    list_input_select(create_dialog, 1, 60 * 60);
    list_input_select(create_dialog, 1, 8 * 60 * 60);
    list_input_select(create_dialog, 1, 24 * 60 * 60);
    list_input_select(create_dialog, 1, 7 * 24 * 60 * 60);
    list_input_select(create_dialog, 1, 4 * 7 * 60 * 60);
    list_input_select(create_dialog, 1, 0);
  }

  switch (key)
  {
    case 27:
    case KEY_EXIT:
      if (create_dialog->lobby)
	GNUNET_CHAT_lobby_close(create_dialog->lobby);

      create_dialog->lobby = NULL;
      create_dialog->win = NULL;
      break;
    case '\n':
    case KEY_ENTER:
      if (create_dialog->uri)
      {
	GNUNET_free(create_dialog->uri);

	create_dialog->lobby = NULL;
	create_dialog->win = NULL;
      }
      else if (!(create_dialog->lobby))
	create_dialog->lobby = GNUNET_CHAT_lobby_open(
	    app->chat.handle,
	    GNUNET_TIME_relative_multiply(
		GNUNET_TIME_relative_get_second_(),
		create_dialog->selected
	    ),
	    _lobby_open_with_uri,
	    create_dialog
	);

      break;
    default:
      break;
  }

  if (!(create_dialog->lobby))
    list_input_event(create_dialog, key)
  else
    list_input_event(create_dialog, KEY_RESIZE);
}

static void
_lobby_iterate_print(UI_LOBBY_CREATE_DIALOG_Handle *create_dialog,
		     const char *label)
{
  list_input_print(create_dialog, 1);

  const int attrs_select = A_BOLD;

  if (selected) wattron(create_dialog->window, attrs_select);

  wmove(create_dialog->window, y, 0);
  wprintw(create_dialog->window, "%s", label);

  if (selected) wattroff(create_dialog->window, attrs_select);
}

void
lobby_create_dialog_print(UI_LOBBY_CREATE_DIALOG_Handle *create_dialog)
{
  if (!(create_dialog->win))
    return;

  create_dialog->window = *(create_dialog->win);

  list_input_reset(create_dialog);
  werase(create_dialog->window);

  if (create_dialog->uri)
    _lobby_iterate_print(create_dialog, create_dialog->uri);
  else
  {
    _lobby_iterate_print(create_dialog, "30 seconds");
    _lobby_iterate_print(create_dialog, "5 minutes");
    _lobby_iterate_print(create_dialog, "1 hour");
    _lobby_iterate_print(create_dialog, "8 hours");
    _lobby_iterate_print(create_dialog, "1 day");
    _lobby_iterate_print(create_dialog, "1 week");
    _lobby_iterate_print(create_dialog, "4 weeks");
    _lobby_iterate_print(create_dialog, "Off");
  }
}
