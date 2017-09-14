#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

#include "ChakraCore.h"
#include "ion.h"
#include "env.h"
#include "process.h"

namespace ion {
namespace core{
namespace runtime{

  class Runtime {
    public:
      Runtime() {
        FAIL_CHECK(JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime));
        FAIL_CHECK(JsCreateContext(runtime, &context));
        FAIL_CHECK(JsSetCurrentContext(context));
        currentSourceContext = 0;
      }

      ~Runtime() {
        Exit();
      }

      void Init(int argc, char** argv) {
        JsValueRef global;
        FAIL_CHECK(JsCreateObject(&global));

        JsValueRef globalObject;
        FAIL_CHECK(JsGetGlobalObject(&globalObject));

        ion_define(globalObject, "global" ,global);                                            

        ion::core::process::Init(global);

        DefineHostCallback(global, "Debug", ion::core::env::Debug, nullptr);

      }

      JsValueRef RunScript(const char* source, std::string filename) {
        JsValueRef result;
        // std::vector<char> wrapper = WrapModule(source);

        JsValueRef sourceUrl;
        FAIL_CHECK(JsCreateString(filename.c_str(), filename.length(), &sourceUrl));

        JsValueRef src;
        FAIL_CHECK(JsCreateString(source, strlen(source), &src));

        FAIL_CHECK(JsRun(src, this->currentSourceContext++, sourceUrl, JsParseScriptAttributeNone, &result));

        return result;
      }

      void Exit() {
        FAIL_CHECK(JsSetCurrentContext(JS_INVALID_REFERENCE));
        FAIL_CHECK(JsDisposeRuntime(runtime));
      }

    private:
      std::vector<char> WrapModule(std::vector<char> source) {
        std::string wrap_header = "(function (__filename, __dirname) {";
        std::string wrap_tail = "})";
        
        // source.insert(wrap_tail.begin(), wrap_tail.length(),source.size());
        return source;
      }

      JsErrorCode DefineHostCallback(JsValueRef globalObject, std::string callbackName, JsNativeFunction callback, void *callbackState)
      {
        JsPropertyIdRef propertyId;

        FAIL_CHECK(JsCreatePropertyId(callbackName.c_str(), callbackName.length(), &propertyId));
      
        JsValueRef function;
        FAIL_CHECK(JsCreateFunction(callback, callbackState, &function));

        FAIL_CHECK(JsSetProperty(globalObject, propertyId, function, true));
      
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