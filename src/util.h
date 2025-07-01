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
 * @file util.h
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdbool.h>
#include <stdlib.h>
#include <curses.h>

#define UNUSED __attribute__((unused))

#define UTIL_LOGO_ROWS 10
#define UTIL_LOGO_COLS 28

#define UTIL_UNIQUE_COLORS 6

/**
 * Prints the main logo of the application
 * onto a specified view.
 *
 * @param[in,out] window Window view
 */
void
util_print_logo(WINDOW *window);

/**
 * Print information on the right side of
 * the application besides the main logo.
 *
 * @param[in,out] window Window view
 * @param[in] info Information
 */
void
util_print_info(WINDOW *window,
                const char *info);

/**
 * Print a prompt with properly cut text
 * into a window view of the application.
 *
 * @param[in,out] window Window view
 * @param[in] prompt Prompt text
 */
void
util_print_prompt(WINDOW *window,
                  const char *prompt);

/**
 * Initializes the unique color attributes
 * for using inside window views.
 */
void
util_init_unique_colors(void);

/**
 * Enables a color attribute representing
 * a unique color for some specific data
 * pointer.
 *
 * @param[in,out] window Window view
 * @param[in] data Data pointer
 */
void
util_enable_unique_color(WINDOW *window,
                         const void *data);

/**
 * Disables a color attribute representing
 * a unique color for some specific data
 * pointer.
 *
 * @param[in,out] window Window view
 * @param[in] data Data pointer
 */
void
util_disable_unique_color(WINDOW *window,
                          const void *data);

#endif /* UTIL_H_ */
