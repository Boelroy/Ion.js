#ifndef ION_ENV
#define ION_ENV

#include <ChakraCore.h>
#include <ion.h>
#include <vector>
#include <iostream>
#include <uv.h>
#include <stdio.h>

namespace ion {
namespace core {
namespace env {
  JsValueRef
#ifdef WIN32
  CALLBACK
#endif
  Debug(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
  {
		for(unsigned int index = 1; index < argumentCount; index++)
		{
			if (index > 1)
			{
				printf(" ");
			}

			JsValueRef stringValue;
			JsConvertValueToString(arguments[index], &stringValue);

			// get string length
			int length;
			JsGetStringLength(stringValue, &length);

			std::vector<char> buff(length+1);
			size_t outputLength;
			JsCopyString(stringValue, buff.data(), length, &outputLength);
			printf("%s", buff.data());
		}
		printf("\n");
		return JS_INVALID_REFERENCE;
  }
	
	class Environment {
		public:
			Environment(uv_loop_t *event_loop, std::string name="default"):
				event_loop_(event_loop),
				name_(name) {}

			uv_loop_t *event_loop() {
				return event_loop_;
			}

			std::string name() {
				return name_;
			}

			static Environment* GetCurrent() {
				void *env;
				JsContextRef currentContext;
				JsGetCurrentContext(&currentContext);
				JsGetContextData(currentContext, &env);
				return static_cast<Environment *>(env);
			}
		private:
			uv_loop_t* event_loop_;
			std::string name_;
	};
}}}

#endif // ! 