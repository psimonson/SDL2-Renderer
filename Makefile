CC = gcc
CFLAGS = -std=c11 -Wall -Werror -D_DEFAULT_SOURCE -g -O0
LDFLAGS = -lSDL2 -lSDL2_image -lm

SRCDIR = $(shell basename $(shell pwd))
DESTDIR ?= 
PREFIX ?= /usr

SRC0 = src/renderer.c src/vecmath.c src/matrix.c src/main.c src/mesh.c
OBJ0 = $(SRC0:%.c=%.c.o)
EXE0 = render

all: $(EXE0)

$(EXE0): $(OBJ0)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ0) $(EXE0)

install:
	cp $(EXE0) $(DESTDIR)$(PREFIX)/bin

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(EXE0)

dist:
	cd .. && tar cvzf $(SRCDIR).tgz ./$(SRCDIR)

