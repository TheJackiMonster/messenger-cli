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
 * @file ui/messages.c
 */

#include "messages.h"

#include "list_input.h"
#include "text_input.h"
#include "../application.h"
#include "../util.h"

struct tm*
_messages_new_day(time_t* current_time,
		              const time_t* timestamp)
{
  struct tm* ts = localtime(timestamp);

  ts->tm_sec = 0;
  ts->tm_min = 0;
  ts->tm_hour = 0;

  const time_t date_time = timelocal(ts);

  if (date_time <= *current_time) {
    return NULL;
  }

  *current_time = date_time;
  return ts;
}

void
_messages_handle_message(UI_MESSAGES_Handle *messages)
{
  switch (GNUNET_CHAT_message_get_kind(messages->selected))
  {
    case GNUNET_CHAT_KIND_INVITATION:
    {
      struct GNUNET_CHAT_Invitation *invitation = (
	      GNUNET_CHAT_message_get_invitation(messages->selected)
      );

      if (invitation)
	      GNUNET_CHAT_invitation_accept(invitation);
      break;
    }
    case GNUNET_CHAT_KIND_FILE:
    {
      struct GNUNET_CHAT_File *file = GNUNET_CHAT_message_get_file(
	      messages->selected
      );

      if ((file) && (GNUNET_YES != GNUNET_CHAT_file_is_downloading(file)))
	      GNUNET_CHAT_file_start_download(file, NULL, NULL);
      break;
    default:
      break;
    }
  }
}

void
messages_event(UI_MESSAGES_Handle *messages,
               MESSENGER_Application *app,
               int key)
{
  list_input_reset(messages);
  messages->line_time = 0;

  UI_MESSAGES_List *element = messages->head;
  while (element)
  {
    struct tm *ts = _messages_new_day(
      &(messages->line_time),
      &(element->timestamp)
    );

    list_input_select(messages, ts? 2 : 1, element->message);
    element = element->next;
  }

  list_input_select(messages, 1, NULL);

  switch (key)
  {
    case 27:
    case KEY_EXIT:
      app->chat.context = NULL;
      break;
    case '\t':
      app->chat.show_members = TRUE;
      break;
    case '\n':
    case KEY_ENTER:
      if (messages->selected)
	      _messages_handle_message(messages);
      else if (messages->text_len > 0)
      {
	      if (0 != strncmp(messages->text,
                      UI_MESSAGES_FILE_PREFIX,
                      UI_MESSAGES_FILE_PREFIX_LEN))
	        goto write_text;

	      const char* filename = messages->text + 5;

        if (0 != access(filename, R_OK | F_OK))
          break;

        GNUNET_CHAT_context_send_file(
          app->chat.context,
          filename,
          NULL,
          NULL
        );

	      goto drop_text;

      write_text:
        GNUNET_CHAT_context_send_text(
          app->chat.context,
          messages->text
        );

      drop_text:
	      messages->text_len = 0;
      }
      break;
    case KEY_BACKSPACE:
      if (messages->selected)
	      GNUNET_CHAT_message_delete(
          messages->selected,
          GNUNET_TIME_relative_get_zero_()
	      );
      break;
    default:
      break;
  }

  if (!(messages->selected))
    text_input_event(messages->text, key);

  list_input_event(messages, key);
}

void
_messages_iterate_print(UI_MESSAGES_Handle *messages,
                        const time_t* timestamp,
                        const struct GNUNET_CHAT_Message *message)
{
  enum GNUNET_CHAT_MessageKind kind = GNUNET_CHAT_message_get_kind(message);

  struct GNUNET_CHAT_Contact *sender = GNUNET_CHAT_message_get_sender(message);

  const char *name = sender? GNUNET_CHAT_contact_get_name(sender) : NULL;
  const char *text = GNUNET_CHAT_message_get_text(message);

  const struct GNUNET_CHAT_File *file = GNUNET_CHAT_message_get_file(message);

  struct tm* ts = localtime(timestamp);
  char time_buf [255];

  strftime(time_buf, sizeof(time_buf), "%H:%M", ts);

  ts = _messages_new_day(&(messages->line_time), timestamp);

  list_input_print(messages, ts? 2 : 1);
  wmove(messages->window, y, 0);

  if (ts) {
    char date_buf [255];

    strftime(date_buf, sizeof(date_buf), "%x", ts);

    const int width = getmaxx(messages->window);

    whline(messages->window, '-', width);
    wmove(messages->window, y, 8);

    wprintw(messages->window, " %s ", date_buf);
    wmove(messages->window, y+1, 0);
  }

  const int attrs_select = A_BOLD;

  if (selected) wattron(messages->window, attrs_select);

  wprintw(messages->window, " %s | ", time_buf);

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
      wprintw(
	      messages->window,
        "%s: %s",
        name,
        text
      );
      break;
    case GNUNET_CHAT_KIND_FILE: {
      const char *filename = GNUNET_CHAT_file_get_name(file);

      const uint64_t localsize = GNUNET_CHAT_file_get_local_size(file);
      const uint64_t filesize = GNUNET_CHAT_file_get_size(file);

      wprintw(
        messages->window,
        "%s shares the file '%s' (%lu / %lu) with you.",
        name,
        filename,
        localsize,
        filesize
      );
      break;
    }
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

  list_input_reset(messages);
  messages->line_time = 0;

  werase(messages->window);

  UI_MESSAGES_List *element = messages->head;
  while (element)
  {
    _messages_iterate_print(messages, &(element->timestamp), element->message);
    element = element->next;
  }

  const int count = messages->line_index;
  const bool selected = (count == messages->line_selected);

  const int width = getmaxx(messages->window);
  const int height = getmaxy(messages->window);
  const int line_height = height - 2;

  wmove(messages->window, line_height, 0);
  whline(messages->window, '-', width);

  const bool is_file_text = (0 == strncmp(
    messages->text,
    UI_MESSAGES_FILE_PREFIX,
    UI_MESSAGES_FILE_PREFIX_LEN
  ));

  const int attrs_select = A_BOLD | (is_file_text? A_ITALIC : A_NORMAL);

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
  if ((!list0) || (!list1))
    return 0;

  if (list0->timestamp > list1->timestamp)
    return -1;
  else if (list0->timestamp < list1->timestamp)
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
    case GNUNET_CHAT_KIND_UPDATE:
    case GNUNET_CHAT_KIND_CONTACT:
    case GNUNET_CHAT_KIND_DELETION:
      return;
    default:
      break;
  }

  list_input_reset(messages);
  messages->line_time = 0;

  UI_MESSAGES_List *element = messages->head;
  while (element)
  {
    struct tm *ts = _messages_new_day(
      &(messages->line_time),
      &(element->timestamp)
    );

    list_input_select(messages, ts? 2 : 1, element->message);
    element = element->next;
  }

  list_input_select(messages, 1, NULL);

  const struct GNUNET_TIME_Absolute abs_time = (
    GNUNET_CHAT_message_get_timestamp(message)
  );

  const struct GNUNET_TIME_Timestamp timestamp = (
    GNUNET_TIME_absolute_to_timestamp(abs_time)
  );

  element = GNUNET_new(UI_MESSAGES_List);
  element->timestamp = GNUNET_TIME_timestamp_to_s(timestamp);
  element->message = message;

  GNUNET_CONTAINER_DLL_insert_sorted(
    UI_MESSAGES_List,
    _message_compare_timestamps,
    NULL,
    messages->head,
    messages->tail,
    element
  );

  list_input_select(messages, 1, NULL);

  if (!(messages->selected))
    list_input_event(messages, KEY_DOWN);
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
