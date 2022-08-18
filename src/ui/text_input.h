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
 * @file ui/text_input.h
 */

#ifndef UI_TEXT_INPUT_H_
#define UI_TEXT_INPUT_H_

#include <ctype.h>

#define text_input_event(text, key) {                     \
  switch (key)                                           \
  {                                                      \
    case KEY_LEFT:                                       \
    {                                                    \
      text##_pos--;                                      \
      break;                                             \
    }                                                    \
    case KEY_RIGHT:                                      \
    {                                                    \
      text##_pos++;                                      \
      break;                                             \
    }                                                    \
    case KEY_BACKSPACE:                                  \
    {                                                    \
      if ((text##_pos < text##_len) && (text##_pos > 0)) \
	for (int i = text##_pos; i < text##_len; i++)    \
	  text[i - 1] = text[i];                         \
                                                         \
      if ((text##_pos > 0) && (text##_len > 0))          \
      {                                                  \
	text##_pos--;                                    \
	text##_len--;                                    \
      }                                                  \
                                                         \
      break;                                             \
    }                                                    \
    default:                                             \
    {                                                    \
      if (!isprint(key))                                 \
	break;                                           \
                                                         \
      for (int i = text##_len - 1; i >= text##_pos; i--) \
	text[i + 1] = text[i];                           \
                                                         \
      text[text##_pos++] = (char) key;                   \
      text##_len++;                                      \
      break;                                             \
    }                                                    \
  }                                                      \
                                                         \
  int max_len = (int) sizeof(text) - 1;                  \
							 \
  if (text##_len > max_len)                              \
    text##_len = max_len;                                \
                                                         \
  text[text##_len] = '\0';                               \
                                                         \
  if (text##_pos < 0)                                    \
    text##_pos = 0;                                      \
                                                         \
  if (text##_pos > text##_len)                           \
    text##_pos = text##_len;                             \
}

#endif /* UI_TEXT_INPUT_H_ */
