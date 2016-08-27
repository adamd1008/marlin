# marlin

A simple logging library written in C, with Lua bindings written in C++.

## Features

* General purpose message printing to file, stdout or stderr
* Prints microsecond time since logging start
* Has wrappers for `malloc()` and `realloc()` which check for allocation errors
* Supports assertions, with failure printed automatically
* Supports hexdumping of arbitrary binary strings and null-terminated strings
* Can print the source location of messages (file, line and function)
* Can specify close-on-`exec()`
* Has several logging levels, which can be filtered at startup

## Example output

Check out the source of these files in `src/test`.

### `simple1.c`

Simple, average usage!

    [    0.000018] <simple1.c:29> "main" INFO: Log library started
    [    0.000031] <simple1.c:29> "main" INFO: loglevels mask: 0x0000ffff
    [    0.000033] <simple1.c:31> "main" INFO: Hello world! My name is (Adam)!
    [    0.000035] <simple1.c:34> "main" ERR : Oh no! This is an error!
    [    0.000037] <simple1.c:36> "main" INFO: Logging stopped

### `hexdump1.c`

Marlin supports hexdumping of arbitrary binary strings and null-terminated strings.

    [    0.000009] <hexdump1.c:31> "main" INFO: Log library started
    [    0.000029] <hexdump1.c:31> "main" INFO: loglevels mask: 0x0000ffff
    [    0.000032] <hexdump1.c:33> "main" INFO: HEX (11 bytes)
    --BEGIN_HEX--
    00000000  41 68 6f 79 20 73 61 69  6c 6f 72                 |Ahoy sailor|
    0000000b
    ---END_HEX---
    [    0.000040] <hexdump1.c:35> "main" WARN: HEX (8 bytes)
    --BEGIN_HEX--
    00000000  98 04 15 47 32 64 00 17                           |...G2d..|
    00000008
    ---END_HEX---
    [    0.000048] <hexdump1.c:37> "main" INFO: Logging stopped

### `lua_simple1.lua`

The Lua library supports all the functionality of the C library!

    [    0.000004] <@lua_simple1.lua:25> "(top level)" INFO: Log library started
    [    0.000015] <@lua_simple1.lua:25> "(top level)" INFO: loglevels mask:    0x0000ffff
    [    0.000028] <@lua_simple1.lua:27> "(top level)" INFO: Hello world!
    [    0.000030] <@lua_simple1.lua:28> "(top level)" WARN: Warning! High      voltage!
    [    0.000031] <@lua_simple1.lua:30> "(top level)" ERR : HEX (8 bytes)
    --BEGIN_HEX--
    00000000  48 65 6c 6c 43 21 fe 23                           |HellC!.#|
    00000008
    ---END_HEX---
    [    0.000062] <@lua_simple1.lua:32> "(top level)" INFO: Logging stopped
