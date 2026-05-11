#pragma once

#include <filesystem>
#include <fstream>

#include <iostream>
#include <vector>

bool fileExists(const std::string &path);
bool saveArrayToFile(const std::vector<uint8_t> & arr, const std::string& filename);

std::string replaceext(const std::string& filename, const std::string & newext);
std::string AppendToBaseFileName(const std::string& filename, const std::string & addtxt);
