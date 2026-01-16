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
 * @file secret.h
 */

#ifndef SECRET_H_
#define SECRET_H_

#include <stdbool.h>
#include <stdint.h>

#include <libsecret/secret.h>

/**
 * Lookup a secret from identity with
 * a given name.
 *
 * @param[in] name Identity name
 * @param[out] secret_len Length of secret
 * @return Secret or NULL
 */
char*
secret_lookup(const char *name,
              uint32_t *secret_len);

/**
 * Stores a secret for identity with
 * a given name.
 *
 * @param[in] name Identity name
 * @param[in] secret Secret
 * @param[in] secret_len Length of secret
 * @return Whether the storage was successful
 */
bool
secret_store(const char *name,
             const char *secret,
             uint32_t secret_len);

/**
 * Delete a secret from identity with
 * a given name.
 *
 * @param[in] name Identity name
 * @return Whether the deletion was successful
 */
bool
secret_delete(const char *name);

/**
 * Wipe a secret from memory.
 *
 * @param[out] secret
 */
void
secret_wipe(char *secret);

/**
 * Wipe and free a secret from memory.
 *
 * @param[out] secret
 */
void
secret_free(char *secret);

#endif /* SECRET_H_ */
