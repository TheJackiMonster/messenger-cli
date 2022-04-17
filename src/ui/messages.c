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
 * @file ui/messages.c
 */

#include "messages.h"

#include "../application.h"

void
messages_event(UI_MESSAGES_Handle *messages,
               struct MESSENGER_Application *app,
	       int key)
{
  // TODO
}

int
_messages_iterate_print(void *cls,
			struct GNUNET_CHAT_Context *context,
			const struct GNUNET_CHAT_Message *message)
{
  UI_MESSAGES_Handle *messages = cls;

  const int y = messages->line_index++;

  enum GNUNET_CHAT_MessageKind kind = GNUNET_CHAT_message_get_kind(message);
  const char *text = GNUNET_CHAT_message_get_text(message);

  struct GNUNET_TIME_Absolute timestamp = GNUNET_CHAT_message_get_timestamp(
      message
  );

  wmove(messages->window, y, 0);
  wprintw(
      messages->window,
      "%s | [%d]: %s",
      GNUNET_TIME_absolute2s(timestamp),
      (int) kind,
      text
  );

  return GNUNET_YES;
}

void
messages_print(UI_MESSAGES_Handle *messages,
               struct MESSENGER_Application *app)
{
  if (!(messages->window))
    return;

  struct GNUNET_CHAT_Context *context = app->chat.context;

  messages->line_index = 0;
  GNUNET_CHAT_context_iterate_messages(
      context,
      _messages_iterate_print,
      messages
  );
}
