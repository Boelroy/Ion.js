#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

#include "ChakraCore.h"
#include "ion.h"
#include "env.h"
#include "process.h"
#include "timer.h"

namespace ion {
namespace core{
namespace runtime{

  class Runtime {
    public:
      Runtime() {
        this->CreateRuntime();
      }

      ~Runtime() {
        Exit();
      }

      napi_status Init(int argc, char** argv) {

        JsValueRef globalObject;
        FAIL_CHECK(JsGetGlobalObject(&globalObject));

        ion_define(globalObject, "global" ,globalObject);                                            

        ion::core::process::Init(globalObject, argc, argv);
        ion::core::timer::Init(globalObject);

        DefineHostCallback(globalObject, "Debug", ion::core::env::Debug, nullptr);
        return napi_ok;
      }

      napi_status RunScript(const char* source, std::string filename) {
        JsValueRef result;
        // std::vector<char> wrapper = WrapModule(source);

        JsValueRef sourceUrl;
        FAIL_CHECK(JsCreateString(filename.c_str(), filename.length(), &sourceUrl));

        JsValueRef src;
        FAIL_CHECK(JsCreateString(source, strlen(source), &src));

        FAIL_CHECK(JsRun(src, this->currentSourceContext++, sourceUrl, JsParseScriptAttributeNone, &result));

        return napi_ok;
      }

      napi_status Exit() {
        FAIL_CHECK(JsSetCurrentContext(JS_INVALID_REFERENCE));
        FAIL_CHECK(JsDisposeRuntime(runtime));
        return napi_ok;
      }

      bool HasError() {
        bool hasException;
        JsHasException(&hasException);
        return hasException;
      }

      static napi_status GetException() {
        JsValueRef exception;
        FAIL_CHECK(JsGetAndClearExceptionWithMetadata(&exception));
        JsPropertyIdRef sourceProperty;
        FAIL_CHECK(JsCreatePropertyId("source", 6, &sourceProperty));

        JsValueRef stackInfo;
        FAIL_CHECK(JsGetProperty(exception, sourceProperty, &stackInfo));
        // log error

        JsValueRef arg[] = {nullptr, stackInfo};
        env::Debug(nullptr, false, arg, 2, nullptr);
        return napi_ok;
      }

    private:
      napi_status CreateRuntime() {
        FAIL_CHECK(JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime));
        FAIL_CHECK(JsCreateContext(runtime, &context));
        FAIL_CHECK(JsSetCurrentContext(context));
        currentSourceContext = 0;
        return napi_ok;
      }

      std::vector<char> WrapModule(std::vector<char> source) {
        // source.insert(wrap_tail.begin(), wrap_tail.length(),source.size());
        return source;
      }

      JsErrorCode DefineHostCallback(JsValueRef globalObject, std::string callbackName, JsNativeFunction callback, void *callbackState)
      {
        JsPropertyIdRef propertyId;

        JsCreatePropertyId(callbackName.c_str(), callbackName.length(), &propertyId);
      
        JsValueRef function;
        JsCreateFunction(callback, callbackState, &function);

        JsSetProperty(globalObject, propertyId, function, true);
      
        return JsNoError;
      }

    private:
      
      JsRuntimeHandle runtime;
      JsContextRef context;
      unsigned currentSourceContext;
      static std::string MODLUE_WRAPPER[];
  };

  std::string Runtime::MODLUE_WRAPPER[] = {
    "(function (__filename, __dirname) {",
    "})"
  };
}}}