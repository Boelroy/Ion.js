#include "ChakraCore.h"
#include <string>
#include "Utils.hpp"
#include "Runtime.hpp"
#include "unicode/utypes.h"

#define FAIL_CHECK(cmd)                             \
 do                                                 \
 {                                                  \
    JsErrorCode errCode = cmd;                      \
    if (errCode != JsNoError)                       \
    {                                               \
      printf("Error %d at '%s' \n", errCode, #cmd); \
    }                                               \
 } while(0)

int main(int argc, char** argv) {

  if (argc < 2) {
    std::perror("you should output a javascript file path");
    return 1;
  }
  std::vector<UChar> src;
  char* filename = argv[1];
  ion::core::utils::ReadFile(std::string(argv[1]), &src);

  ion::core::runtime::Init();
  
  return 0;
}