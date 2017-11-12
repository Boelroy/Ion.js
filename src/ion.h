#ifndef ION_ION_H
#define ION_ION_H

#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
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
     return ion_set_last_error(errCode);           \
   }                                               \
} while(0)

#define FAIL_CHECK1(cmd)                           \
do                                                 \
{                                                  \
   JsErrorCode errCode = cmd;                      \
   if (errCode != JsNoError)                       \
   {                                               \
      ion_set_last_error(errCode);                 \
   }                                               \
} while(0)


#define ION_SET_PROTOTYPE_METHOD(proto, name, callback) \
do                                                   \
{                                                    \
  JsValueRef function;                              \
  ion_create_function(callback, nullptr, &function);\
  ion_define(proto, name, function);            \
} while(0)

#define ION_SET_METHOD(object, name, callback) \
do                                                   \
{                                                    \
  JsValueRef function;                              \
  ion_create_function(callback, nullptr, &function);\
  ion_define(object, name, function);            \
} while(0)

#define ION_FUNCTION(name) napi_value name(napi_value callee, bool isConstructCall, napi_value *arguments, unsigned short argumentCount, void *callbackState)

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

napi_status ion_create_array(napi_value *ref) {
  FAIL_CHECK(JsCreateArray(0, ref));
  return napi_ok;
}

napi_status ion_create_string_utf8(napi_value *value, const char* str) {
	size_t length = strlen(str);
	FAIL_CHECK(JsCreateString(str, length, value));
	return napi_ok;
}

napi_status ion_set_array_at_index(napi_value array, int index, napi_value value) {
  napi_value _interger;
  FAIL_CHECK(JsIntToNumber(index, &_interger));
  FAIL_CHECK(JsSetIndexedProperty(array, _interger, value));
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

std::string ion_get_string(JsValueRef jsStr) {
  int length;
	JsGetStringLength(jsStr, &length);

  std::vector<char> str;
  str.resize(length + 1);
	size_t outputLength;
  JsCopyString(jsStr, str.data(), length, &outputLength);
  str[length] = '\0';
  return std::string(&str[0]);
}

#endif