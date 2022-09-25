# This Makefile.am is in the public domain

SUBDIRS = ui

bin_PROGRAMS = messenger-cli

messenger_cli_SOURCES = \
  application.c application.h \
  chat.c chat.h \
  util.c util.h \
  messenger_cli.c

messenger_cli_LDADD = \
  ui/libui.a

messenger_cli_LDFLAGS = \
  -lgnunetchat \
  -lgnunetutil \
  -lncurses

messenger_cli_CFLAGS = \
  -pedantic -Wall -Wextra -Wno-overlength-strings

if DEBUG
messenger_cli_CFLAGS += \
  -O0 -D _DEBUG -ggdb3
else
messenger_cli_CFLAGS += \
  -O2 -D NDEBUG
endif
