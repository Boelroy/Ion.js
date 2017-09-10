#ifndef ION_ION_H
#define ION_ION_H

#include <cstring>
#include <stdio.h>
#include "ChakraCore.h"
#include "wrapper.h"



#define FAIL_CHECK(cmd)                             \
do                                                 \
{                                                  \
   JsErrorCode errCode = cmd;                      \
   if (errCode != JsNoError)                       \
   {                                               \
     printf("Error %d at '%s' \n", errCode, #cmd); \
   }                                               \
} while(0)


void ION_DEFINLE(napi_value target, const char* name, napi_value prop)                     
{                                                         
  JsPropertyIdRef _ref;                                       
  JsCreatePropertyId(name, strlen(name), &_ref);            
  JsSetProperty(target, _ref, prop, false);                   
}


napi_status ion_define_string_utf8(napi_value env, const char* name,const char* str) {
  size_t length = strlen(str);
  napi_value _str;
  FAIL_CHECK(JsCreateString(str, length, &_str));
  ION_DEFINLE(env, name, _str);
  return napi_ok;
}


napi_status ion_define_int(napi_value env, const char* name,const int integer) {
  napi_value _interger;
  FAIL_CHECK(JsIntToNumber(integer, &_interger));
  ION_DEFINLE(env, name, _interger);  
  return napi_ok;
}

napi_status ion_create_object(napi_value *ref) {
  FAIL_CHECK(JsCreateObject(ref));
  return napi_ok;
}

#endif