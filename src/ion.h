#ifndef ION_ION_H
#define ION_ION_H

#include <cstring>
#include <stdio.h>
#include "ChakraCore.h"
#include "wrapper.h"

static napi_status ion_set_last_error(JsErrorCode errCode) {
  napi_status status;
  switch (errCode)
  {
    case JsNoError: 
      status = napi_ok;
      break;
    case JsErrorNullArgument:
    case JsErrorInvalidArgument: 
      status = napi_invalid_arg;
      break;
    case JsErrorPropertyNotString:
      status = napi_string_expected;
      break;
    case JsErrorArgumentNotObject:
      status = napi_object_expected;
      break;
    case JsErrorScriptException:
    case JsErrorInExceptionState:
      status = napi_pending_exception;
      break;
    default:
      status = napi_generic_failure;
      break;
  }
  return status;
}

#define FAIL_CHECK(cmd)                            \
do                                                 \
{                                                  \
   JsErrorCode errCode = cmd;                      \
   if (errCode != JsNoError)                       \
   {                                               \
     printf("Error %d at '%s' \n", errCode, #cmd); \
     return ion_set_last_error(errCode);           \
   }                                               \
} while(0)

#define FAIL_CHECK1(cmd)                           \
do                                                 \
{                                                  \
   JsErrorCode errCode = cmd;                      \
   if (errCode != JsNoError)                       \
   {                                               \
     printf("Error %d at '%s' \n", errCode, #cmd); \
    ion_set_last_error(errCode);                   \
   }                                               \
} while(0)


#define ION_SET_PROTOTYPE_METHOD(proto, name, callback) \
do                                                   \
{                                                    \
  JsValueRef function;                              \
  ion_create_function(callback, nullptr, &function);\
  ion_define(proto, name, function);            \
} while(0)

void ion_define(napi_value target, const char* name, napi_value prop)                     
{                                                         
  JsPropertyIdRef _ref;                        
  JsCreatePropertyId(name, strlen(name), &_ref);            
  JsSetProperty(target, _ref, prop, false);                   
}


napi_status ion_define_string_utf8(napi_value env, const char* name,const char* str) {
  size_t length = strlen(str);
  napi_value _str;
  FAIL_CHECK(JsCreateString(str, length, &_str));
  ion_define(env, name, _str);
  return napi_ok;
}


napi_status ion_define_int(napi_value env, const char* name,const int integer) {
  napi_value _interger;
  FAIL_CHECK(JsIntToNumber(integer, &_interger));
  ion_define(env, name, _interger);  
  return napi_ok;
}

napi_status ion_get_and_clear_last(napi_value env, napi_value *result) {
  FAIL_CHECK(JsGetAndClearException(result));
  if (result == nullptr) {
    
  }
  return napi_ok;
}

napi_status ion_create_object(napi_value *ref) {
  FAIL_CHECK(JsCreateObject(ref));
  return napi_ok;
}

napi_status ion_create_name_function(JsNativeFunction fn, const char* name, void* callBackStatus, JsValueRef *result) {
  napi_value fnName;
  FAIL_CHECK1(JsCreateString(name, strlen(name), &fnName));
  FAIL_CHECK(JsCreateNamedFunction(fnName, fn, callBackStatus, result));
  return napi_ok;
}

napi_status ion_create_function(JsNativeFunction fn, void* callBackStatus, JsValueRef *result) {
  FAIL_CHECK(JsCreateFunction(fn, callBackStatus, result));
  return napi_ok;
}

#endif