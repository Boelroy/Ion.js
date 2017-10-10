#ifndef ION_ENV
#define ION_ENV

#include "ChakraCore.h"
#include "ion.h"
#include <vector>
#include "stdio.h"

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
  
}}}

#endif // ! 