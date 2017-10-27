#ifndef ION_VM
#define ION_VM

#include "ion.h"
#include "ChakraCore.h"
#include "iostream"
#include <uv.h>
#include <vector>
#include <fcntl.h>

namespace ion{
namespace core{
namespace vm{
    
#define ion_define_constants(object, constant) ion_define_int(object, #constant, constant);


napi_value
Compile(napi_value callle, bool isConstructCall, napi_value *arguments, unsigned short argumentCount, void *callbackState) {
  napi_value result;
   std::string source = ion_get_string(arguments[1]);
   std::string url = ion_get_string(arguments[2]);

   JsValueRef sourceUrl;
   FAIL_CHECK1(JsCreateString(url.c_str(), url.length(), &sourceUrl));

   JsValueRef src;
   FAIL_CHECK1(JsCreateString(source.data(), source.length(), &src));

  JsRun(arguments[1], 3, arguments[2], JsParseScriptAttributeNone, &result);
  return result;
}

void DefineMethod(napi_value fs) {
	ION_SET_METHOD(fs, "compile", Compile);
}
void Init(napi_value env) {
	napi_value vm;
	ion_create_object(&vm);

	DefineMethod(vm);

	ion_define(env, "vm", vm);
}

}}}

#endif
