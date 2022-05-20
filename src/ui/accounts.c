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
 * @file ui/accounts.c
 */

#include "accounts.h"

#include "../application.h"
#include "../util.h"

int
_accounts_iterate(void *cls,
                  UNUSED const struct GNUNET_CHAT_Handle *handle,
                  struct GNUNET_CHAT_Account *account)
{
  UI_ACCOUNTS_Handle *accounts = cls;

  const bool selected = (accounts->line_selected == accounts->line_index);

  accounts->line_index++;

  if (selected)
    accounts->selected = account;

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

  accounts->line_index = 0;
  accounts->selected = NULL;

  int count = GNUNET_CHAT_iterate_accounts(
    app->chat.handle,
    &_accounts_iterate,
    accounts
  ) + 1;

  switch (key)
  {
    case 27:
    case KEY_EXIT:
    {
      app->chat.quit = GNUNET_YES;
      break;
    }
    case KEY_UP:
    {
      accounts->line_selected--;
      break;
    }
    case KEY_DOWN:
    {
      accounts->line_selected++;
      break;
    }
    case '\n':
    case KEY_ENTER:
    {
      if (accounts->selected)
	GNUNET_CHAT_connect(app->chat.handle, accounts->selected);
      else
	accounts->create_dialog.window = accounts->window;
      break;
    }
    default:
      break;
  }

  if (accounts->line_selected < 0)
    accounts->line_selected = 0;
  else if (accounts->line_selected >= count)
    accounts->line_selected = count - 1;

  if (!(accounts->window))
    return;

  const int height = getmaxy(accounts->window) - getbegy(accounts->window);
  const int y = accounts->line_selected - accounts->line_offset;

  if (y < 0)
    accounts->line_offset += y;
  else if (y + 1 >= height)
    accounts->line_offset += y + 1 - height;

  if (accounts->line_offset < 0)
    accounts->line_offset = 0;
  else if (accounts->line_offset >= count)
    accounts->line_offset = count - 1;
}

static int
_accounts_print_entry(UI_ACCOUNTS_Handle *accounts,
		      char type,
		      const char *text)
{
  const bool selected = (accounts->line_selected == accounts->line_index);
  const int y = accounts->line_index - accounts->line_offset;

  accounts->line_index++;

  if (y < 0)
    return GNUNET_YES;

  const int height = getmaxy(accounts->window) - getbegy(accounts->window);

  if (y >= height)
    return GNUNET_NO;

  const int attrs_select = A_BOLD;

  if (selected) wattron(accounts->window, attrs_select);

  wmove(accounts->window, y, 0);
  wprintw(accounts->window, "[%c] %s", selected? type : ' ', text);

  if (selected) wattroff(accounts->window, attrs_select);

  return GNUNET_YES;
}

int
_accounts_iterate_print(void *cls,
                        UNUSED const struct GNUNET_CHAT_Handle *handle,
                        struct GNUNET_CHAT_Account *account)
{
  UI_ACCOUNTS_Handle *accounts = cls;

  const char *name = GNUNET_CHAT_account_get_name(account);

  return _accounts_print_entry(accounts, 'x', name);
}

void
accounts_print(UI_ACCOUNTS_Handle *accounts,
	       MESSENGER_Application *app)
{
  if (accounts->create_dialog.window)
  {
    account_create_dialog_print(&(accounts->create_dialog), app);
    return;
  }

  if (!(accounts->window))
    return;

  accounts->line_index = 0;
  werase(accounts->window);

  GNUNET_CHAT_iterate_accounts(
      app->chat.handle,
      &_accounts_iterate_print,
      accounts
  );

  _accounts_print_entry(accounts, '+', "Add account");
}
