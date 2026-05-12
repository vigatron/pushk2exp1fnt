#pragma once

#include <vector>
#include <string>

void dbg_msg(const std::string & msg);
void dbg_i1t(const std::string & msg, int v1);
void dbg_i2t(const std::string & msg, int v1, int v2);
void dbg_8x8(std::vector<uint8_t> arr);
