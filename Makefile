VERSION = 0.1

CC      ?= gcc
LIBS     = -lm -lxcb -lxcb-icccm -lxcb-ewmh
CFLAGS  += -std=c99 -pedantic -Wall -Wextra -I$(PREFIX)/include
CFLAGS  += -D_POSIX_C_SOURCE=200112L -DVERSION=\"$(VERSION)\"
LDFLAGS += -L$(PREFIX)/lib

PREFIX    ?= /usr/local
BINPREFIX  = $(PREFIX)/bin

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: CFLAGS += -Os
all: LDFLAGS += -s
all: xlsw

include Sourcedeps

$(OBJ): Makefile

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

xlsw: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

install:
	mkdir -p "$(DESTDIR)$(BINPREFIX)"
	cp -p xlsw "$(DESTDIR)$(BINPREFIX)"

uninstall:
	rm -f $(DESTDIR)$(BINPREFIX)/xlsw

clean:
	rm -f $(OBJ) xlsw

.PHONY: all clean install uninstall
