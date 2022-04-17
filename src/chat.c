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

static void
_chat_refresh(MESSENGER_Application *app)
{
  const struct GNUNET_CHAT_Account *account = GNUNET_CHAT_get_connected(
      app->chat.handle
  );

  app->accounts.window = NULL;
  app->messages.window = NULL;

  if (!account)
    app->accounts.window = stdscr;
  else if (app->chat.context)
    app->messages.window = stdscr;

  accounts_print(&(app->accounts), app);
  messages_print(&(app->messages), app);
}

static int
_chat_event(MESSENGER_Application *app,
	    int key)
{
  if (key < 0)
    goto refresh;

  const struct GNUNET_CHAT_Account *account = GNUNET_CHAT_get_connected(
      app->chat.handle
  );

  if ('q' == key)
    return 1;

  if (!account)
    accounts_event(&(app->accounts), app, key);
  else if (app->chat.context)
    messages_event(&(app->messages), app, key);
  else
  {
    struct GNUNET_CHAT_Group *test = GNUNET_CHAT_group_create(
	app->chat.handle,
	"test"
    );

    app->chat.context = GNUNET_CHAT_group_get_context(test);
  }

refresh:
  _chat_refresh(app);
  return 0;
}

int lc = 0;

static int
_chat_message(void *cls,
	      UNUSED struct GNUNET_CHAT_Context *context,
	      UNUSED const struct GNUNET_CHAT_Message *message)
{
  MESSENGER_Application *app = cls;
  _chat_event(app, KEY_RESIZE);
  return GNUNET_YES;
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
      &_chat_message,
      app
  );

  chat->context = NULL;

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

  GNUNET_CHAT_stop(chat->handle);
  chat->handle = NULL;
}
