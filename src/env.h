#include "ChakraCore.h"
#include "ion.h"
#include "stdio.h"
#include "unicode.h"

namespace ion {
namespace core {
namespace env {
  JsValueRef Debug(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
  {
		for(unsigned int index = 1; index < argumentCount; index++)
		{
			if (index > 1)
			{
				printf(" ");
			}

			JsValueRef stringValue;
			FAIL_CHECK(JsConvertValueToString(arguments[index], &stringValue));

			// get string length
			int length;
			FAIL_CHECK(JsGetStringLength(stringValue, &length));

			std::vector<char> buff(length+1);
			size_t outputLength;
			FAIL_CHECK(JsCopyString(stringValue, buff.data(), length, &outputLength, &outputLength));
			printf("%s", buff.data());
		}
		printf("\n");
		return JS_INVALID_REFERENCE;
  }
  
}}}
