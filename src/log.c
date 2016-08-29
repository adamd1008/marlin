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

#define _DEFAULT_SOURCE
#define _POSIX_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include "log.h"

#ifdef __linux
#include <execinfo.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <DbgHelp.h>
#endif

#define LOG_MAX_LEN PIPE_BUF

/* Shortcut macros */
#define LOG_PTR_IS_LL(x)   ((handle->logLevels & x) == x)
#define LOG_IS_LL(x)       ((handle.logLevels & x) == x)
#define LOG_PTR_IS_FLAG(x) ((handle->flags & x) == x)
#define LOG_IS_FLAG(x)     ((handle.flags & x) == x)

log_t* m_logHandle = NULL;

static const char* const logLevelInfoStr = "INFO";
static const char* const logLevelWarnStr = "WARN";
static const char* const logLevelErrStr  = "ERR ";
static const char* const logLevelBugStr  = "BUG ";
static const char* const logLevelL1Str   = "L1  ";
static const char* const logLevelL2Str   = "L2  ";
static const char* const logLevelL3Str   = "L3  ";
static const char* const logLevelUnknStr = "UNKN";

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

logTime_t m_timeNow()
{
   logTime_t ret;

#ifdef __linux
   struct timeval tv;
   gettimeofday(&tv, NULL);

   ret.sec = tv.tv_sec;
   ret.usec = tv.tv_usec;
#elif defined(_WIN32)
   FILETIME time;
   ULONGLONG qwResult;
   GetSystemTimeAsFileTime(&time);

   qwResult = (((ULONGLONG) time.dwHighDateTime) << 32) +
              time.dwLowDateTime;

   /* Convert from 100-nanoseconds to 1-microseconds */
   qwResult /= 10lu;

   ret.sec = (long int) qwResult / 1000000lu;
   ret.usec = (long int) qwResult % 1000000lu;
#endif
   
   return ret;
}

logTime_t m_timeSub(logTime_t timeNow, logTime_t timeAtStart)
{
   logTime_t ret;

   ret.sec = timeNow.sec - timeAtStart.sec;
   ret.usec = timeNow.usec - timeAtStart.usec;
   
   if (ret.usec < 0)
   {
      ret.usec += 1000000;
      ret.sec--;
   }

   return ret;
}

log_t* m_logInit(const char* fileName, const char* file, const int line,
                 const char* func, const int logLevelMask, const int flagMask)
{
   log_t* handle;
   
   handle = malloc(sizeof(log_t));
   
   if (handle == NULL)
      return NULL;
   
   handle->logFileStr = malloc(sizeof(char) * PATH_MAX);
   
   if (handle->logFileStr == NULL)
      abort();
   
   handle->timeAtStart = m_timeNow();
   
   handle->logFileStr[0] = 0;
   handle->logLevels = logLevelMask;
   handle->flags = flagMask;
   
   if ((fileName != NULL) && (fileName[0] != 0))
   {
      if ((handle->logFile = fopen(fileName, "w")) == NULL)
      {
         perror("fopen");
         abort();
      }
      
      strncpy(handle->logFileStr, fileName, PATH_MAX);
      handle->logFileStr[PATH_MAX - 1] = 0;
   }
   else
      if (LOG_PTR_IS_FLAG(LOG_FLAG_STDOUT))
         handle->logFile = stdout;
      else
         handle->logFile = stderr;
   
#ifdef __linux
   setlinebuf(handle->logFile); /* Essential! */
   
   if (LOG_PTR_IS_FLAG(LOG_FLAG_CLOEXEC))
   {
      int logFD = fileno(handle->logFile);
      
      if (logFD == -1)
      {
         perror("fileno");
         abort();
      }
      
      if ((fcntl(logFD, F_SETFD, FD_CLOEXEC)) == -1)
      {
         perror("fcntl");
         abort();
      }
   }
#endif
   
   m_logPrint(handle, file, line, func, LOG_INFO, "Log library started");
   m_logPrint(handle, file, line, func, LOG_INFO, "loglevels mask: 0x%.08x",
              logLevelMask);
   
   return handle;
}

const char* _logGetLogLevelStr(const int logLevel)
{
   const char* ret = logLevelUnknStr;
   
   switch (logLevel)
   {
      case LOG_INFO:
         ret = logLevelInfoStr;
         break;
      case LOG_WARN:
         ret = logLevelWarnStr;
         break;
      case LOG_ERR:
         ret = logLevelErrStr;
         break;
      case LOG_BUG:
         ret = logLevelBugStr;
         break;
      case LOG_L1:
         ret = logLevelL1Str;
         break;
      case LOG_L2:
         ret = logLevelL2Str;
         break;
      case LOG_L3:
         ret = logLevelL3Str;
         break;
      default:
         break;
   }
   
   return ret;
}

void m_logPrint(const log_t* handle, const char* file, const int line,
                const char* func, const int logLevel, const char* fmt, ...)
{
   va_list ap;
   char logLevelStr[8];
   char logEntry[LOG_MAX_LEN];
   logTime_t tv;
   
   if (LOG_PTR_IS_LL(logLevel))
   {
      strncpy(logLevelStr, _logGetLogLevelStr(logLevel), 8);
      logLevelStr[7] = 0;
      
      va_start(ap, fmt);
      vsnprintf(logEntry, LOG_MAX_LEN, fmt, ap);
      va_end(ap);
      
      tv = m_timeNow();
      tv = m_timeSub(tv, handle->timeAtStart);
      
      if (LOG_PTR_IS_FLAG(LOG_FLAG_SRC_INFO))
      {
         if ((fprintf(handle->logFile,
                      "[%5ld.%.06ld] <%s:%d> \"%s\" %s: %s\n",
                      tv.sec, tv.usec, file, line, func, logLevelStr,
                      logEntry)) < 0)
            abort();
      }
      else
      {
         if ((fprintf(handle->logFile,
                      "[%5ld.%.06ld] \"%s\" %s: %s\n",
                      tv.sec, tv.usec, func, logLevelStr, logEntry)) < 0)
            abort();
      }
   }
}

void m_logHexdump(const log_t* handle, const char* file, const int line,
                  const char* func, const int logLevel, const char* str,
                  const int len)
{
   char logLevelStr[8];
   int i, j;
   logTime_t tv;
   
   if (LOG_PTR_IS_LL(logLevel))
   {
      strncpy(logLevelStr, _logGetLogLevelStr(logLevel), 8);
      logLevelStr[7] = 0;

      tv = m_timeNow();
      tv = m_timeSub(tv, handle->timeAtStart);
      
      if (LOG_PTR_IS_FLAG(LOG_FLAG_SRC_INFO))
      {
         fprintf(handle->logFile,
                 "[%5ld.%.06ld] <%s:%d> \"%s\" %s: HEX (%d bytes)\n",
                 (long int) tv.sec, (long int) tv.usec, file, line,
                 func, logLevelStr, len);
      }
      else
      {
         fprintf(handle->logFile, "[%5ld.%.06ld] \"%s\" %s: HEX (%d bytes)\n",
                 (long int) tv.sec, (long int) tv.usec, func,
                 logLevelStr, len);
      }
      
      fprintf(handle->logFile, "--BEGIN_HEX--");
      
      for (i = 0; i < len; i++)
      {
         if ((i % 16) == 0)
            fprintf(handle->logFile, "\n%.08x  ", i);
         
         /* ISO C90 does not support the 'hh' length modifier, so use this
          * interesting solution...
          * 
          * XXX Will this work on big endian systems?
          */
         fprintf(handle->logFile, "%.02hx ", str[i] & 0xff);
         
         if (i > 0)
         {
            if ((i % 8) == 7)
               fputc(' ', handle->logFile);
            
            if ((i % 16) == 15)
            {
               /* Use isprint() to print the chars, then print offset after new
                  line */
               
               fputc('|', handle->logFile);
               
               for (j = i - 15; j <= i; j++)
                  if (isprint(str[j]))
                     fputc(str[j], handle->logFile);
                  else
                     fputc('.', handle->logFile);
               
               fputc('|', handle->logFile);
            }
         }
      }
      
      /* What if the hexdump didn't finish perfectly at (i % 16) == 0? */
      
      if ((i % 16) != 0)
      {
         /* Calculate the number of spaces to add to the ASCII area */
         int spaces = (16 - (i % 16)) * 3 + 1;
         
         if ((i % 16) < 8)
            spaces++;
            /* Don't forget the space in the middle! */
         
         for (j = 0; j < spaces; j++)
            fputc(' ', handle->logFile);
         
         fputc('|', handle->logFile);
         
         for (j = i - (i % 16); j < i; j++)
            if (isprint(str[j]))
               fputc(str[j], handle->logFile);
            else
               fputc('.', handle->logFile);
         
         fputc('|', handle->logFile);
      }
      
      fprintf(handle->logFile, "\n%.08x\n---END_HEX---\n", i);
   }
}

void m_logHexdumpz(const log_t* handle, const char* file, const int line,
                   const char* func, const int logLevel, const char* str)
{
   m_logHexdump(handle, file, line, func, logLevel, str, strlen(str));
}

void m_logExit(const log_t* handle, const char* file, const int line,
               const char* func, int exitCode)
{
   m_logPrint(handle, file, line, func, LOG_INFO,
              "Exiting application with code %d", exitCode);
   m_logCleanup(handle, file, line, func);
   
   exit(exitCode);
}

void m_logCleanup(const log_t* handle, const char* file, const int line,
                  const char* func)
{
   m_logPrint(handle, file, line, func, LOG_INFO, "Logging stopped");
   
   if (handle->logFileStr[0] != 0)
      if ((fclose(handle->logFile)) != 0)
         perror("fclose");
   
   free(handle->logFileStr);
   free((log_t*) handle); /* Cast to remove compiler const complaint */
}

int m_logGetLogLevel(const log_t* handle, int logLevel)
{
   return LOG_PTR_IS_LL(logLevel);
}

void m_logBacktrace(const log_t* handle, const char* file, const int line,
                    const char* func)
{
#ifdef _WIN32
   unsigned int i;
   void* stack[100];
   HANDLE process;
   unsigned short frames;
   SYMBOL_INFO* symbol;
#elif defined(__linux)
   void* buf[128];
   int nptrs, i;
   char** strings;
#endif
   
   logTime_t tv = m_timeNow();
   tv = m_timeSub(tv, handle->timeAtStart);

   if (LOG_PTR_IS_FLAG(LOG_FLAG_SRC_INFO))
   {
      fprintf(handle->logFile,
              "[%5ld.%.06ld] <%s:%d> \"%s\" Stack backtrace:\n",
              (long int) tv.sec, (long int) tv.usec, file, line, func);
   }
   else
   {
      fprintf(handle->logFile, "[%5ld.%.06ld] \"%s\" Stack backtrace:\n",
              (long int) tv.sec, (long int) tv.usec, func);
   }

   fprintf(handle->logFile, "--BEGIN_BACKTRACE--\n");

#ifdef __linux
   nptrs = backtrace(buf, 128);
   
   strings = backtrace_symbols(buf, nptrs);
   
   for (i = 0; i < nptrs; i++)
      fprintf(handle->logFile, "%s\n", strings[i]);
   
   free(strings);
#else
   process = GetCurrentProcess();
   SymInitialize(process, NULL, TRUE);
   frames = CaptureStackBackTrace(0, 100, stack, NULL);

   symbol = (SYMBOL_INFO*) malloc(sizeof(SYMBOL_INFO) * 256);
   symbol->MaxNameLen = 255;
   symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

   for (i = 0; i < frames; i++)
   {
      SymFromAddr(process, (DWORD64) stack[i], 0, symbol);

      fprintf(handle->logFile, "%d: %s [0x%I64x]\n", frames - i - 1, symbol->Name,
              symbol->Address);
   }

   free(symbol);
#endif
   
   fprintf(handle->logFile, "---END_BACKTRACE---\n");
}

void m_logAbort(const log_t* handle, const char* file, const int line,
                const char* func)
{
   m_logPrint(handle, file, line, func, LOG_INFO, "Process is now aborting");
   
   m_logBacktrace(handle, file, line, func);
   m_logCleanup(handle, file, line, func);
   
   abort();
}

void __logAssert(const log_t* handle, const char* file, const int line,
                 const char* func, const char* cond)
{
   if (LOG_PTR_IS_FLAG(LOG_FLAG_ENABLE_ASSERTS))
   {
      m_logPrint(handle, file, line, func, LOG_ERR, "Assertion \"%s\" failed",
                 cond);
      m_logAbort(handle, file, line, func);
   }
}

void* m_logMalloc(const log_t* handle, const char* file, const int line,
                  const char* func, size_t size)
{
   void* ret = malloc(size);
   
   if (ret == NULL)
   {
      m_logPrint(handle, file, line, func, LOG_ERR, "malloc: Out of memory");
      m_logAbort(handle, file, line, func);
   }
   
   return ret;
}

void* m_logRealloc(const log_t* handle, const char* file, const int line,
                   const char* func, void* ptr, size_t size)
{
   void* ret = realloc(ptr, size);
   
   if (ret == NULL)
   {
      m_logPrint(handle, file, line, func, LOG_ERR, "realloc: Out of memory");
      m_logAbort(handle, file, line, func);
   }
   
   return ret;
}

#ifdef _WIN32
#pragma warning(pop)
#endif
