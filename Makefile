VERSION = 0.1

CC      = gcc
LIBS    = -lm -lxcb -lxcb-icccm -lxcb-ewmh
CFLAGS  = -std=c99 -pedantic -Wall -Wextra -I$(PREFIX)/include
CFLAGS  += -D_POSIX_C_SOURCE=200112L -DVERSION=\"$(VERSION)\"
LDFLAGS = -L$(PREFIX)/lib

PREFIX    ?= /usr/local
BINPREFIX = $(PREFIX)/bin
MANPREFIX = $(PREFIX)/share/man

SRC = xlsw.c helpers.c
OBJ = $(SRC:.c=.o)

all: CFLAGS += -Os
all: LDFLAGS += -s
all: options xlsw

options:
	@echo "xlsw build options:"
	@echo "CC      = $(CC)"
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "LIBS    = $(LIBS)"
	@echo "PREFIX  = $(PREFIX)"

.c.o:
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c -o $@ $<

xlsw: $(OBJ)
	@echo CC -o $@
	@$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

clean:
	@echo "cleaning"
	@rm -f $(OBJ) xlsw

install:
	@echo "installing executable files to $(DESTDIR)$(BINPREFIX)"
	@mkdir -p "$(DESTDIR)$(BINPREFIX)"
	@cp xlsw "$(DESTDIR)$(BINPREFIX)"
	@chmod 755 "$(DESTDIR)$(BINPREFIX)/xlsw"

uninstall:
	@echo "removing executable files from $(DESTDIR)$(BINPREFIX)"
	@rm -f $(DESTDIR)$(BINPREFIX)/xlsw

.PHONY: all options clean install uninstall
