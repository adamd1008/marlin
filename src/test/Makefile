CC=gcc
CFLAGS=-std=c99 -g -Wall -Wextra -pedantic -I..
TESTS=simple1 hexdump1
LIBS=../../lib/libmarlin.a

all: $(TESTS) symlink

$(TESTS):
	$(CC) $(CFLAGS) -o $@ $@.c $(LIBS)

symlink:
	ln -sf ../../lib/libmarlinlua.so .

clean:
	rm -f $(TESTS) *~ libmarlinlua.so