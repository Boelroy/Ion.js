#include <limits.h>
#include "ChakraCore.h"
#include "ion.h"
#include "uv.h"

typedef struct napi_callback_info__ *napi_callback_info;

namespace ion {
namespace core {
namespace process
{
  static void DefineArgs(JsValueRef process, int argc, char** argv) {
    napi_value js_argv;
    ion_create_array(&js_argv);
    for (int i = 0; i < argc; i++) {
      napi_value str;
      ion_create_string_utf8(&str, argv[i]);
      ion_set_array_at_index(js_argv, i, str);
    }
    ion_define(process, "argv", js_argv);
  }

  static void CreateEnv(JsValueRef global, int argc, char** argv) {
    napi_value process;
    ion_create_object(&process);

    napi_value env;
    ion_create_object(&env);

#ifdef WIN32
    char buf[MAX_PATH];
#else

    char buf[PATH_MAX];
#endif
    size_t cwd_len = sizeof(buf);
    int err = uv_cwd(buf, &cwd_len);
    if (err)
      buf[0] = '\0';
    ion_define_string_utf8(env, "PWD", buf);

    DefineArgs(process, argc, argv);

    ion_define(process, "env", env);

    ion_define(global, "process", process);
  }

  void Init(JsValueRef env, int argc, char** argv) {
    CreateEnv(env, argc, argv);
  }
}}}

