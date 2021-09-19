/*
   This file is part of GNUnet.
   Copyright (C) 2021 GNUnet e.V.

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
 * @file messenger_cli.c
 */

#include <gnunet/platform.h>
#include <gnunet/gnunet_chat_lib.h>
#include <gnunet/gnunet_util_lib.h>

static void
run (void *cls, char* const* args,
     const char *cfgfile,
	 const struct GNUNET_CONFIGURATION_Handle *cfg)
{
  struct GNUNET_CHAT_Handle *handle = GNUNET_CHAT_start(
    cfg,
	"appdir",
	"username",
	NULL, NULL,
	NULL, NULL
  );

  //

  GNUNET_CHAT_stop(handle);
}

int
main (int argc, char* const* argv)
{
  struct GNUNET_GETOPT_CommandLineOption options[] = {
    GNUNET_GETOPT_OPTION_END
  };

  int result = GNUNET_PROGRAM_run(
    argc, argv,
	"messenger_cli",
	gettext_noop("A CLI for the Messenger service of GNUnet."),
	options,
	&run,
	NULL
  );

  if (result != GNUNET_OK)
    return EXIT_FAILURE;
  else
    return EXIT_SUCCESS;
}
