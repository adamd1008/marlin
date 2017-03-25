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
   char* myString = "Ahoy, Sailor! Waiting patiently for Jai...";
   char mySecretCode[8] = {(char) 0x98, (char) 0x04, (char) 0x15, (char) 0x47,
                           (char) 0x32, (char) 0x64, (char) 0x00, (char) 0x17};
   
   m_logHandle = m_logInit(NULL, LOG_ARGS, LOG_ALL,
                           LOG_FLAG_ALL ^ LOG_FLAG_LONG_MSG);
   
   /* The `z' in the name means the string is null-terminated. */
   m_logHexdumpz(m_logHandle, LOG_ARGS, LOG_INFO, myString);
   
   /* We must provide the length of the string if not null-terminated! */
   m_logHexdump(m_logHandle, LOG_ARGS, LOG_WARN, mySecretCode, 8);
   
   m_logCleanup(m_logHandle, LOG_ARGS);
   
   return 0;
}
