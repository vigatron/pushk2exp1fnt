#pragma once

#include <cstdint>

namespace VHCLR256 {

#define CNSU8_t  constexpr inline uint8_t
#define CNSU32_t constexpr inline uint32_t

CNSU8_t  col2b(uint32_t x, uint8_t shl) noexcept { return (x >> shl) & 0x3; }
CNSU8_t  colDbl(uint8_t c2b)            noexcept { return (c2b << 2) | c2b; }
CNSU8_t  colQrt(uint8_t c4b)            noexcept { return (c4b << 4) | c4b; }
CNSU8_t  colXor(uint32_t x)             noexcept { return (((x >> 5) & 0x4) | ((x >> 5) & 0x2)); }
CNSU8_t  colF8(uint32_t x, uint8_t n)   noexcept { return colQrt(colDbl(col2b(x, n)) ^ colXor(x)); }
CNSU8_t  clrR8(uint32_t x)              noexcept { return colF8(x, 0); }
CNSU8_t  clrG8(uint32_t x)              noexcept { return colF8(x, 2); }
CNSU8_t  clrB8(uint32_t x)              noexcept { return colF8(x, 4); }
CNSU32_t clrSh(uint32_t x, uint8_t n)   noexcept { return x << n; }
CNSU32_t clr32(uint32_t x)              noexcept { return clrSh(clrR8(x), 0) | clrSh(clrG8(x), 8) | clrSh(clrB8(x), 16); }
CNSU32_t swapRB(uint32_t x)             noexcept { return ((x & 0xFF) << 16) | (x & 0xFF00) | ((x >> 16) & 0xFF); }

};