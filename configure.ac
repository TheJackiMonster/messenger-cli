# This file is part of GNUnet.
# Copyright (C) 2022 GNUnet e.V.
#
# GNUnet is free software: you can redistribute it and/or modify it
# under the terms of the GNU Affero General Public License as published
# by the Free Software Foundation, either version 3 of the License,
# or (at your option) any later version.
#
# GNUnet is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# SPDX-License-Identifier: AGPL3.0-or-later
#
# Process this file with autoconf to produce a configure script.
#
#
AC_INIT([messenger-cli],[m4_esyscmd_s(sh contrib/get_version.sh)],[bug-gnunet@gnu.org])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIRS([m4])

AC_CONFIG_HEADERS([
  src/messenger_cli_config.h
])

AC_CANONICAL_TARGET
AC_CANONICAL_HOST

AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])

# maybe use AC_USE_SYSTEM_EXTENSIONS?
AH_TOP([#define _GNU_SOURCE 1])

AC_PROG_AWK
AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
PKG_PROG_PKG_CONFIG([0.29.2])

AC_DISABLE_STATIC
LT_INIT

AC_ARG_ENABLE([debug],
[  --enable-debug          turn on debugging],
[case "${enableval}" in
  yes) debug=true ;;
  no)  debug=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],[debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  src/ui/Makefile
])

AC_OUTPUT
