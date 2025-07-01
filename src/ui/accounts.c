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
 * @file ui/accounts.c
 */

#include "accounts.h"

#include "list_input.h"
#include "../application.h"
#include "../util.h"

int
_accounts_iterate(void *cls,
                  UNUSED struct GNUNET_CHAT_Handle *handle,
                  struct GNUNET_CHAT_Account *account)
{
  UI_ACCOUNTS_Handle *accounts = cls;
  list_input_select(accounts, 1, account);
  return GNUNET_YES;
}

void
accounts_event(UI_ACCOUNTS_Handle *accounts,
               MESSENGER_Application *app,
               int key)
{
  if (accounts->create_dialog.window)
  {
    account_create_dialog_event(&(accounts->create_dialog), app, key);
    return;
  }

  list_input_reset(accounts);

  GNUNET_CHAT_iterate_accounts(
    app->chat.handle,
    &_accounts_iterate,
    accounts
  );

  list_input_select(accounts, 1, NULL);

  switch (key)
  {
    case 27:
    case KEY_EXIT:
      app->chat.quit = TRUE;
      break;
    case '\n':
    case KEY_ENTER:
      if (accounts->selected)
	GNUNET_CHAT_connect(app->chat.handle, accounts->selected);
      else
	accounts->create_dialog.window = &(accounts->window);
      break;
    default:
      break;
  }

  list_input_event(accounts, key);
}

static int
_accounts_print_entry(UI_ACCOUNTS_Handle *accounts,
                      char type,
                      const char *text,
                      const void *data)
{
  list_input_print_gnunet(accounts, 1);

  const int attrs_select = A_BOLD;

  if (selected) wattron(accounts->window, attrs_select);
  else type = ' ';

  wmove(accounts->window, y, 0);

  util_enable_unique_color(accounts->window, data);

  wprintw(accounts->window, "[%c] %s", type, text);

  util_disable_unique_color(accounts->window, data);

  if (selected) wattroff(accounts->window, attrs_select);

  return GNUNET_YES;
}

int
_accounts_iterate_print(void *cls,
                        UNUSED struct GNUNET_CHAT_Handle *handle,
                        struct GNUNET_CHAT_Account *account)
{
  UI_ACCOUNTS_Handle *accounts = cls;
  const char *name = GNUNET_CHAT_account_get_name(account);
  return _accounts_print_entry(accounts, 'x', name, account);
}

void
accounts_print(UI_ACCOUNTS_Handle *accounts,
               MESSENGER_Application *app)
{
  if (accounts->create_dialog.window)
  {
    account_create_dialog_print(&(accounts->create_dialog));
    return;
  }

  if (!(accounts->window))
    return;

  list_input_reset(accounts);
  werase(accounts->window);

  GNUNET_CHAT_iterate_accounts(
      app->chat.handle,
      &_accounts_iterate_print,
      accounts
  );

  _accounts_print_entry(accounts, '+', "Add account", NULL);
}
