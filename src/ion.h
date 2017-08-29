#ifndef ION_ION_H
#define ION_ION_H

#include "ChakraCore.h"


#ifdef _WIN32
typedef wchar_t UChar;
#else

#endif

#define FAIL_CHECK(cmd)                             \
do                                                 \
{                                                  \
   JsErrorCode errCode = cmd;                      \
   if (errCode != JsNoError)                       \
   {                                               \
     printf("Error %d at '%s' \n", errCode, #cmd); \
   }                                               \
} while(0)




#endif