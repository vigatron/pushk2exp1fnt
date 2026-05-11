#pragma once
#include <vector>
#include <cstdint>
#include <fstream>

class CELLS256 {
public:
    static constexpr int CELL_W = 8;
    static constexpr int CELL_H = 8;
    static constexpr int CELL_SIZE = CELL_W * CELL_H; // 64
    static constexpr int CELL_COUNT = 256;

    static constexpr int ROW_BLOCK_SIZE = CELL_COUNT * CELL_W; // 256 * 8 = 2048
    static constexpr int TOTAL_SIZE = ROW_BLOCK_SIZE * CELL_H; // 2048 * 8 = 16384

    CELLS256() : arr(TOTAL_SIZE, 0) {}

    // Прямой доступ к данным
    uint8_t* data() { return arr.data(); }
    const uint8_t* data() const { return arr.data(); }
    size_t size() const { return arr.size(); }

    // Установка пикселя
    void setPixel(int cellIndex, int x, int y, uint8_t val) {
        if (cellIndex < 0 || cellIndex >= CELL_COUNT) return;
        if (x < 0 || x >= CELL_W) return;
        if (y < 0 || y >= CELL_H) return;

        size_t offs =
            static_cast<size_t>(y) * ROW_BLOCK_SIZE +
            static_cast<size_t>(cellIndex) * CELL_W +
            static_cast<size_t>(x);

        arr[offs] = val;
    }

    // Чтение пикселя
    uint8_t getPixel(int cellIndex, int x, int y) const {
        if (cellIndex < 0 || cellIndex >= CELL_COUNT) return 0;
        if (x < 0 || x >= CELL_W) return 0;
        if (y < 0 || y >= CELL_H) return 0;

        size_t offs =
            static_cast<size_t>(y) * ROW_BLOCK_SIZE +
            static_cast<size_t>(cellIndex) * CELL_W +
            static_cast<size_t>(x);

        return arr[offs];
    }

    // Загрузка из файла
    bool loadFromFile(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary);
        if (!in.is_open()) return false;

        in.read(reinterpret_cast<char*>(arr.data()), arr.size());
        return in.good();
    }

    // Сохранение в файл
    bool saveToFile(const std::string& filename) const {
        std::ofstream out(filename, std::ios::binary);
        if (!out.is_open()) return false;

        out.write(reinterpret_cast<const char*>(arr.data()), arr.size());
        return out.good();
    }

private:
    std::vector<uint8_t> arr;
};
