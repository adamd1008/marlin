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

#include <log.h>

int main()
{
   int myInteger = 26;
   const char* myString = "Sailor";
   
   /* `m_logHandle' is a global log handle exported by the marlin library; you
    * may as well use it if you don't plan to have multiple handles. */
   m_logHandle = m_logInit(NULL, LOG_ARGS, LOG_ALL,
                           LOG_FLAG_ALL ^ LOG_FLAG_LONG_MSG);
   
   /* Note the use of the macro `LOG_ARGS' - this contains the source file, line
    * and function, in that order and passes them as the three arguments used
    * by all functions (except the m_logAssert() macro). */
   
   m_logPrint(m_logHandle, LOG_ARGS, LOG_INFO, "Hello %s! My name is Adam and "
              "I am %d years old", myString, myInteger);
   
   m_logPrint(m_logHandle, LOG_ARGS, LOG_WARN, "I... am... angry!");
   
   m_logPrint(m_logHandle, LOG_ARGS, LOG_ERR, "Get outta here!!");
   
   m_logAssert(m_logHandle, "This is an assert; note the lack of LOG_ARGS!");
   
   m_logCleanup(m_logHandle, LOG_ARGS);
   
#ifdef _WIN32
   system("pause");
#endif

   return 0;
}
