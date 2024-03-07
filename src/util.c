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
 * @file util.c
 */

#include "util.h"
#include <string.h>

void
util_print_logo(WINDOW *window)
{
  int x = getcurx(window);
  int y = getcury(window);

  wmove(window, y++, x); wprintw(window, "                            ");
  wmove(window, y++, x); wprintw(window, "    o/                 \\o   ");
  wmove(window, y++, x); wprintw(window, "  ooo                   oo  ");
  wmove(window, y++, x); wprintw(window, "    \\oooo\\        /oooo/    ");
  wmove(window, y++, x); wprintw(window, "         oo      ooo        ");
  wmove(window, y++, x); wprintw(window, "          oo    ooo         ");
  wmove(window, y++, x); wprintw(window, "           ooooooo          ");
  wmove(window, y++, x); wprintw(window, "           \\oooo/           ");
  wmove(window, y++, x); wprintw(window, "            oooo            ");
  wmove(window, y++, x); wprintw(window, "                            ");
}

void
util_print_info(WINDOW *window,
                const char *info)
{
  const int x = getmaxx(window) - strlen(info) - 1;
  const int y = getcury(window);

  if ((x + (y - 2) / 2 < UTIL_LOGO_COLS - 7) ||
      ((y < 4) && (x < UTIL_LOGO_COLS)))
    return;

  wmove(window, y, x);
  wprintw(window, "%s", info);
}
