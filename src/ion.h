#ifndef ION_ION_H
#define ION_ION_H

#include "ChakraCore.h"

#define FAIL_CHECK(cmd)                             \
do                                                 \
{                                                  \
   JsErrorCode errCode = cmd;                      \
   if (errCode != JsNoError)                       \
   {                                               \
     printf("Error %d at '%s' \n", errCode, #cmd); \
   }                                               \
} while(0)


// #define ION_SYMBOL(name)                          \
// JsValueRef _ref;                                  \
// JsCreatePropertyId(name, name.size() ,&ref);      \


#endif