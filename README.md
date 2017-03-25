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

    [    0.000007] <simple1.c:32> "main" INFO: Log library started
    [    0.000020] <simple1.c:32> "main" INFO: loglevels mask: 0x0000ffff
    [    0.000023] <simple1.c:39> "main" INFO: Hello Sailor! My name is Adam and I am 26 years old
    [    0.000025] <simple1.c:42> "main" WARN: I... am... angry!
    [    0.000027] <simple1.c:44> "main"  ERR: Get outta here!!
    [    0.000028] <simple1.c:48> "main" INFO: Logging stopped

### `hexdump1.c`

Marlin supports hexdumping of arbitrary binary strings and null-terminated strings.

    [    0.000007] <hexdump1.c:31> "main" INFO: Log library started
    [    0.000039] <hexdump1.c:31> "main" INFO: loglevels mask: 0x0000ffff
    [    0.000043] <hexdump1.c:35> "main" INFO: HEX (42 bytes)
    00000000  41 68 6f 79 2c 20 53 61  69 6c 6f 72 21 20 57 61  |Ahoy, Sailor! Wa|
    00000010  69 74 69 6e 67 20 70 61  74 69 65 6e 74 6c 79 20  |iting patiently |
    00000020  66 6f 72 20 4a 61 69 2e  2e 2e                    |for Jai...|
    [    0.000058] <hexdump1.c:38> "main" WARN: HEX (8 bytes)
    00000000  98 04 15 47 32 64 00 17                           |...G2d..|
    [    0.000066] <hexdump1.c:40> "main" INFO: Logging stopped

### `lua_simple1.lua`

The Lua library supports all the functionality of the C library (excluding memory allocation functions)!

    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    [    0.000002] <@lua_simple1.lua:25> "(top level)" [2017/03/25 16:07:36.927327] INFO
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    Log library started
    
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    [    0.000065] <@lua_simple1.lua:25> "(top level)" [2017/03/25 16:07:36.927390] INFO
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    loglevels mask: 0x0000ffff
    
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    [    0.000091] <@lua_simple1.lua:27> "(top level)" [2017/03/25 16:07:36.927416] INFO
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    Hello world!
    
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    [    0.000105] <@lua_simple1.lua:28> "(top level)" [2017/03/25 16:07:36.927430] WARN
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    Warning! High voltage!
    
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    [    0.000119] <@lua_simple1.lua:30> "(top level)" [2017/03/25 16:07:36.927444]  ERR
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    HEX (8 bytes)
    00000000  48 65 6c 6c 43 21 fe 23                           |HellC!.#|
    
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    [    0.000142] <@lua_simple1.lua:32> "(top level)" [2017/03/25 16:07:36.927467] INFO
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    Logging stopped
