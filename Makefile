
SOURCE_DIR  = src/
INSTALL_DIR ?= /usr/local/

BINARY  = messenger-cli
SOURCES = messenger_cli.c\
          application.c\
          chat.c\
          ui/accounts.c\
          ui/messages.c
HEADERS = application.h\
          chat.h\
          ui/accounts.h\
          ui/messages.h

LIBRARIES = gnunetchat gnunetutil ncurses

GNU_CC ?= gcc
GNU_LD ?= gcc
GNU_RM ?= rm

CFLAGS  += -pedantic -Wall -Wextra -ggdb3 -Wno-overlength-strings
LDFLAGS += 

DEBUGFLAGS   = -O0 -D _DEBUG
RELEASEFLAGS = -O2 -D NDEBUG

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

.PHONY: clean

clean:
	$(GNU_RM) -f $(BINARY)
	$(GNU_RM) -f $(OBJECT_FILES)
