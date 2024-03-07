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
 * @file application.c
 */

#include "application.h"

#include "util.h"

#ifndef MESSENGER_CLI_BINARY
#define MESSENGER_CLI_BINARY "messenger_cli"
#endif

#ifndef MESSENGER_CLI_VERSION
#define MESSENGER_CLI_VERSION "unknown"
#endif

#ifndef MESSENGER_CLI_DESC
#define MESSENGER_CLI_DESC "A CLI for the Messenger service of GNUnet."
#endif

void
application_clear(MESSENGER_Application *app)
{
  app->accounts.window = NULL;
  app->chats.window = NULL;
  app->current.members.window = NULL;
  app->current.messages.window = NULL;
}

void
application_init(MESSENGER_Application *app,
                 int argc,
                 char **argv)
{
  const struct GNUNET_GETOPT_CommandLineOption options [] = {
    GNUNET_GETOPT_option_version(MESSENGER_CLI_VERSION),
    GNUNET_GETOPT_option_help(MESSENGER_CLI_DESC),
    GNUNET_GETOPT_OPTION_END
  };

  memset(app, 0, sizeof(*app));

  app->argc = argc;
  app->argv = argv;

  const int parsing = GNUNET_GETOPT_run(
    MESSENGER_CLI_BINARY,
    options,
    app->argc,
    app->argv
  );

  if (parsing <= 0)
  {
    app->window = NULL;
    app->status = GNUNET_SYSERR == parsing? GNUNET_SYSERR : GNUNET_OK;
    return;
  }

  app->window = initscr();

  if (!(app->window))
  {
    app->status = GNUNET_SYSERR;
    return;
  }

  application_refresh(app);

  noecho();

  keypad(app->window, TRUE);
  wtimeout(app->window, 10);
}

void
application_refresh(MESSENGER_Application *app)
{
  if (app->ui.logo) delwin(app->ui.logo);
  if (app->ui.main) delwin(app->ui.main);
  if (app->ui.left) delwin(app->ui.left);
  if (app->ui.right) delwin(app->ui.right);
  if (app->ui.input) delwin(app->ui.input);

  memset(&(app->ui), 0, sizeof(app->ui));

  curs_set(0);
}

static void
run (void *cls,
     UNUSED char* const* args,
     UNUSED const char *cfgfile,
     const struct GNUNET_CONFIGURATION_Handle *cfg)
{
  MESSENGER_Application *app = cls;

  if (!(app->window))
    return;

  chat_start(&(app->chat), app, cfg);
}

void
application_run(MESSENGER_Application *app)
{
  const struct GNUNET_GETOPT_CommandLineOption options [] = {
    GNUNET_GETOPT_OPTION_END
  };

  app->status = GNUNET_PROGRAM_run(
    1,
    app->argv,
    MESSENGER_CLI_BINARY,
    gettext_noop(MESSENGER_CLI_DESC),
    options,
    &run,
    app
  );

  members_clear(&(app->current.members));
  messages_clear(&(app->current.messages));

  application_clear(app);

  if (app->window)
    delwin(app->window);

  if (ERR == endwin())
  {
    app->status = GNUNET_SYSERR;
    return;
  }
}

int
application_status(MESSENGER_Application *app)
{
  if (app->status != GNUNET_OK)
    return EXIT_FAILURE;
  else
    return EXIT_SUCCESS;
}
