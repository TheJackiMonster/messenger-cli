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
 * @file application.c
 */

#include "application.h"

void
application_init(MESSENGER_Application *app,
		 int argc,
		 char **argv)
{
  memset(app, 0, sizeof(*app));

  app->argc = argc;
  app->argv = argv;

  app->window = initscr();

  if (!(app->window))
  {
    app->status = GNUNET_SYSERR;
    return;
  }

  noecho();

  keypad(app->window, TRUE);
  timeout(100);
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
  struct GNUNET_GETOPT_CommandLineOption options[] = {
      GNUNET_GETOPT_OPTION_END
  };

  app->status = GNUNET_PROGRAM_run(
      app->argc,
      app->argv,
      "messenger_cli",
      gettext_noop("A CLI for the Messenger service of GNUnet."),
      options,
      &run,
      app
  );

  messages_clear(&(app->messages));

  if (app->window)
    delwin(app->window);

  endwin();
}

int
application_status(MESSENGER_Application *app)
{
  if (app->status != GNUNET_OK)
    return EXIT_FAILURE;
  else
    return EXIT_SUCCESS;
}
