CC=gcc
CFLAGS=-std=c99 -g -c -Wall -Wextra -Werror -rdynamic -fPIC
LIB_C_SOURCES=log.c
LIB_C_OBJECTS=$(LIB_C_SOURCES:.c=.o)
LIB_DIR=../lib
C_LIBRARY=libmarlin.a

all: $(LIB_C_SOURCES) $(C_LIBRARY) lualib test

$(C_LIBRARY): $(LIB_C_OBJECTS)
	@echo "** Building static lib '$@'"
	ar rcs $(LIB_DIR)/$@ $(LIB_C_OBJECTS)

.c.o:
	@echo "** Compiling file '$<'"
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean test lualib

clean:
	rm -f \
	$(LIB_DIR)/$(C_LIBRARY) *.o *~
	$(MAKE) -C lualib clean
	$(MAKE) -C test clean

lualib:
	$(MAKE) -C lualib

test:
	$(MAKE) -C test
