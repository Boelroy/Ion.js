#include "ChakraCore.h"
#include "ion.h"
#include <iostream>
#include "unicode.h"

namespace ion {
namespace core {
namespace env {
  JsValueRef Console(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
	{
		for(unsigned int index = 1; index < argumentCount; index++)
		{
			if (index > 1)
			{
				CONSOLE(" ");
			}

			JsValueRef stringValue;
			FAIL_CHECK(JsConvertValueToString(arguments[index], &stringValue));

			// get string length
			int length;
			FAIL_CHECK(JsGetStringLength(stringValue, &length));

			Char strs[length+1];
			size_t outputLength;
			FAIL_CHECK(JsCopyStringUtf16(stringValue, 0, length, strs, &outputLength));
			strs[length] = '\0';
			CONSOLE("%S", strs);
		}
		CONSOLE("\n");
		return JS_INVALID_REFERENCE;
  }
  
}}}
