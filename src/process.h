#include <limits.h>
#include "ChakraCore.h"
#include "ion.h"
#include "uv.h"

typedef struct napi_callback_info__ *napi_callback_info;

namespace ion {
namespace core {
namespace process
{
  static void CreateEnv(JsValueRef global) {
    napi_value process;
    ion_create_object(&process);

    napi_value env;
    ion_create_object(&env);

    char buf[PATH_MAX];
    size_t cwd_len = sizeof(buf);
    int err = uv_cwd(buf, &cwd_len);
    if (err)
      buf[0] = '\0';
    ion_define_string_utf8(env, "PWD", buf);


    ion_define(process, "env", env);

    ion_define(global, 'process', process);
  }

  
  void Init(JsValueRef env) {
    CreateEnv(env);
  }  
}}}

