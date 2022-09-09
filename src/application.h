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
 * @file application.h
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdlib.h>
#include <curses.h>

#include "chat.h"
#include "util.h"

#include "ui/accounts.h"
#include "ui/chat.h"
#include "ui/chats.h"

/**
 * @struct MESSENGER_Application
 */
typedef struct MESSENGER_Application
{
  char **argv;
  int argc;

  int status;
  WINDOW *window;

  struct {
    WINDOW *logo;
    WINDOW *main;
    WINDOW *left;
    WINDOW *right;
    WINDOW *input;
  } ui;

  MESSENGER_Chat chat;

  UI_ACCOUNTS_Handle accounts;
  UI_CHATS_Handle chats;
  UI_CHAT_Handle current;
} MESSENGER_Application;

/**
 * Clears the application handle to reset all views
 * which might have been in use.
 *
 * @param[out] app Application handle
 */
void
application_clear(MESSENGER_Application *app);

/**
 * Initializes the application handle with the program
 * arguments provided from the main function.
 *
 * @param[out] app Application handle
 * @param[in] argc Argument count
 * @param[in] argv Argument array
 */
void
application_init(MESSENGER_Application *app,
		 int argc,
		 char **argv);

/**
 * Refreshes the application handle freeing all temporary
 * WINDOW handles to manage the different views which
 * were used.
 *
 * @param[out] app Application handle
 */
void
application_refresh(MESSENGER_Application *app);

/**
 * Starts the main loop of the application which will
 * be processed via GNUnet and its callback management.
 *
 * @param[in,out] app Application handle
 */
void
application_run(MESSENGER_Application *app);

/**
 * Returns the status code by the given application
 * at current state.
 *
 * @param[in] app Application handle
 * @return #EXIT_FAILURE on failure, otherwise #EXIT_SUCCESS
 */
int
application_status(MESSENGER_Application *app);

#endif /* APPLICATION_H_ */
