#include "genbmp.hpp"
#include "colors.hpp"

void saveBMP(const std::string& filename,
             const std::vector<uint32_t>& pixels,
             int width, int height)
{
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    infoHeader.biWidth  = width;
    infoHeader.biHeight = height;

    // каждая строка должна быть выровнена по 4 байта
    int rowSize = ((width * 3 + 3) / 4) * 4;
    infoHeader.biSizeImage = rowSize * height;
    fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;

    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Не удалось открыть файл для записи");
    }

    // заголовки
    out.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    out.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    // пиксели (BMP хранит строки снизу вверх)
    std::vector<uint8_t> row(rowSize);
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            uint32_t color = pixels[y * width + x];
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = (color >> 0) & 0xFF;
            row[x * 3 + 0] = b;
            row[x * 3 + 1] = g;
            row[x * 3 + 2] = r;
        }
        out.write(reinterpret_cast<char*>(row.data()), rowSize);
    }
}

//
verr genbmp( CELLS256 & c256, const std::string & fname, bool forcebw ) {

    int width = 256;
    int height = 64;
    std::vector<uint32_t> pixels(width * height);

    for(int symn=0;symn<256;symn++) {
        for(int y=0;y<8;y++) {
            for(int x=0;x<8;x++) {
                int cx = (symn % 32 ) * 8 + x;
                int cy = (symn / 32 ) * 8 + y;
                int offs = cx + cy * 256;
                uint8_t color8bit = c256.getPixel(symn, x, y);
                int colorbw  = color8bit ? 0xFFFFFF : 0;
                int colorrgb = VHCLR256::clr32(color8bit);
                pixels[offs] = forcebw ? colorbw : colorrgb; } } }

    saveBMP(fname, pixels, width, height);

    return vok; }