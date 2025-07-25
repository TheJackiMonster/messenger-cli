/*
   This file is part of GNUnet.
   Copyright (C) 2022--2025 GNUnet e.V.

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

void
util_print_prompt(WINDOW *window,
                  const char *prompt)
{
  const int x = getcurx(window);
  const int y = getcury(window);
  int remaining = getmaxx(window) - x;

  if (strlen(prompt) < remaining)
    wprintw(window, "%s", prompt);
  else
  {
    for (int i = 0; i < remaining; i++)
    {
      wmove(window, y, x + i);
      wprintw(window, "%c", prompt[i]);
    }
  }
}

void
util_init_unique_colors()
{
  start_color();

  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
}

void
util_enable_unique_color(WINDOW *window,
                         const void *data)
{
  if ((FALSE == has_colors()) || (!data))
    return;

  const int attr_color = COLOR_PAIR(1 + ((size_t) data) % UTIL_UNIQUE_COLORS);
  wattron(window, attr_color);
}

void
util_disable_unique_color(WINDOW *window,
                          const void *data)
{
  if ((FALSE == has_colors()) || (!data))
    return;

  const int attr_color = COLOR_PAIR(1 + ((size_t) data) % UTIL_UNIQUE_COLORS);
  wattroff(window, attr_color);
}
