
SOURCE_DIR  = src/
INSTALL_DIR ?= /usr/local/

BINARY  = messenger-cli
SOURCES = messenger_cli.c
HEADERS = 

LIBRARIES = gnunetchat

CC ?= gcc
LD ?= gcc
RM ?= rm

CFLAGS  += -pedantic -Wall -Wextra -march=native -ggdb3
LDFLAGS += 

DEBUGFLAGS   = -O0 -D _DEBUG
RELEASEFLAGS = -O2 -D NDEBUG -fwhole-program

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
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBRARY_FLAGS)

$(BINARY): $(OBJECT_FILES)
	$(LD) $(LDFLAGS) $^ -o $@ $(LIBRARY_FLAGS)

.PHONY: install

install:
	install -m 755 $(BINARY) $(addprefix $(INSTALL_DIR), bin/)

.PHONY: uninstall

uninstall:
	$(RM) -f $(addsuffix $(BINARY), $(addprefix $(INSTALL_DIR), bin/))

.PHONY: clean

clean:
	$(RM) -f $(BINARY)
	$(RM) -f $(OBJECT_FILES)
