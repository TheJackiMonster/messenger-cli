
VERSION		= 0.1.0
TARGET_NAME	= messenger-cli

SOURCE_DIR  = src/
INSTALL_DIR ?= /usr/local/

PACKAGE = $(TARGET_NAME)
BINARY  = $(TARGET_NAME)
SOURCES = messenger_cli.c\
          application.c\
          chat.c\
          util.c\
          ui/account_create_dialog.c\
          ui/accounts.c\
          ui/chat_open_dialog.c\
          ui/chats.c\
          ui/lobby_create_dialog.c\
          ui/lobby_enter_dialog.c\
          ui/members.c\
          ui/messages.c
HEADERS = application.h\
          chat.h\
          util.h\
          ui/account_create_dialog.h\
          ui/accounts.h\
          ui/chat.h\
          ui/chat_open_dialog.h\
          ui/chats.h\
          ui/list_input.h\
          ui/lobby_create_dialog.h\
          ui/lobby_enter_dialog.h\
          ui/members.h\
          ui/messages.h\
          ui/text_input.h

LIBRARIES = gnunetchat gnunetutil ncurses

DIST_FILES = Makefile\
			 AUTHORS\
			 CHANGES.md\
			 COPYING\
			 README.md

GNU_CC  ?= gcc
GNU_LD  ?= gcc
GNU_RM  ?= rm
GNU_CP  ?= cp
GNU_TAR ?= tar

CFLAGS  += -pedantic -Wall -Wextra -ggdb3 -Wno-overlength-strings
LDFLAGS += 

DEBUGFLAGS   = -O0 -D _DEBUG
RELEASEFLAGS = -O2 -D NDEBUG

DIST_DIR = $(PACKAGE)-$(VERSION)/
DIST_TAR = $(PACKAGE)-$(VERSION).tar.gz

SOURCE_FILES  = $(addprefix $(SOURCE_DIR), $(SOURCES))
OBJECT_FILES  = $(SOURCE_FILES:%.c=%.o)
HEADER_FILES  = $(addprefix $(SOURCE_DIR), $(HEADERS))
LIBRARY_FLAGS = $(addprefix -l, $(LIBRARIES))

all: $(BINARY)

debug: CFLAGS += $(DEBUGFLAGS)
debug: $(BINARY)

release: CFLAGS += $(RELEASEFLAGS)
release: $(BINARY)

%.o: %.c
	$(GNU_CC) $(CFLAGS) -c $< -o $@ $(LIBRARY_FLAGS)

$(BINARY): $(OBJECT_FILES)
	$(GNU_LD) $(LDFLAGS) $^ -o $@ $(LIBRARY_FLAGS)

.PHONY: install

install:
	install -m 755 $(BINARY) $(addprefix $(INSTALL_DIR), bin/)

.PHONY: uninstall

uninstall:
	$(GNU_RM) -f $(addsuffix $(BINARY), $(addprefix $(INSTALL_DIR), bin/))

.PHONY: dist

dist: clean
	mkdir $(DIST_DIR)
	$(GNU_CP) -r $(SOURCE_DIR) $(DIST_DIR)
	$(foreach DIST_FILE,$(DIST_FILES),$(GNU_CP) $(DIST_FILE) $(addprefix $(DIST_DIR), $(DIST_FILE));)
	$(GNU_TAR) -czf $(DIST_TAR) $(DIST_DIR)
	$(GNU_RM) -r $(DIST_DIR)

.PHONY: clean

clean:
	$(GNU_RM) -f $(BINARY)
	$(GNU_RM) -f $(OBJECT_FILES)
