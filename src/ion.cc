#include <string>
#include <iostream>

#include "utils.h"
#include "runtime.h"
#include "natives.h"

// typedef ion::core::runtime::Runtime Runtime;


int main(int argc, char** argv) {

  uv_loop_t * loop = uv_default_loop();
  char cout[] = "hello";
  loop->data = cout;

  uv_loop_configure(uv_default_loop(), UV_LOOP_BLOCK_SIGNAL, SIGPROF);
  
  std::auto_ptr<ion::core::runtime::Runtime> runtime (new ion::core::runtime::Runtime());

  runtime->Init(argc, argv);
  // if (runtime->HasError()) goto native_js_error;

  runtime->RunScript(ion::native_bootstrap_ion, std::string("bootstrap_ion.js"));
  runtime->RunScript(ion::native_fs, std::string("fs.js"));
  runtime->RunScript(ion::native_module, std::string("module.js"));

  if (runtime->HasError()) goto native_js_error;

  

  uv_run(uv_default_loop(), UV_RUN_DEFAULT);


  return 0;
native_js_error:
  // ReportExcetion(runtime->GetException())
  runtime->GetException();

#ifdef _WIN32
  system("pause");
#endif
  return 0;
}
