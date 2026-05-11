#pragma once

// метод преобразования символа 8x8 пикселей
// -----------------------------------------

enum enattr {

    noattr = 0,         // #0   8 pixels | no split

    // No axis - horizontal split into 2 equal pieces
    hsplit,             // #1   8 pixels | split

    // Horizontal axis
    hattr404,           // #2   8 cols, 4 xored | ++++####
    hattr313,           // #3   7 cols, 3 xored | +++A###.
    hattr303,           // #4   6 cols, 3 xored | +++###..

    // Vertical axis
    vattr313,           // #5   7 rows, 3 xored
    vattr413,           // #6   8 rows, 3 xored
    vattr404,           // #7   8 rows, 4 xored

    enattr_last         // Not used
};
