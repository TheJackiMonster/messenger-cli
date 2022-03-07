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

int
_accounts_iterate(void *cls,
                  UNUSED const struct GNUNET_CHAT_Handle *handle,
                  struct GNUNET_CHAT_Account *account)
{
  UI_ACCOUNTS_Handle *accounts = cls;

  if (accounts->line_index++ == accounts->line_selected)
    accounts->selected = account;

  return GNUNET_NO;
}

void
accounts_event(UI_ACCOUNTS_Handle *accounts,
               struct MESSENGER_Application *app,
	       int key)
{
  accounts->line_index = 0;
  accounts->selected = NULL;

  int count = GNUNET_CHAT_iterate_accounts(
    app->chat.handle,
    &_accounts_iterate,
    accounts
  );

  switch (key)
  {
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
    case KEY_ENTER:
    {
      if (accounts->selected)
	GNUNET_CHAT_connect(app->chat.handle, accounts->selected);

      break;
    }
    default:
      if (accounts->line_selected < 0)
	accounts->line_selected = 0;
      else if (accounts->line_selected >= count)
	accounts->line_selected = count - 1;
      break;
  }
}

int
_accounts_iterate_print(void *cls,
                        UNUSED const struct GNUNET_CHAT_Handle *handle,
                        struct GNUNET_CHAT_Account *account)
{
  UI_ACCOUNTS_Handle *accounts = cls;

  const int y = accounts->line_index - accounts->line_offset;

  if (accounts->line_index++ < accounts->line_offset)
    return GNUNET_YES;

  const int height = getmaxy(accounts->window) - getbegy(accounts->window);

  if (y >= height)
    return GNUNET_NO;

  const char *name = GNUNET_CHAT_account_get_name(account);

  move(y, 0);
  wprintw(accounts->window, "%s", name);
  return GNUNET_YES;
}

void
accounts_print(UI_ACCOUNTS_Handle *accounts,
               struct MESSENGER_Application *app)
{
  accounts->line_index = 0;

  GNUNET_CHAT_iterate_accounts(
      app->chat.handle,
      &_accounts_iterate_print,
      accounts
  );
}
