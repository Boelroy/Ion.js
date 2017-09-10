#include "ChakraCore.h"
#include "ion.h"
#include "unistd.h"
#include <pwd.h>

namespace ion {
namespace core {
namespace process
{
  static void CreateEnv(JsValueRef process) {
    napi_value env;
    ion_create_object(&env);
  
    struct passwd *pw = getpwuid(getuid());
    ion_define_string_utf8(env, "PWD", pw->pw_dir);

    ION_DEFINLE(process, "env", env);
  }
  
  void Init(JsValueRef env) {
    CreateEnv(env);
  }  
}}}

