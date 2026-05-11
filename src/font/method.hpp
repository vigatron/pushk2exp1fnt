#pragma once

#include <stdint.h>

enum emethod {
    opN = 0,
    opH,    // Horizontal
    opV,    // Vertical
    opA,    // Any ?
    opG,    // Pseudo-graph
    op_last };

uint8_t getmethod(uint8_t symn);
