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
 * @file ui/list_input.h
 */

#ifndef UI_LIST_INPUT_H_
#define UI_LIST_INPUT_H_

#include <stdbool.h>
#include <stdlib.h>

#define list_input_reset(list) { \
  (list)->line_prev = 0;         \
  (list)->line_next = 0;         \
  (list)->line_index = 0;        \
  (list)->selected = 0;          \
}

#define list_input_select(list, line_width, item) {                \
  const bool selected = (                                          \
      ((list)->line_selected >= (list)->line_index) &&             \
      ((list)->line_selected < (list)->line_index + (line_width))  \
  );                                                               \
                                                                   \
  if ((!selected) && ((list)->line_selected > (list)->line_index)) \
    (list)->line_prev = (list)->line_index;                        \
                                                                   \
  (list)->line_index += (line_width);                              \
                                                                   \
  if (selected) {                                                  \
    (list)->line_next = (list)->line_index;                        \
    (list)->selected = item;                                       \
  }                                                                \
}

#define list_input_event(list, key) {                          \
  int count = (list)->line_index;                              \
                                                               \
  switch (key)                                                 \
  {                                                            \
    case KEY_UP:                                               \
    {                                                          \
      (list)->line_selected = (list)->line_prev;               \
      break;                                                   \
    }                                                          \
    case KEY_DOWN:                                             \
    {                                                          \
      (list)->line_selected = (list)->line_next;               \
      break;                                                   \
    }                                                          \
    default:                                                   \
      break;                                                   \
  }                                                            \
                                                               \
  if ((list)->line_selected < 0)                               \
    (list)->line_selected = 0;                                 \
  else if ((list)->line_selected >= count)                     \
    (list)->line_selected = count - 1;                         \
                                                               \
  if ((list)->window)                                          \
  {                                                            \
    const int height = getmaxy((list)->window);                \
    const int y = (list)->line_selected - (list)->line_offset; \
                                                               \
    if (y < 0)                                                 \
      (list)->line_offset += y;                                \
    else if (y + 1 >= height)                                  \
      (list)->line_offset += y + 1 - height;                   \
                                                               \
    if ((list)->line_offset < 0)                               \
      (list)->line_offset = 0;                                 \
    else if ((list)->line_offset >= count)                     \
      (list)->line_offset = count - 1;                         \
  }                                                            \
}

#define list_input_print_(list, line_width, yes_res, no_res)      \
  const bool selected = (                                         \
      ((list)->line_selected >= (list)->line_index) &&            \
      ((list)->line_selected < (list)->line_index + (line_width)) \
  );                                                              \
                                                                  \
  const int y = (list)->line_index - (list)->line_offset; {       \
                                                                  \
  (list)->line_index += (line_width);                             \
                                                                  \
  if (y + (line_width) < 1)                                       \
    return yes_res;                                               \
                                                                  \
  const int height = getmaxy((list)->window);                     \
                                                                  \
  if (y >= height)                                                \
    return no_res;                                                \
}

#define list_input_print_gnunet(list, line_width) \
    list_input_print_(list, line_width, GNUNET_YES, GNUNET_NO)

#define list_input_print(list, line_width) \
    list_input_print_(list, line_width, , )

#endif /* UI_LIST_INPUT_H_ */
