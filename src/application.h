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
//#include "ui/accounts.h"

typedef struct MESSENGER_Application
{
  char **argv;
  int argc;

  int status;

  MESSENGER_Chat chat;
} MESSENGER_Application;

void
application_init(MESSENGER_Application *app,
		 int argc,
		 char **argv);

void
application_run(MESSENGER_Application *app);

int
application_status(MESSENGER_Application *app);

#endif /* APPLICATION_H_ */
