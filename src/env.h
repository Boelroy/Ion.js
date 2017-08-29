#include "ChakraCore.h"

namespace ion {
namespace core {
namespace env {
  JsValueRef CALLBACK Console(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
	{
		for(unsigned int index = 1; index < argumentCount; index++)
		{
			if (index > 1)
			{
				wprintf(L" ");
			}

			JsValueRef stringValue;
			FAIL_CHECK(JsConvertValueToString(arguments[index], &stringValue));

			const wchar_t *string;
			size_t length;
			FAIL_CHECK(JsStringToPointer(stringValue, &string, &length));

			wprintf(L"%s", string);
		}
		wprintf(L"\n");
		return JS_INVALID_REFERENCE;
  }
  
}}}