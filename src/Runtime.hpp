#include "ChakraCore.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>

#define FAIL_CHECK(cmd)                             \
do                                                 \
{                                                  \
   JsErrorCode errCode = cmd;                      \
   if (errCode != JsNoError)                       \
   {                                               \
     printf("Error %d at '%s' \n", errCode, #cmd); \
   }                                               \
} while(0)

namespace ion {
namespace core{
namespace runtime{

  void Init() {
    JsRuntimeHandle runtime;
    JsContextRef context;
    JsValueRef result;
    unsigned currentSourceContext = 0;
    
    // create a runtime
    FAIL_CHECK(JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime));
  
    // create an execution context
    FAIL_CHECK(JsCreateContext(runtime, &context));
  
    // Now set the current execution context
    FAIL_CHECK(JsSetCurrentContext(context));
  
    JsValueRef fname;
    FAIL_CHECK(JsCreateString(filename, strlen(filename), &fname));
  
    JsValueRef scriptSource;
    FAIL_CHECK(JsCreateExternalArrayBuffer((void*)src.data(), (unsigned int)src.size(),
      nullptr, nullptr, &scriptSource));
  
    FAIL_CHECK(JsRun(scriptSource, currentSourceContext++, fname, JsParseScriptAttributeNone, &result));
  
    JsValueRef resultJSString;
    FAIL_CHECK(JsConvertValueToString(result, &resultJSString));
  
    char *resultSTR = nullptr;
    size_t stringLength;
    FAIL_CHECK(JsCopyString(resultJSString, nullptr, 0, &stringLength));
  
    resultSTR = (char*) malloc(stringLength + 1);
    FAIL_CHECK(JsCopyString(resultJSString, resultSTR, stringLength + 1, nullptr));
  
    resultSTR[stringLength] = 0;
  
    printf("Result -> %s \n", resultSTR);
    free(resultSTR);
  }

}}}