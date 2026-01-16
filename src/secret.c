/*
   This file is part of GNUnet.
   Copyright (C) 2026 GNUnet e.V.

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
 * @file secret.c
 */

#include "secret.h"

#include <gnunet/gnunet_util_lib.h>
#include <string.h>

#define SECRET_APP_ID "org.gnunet.Messenger"

const SecretSchema *
_secret_schema(void)
{
  static const SecretSchema schema = {
    "org.gnunet.chat.AccountSecret", SECRET_SCHEMA_NONE,
    {
      { "name", SECRET_SCHEMA_ATTRIBUTE_STRING },
      { "app_id", SECRET_SCHEMA_ATTRIBUTE_STRING },
      { "NULL", 0 },
    }
  };
  return &schema;
}

char*
_secret_description(const char *name)
{
  char *desc;

  GNUNET_asprintf(
    &desc,
    "GNUnet Messenger account secret for identity %s",
    name
  );

  return desc;
}

char*
secret_lookup(const char *name,
              uint32_t *secret_len)
{
  GError *error = NULL;
  gchar *password;
  
  password = secret_password_lookup_sync(
    _secret_schema(),
    NULL,
    &error,
    "name", name,
    "app_id", SECRET_APP_ID,
    NULL
  );

  if (error)
  {
    return NULL;
  }
  else if (password)
  {
    *secret_len = g_utf8_strlen(password, -1);
    return password;
  }
  else
  {
    *secret_len = 0;
    return NULL;
  }
}

bool
secret_store(const char *name,
             const char *secret,
             uint32_t secret_len)
{
  GError *error = NULL;
  gboolean result;

  if (strlen(secret) != secret_len)
    return false;

  result = secret_password_store_sync (
    _secret_schema(),
    SECRET_COLLECTION_DEFAULT,
    _secret_description(name),
    secret,
    NULL,
    &error,
    "name", name,
    "app_id", SECRET_APP_ID,
    NULL
  );

  if (error)
    return false;
  else
    return result;
}

bool
secret_delete(const char *name)
{
  GError *error = NULL;
  gboolean result;

  result = secret_password_clear_sync(
    _secret_schema(),
    NULL,
    &error,
    "name", name,
    "app_id", SECRET_APP_ID,
    NULL
  );

  if (error)
    return false;
  else
    return result;
}

void
secret_wipe(char *secret)
{
  gchar *password = secret;

  if (password)
    secret_password_wipe(password);
}

void
secret_free(char *secret)
{
  gchar *password = secret;

  if (password)
    secret_password_free(password);
}
