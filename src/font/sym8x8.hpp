#pragma once

#include "vhplatform.hpp"
#include "enumsymm.hpp"
#include "method.hpp"

enum endirs { enup=0, endown, endleft, enright };


class sym8x8 {

    public:

        sym8x8() { }
        ~sym8x8() { }

        void set(const std::vector<uint8_t> & src)  { arr = src; }

        //-----------------------------------------------------------------------------------------------------
        uint8_t     offs    (uint8_t x, uint8_t y)  const       { return y*8+x; }
        uint8_t     getpxl  (uint8_t x, uint8_t y)  const       { return arr[offs(x,y)]; }

        std::vector<uint8_t> get()                  const       { return arr; }

        void        setpxl  (uint8_t x, uint8_t y, uint8_t c)   { arr[offs(x,y)] = c; }
        void        invpxl  (uint8_t x, uint8_t y)              { setpxl(x, y, getpxl(x, y) ? 0 : 1); }

        //-----------------------------------------------------------------------------------------------------
        void applyfilter( enattr flt) {
            switch(flt) {
                case hattr404:  apply_flt_h404(); break;
                case hattr313:  apply_flt_h313(); break;
                case hattr303:  apply_flt_h303(); break;
                case vattr313:  apply_flt_v313(); break;
                case vattr413:  apply_flt_v413(); break;
                case vattr404:  apply_flt_v404(); break;
                default: break; } }

        //-----------------------------------------------------------------------------------------------------
        uint8_t countFilled() {
            uint8_t r=0;
            for(uint8_t s:arr) { r += s ? 1:0; }
            return r; }

        //-----------------------------------------------------------------------------------------------------
        bool checkline(uint8_t y) {
            for(int i=y*8; i<(y*8+8);i++)
                if(arr[i]) return true;
            return false; }

        //-----------------------------------------------------------------------------------------------------
        uint8_t countLines() {
            uint8_t r = 0;
            for(uint8_t y=0;y<8;y++)
                if(checkline(y)) r++;
            return r; }

        //-----------------------------------------------------------------------------------------------------
        bool rowvalid(uint8_t y) const { for(uint8_t x=0;x<8;x++) if(getpxl(x,y)) return true; return false; }

        //-----------------------------------------------------------------------------------------------------
        bool colvalid(uint8_t x) const { for(uint8_t y=0;y<8;y++) if(getpxl(x,y)) return true; return false; }

        //-----------------------------------------------------------------------------------------------------
        uint8_t row_xposl(uint8_t y) const { for(int x=0;x<8;x++) if(getpxl(x,y)) return x; return 8; }
        uint8_t row_xposr(uint8_t y) const { for(int x=0;x<8;x++) if(getpxl(7-x,y)) return 7-x; return 8; }

        //-----------------------------------------------------------------------------------------------------
        uint8_t col_yposu(uint8_t x) const { for(int y=0;y<8;y++) if(getpxl(x,y)) return y; return 8; }
        uint8_t col_yposd(uint8_t x) const { for(int y=0;y<8;y++) if(getpxl(x,7-y)) return 7-y; return 8; }

        //-----------------------------------------------------------------------------------------------------
        uint8_t firstcol() {
            for(uint8_t x=0;x<8;x++)
                if(colvalid(x)) return x;
            return 8; }


        //-----------------------------------------------------------------------------------------------------
        enattr HorizontalFilter() {
            enattr r = noattr;
            sym8x8 tmp8x8;
            enattr tryattr;
            uint8_t pxcnt = countFilled();
            uint8_t fcol  = firstcol();

            enattr tbl[] = { hattr404, hattr313, hattr303 };

            for( int i = 0; i < 3; i++) {
                tryattr = tbl[i];
                tmp8x8.set(arr);

                // tmp8x8.debug();
                tmp8x8.applyfilter(tryattr);
                // tmp8x8.debug();

                uint8_t newcnt = tmp8x8.countFilled();
                uint8_t fcolnew = tmp8x8.firstcol();
                if(fcolnew > fcol)  { fcol = fcolnew; r = tryattr; }
                // else                { if(newcnt<pxcnt) { pxcnt = newcnt; r = tryattr; } }
                }

            return r; }

        //-----------------------------------------------------------------------------------------------------
        enattr VerticalFilter() {
            enattr r = noattr;
            sym8x8 tmp8x8;
            enattr tryattr;
            uint8_t linescnt = countLines();
            enattr tbl[] = { vattr404, vattr313, vattr413 };

            for( int i = 0; i < 3; i++) {
                tryattr = tbl[i];
                tmp8x8.set(arr);
                tmp8x8.applyfilter(tryattr);
                int newcnt = tmp8x8.countLines();
                if(newcnt < linescnt) { linescnt = newcnt; r = tryattr; } }

            return r; }

        //-----------------------------------------------------------------------------------------------------
        // symn need for filter selection
        //-----------------------------------------------------------------------------------------------------
        enattr FindOptimalFilter(uint8_t symn) {
            enattr r = noattr;

            // Debug
            if(symn == 127) {
                asm("nop");
            }

            uint8_t m = getmethod(symn);
            switch(m) {
                case opH : return HorizontalFilter(); break;
                case opV : return VerticalFilter(); break;
                default : break; }
            return r; }


    private:

        std::vector<uint8_t> arr;

        // Horizontal filters

        void apply_flt_h404() {
            for (int y = 0; y < 8; y++) {
                for (int x = 4; x < 8; x++) {
                    uint8_t pixl = getpxl(x, y), posx = 3 - (x - 4);
                    if (pixl) { invpxl(posx, y); } } } }

        void apply_flt_h313() {
            for (int y = 0; y < 8; y++) {
                for (int x = 4; x < 7; x++) {
                    uint8_t pixl = getpxl(x, y), posx = 2 - (x - 4);
                    if (pixl) { invpxl(posx, y); } } } }

        void apply_flt_h303() {
            for (int y = 0; y < 6; y++) {
                for (int x = 3; x < 6; x++) {
                    uint8_t pixl = getpxl(x, y), posx = 2 - (x - 3);
                    if (pixl) { invpxl(posx, y); } } } }

        // Vertical filters

        void apply_flt_v313() {
            for(int x=0;x<8;x++) {
                for(int y=4; y<7;y++) {
                    uint8_t pixl = getpxl(x, y), posy = 2 - (y - 4);
                    if (pixl) { invpxl(x, posy); } } } }

        void apply_flt_v413() {
            for(int x=0;x<8;x++) {
                for(int y=5; y<8;y++) {
                    uint8_t pixl = getpxl(x, y), posy = 3 - (y - 5);
                    if (pixl) { invpxl(x, posy); } } } }

        void apply_flt_v404() {
            for(int x=0;x<8;x++) {
                for(int y=4; y<8;y++) {
                    uint8_t pixl = getpxl(x, y), posy = 3 - (y - 4);
                    if (pixl) { invpxl(x, posy); } } } }

};
