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

  application_clear(app);

  if (!account)
    app->accounts.window = app->window;
  else if (app->chat.context)
  {
    if (app->chat.show_members)
      app->current.members.window = app->window;
    else
      app->current.messages.window = app->window;
  }
  else
    app->chats.window = app->window;

  accounts_print(&(app->accounts), app);
  chats_print(&(app->chats), app);
  members_print(&(app->current.members));
  messages_print(&(app->current.messages));
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
  else if ((GNUNET_CHAT_KIND_JOIN != kind) || (new_member))
    messages_add(&(current->messages), message);
}
