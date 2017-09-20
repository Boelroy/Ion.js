#include <limits.h>
#include <iostream>
#include <assert.h>
#include "ChakraCore.h"
#include "ion.h"
#include "env.h"
#include "uv.h"

namespace ion {
namespace core {
namespace timer{

static JsValueRef TimerProtype;

class Timer {
  public:
    Timer(JsValueRef callback);
    ~Timer();
  
  static JsValueRef Constructor(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
  static JsValueRef Start(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
  static JsValueRef Close(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
  std::string name;
  private:
    static void OnTimeOut(uv_timer_t* handle);
    uv_timer_t _handler;
    JsValueRef callback;
};

Timer::Timer(JsValueRef callback_) {
  uv_timer_init(uv_default_loop(), &_handler);
  _handler.data = this;
  callback = callback_;
}

JsValueRef
Timer::Constructor(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState) {
  assert(isConstructCall);
  
  Timer *t = new Timer(arguments[1]);

  JsValueRef output = JS_INVALID_REFERENCE;
  JsCreateExternalObject(t, nullptr, &output);
  JsSetPrototype(output, TimerProtype);
  return output;
}

JsValueRef
Timer::Start(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState) {
  if (argumentCount < 2) {
    return JS_INVALID_REFERENCE;
  }
  JsValueRef thisValue = arguments[0];
  void* timerPtr;
  JsGetExternalData(thisValue, &timerPtr);
  Timer *timer = static_cast<Timer *>(timerPtr);

  int delay, repeat;
  JsNumberToInt(arguments[1], &delay);
  JsNumberToInt(arguments[2], &repeat);

  uint64_t dly = static_cast<uint64_t>(delay);
  uint64_t rpt = static_cast<uint64_t>(repeat);
  uv_timer_start(&timer->_handler, OnTimeOut, dly, repeat);

  JsValueRef undefinedValue;
  JsGetUndefinedValue(&undefinedValue);
  return undefinedValue;
}

JsValueRef
Timer::Close(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState) 
{
  JsValueRef thisValue = arguments[0];
  void* timerPtr;
  JsGetExternalData(thisValue, &timerPtr);
  Timer *timer = static_cast<Timer *>(timerPtr);

  uv_timer_stop(&timer->_handler);

  JsValueRef undefinedValue;
  JsGetUndefinedValue(&undefinedValue);
  return undefinedValue;
}

void
Timer::OnTimeOut(uv_timer_t *handler) {

  Timer *timer = static_cast<Timer*>(handler->data);
  
  JsValueRef undefinedValue;
  JsGetUndefinedValue(&undefinedValue);

  JsValueRef result;
  FAIL_CHECK1(JsCallFunction(timer->callback, &undefinedValue, 1, &result));

  // try catch
}

void
Init(JsValueRef env) {
  std::string TimerName = "Timer";
  std::string TimerStartName = "Start";
  std::string TimeCloserName = "Close";

  JsCreateObject(&TimerProtype);

  JsValueRef timerConstructor;
  ion_create_name_function(Timer::Constructor, TimerName.c_str(), nullptr, &timerConstructor);
  JsSetPrototype(timerConstructor, TimerProtype);

  ION_SET_PROTOTYPE_METHOD(TimerProtype, TimerStartName.c_str(), Timer::Start);
  ION_SET_PROTOTYPE_METHOD(TimerProtype, TimeCloserName.c_str(), Timer::Close);

  ion_define(env, TimerName.c_str(), timerConstructor);
}

}}}