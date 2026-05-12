#pragma once

#include "vhplatform.hpp"
#include "sym8x8.hpp"
#include "dbg.hpp"

struct psmeta {
    uint8_t cpointx : 2;    // Values: 2, 3, 4, 5
    uint8_t cpointy : 2;    // Values: 2, 3, 4, 5
    uint8_t wdtx    : 1;    // Values: 1, 2
    uint8_t wdty    : 1;    // Values: 1, 2
    uint8_t spcx    : 1;    // Values: 1, 2
    uint8_t spcy    : 1;    // Values: 1, 2
};


class GlyphMeta {

    public:

        void FindCenterPointSingle(const sym8x8 & objsym) { find_cpnt_single(objsym, &smeta); }
        void FindCenterPointDouble(const sym8x8 & objsym) { find_cpnt_double(objsym, &smeta); }

        uint8_t     cpx()   const { return 2 + smeta.cpointx; }
        uint8_t     cpy()   const { return 2 + smeta.cpointy; }
        uint8_t     wx()    const { return smeta.wdtx + 1; }
        uint8_t     wy()    const { return smeta.wdty + 1; }
        uint8_t     spx()   const { return smeta.spcx + 1; }
        uint8_t     spy()   const { return smeta.spcy + 1; }

        bool        wx2()   const { return smeta.wdtx; }
        bool        wy2()   const { return smeta.wdty; }

    private:

        psmeta smeta;

        //------------------------------------------------------------------------------
        // Detect center point for single pseudo graph
        //------------------------------------------------------------------------------
        void find_cpnt_single(const sym8x8 & cpnt, psmeta * pmeta) {

            uint8_t x = cpnt.row_xposl(0);
            uint8_t y = cpnt.col_yposu(0);

            pmeta->cpointx = x - 2;
            pmeta->cpointy = y - 2;
            pmeta->wdtx = cpnt.getpxl(x+1, 0) ? 1:0;
            pmeta->wdty = cpnt.getpxl(0, y+1) ? 1:0;
            pmeta->spcx = 0;
            pmeta->spcy = 0;

            dump_psmeta("*** Pseudo glyphs single center point:", pmeta); }

        //------------------------------------------------------------------------------
        // Detect center point for double pseudo graph
        //------------------------------------------------------------------------------
        void find_cpnt_double(const sym8x8 & cpnt, psmeta * pmeta) {

            uint8_t xl = cpnt.row_xposl(0);
            uint8_t xr = cpnt.row_xposr(0);
            uint8_t x  = xl + (xr - xl)/2;
            uint8_t wx = x - xl;

            uint8_t yu = cpnt.col_yposu(0);
            uint8_t yd = cpnt.col_yposd(0);
            uint8_t y  = yu + (yd - yu)/2;
            uint8_t wy = y - yu;

            pmeta->cpointx = x  - 2;
            pmeta->cpointy = y  - 2;
            pmeta->wdtx    = wx - 1;
            pmeta->wdty    = wy - 1;
            pmeta->spcx    = cpnt.colvalid(x+1) ? 0:1;
            pmeta->spcy    = cpnt.rowvalid(y+1) ? 0:1;

            dump_psmeta("*** Pseudo glyphs double center point:", pmeta); }

    //------------------------------------------------------------------------------
    //
    //------------------------------------------------------------------------------
    void dump_psmeta(const std::string & msg, psmeta * pmeta) {
        dbg_msg(msg); // Dump info string
        dbg_i2t("x : y  %d:%d", pmeta->cpointx + 2, pmeta->cpointy + 2);
        dbg_i2t("wx:wy  %d:%d", pmeta->wdtx+1, pmeta->wdty+1);
        dbg_i2t("sx:sy  %d:%d", pmeta->spcx+1, pmeta->spcy+1); }

};
