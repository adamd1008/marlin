/*
   Copyright (c) 2016 Adam Dodd

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#ifndef _PANIC_LOG_H_
#define _PANIC_LOG_H_

#include <stdio.h>
#include <stdlib.h>

#ifdef __linux
#include <linux/limits.h>
#include <sys/time.h>
#elif defined(_WIN32)
#include <Windows.h>
#define __func__ __FUNCDNAME__
#endif

#define LOG_INFO              0x0000
#define LOG_WARN              0x0001
#define LOG_ERR               0x0002
#define LOG_BUG               0x0004
#define LOG_L1                0x0010
#define LOG_L2                0x0020
#define LOG_L3                0x0040
#define LOG_ALL               0xffff

#define LOG_FLAG_NONE            0x0000
#define LOG_FLAG_CLOEXEC         0x0001
/* Log sets the CLOEXEC flag on its out file */
#define LOG_FLAG_SRC_INFO        0x0002
/* Log also prints out file and line at call! */
#define LOG_FLAG_STDOUT          0x0004
/* When no log file is specified, output to stdout instead */
#define LOG_FLAG_ENABLE_ASSERTS  0x0008
/* Asserts are active with this flag set */
#define LOG_FLAG_LONG_MSG        0x0010
/* Print large messags with better layout and more date & time info */
#define LOG_FLAG_ALL             0xffff

/* Can't use a variadic macro in conjunction with a variadic function, so sadly
   you need to explicitly type __FILE__ and __LINE__, so use this shortcut! */
#define LOG_ARGS           __FILE__, __LINE__, __func__

typedef struct _logTime_t
{
   long int sec;
   long int usec;
} logTime_t;

typedef struct
{
   char* logFileStr;
   FILE* logFile;
   logTime_t timeAtStart;
   int logLevels;
   int flags;
} log_t;

#ifdef __cplusplus
extern "C" {
#endif

extern log_t* m_logHandle;

/**
 * Initialise a new logging instance
 * 
 * @param fileName     File path of output log file (can be NULL to indicate
 *                     output to stderr/stdout
 * @param logLevelMask Mask of log levels that this instance will output
 * @param flagMask     Mask of flags to set parameters
 * @return             Const pointer to the instance (or NULL on failure)
 */
log_t* m_logInit(const char* fileName, const char* file, const int line,
                 const char* func, const int logLevelMask, const int flagMask);

/**
 * Print a variadic message to the log file
 * 
 * @param handle   Log handle pointer
 * @param file     Source file name (__FILE__ from LOG_ARGS)
 * @param line     Source line number (__LINE__ from LOG_ARGS)
 * @param func     Current function (__func__ from LOG_ARGS)
 * @param logLevel Log level of this message
 * @param fmt      Variadic message format
 * @param ...      Variadic arguments
 */
void m_logPrint(const log_t* handle, const char* file, const int line,
                const char* func, const int logLevel, const char* fmt, ...);

/**
 * Print the header of a log message; use `m_logPrintBody()` and
 * `m_logPrintFooter()` to construct the full log message
 * 
 * @param handle   Log handle pointer
 * @param file     Source file name (__FILE__ from LOG_ARGS)
 * @param line     Source line number (__LINE__ from LOG_ARGS)
 * @param func     Current function (__func__ from LOG_ARGS)
 * @param logLevel Log level of this message
 */
void m_logPrintHeader(const log_t* handle, const char* file, const int line,
                      const char* func, const int logLevel);

/**
 * Segmented print to the log file
 * 
 * @param handle   Log handle pointer
 * @param fmt      Variadic message format
 * @param ...      Variadic arguments
 */
void m_logPrintBody(const log_t* handle, const char* fmt, ...);

/**
 * Finalise a segmented log message
 * 
 * @param handle   Log handle pointer
 * @param file     Source file name (__FILE__ from LOG_ARGS)
 * @param line     Source line number (__LINE__ from LOG_ARGS)
 * @param func     Current function (__func__ from LOG_ARGS)
 * @param logLevel Log level of this message
 * @param fmt      Variadic message format
 * @param ...      Variadic arguments
 */
void m_logPrintFooter(const log_t* handle);

/**
 * Print a hexdump of the given string to the log file
 * 
 * Note: this function is for strings that are not null-terminated, and/or
 * contains nulls. For null-terminated strings, use logHexdumpz().
 * 
 * @param handle   Log handle pointer
 * @param file     Source file name (__FILE__ from LOG_ARGS)
 * @param line     Source line number (__LINE__ from LOG_ARGS)
 * @param func     Current function (__func__ from LOG_ARGS)
 * @param logLevel Log level of this message
 * @param str      String to output
 * @param len      Length of string
 */
void m_logHexdump(const log_t* handle, const char* file, const int line,
                  const char* func, const int logLevel, const char* str,
                  const unsigned int len);

/**
 * Wrapper for logHexdump() for null-terminated strings
 * 
 * @param handle   Log handle pointer
 * @param file     Source file name (__FILE__ from LOG_ARGS)
 * @param line     Source line number (__LINE__ from LOG_ARGS)
 * @param func     Current function (__func__ from LOG_ARGS)
 * @param logLevel Log level of this message
 * @param str      Null-terminated string to output
 */
void m_logHexdumpz(const log_t* handle, const char* file, const int line,
                   const char* func, const int logLevel, const char* str);

/**
 * Gracefully exit the application. No need to call logCleanup(), this method
 * does that.
 * 
 * Never returns - because it exits the application!
 * 
 * @param handle   Log handle pointer
 * @param file     Source file name (__FILE__ from LOG_ARGS)
 * @param line     Source line number (__LINE__ from LOG_ARGS)
 * @param func     Current function (__func__ from LOG_ARGS)
 * @param exitCode Code to exit with
 */
void m_logExit(const log_t* handle, const char* file, const int line,
               const char* func, int exitCode);

/**
 * Gracefully close a log instance
 * 
 * @param handle Log handle pointer
 */
void m_logCleanup(const log_t* handle, const char* file, const int line,
                  const char* func);

/**
 * Determine whether a particular log level is enabled for the given instance
 * 
 * @param handle   Log handle pointer
 * @param logLevel Log level to test
 * @return         Zero if false; otherwise true
 */
int m_logGetLogLevel(const log_t* handle, int logLevel);

/**
 * Print a backtrace to the log file
 * 
 * Note that applications should be compiled with the `-rdynamic` switch for the
 * information printed by this function to be meaningful.
 * 
 * @param handle Log handle pointer
 * @param file   Source file name (__FILE__ from LOG_ARGS)
 * @param line   Source line number (__LINE__ from LOG_ARGS)
 * @param func   Current function (__func__ from LOG_ARGS)
 */
void m_logBacktrace(const log_t* handle, const char* file, const int line,
                    const char* func);

/**
 * Abort the application and print source location of failure
 * 
 * @param handle Log handle pointer
 * @param file   Source file name (__FILE__ from LOG_ARGS)
 * @param line   Source line number (__LINE__ from LOG_ARGS)
 * @param func   Current function (__func__ from LOG_ARGS)
 */
void m_logAbort(const log_t* handle, const char* file, const int line,
                const char* func);

/**
 * Perform an assertion and output to log and abort in the event of failure
 * 
 * Note: use logAssert(handle ptr, assertion)
 * 
 * @param handle Log handle pointer
 * @param file   Source file name (__FILE__ from LOG_ARGS)
 * @param line   Source line number (__LINE__ from LOG_ARGS)
 * @param func   Current function (__func__ from LOG_ARGS)
 * @param cond   String representation of assertion failure
 */
void __logAssert(const log_t* handle, const char* file, const int line,
                 const char* func, const char* cond);

/* NOTE: cannot enforce the normal convention of using LOG_ARGS as the
 * preprocessor doesn't substitute it at compile time. See error:
 * 
   ...... error: macro "logAssert" requires 5 arguments, but only 3 given
       logAssert(cfg->log, LOG_ARGS, cfg->lua);
 */

#define m_logAssert(handle, cond)                                              \
   ((cond)                                                                     \
    ? (void) (0)                                                               \
    : __logAssert(handle, __FILE__, __LINE__, __func__, #cond))

/**
 * Allocate memory, or abort and print log message if out of memory
 * 
 * @param handle Log handle pointer
 * @param file   Source file name (__FILE__ from LOG_ARGS)
 * @param line   Source line number (__LINE__ from LOG_ARGS)
 * @param func   Current function (__func__ from LOG_ARGS)
 * @param size   Bytes to allocate
 * @return       Pointer to start of allocated region
 */
void* m_logMalloc(const log_t* handle, const char* file, const int line,
                  const char* func, size_t size);

/**
 * Allocate memory, or abort and print log message if out of memory
 * 
 * @param handle Log handle pointer
 * @param file   Source file name (__FILE__ from LOG_ARGS)
 * @param line   Source line number (__LINE__ from LOG_ARGS)
 * @param func   Current function (__func__ from LOG_ARGS)
 * @param ptr    Pointer to existing malloc'd memory
 * @param size   Bytes to allocate
 * @return       Pointer to start of allocated region
 */
void* m_logRealloc(const log_t* handle, const char* file, const int line,
                   const char* func, void* ptr, size_t size);

#ifdef __cplusplus
}
#endif

#endif
