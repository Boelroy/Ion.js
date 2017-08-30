#include <string>
#include <iostream>

#include "utils.h"
#include "runtime.h"
#include "natives.h"

// typedef ion::core::runtime::Runtime Runtime;

int main(int argc, char** argv) {

  std::vector<char> bootstrapSource;
  

  std::auto_ptr<ion::core::runtime::Runtime> runtime (new ion::core::runtime::Runtime());

  runtime->Init(argc, argv);
  runtime->RunScript(ion::native_bootstrap_ion, std::string("bootstrap_ion.js"));

#ifdef _WIN32
  system("pause");
#endif

  return 0;
}
