#ifndef ION_CORE_UTILS
#define ION_CORE_UTILS


#include <vector>
#include <cstdio>
#include <string>
#include "unicode/utypes.h"

namespace ion {
namespace core{
namespace utils{

  inline bool ReadFile(const std::string& filename, std::vector<UChar>* out) {
    if (std::FILE* fp = fopen(filename.c_str(), "rb")) {
      std::fseek(fp, 0L, SEEK_END);
      const std::size_t filesize = std::ftell(fp);
      if (filesize) {
        std::rewind(fp);
        const std::size_t offset = out->size();
        out->resize(offset + filesize);
        if (std::fread(out->data()+ offset, filesize, 1, fp) < 1) {
          const std::string err = "ion can't read \"" + filename + "\"";
          std::perror(err.c_str());
          std::fclose(fp);
          return false;
        }
        std::fclose(fp);
        return true;
      }
    }
    const std::string err = "ion can't open \"" + filename + "\"";
    std::perror(err.c_str());
    return false;
  }
  
}}}

#endif