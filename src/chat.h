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

/**
 * @struct MESSENGER_Chat
 */
typedef struct MESSENGER_Chat
{
  struct GNUNET_CHAT_Handle *handle;
  struct GNUNET_CHAT_Context *context;

  struct GNUNET_SCHEDULER_Task *idle;

  bool show_members;
  bool quit;
} MESSENGER_Chat;

/**
 * Starts the processing of the given applications
 * chat handle.
 *
 * @param[out] chat Application chat handle
 * @param[in,out] app Application handle
 * @param[in] cfg Configuration
 */
void
chat_start(MESSENGER_Chat *chat,
	   struct MESSENGER_Application *app,
	   const struct GNUNET_CONFIGURATION_Handle *cfg);

/**
 * Stops the processing of the given applications
 * chat handle.
 *
 * @param[in,out] chat Application chat handle
 */
void
chat_stop(MESSENGER_Chat *chat);

/**
 * Updates the layout of the applications views depending
 * on the main windows resolution and the current state
 * of the applications chat handle.
 *
 * @param[in] chat Application chat handle
 * @param[out] app Application handle
 */
void
chat_update_layout(MESSENGER_Chat *chat,
		   struct MESSENGER_Application *app);

/**
 * Processes a chat message to update the list of
 * required resources to handle visual representation
 * of current members in a chat or the messages.
 *
 * @param[in,out] chat Application chat handle
 * @param[in] context Chat context of the message
 * @param[in] message Chat message
 */
void
chat_process_message(MESSENGER_Chat *chat,
		     struct GNUNET_CHAT_Context *context,
		     const struct GNUNET_CHAT_Message *message);

#endif /* CHAT_H_ */
