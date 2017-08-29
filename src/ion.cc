#include <string>
#include <iostream>

#include "utils.h"
#include "runtime.h"

// typedef ion::core::runtime::Runtime Runtime;

int main(int argc, char** argv) {
  if (argc < 2) {
    std::perror("you should output a javascript file path");
    return 1;
  }
  std::vector<char> src;
  char* filename = argv[1];
  ion::core::utils::ReadFile(std::string(argv[1]), &src);

  std::auto_ptr<ion::core::runtime::Runtime> runtime (new ion::core::runtime::Runtime());

  runtime->RunScript(src, std::string(filename));
  return 0;
}
