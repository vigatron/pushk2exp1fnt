#pragma once

#include "vhplatform.hpp"
#include "c256cell.hpp"

#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t bfType{0x4D42};   // "BM"
    uint32_t bfSize{0};        // размер файла
    uint16_t bfReserved1{0};
    uint16_t bfReserved2{0};
    uint32_t bfOffBits{54};    // смещение до пиксельных данных
};

struct BMPInfoHeader {
    uint32_t biSize{40};       // размер структуры
    int32_t  biWidth{0};
    int32_t  biHeight{0};
    uint16_t biPlanes{1};
    uint16_t biBitCount{24};   // RGB 8:8:8
    uint32_t biCompression{0};
    uint32_t biSizeImage{0};
    int32_t  biXPelsPerMeter{0};
    int32_t  biYPelsPerMeter{0};
    uint32_t biClrUsed{0};
    uint32_t biClrImportant{0};
};
#pragma pack(pop)

verr genbmp( CELLS256 & c256, const std::string & fname , bool forcebw=false);

void saveBMP(const std::string& filename,
             const std::vector<uint32_t>& pixels,
             int width, int height);
