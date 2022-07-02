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
 * @file chat.h
 */

#ifndef CHAT_H_
#define CHAT_H_

#include <gnunet/platform.h>
#include <gnunet/gnunet_chat_lib.h>
#include <gnunet/gnunet_util_lib.h>

struct MESSENGER_Application;

typedef struct MESSENGER_Chat
{
  struct GNUNET_CHAT_Handle *handle;
  struct GNUNET_CHAT_Context *context;

  struct GNUNET_SCHEDULER_Task *idle;

  bool show_members;
  bool quit;
} MESSENGER_Chat;

void
chat_start(MESSENGER_Chat *chat,
	   struct MESSENGER_Application *app,
	   const struct GNUNET_CONFIGURATION_Handle *cfg);

void
chat_stop(MESSENGER_Chat *chat);

void
chat_process_message(MESSENGER_Chat *chat,
		     struct GNUNET_CHAT_Context *context,
		     const struct GNUNET_CHAT_Message *message);

#endif /* CHAT_H_ */
