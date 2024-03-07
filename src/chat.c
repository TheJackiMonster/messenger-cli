/*
   This file is part of GNUnet.
   Copyright (C) 2022--2024 GNUnet e.V.

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
#include "util.h"
#include <curses.h>
#include <gnunet/gnunet_chat_lib.h>

#ifndef MESSENGER_CLI_BINARY
#define MESSENGER_CLI_BINARY "messenger_cli"
#endif

#ifndef MESSENGER_CLI_VERSION
#define MESSENGER_CLI_VERSION "unknown"
#endif

static void
_chat_refresh(MESSENGER_Application *app)
{
  application_clear(app);
  chat_update_layout(&(app->chat), app);

  accounts_print(&(app->accounts), app);
  chats_print(&(app->chats), app);
  members_print(&(app->current.members));
  messages_print(&(app->current.messages));

  if (!app->ui.logo)
    return;

  werase(app->ui.logo);
  wmove(app->ui.logo, 0, 0);

  util_print_logo(app->ui.logo);

  int x = getcurx(app->ui.logo);
  int y = getcury(app->ui.logo);

  util_print_info(app->ui.logo, MESSENGER_CLI_VERSION);

  wmove(app->ui.logo, --y, x);
  util_print_info(app->ui.logo, MESSENGER_CLI_BINARY);
}

static bool
_chat_event(MESSENGER_Application *app,
	    int key)
{
  if (key < 0)
    goto refresh;

  const struct GNUNET_CHAT_Account *account = GNUNET_CHAT_get_connected(
      app->chat.handle
  );

  if (!account)
    accounts_event(&(app->accounts), app, key);
  else if (app->chat.context)
  {
    if (app->chat.show_members)
      members_event(&(app->current.members), app, key);
    else
      messages_event(&(app->current.messages), app, key);
  }
  else
    chats_event(&(app->chats), app, key);

  if (app->chat.quit)
    return TRUE;

refresh:
  _chat_refresh(app);
  return FALSE;
}

static int
_chat_message(void *cls,
	      struct GNUNET_CHAT_Context *context,
	      const struct GNUNET_CHAT_Message *message)
{
  MESSENGER_Application *app = cls;

  chat_process_message(&(app->chat), context, message);

  _chat_event(app, KEY_RESIZE);
  return GNUNET_YES;
}

static void
_chat_idle(void *cls)
{
  MESSENGER_Application *app = cls;
  app->chat.idle = NULL;

  if (app->chat.quit)
    return;

  if (_chat_event(app, wgetch(app->window)))
  {
    chat_stop(&(app->chat));
    return;
  }

  app->chat.idle = GNUNET_SCHEDULER_add_delayed_with_priority(
      GNUNET_TIME_relative_multiply(
	  GNUNET_TIME_relative_get_millisecond_(),
	  wgetdelay(app->window)
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

  chat->quit = FALSE;
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

  chat->quit = TRUE;
}

void
_chat_update_layout_accounts(struct MESSENGER_Application *app)
{
  int rows, cols;
  getmaxyx(app->window, rows, cols);

  if (rows >= UTIL_LOGO_ROWS + UI_ACCOUNTS_ROWS_MIN)
  {
    const int offset = UTIL_LOGO_ROWS + 1;

    app->ui.logo = subwin(app->window, UTIL_LOGO_ROWS, cols, 0, 0);
    app->ui.main = subwin(app->window, rows - offset, cols, offset, 0);

    wmove(app->window, UTIL_LOGO_ROWS, 0);
    whline(app->window, ACS_HLINE, cols);
  }
  else
    app->ui.main = subwin(app->window, rows, cols, 0, 0);

  app->accounts.window = app->ui.main;
}

void
_chat_update_layout_chats(struct MESSENGER_Application *app)
{
  int rows, cols;
  getmaxyx(app->window, rows, cols);

  int min_rows = UI_CHATS_ROWS_MIN;
  int offset_x = 0;
  int offset_y = 0;

  if (cols >= UI_ACCOUNTS_COLS_MIN + UI_CHATS_COLS_MIN)
  {
    offset_x = UI_ACCOUNTS_COLS_MIN + 1;

    if (UI_ACCOUNTS_ROWS_MIN > min_rows) min_rows = UI_ACCOUNTS_ROWS_MIN;
  }

  if (rows >= UTIL_LOGO_ROWS + min_rows)
  {
    offset_y = UTIL_LOGO_ROWS + 1;

    app->ui.logo = subwin(app->window, UTIL_LOGO_ROWS, cols, 0, 0);

    wmove(app->window, UTIL_LOGO_ROWS, 0);
    whline(app->window, ACS_HLINE, cols);
  }

  if (offset_x > 0)
  {
    app->ui.left = subwin(
      app->window,
      rows - offset_y,
      UI_ACCOUNTS_COLS_MIN,
      offset_y,
      0
    );

    wmove(app->window, offset_y > 0? offset_y : 0, UI_ACCOUNTS_COLS_MIN);
    wvline(app->window, ACS_VLINE, rows - offset_y);

    if (offset_y > 0)
    {
      wmove(app->window, offset_y - 1, UI_ACCOUNTS_COLS_MIN);
      waddch(app->window, ACS_TTEE);
    }
  }

  app->ui.main = subwin(
      app->window,
      rows - offset_y,
      cols - offset_x,
      offset_y,
      offset_x
  );

  app->accounts.window = app->ui.left;
  app->chats.window = app->ui.main;
}

void
_chat_update_layout_messages(struct MESSENGER_Application *app)
{
  int rows, cols;
  getmaxyx(app->window, rows, cols);

  const int cols_min_left = (UTIL_LOGO_COLS > UI_CHATS_COLS_MIN?
      UTIL_LOGO_COLS : UI_CHATS_COLS_MIN
  );

  int offset_x, cut_x;
  cut_x = 0;

  if (cols >= cols_min_left + UI_MESSAGES_COLS_MIN)
    offset_x = cols_min_left + 1;
  else
  {
    offset_x = 0;
    goto skip_left_split;
  }

  if (rows >= UTIL_LOGO_ROWS + UI_CHATS_ROWS_MIN)
  {
    const int offset = UTIL_LOGO_ROWS + 1;

    app->ui.logo = subwin(app->window, UTIL_LOGO_ROWS, cols_min_left, 0, 0);
    app->ui.left = subwin(app->window, rows - offset, cols_min_left, offset, 0);

    wmove(app->window, UTIL_LOGO_ROWS, 0);
    whline(app->window, ACS_HLINE, cols_min_left);
  }
  else
    app->ui.left = subwin(app->window, rows, cols_min_left, 0, 0);

  if (cols >= cols_min_left + UI_MESSAGES_COLS_MIN + UI_MEMBERS_COLS_MIN)
  {
    cut_x = UI_MEMBERS_COLS_MIN + 1;

    app->ui.right = subwin(
	app->window,
	rows,
	UI_MEMBERS_COLS_MIN,
	0,
	cols - UI_MEMBERS_COLS_MIN
    );

    wmove(app->window, 0, cols - cut_x);
    wvline(app->window, ACS_VLINE, rows);
  }

  wmove(app->window, 0, cols_min_left);
  wvline(app->window, ACS_VLINE, rows);

skip_left_split:
  app->ui.main = subwin(
      app->window,
      rows,
      cols - offset_x - cut_x,
      0,
      offset_x
  );

  app->chats.window = app->ui.left;

  if (app->ui.right)
  {
    app->current.members.window = app->ui.right;
    app->current.messages.window = app->ui.main;
    return;
  }

  if (app->chat.show_members)
    app->current.members.window = app->ui.main;
  else
    app->current.messages.window = app->ui.main;
}

void
chat_update_layout(MESSENGER_Chat *chat,
		   struct MESSENGER_Application *app)
{
  const struct GNUNET_CHAT_Account *account = GNUNET_CHAT_get_connected(
      chat->handle
  );

  application_refresh(app);

  if (!account)
    _chat_update_layout_accounts(app);
  else if (app->chat.context)
    _chat_update_layout_messages(app);
  else
    _chat_update_layout_chats(app);
}

void
chat_process_message(UNUSED MESSENGER_Chat *chat,
		     struct GNUNET_CHAT_Context *context,
		     const struct GNUNET_CHAT_Message *message)
{
  enum GNUNET_CHAT_MessageKind kind = GNUNET_CHAT_message_get_kind(message);

  struct GNUNET_CHAT_Contact *sender = GNUNET_CHAT_message_get_sender(message);

  UI_CHAT_Handle *current = (UI_CHAT_Handle*) (
      GNUNET_CHAT_context_get_user_pointer(context)
  );

  if (!current)
    return;

  bool new_member = FALSE;

  if (GNUNET_CHAT_KIND_LEAVE == kind)
    members_remove(&(current->members), sender);
  else if (GNUNET_CHAT_KIND_JOIN == kind)
    new_member = members_add(&(current->members), sender);

  if (GNUNET_CHAT_KIND_DELETION == kind)
    messages_remove(
	&(current->messages),
	GNUNET_CHAT_message_get_target(message)
    );
  else if (GNUNET_YES == GNUNET_CHAT_message_is_deleted(message))
    messages_remove(&(current->messages), message);
  else if ((GNUNET_CHAT_KIND_JOIN != kind) || (new_member))
    messages_add(&(current->messages), message);
}
