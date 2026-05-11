#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

class BinaryBuffer {
public:
    BinaryBuffer() = default;

    BinaryBuffer(const void* data, size_t size) {
        if (!data && size != 0) {
            throw std::invalid_argument("Null data with non-zero size");
        }

        if (size == 0) return;

        allocate(size);
        std::memcpy(buffer_.get(), data, size);
    }

    BinaryBuffer(const BinaryBuffer& other) {
        if (other.size_ == 0) return;

        allocate(other.size_);
        std::memcpy(buffer_.get(), other.buffer_.get(), other.size_);
    }

    BinaryBuffer(BinaryBuffer&& other) noexcept {
        moveFrom(std::move(other));
    }

    BinaryBuffer& operator=(const BinaryBuffer& other) {
        if (this == &other) return *this;

        BinaryBuffer tmp(other);
        swap(tmp);
        return *this;
    }

    BinaryBuffer& operator=(BinaryBuffer&& other) noexcept {
        if (this == &other) return *this;

        clear();
        moveFrom(std::move(other));
        return *this;
    }

    ~BinaryBuffer() {
        clear();
    }

    // ============================================================
    // LOAD
    // ============================================================
    bool loadFromFile(const std::string& filename) {
        clear();

        if (filename.empty()) return false;

        FILE* file = std::fopen(filename.c_str(), "rb");
        if (!file) return false;

        if (std::fseek(file, 0, SEEK_END) != 0) {
            std::fclose(file);
            return false;
        }

        long fileSize = std::ftell(file);
        if (fileSize < 0) {
            std::fclose(file);
            return false;
        }

        if (static_cast<unsigned long>(fileSize) > maxAllowedSize()) {
            std::fclose(file);
            return false;
        }

        if (fileSize == 0) {
            std::fclose(file);
            return true;
        }

        std::rewind(file);

        allocate(static_cast<size_t>(fileSize));

        size_t readBytes = std::fread(buffer_.get(), 1, size_, file);

        std::fclose(file);

        if (readBytes != size_) {
            clear();
            return false;
        }

        return true;
    }

    // ============================================================
    // SAVE
    // ============================================================
    bool saveToFile(const std::string& filename) const {
        if (filename.empty()) return false;

        if (!buffer_ && size_ != 0) return false;

        FILE* file = std::fopen(filename.c_str(), "wb");
        if (!file) return false;

        if (size_ == 0) {
            std::fclose(file);
            return true;
        }

        size_t written = std::fwrite(buffer_.get(), 1, size_, file);

        if (written != size_) {
            std::fclose(file);
            return false;
        }

        if (std::fclose(file) != 0) {
            return false;
        }

        return true;
    }

    // --- Доступ ---
    const uint8_t* data() const { return buffer_.get(); }
    uint8_t* data() { return buffer_.get(); }

    const uint8_t* raw() const { return buffer_.get(); }
    uint8_t* raw() { return buffer_.get(); }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    uint8_t at(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return buffer_.get()[index];
    }

    // --- Клонирование ---
    BinaryBuffer clone() const {
        return BinaryBuffer(*this);
    }

    BinaryBuffer cloneRange(size_t offset, size_t length) const {
        if (offset > size_) {
            throw std::out_of_range("Offset out of range");
        }

        if (length == 0) {
            return BinaryBuffer();
        }

        if (offset + length > size_) {
            throw std::out_of_range("Range exceeds buffer size");
        }

        BinaryBuffer result;
        result.allocate(length);

        std::memcpy(result.buffer_.get(), buffer_.get() + offset, length);

        return result;
    }

    void clear() {
        buffer_.reset();
        size_ = 0;
    }

private:
    std::unique_ptr<uint8_t[]> buffer_{nullptr};
    size_t size_ = 0;

private:
    void allocate(size_t size) {
        if (size == 0) return;

        if (size > maxAllowedSize()) {
            throw std::overflow_error("Allocation too large");
        }

        buffer_.reset(new uint8_t[size]);
        size_ = size;
    }

    void moveFrom(BinaryBuffer&& other) {
        buffer_ = std::move(other.buffer_);
        size_ = other.size_;
        other.size_ = 0;
    }

    void swap(BinaryBuffer& other) noexcept {
        std::swap(buffer_, other.buffer_);
        std::swap(size_, other.size_);
    }

    static size_t maxAllowedSize() {
        return static_cast<size_t>(1024ULL * 1024ULL * 1024ULL); // 1 GB
    }
};