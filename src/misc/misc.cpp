#include "vhplatform.hpp"

// std::string ospath(const std::string & path, const std::string & fname);

//------------------------------------------------------------------------------
int verror(const std::string msg, int errcode) {
  std::cout << "Error: " << errcode;
  std::cout << "  msg: " << msg << std::endl;
  return errcode;
}

//------------------------------------------------------------------------------
std::string ospath(const std::string & path, const std::string & fname) {
    std::string r = path + "/" + fname;
    return r;
}
