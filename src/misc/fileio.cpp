#include "fileio.hpp"

//------------------------------------------------------------------------------
bool fileExists(const std::string &path) {
  try {
    return std::filesystem::exists(path) &&
           std::filesystem::is_regular_file(path);
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Ошибка доступа: " << e.what() << '\n';
    return false; } }

//------------------------------------------------------------------------------
bool saveArrayToFile(const std::vector<uint8_t> & arr, const std::string & filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) return false;
    out.write(reinterpret_cast<const char*>(arr.data()), arr.size());
    return out.good(); }

//------------------------------------------------------------------------------
std::string replaceext(const std::string& filename, const std::string & newext) {
    std::string r;
    return r;
}

//------------------------------------------------------------------------------
std::string AppendToBaseFileName(const std::string& filename, const std::string & addtxt) {
    std::string r;
    return r; }
