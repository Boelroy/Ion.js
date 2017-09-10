#ifndef ION_WRAPPER
#define ION_WRAPPER

#include "ChakraCore.h"
// property define

typedef enum {
  napi_ok,
  napi_invalid_arg,
  napi_object_expected,
  napi_string_expected,
  napi_array_expected,
  napi_function_expected,
  napi_number_expected,
  napi_boolean_expected,
  napi_name_expected
} napi_status;

typedef struct {
  const char* error_message;
  void* engine_reserved;
  unsigned int engine_error_code;
  napi_status error_code;
} napi_extended_error_info;


typedef JsValueRef napi_value;
// method define
#endif