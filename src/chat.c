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
 * @file chat.c
 */

#include "chat.h"

#include "application.h"
#include "ui/accounts.h"

UI_ACCOUNTS_Handle accounts;

int lc = 0;

static int
_chat_message(UNUSED void *cls,
	      UNUSED struct GNUNET_CHAT_Context *context,
	      UNUSED const struct GNUNET_CHAT_Message *message)
{
  // MESSENGER_Application *app = cls;

  enum GNUNET_CHAT_MessageKind kind = GNUNET_CHAT_message_get_kind(
      message
  );

  move(lc++, 50);
  printw("TEST %d", (int) kind);

  return GNUNET_YES;
}

static void
_chat_refresh(MESSENGER_Application *app)
{
  // TODO

  accounts.window = stdscr;
  accounts_print(&accounts, app);
}

static int
_chat_event(MESSENGER_Application *app,
	    int key)
{
  if (key < 0)
    goto refresh;

  if ('q' == key)
    return 1;

  move(lc++, 0);
  printw("KEY %d", key);

  accounts_event(&accounts, app, key);

refresh:
  _chat_refresh(app);
  return 0;
}

static void
_chat_idle(void *cls)
{
  MESSENGER_Application *app = cls;
  app->chat.idle = NULL;

  if (0 != _chat_event(app, getch()))
  {
    chat_stop(&(app->chat));
    return;
  }

  app->chat.idle = GNUNET_SCHEDULER_add_delayed_with_priority(
      GNUNET_TIME_relative_multiply(
	  GNUNET_TIME_relative_get_millisecond_(),
	  wgetdelay(stdscr)
      ),
      GNUNET_SCHEDULER_PRIORITY_IDLE,
      &_chat_idle,
      app
  );
}

void
chat_start(MESSENGER_Chat *chat,
	   struct MESSENGER_Application *app,
	   const struct GNUNET_CONFIGURATION_Handle *cfg)
{
  chat->handle = GNUNET_CHAT_start(
      cfg,
      "appdir",
      &_chat_message,
      app
  );

  chat->idle = GNUNET_SCHEDULER_add_now(
      &_chat_idle,
      app
  );
}

void
chat_stop(MESSENGER_Chat *chat)
{
  if (chat->idle)
  {
    GNUNET_SCHEDULER_cancel(chat->idle);
    chat->idle = NULL;
  }

  if (chat->key)
  {
    GNUNET_SCHEDULER_cancel(chat->key);
    chat->key = NULL;
  }

  if (chat->fdset)
  {
    GNUNET_NETWORK_fdset_destroy(chat->fdset);
    chat->fdset = NULL;
  }

  GNUNET_CHAT_stop(chat->handle);
  chat->handle = NULL;
}
