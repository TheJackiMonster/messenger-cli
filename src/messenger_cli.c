/*
   This file is part of GNUnet.
   Copyright (C) 2021--2022 GNUnet e.V.

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

#include <stdlib.h>
#include <curses.h>

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
	NULL, NULL
  );

  initscr();
  noecho();

  int bx, by, mx, my;
  getbegyx(stdscr, by, bx);
  getmaxyx(stdscr, my, mx);

  WINDOW *win = newwin(15, 30, by + (my - by - 15) / 2, bx + (mx - bx - 30) / 2);
  keypad(win, TRUE);

  int selected = 0;

  int c;
  do {
	getbegyx(stdscr, by, bx);
  	getmaxyx(stdscr, my, mx);

	int x = bx + (mx - bx - 30) / 2;
    int y = by + (my - by - 15) / 2;

	int w = 30;
	int h = 15;

	if (mx - x < w)
	  w = (mx - x > 0? mx - x : 0);

	if (my - y < h)
	  h = (my - y > 0? my - y : 0);

	if (w * h > 0)
	{
	  mvwin(win, y, x);
	  wresize(win, h, w);
    
	  werase(win);
	  box(win, 0, 0);

      wmove(win, 1, 1);
      wprintw(win, "%d %d, %d %d | %d %d", bx, by, mx, my, c, KEY_DOWN);

	  const int attrs_select = A_BOLD;

	  for (int i = 0; i < 5; i++) {
        if (i == selected) wattron(win, attrs_select);
		
		wmove(win, i+2, 1);
		wprintw(win, "Option %d", i+1);

		if (i == selected) wattroff(win, attrs_select);
	  }

	  wmove(win, 7, 1);
	  c = wgetch(win);
	}
	else
	{
	  c = getch();
	}

	if (KEY_UP == c) selected = (selected > 0? selected - 1 : 0);
	else if (KEY_DOWN == c) selected = (selected < 4? selected + 1 : 4);

	clear();
	refresh();
  } while (c != 'q');

  delwin(win);

  endwin();

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
