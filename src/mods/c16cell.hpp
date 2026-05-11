#pragma once

#include "bmpbw.hpp"
#include "misc/fileio.hpp"
#include "misc/membuff.hpp"
#include "vhplatform.hpp"

#include <vector>

#include "enumsymm.hpp"


class c16cell {

    public:

    void set(const c16cell & src ) { blk = src.blk; }

    //------------------------------------------------------------------------------
    bool loadFromFile(const std::string &filename) {
        bool r = blk.loadFromFile(filename);
        return r;
    }

    //------------------------------------------------------------------------------
    bool saveToFile(const std::string &filename) {
        bool r = blk.saveToFile(filename);
        return r;
    }

    //------------------------------------------------------------------------------
    int getpxl(uint8_t symn, uint8_t x, uint8_t y) const {
        uint16_t offs;
        uint8_t half;
        c16calcpos(symn, x, y, &offs, &half);
        uint8_t r = *(blk.data() + offs); // read 2pxls source byte
        return (half ? (r >> 4) : r) & 0x0F;
    }

    //------------------------------------------------------------------------------
    void setpxl(uint8_t sym, uint8_t x, uint8_t y, uint8_t v) {
        uint16_t offs;
        uint8_t half;
        c16calcpos(sym, x, y, &offs, &half);
        uint8_t r = *(blk.data() + offs);
        v &= 0x0F;
        if (half)   { r &= 0x0F; r |= v << 4; }
        else        { r &= 0xF0; r |= v; }
        // write 2pxls result byte
        *(blk.data() + offs) = r; }

    //------------------------------------------------------------------------------
    std::vector<uint8_t> getsinglecell(uint8_t symn) const {
        std::vector<uint8_t> r;
        r.reserve(64);
        for(int y=0;y<8;y++) {
            for(int x=0;x<8;x++) { r.push_back( getpxl(symn,x,y) ? 1 : 0 ); } }
        return r; }

    //------------------------------------------------------------------------------
    void applyxor(uint8_t symn, const std::vector<uint8_t> & xorarr) {
        int offs;
        for(int j=0;j<8;j++) {
            for(int i=0;i<8;i++) {
                offs = j*8+i;
                uint8_t srcpxl = getpxl(symn, i, j);
                if(xorarr[offs]) setpxl(symn, i, j, srcpxl?0:0x0F); } } }

    //------------------------------------------------------------------------------
    void setsym(uint8_t symn, const std::vector<uint8_t> & arr) {
        for(int y=0;y<8;y++) {
            for(int x=0;x<8;x++) {
                uint8_t offs = y * 8 + x, c = arr[offs];
                setpxl(symn,x,y, c ? 0x0F : 0 ); } } }

    //------------------------------------------------------------------------------
    void restore() {

    }

    //------------------------------------------------------------------------------
    void saveasbmpbw(const std::string fname) const {

        std::vector<uint8_t> arr(256 * 64, 0);

        //
        for (int symn = 0; symn < 256; symn++) {
            int symx = symn % 32;
            int symy = symn / 32;
            for (int pixy = 0; pixy < 8; pixy++) {
            for (int pixx = 0; pixx < 8; pixx++) {
                int pixv = getpxl(symn, pixx, pixy);
                int bmpx = symx * 8 + pixx;
                int bmpy = symy * 8 + pixy;
                int bmpo = bmpy * 256 + bmpx;
                arr[bmpo] = pixv ? 1 : 0;
                // printf("SYMN=%d [%d:%d] PIX [%d:%d] OFFS=%d\n", symn, symx, symy,
                // bmpx, bmpy, bmpo);
            }
        }
    }

    auto bmp = BMPBW::generate(arr);
    BMPBW::saveToFile(fname, bmp);
    }

private:
    //
    BinaryBuffer blk;
    //  std::vector<enattr> arratt;

    //
    void c16calcpos(uint8_t sym, uint8_t x, uint8_t y, uint16_t *offs, uint8_t *half) const {
        int pos = 6;
        pos += x >> 1;
        pos += y * F1K;
        pos += sym * 4;
        *offs = pos;
        *half = (x & 1) ^ 1;
    }
};
