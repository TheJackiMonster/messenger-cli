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
 * @file ui/messages.c
 */

#include "messages.h"

#include "../application.h"
#include "../util.h"

void
_messages_iterate(UI_MESSAGES_Handle *messages,
		  const struct GNUNET_CHAT_Message *message)
{
  const bool selected = (messages->line_selected == messages->line_index);

  messages->line_index++;

  if (selected)
    messages->selected = message;
}

void
messages_event(UI_MESSAGES_Handle *messages,
	       MESSENGER_Application *app,
	       int key)
{
  messages->line_index = 0;
  messages->selected = NULL;

  int count = 1;

  UI_MESSAGES_List *element = messages->head;
  while (element)
  {
    _messages_iterate(messages, element->message);
    count++;

    element = element->next;
  }

  switch (key)
  {
    case 27:
    case KEY_EXIT:
    {
      app->chat.context = NULL;
      break;
    }
    case KEY_LEFT:
    {
      messages->text_pos--;
      break;
    }
    case KEY_RIGHT:
    {
      messages->text_pos++;
      break;
    }
    case KEY_UP:
    {
      messages->line_selected--;
      break;
    }
    case KEY_DOWN:
    {
      messages->line_selected++;
      break;
    }
    case '\t':
    {
      app->chat.show_members = TRUE;
      break;
    }
    case '\n':
    case KEY_ENTER:
    {
      if ((!(messages->selected)) && (messages->text_len > 0))
      {
	GNUNET_CHAT_context_send_text(app->chat.context, messages->text);
	messages->text_len = 0;
      }
      break;
    }
    case KEY_BACKSPACE:
    {
      if (messages->selected)
      {
	GNUNET_CHAT_message_delete(
	    messages->selected,
	    GNUNET_TIME_relative_get_zero_()
	);
	break;
      }

      if ((messages->text_pos < messages->text_len) &&
	  (messages->text_pos > 0))
	for (int i = messages->text_pos; i < messages->text_len; i++)
	  messages->text[i - 1] = messages->text[i];

      if ((messages->text_pos > 0) && (messages->text_len > 0))
      {
	messages->text_pos--;
	messages->text_len--;
      }

      break;
    }
    default:
    {
      if ((messages->selected) || (!isprint(key)))
	break;

      for (int i = messages->text_len - 1; i >= messages->text_pos; i--)
	messages->text[i + 1] = messages->text[i];

      messages->text[messages->text_pos++] = (char) key;
      messages->text_len++;
      break;
    }
  }

  if (messages->text_len >= TEXT_LEN_MAX)
    messages->text_len = TEXT_LEN_MAX - 1;

  messages->text[messages->text_len] = '\0';

  if (messages->text_pos < 0)
    messages->text_pos = 0;

  if (messages->text_pos > messages->text_len)
    messages->text_pos = messages->text_len;

  if (messages->line_selected < 0)
    messages->line_selected = 0;
  else if (messages->line_selected >= count)
    messages->line_selected = count - 1;

  if (!(messages->window))
    return;

  const int height = getmaxy(messages->window);
  const int y = messages->line_selected - messages->line_offset;

  const int line_height = height - 2;

  if (y < 0)
    messages->line_offset += y;
  else if (y + 1 >= line_height)
    messages->line_offset += y + 1 - line_height;

  if (messages->line_offset < 0)
    messages->line_offset = 0;
  else if (messages->line_offset >= count)
    messages->line_offset = count - 1;
}

void
_messages_iterate_print(UI_MESSAGES_Handle *messages,
			const struct GNUNET_CHAT_Message *message)
{
  enum GNUNET_CHAT_MessageKind kind = GNUNET_CHAT_message_get_kind(message);

  const bool selected = (messages->line_selected == messages->line_index);
  const int y = messages->line_index - messages->line_offset;

  messages->line_index++;

  if (y < 0)
    return;

  const int height = getmaxy(messages->window);
  const int line_height = height - 2;

  if (y >= line_height)
    return;

  struct GNUNET_CHAT_Contact *sender = GNUNET_CHAT_message_get_sender(message);

  const char *name = sender? GNUNET_CHAT_contact_get_name(sender) : NULL;
  const char *text = GNUNET_CHAT_message_get_text(message);

  struct GNUNET_TIME_Absolute abs_time = GNUNET_CHAT_message_get_timestamp(
      message
  );

  struct GNUNET_TIME_Timestamp timestamp = GNUNET_TIME_absolute_to_timestamp(
      abs_time
  );

  const time_t s_after_epoch = (
      GNUNET_TIME_timestamp_to_s(timestamp)
  );

  struct tm* ts = localtime(&s_after_epoch);
  char time_buf [255];

  strftime(time_buf, sizeof(time_buf), "%H:%M", ts);

  const int attrs_select = A_BOLD;

  if (selected) wattron(messages->window, attrs_select);

  wmove(messages->window, y, 0);
  wprintw(messages->window, "%s | ", time_buf);

  switch (kind) {
    case GNUNET_CHAT_KIND_JOIN:
      wprintw(
      	  messages->window,
      	  "%s joins the room.",
      	  name
      );
      break;
    case GNUNET_CHAT_KIND_LEAVE:
      wprintw(
	    messages->window,
	    "%s leaves the room.",
	    name
      );
      break;
    case GNUNET_CHAT_KIND_INVITATION:
      wprintw(
	    messages->window,
	    "%s invites you to a room.",
	    name
      );
      break;
    case GNUNET_CHAT_KIND_TEXT:
    case GNUNET_CHAT_KIND_FILE:
      wprintw(
	  messages->window,
	  "%s: %s",
	  name,
	  text
      );
      break;
    default:
      wprintw(
	  messages->window,
	  "[%d] %s: %s",
	  (int) kind,
	  name,
	  text
      );
      break;
  }


  if (selected) wattroff(messages->window, attrs_select);
}

void
messages_print(UI_MESSAGES_Handle *messages)
{
  if (!(messages->window))
    return;

  messages->line_index = 0;
  werase(messages->window);

  int count = 0;

  UI_MESSAGES_List *element = messages->head;
  while (element)
  {
    _messages_iterate_print(messages, element->message);
    count++;

    element = element->next;
  }

  const bool selected = (count == messages->line_selected);

  const int width = getmaxx(messages->window);
  const int height = getmaxy(messages->window);
  const int line_height = height - 2;

  wmove(messages->window, line_height, 0);
  whline(messages->window, '-', width);

  const int attrs_select = A_BOLD;

  if (selected) wattron(messages->window, attrs_select);

  wmove(messages->window, height - 1, 0);
  wprintw(messages->window, "%s", messages->text);

  if (selected) wattroff(messages->window, attrs_select);

  wmove(messages->window, height - 1, messages->text_pos);

  if (selected)
  {
    wcursyncup(messages->window);
    curs_set(1);
  }
}

void
messages_clear(UI_MESSAGES_Handle *messages)
{
  UI_MESSAGES_List *element;
  while (messages->head)
  {
    element = messages->head;

    GNUNET_CONTAINER_DLL_remove(
	messages->head,
	messages->tail,
	element
    );

    GNUNET_free(element);
  }
}

static int
_message_compare_timestamps(UNUSED void *cls,
			    UI_MESSAGES_List *list0,
			    UI_MESSAGES_List *list1)
{
  struct GNUNET_TIME_Absolute time0, time1;

  if ((!list0) || (!list1))
    return 0;

  time0 = GNUNET_CHAT_message_get_timestamp(list0->message);
  time1 = GNUNET_CHAT_message_get_timestamp(list1->message);

  if (GNUNET_TIME_absolute_cmp(time0, >, time1))
    return -1;
  else if (GNUNET_TIME_absolute_cmp(time0, <, time1))
    return +1;
  else
    return 0;
}

void
messages_add(UI_MESSAGES_Handle *messages,
	     const struct GNUNET_CHAT_Message *message)
{
  enum GNUNET_CHAT_MessageKind kind = GNUNET_CHAT_message_get_kind(message);

  switch (kind) {
    case GNUNET_CHAT_KIND_CONTACT:
    case GNUNET_CHAT_KIND_WHISPER:
    case GNUNET_CHAT_KIND_DELETION:
      return;
    default:
      break;
  }

  const int height = getmaxy(messages->window);
  const int line_height = height - 2;

  int count = 0;

  UI_MESSAGES_List *element = messages->head;
  while (element)
  {
    count++;
    element = element->next;
  }

  element = GNUNET_new(UI_MESSAGES_List);
  element->message = message;

  GNUNET_CONTAINER_DLL_insert_sorted(
    UI_MESSAGES_List,
    _message_compare_timestamps,
    NULL,
    messages->head,
    messages->tail,
    element
  );

  if (messages->line_selected >= count)
    messages->line_selected = count + 1;

  if ((line_height > 0) && (messages->line_offset + line_height >= count))
    messages->line_offset = count + 1 - line_height;
}

void
messages_remove(UI_MESSAGES_Handle *messages,
	        const struct GNUNET_CHAT_Message *message)
{
  UI_MESSAGES_List *element = messages->head;
  while (element)
  {
    if (element->message == message)
      break;

    element = element->next;
  }

  if (element)
    GNUNET_CONTAINER_DLL_remove(
	messages->head,
	messages->tail,
	element
    );
}
