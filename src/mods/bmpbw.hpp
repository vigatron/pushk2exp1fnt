#pragma once
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <vector>

class BMPBW {
public:
  // Генерация BMP содержимого из массива arrin (16384 байт = 256x64)
  static std::vector<uint8_t> generate(const std::vector<uint8_t> &arrin) {
    if (arrin.size() != 256 * 64) {
      throw std::runtime_error("arrin must be 16384 bytes (256x64)");
    }

    const int width = 256;
    const int height = 64;

    // Размер строки в байтах с выравниванием на 4
    int rowSize = ((width + 31) / 32) * 4;
    int dataSize = rowSize * height;

    // Заголовки BMP
    struct BMPFileHeader {
      uint16_t bfType{0x4D42}; // "BM"
      uint32_t bfSize;
      uint16_t bfReserved1{0};
      uint16_t bfReserved2{0};
      uint32_t bfOffBits;
    } __attribute__((packed));

    struct BMPInfoHeader {
      uint32_t biSize{40};
      int32_t biWidth;
      int32_t biHeight;
      uint16_t biPlanes{1};
      uint16_t biBitCount{1}; // 1 бит на пиксель
      uint32_t biCompression{0};
      uint32_t biSizeImage;
      int32_t biXPelsPerMeter{0};
      int32_t biYPelsPerMeter{0};
      uint32_t biClrUsed{2};
      uint32_t biClrImportant{2};
    } __attribute__((packed));

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    infoHeader.biWidth = width;
    infoHeader.biHeight = height; // положительное значение → снизу вверх
    infoHeader.biSizeImage = dataSize;

    fileHeader.bfOffBits =
        sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + 8; // палитра
    fileHeader.bfSize = fileHeader.bfOffBits + dataSize;

    std::vector<uint8_t> bmp;
    bmp.reserve(fileHeader.bfSize);

    // Вспомогательная функция записи
    auto append = [&](const void *ptr, size_t sz) {
      const uint8_t *p = reinterpret_cast<const uint8_t *>(ptr);
      bmp.insert(bmp.end(), p, p + sz);
    };

    // Заголовки
    append(&fileHeader, sizeof(fileHeader));
    append(&infoHeader, sizeof(infoHeader));

    // Палитра: черный и белый
    uint32_t palette[2] = {0x00000000, 0x00FFFFFF};
    append(palette, sizeof(palette));

    // Пиксели: идём снизу вверх (BMP convention)
    std::vector<uint8_t> row(rowSize, 0);
    for (int y = height - 1; y >= 0; --y) {
      for (int x = 0; x < width; ++x) {
        uint8_t v = arrin[y * width + x];
        if (v != 0) {
          row[x / 8] |= (0x80 >> (x % 8));
        }
      }
      append(row.data(), rowSize);
      std::fill(row.begin(), row.end(), 0);
    }

    return bmp;
  }

  // Сохранение содержимого BMP в файл
  static void saveToFile(const std::string &filename,
                         const std::vector<uint8_t> &bmp) {
    std::ofstream fout(filename, std::ios::binary);
    if (!fout)
      throw std::runtime_error("Cannot open file for writing");
    fout.write(reinterpret_cast<const char *>(bmp.data()), bmp.size());
  }
};

// Пример: нарисуем диагональ
//   int x, y;
//   for (int i = 0; i < 256; ++i) {
//     x = i;
//     y = i / 8;
//     arr[y * 256 + x] = 1; // несколько точек
//   }

// Пример: нарисуем 4 точки в углах
//   arr[256 * 0 + 0] = 1;
//   arr[256 * 0 + 255] = 1;
//   arr[256 * 63 + 0] = 1;
//   arr[256 * 63 + 255] = 1;
