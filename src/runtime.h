#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "ChakraCore.h"
#include "ion.h"

namespace ion {
namespace core{
namespace runtime{
  
  class Runtime {
    public:
      Runtime() {
        FAIL_CHECK(JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime));
        FAIL_CHECK(JsCreateContext(runtime, &context));
        FAIL_CHECK(JsSetCurrentContext(context));
      }

      ~Runtime() {
        Exit();
      }

      JsValueRef RunScript(std::vector<char> source, std::string filename) {
        JsValueRef result;
        FAIL_CHECK(JsRunScript(source.data(), 0, filename.c_str(), &result));

        return result;
      }

      void Exit() {
        FAIL_CHECK(JsSetCurrentContext(JS_INVALID_REFERENCE), "failed to cleanup current context.");
        FAIL_CHECK(JsDisposeRuntime(runtime), "failed to cleanup runtime.");
      }

    private:
      JsRuntimeHandle runtime;
      JsContextRef context;
      unsigned currentSourceContext = 0;
  };

}}}