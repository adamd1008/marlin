CC=g++
CPPFLAGS=-std=c++11 -g -c -I. -I.. -Wall -Wextra -pedantic -Werror -rdynamic \
	 -fPIC
CPP_LIB_LDFLAGS=-rdynamic -shared -Wl,-soname,libmarlinlua.so
CPP_LIB_SOURCES=Lua_Utl.cpp Lua_RegObj.cpp Lua_Log.cpp
CPP_LIB_OBJECTS=$(CPP_LIB_SOURCES:.cpp=.o)
LIB_DIR=../../lib
CPP_LIBRARY=libmarlinlua.so

all: $(CPP_LIB_SOURCES) $(CPP_LIBRARY)

$(CPP_LIBRARY): $(CPP_LIB_OBJECTS)
	@echo "** Building dynamic lib '$@'"
	$(CC) $(CPP_LIB_LDFLAGS) -o $(LIB_DIR)/$@ $(CPP_LIB_OBJECTS) ../log.o

.cpp.o:
	@echo "** Compiling file '$<'"
	$(CC) $(CPPFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f \
	$(LIB_DIR)/$(CPP_LIBRARY) *.o *~
