#
# This file is part of GNUnet.
# Copyright (C) 2023 GNUnet e.V.
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

project(
    'messenger-cli',
    'c',
    license: 'AGPL3.0-or-later',
    version: run_command('contrib/get_version.sh').stdout().strip(),
)

src_directory = include_directories('src')

messenger_cli_deps = [
    dependency('gnunetchat'),
    dependency('gnunetutil'),
    dependency('ncurses'),
]

subdir('src')

messenger_cli_exec = executable(
    'messenger-cli',
    messenger_cli_sources,
    install: true,
    dependencies: messenger_cli_deps,
    include_directories: src_directory,
)
