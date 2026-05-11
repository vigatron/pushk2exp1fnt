#pragma once

#include "vhplatform.hpp"
#include "sym8x8.hpp"
#include "pgt.hpp"


#define CENTER_POINT_SYM_S  197
#define CENTER_POINT_SYM_D  206


// --------------
// external
// --------------

void dbg_msg(const std::string & msg);
void dbg_i1t(const std::string & msg, int v1);
void dbg_i2t(const std::string & msg, int v1, int v2);
void dbg_8x8(std::vector<uint8_t> arr);


// --------------
// external
// --------------

class PGlyph {

    public:

    enum enpart { ennone=0, ensin, endbl };
    enum enline { eNoLine=0, eSingleLine, eDoubleLine };

    struct psmeta {
        uint8_t cpointx : 2;    // 2, 3, 4, 5
        uint8_t cpointy : 2;    // 2, 3, 4, 5
        uint8_t wdtx : 1;       // 1, 2
        uint8_t wdty : 1;       // 1, 2
        uint8_t spcx : 1;       // 1, 2
        uint8_t spcy : 1;       // 1, 2
    };

    psmeta sl;
    psmeta dl;

    //------------------------------------------------------------------------------
    // Detect center point for single pseudo graph
    //------------------------------------------------------------------------------
    void find_cpnt_single(const sym8x8 & cpnt) {
        uint8_t x = cpnt.row_xposl(0);
        uint8_t y = cpnt.col_yposu(0);
        sl.cpointx = x - 2;
        sl.cpointy = y - 2;
        sl.wdtx = cpnt.getpxl(x+1, 0) ? 1:0;
        sl.wdty = cpnt.getpxl(0, y+1) ? 1:0;
        dump_psmeta("*** Pseudo glyphs single center point:", sl); }

    //------------------------------------------------------------------------------
    // Detect center point for double pseudo graph
    //------------------------------------------------------------------------------
    void find_cpnt_double(const sym8x8 & cpnt) {

        uint8_t xl = cpnt.row_xposl(0);
        uint8_t xr = cpnt.row_xposr(0);
        uint8_t x  = xl + (xr - xl)/2;
        uint8_t wx = x - xl;

        uint8_t yu = cpnt.col_yposu(0);
        uint8_t yd = cpnt.col_yposd(0);
        uint8_t y  = yu + (yd - yu)/2;
        uint8_t wy = y - yu;

        dl.cpointx = x  - 2;
        dl.cpointy = y  - 2;
        dl.wdtx    = wx - 1;
        dl.wdty    = wy - 1;
        dl.spcx    = cpnt.colvalid(x+1) ? 0:1;
        dl.spcy    = cpnt.rowvalid(y+1) ? 0:1;

        dump_psmeta("*** Pseudo glyphs double center point:", dl); }

    //------------------------------------------------------------------------------
    //
    //------------------------------------------------------------------------------
    const std::vector<uint8_t> & recreate(uint8_t symn) {
        
        arr.assign(64, 0);

        if(symn < SYM_PSEUDO_FIRST) return arr;
        if(symn > SYM_PSEUDO_LAST ) return arr;

        if(symn==182) {
            asm("nop"); }

        PGT pgt(symn);
        subcreate(pgt);

        dbg_i1t("\nPS-Symn #%d", symn);
        dbg_8x8(arr);
        return arr; }

    //------------------------------------------------------------------------------
    //
    //------------------------------------------------------------------------------
    void dump_psmeta(const std::string & msg, const psmeta & obj) {
        // Dump info
        dbg_msg(msg);
        dbg_i2t("x : y  %d:%d", obj.cpointx + 2, obj.cpointy + 2);
        dbg_i2t("wx:wy  %d:%d", obj.wdtx+1, obj.wdty+1);
        dbg_i2t("sx:sy  %d:%d", obj.spcx+1, obj.spcy+1); }


    private:

        std::vector<uint8_t>    arr{64, 0};

        void setpix(uint8_t x, uint8_t y, uint8_t v) { arr[y*8+x] = v; }

        // Координаты центральной точки (single)
        uint8_t sngcpx() { return 2+sl.cpointx; }
        uint8_t sngcpy() { return 2+sl.cpointy; }

        // Координаты центральной точки (double top-left)
        uint8_t dblcpx() { return 2+dl.cpointx; }
        uint8_t dblcpy() { return 2+dl.cpointy; }

        //
        uint8_t sng_sx()  { return sngcpx(); }
        uint8_t sng_sy()  { return sngcpy(); }

        //
        uint8_t dbl_sx1() { return dblcpx()  - (dl.wdtx+1); }
        uint8_t dbl_ex1() { return dbl_sx1() + dl.wdtx; }
        uint8_t dbl_sx2() { return dblcpx()  + (dl.spcx+1); }
        uint8_t dbl_ex2() { return dbl_sx2() + dl.wdtx; }

        //
        uint8_t dbl_sy1() { return dblcpy()  - (dl.wdty+1); }
        uint8_t dbl_ey1() { return dbl_sy1() + dl.wdty; }

        uint8_t dbl_sy2() { return dblcpy()  + (dl.spcy+1); }
        uint8_t dbl_ey2() { return dbl_sy2() + dl.wdty; }


        // ----------------------------
        // Sng pxls / Draw Single parts
        // ----------------------------

        // sng:  -**--
        void sdraw_hpart(uint8_t y) { setpix(sngcpx(), y, 1); if(sl.wdtx) setpix(sngcpx() + 1, y, 1); }
        // sng: |
        //      *
        //      |
        void sdraw_vpart(uint8_t x) { setpix(x, sngcpy(), 1); if(sl.wdty) setpix(x, sngcpy() + 1, 1); }

        // Горизонтальная отрисовка одиночного фрагмента
        void sdraw_hrange(uint8_t xf, uint8_t xt) { for(int x=xf;x<=xt;x++) sdraw_vpart(x); }

        // Вертикальная отрисовка одиночного фрагмента
        void sdraw_vrange(uint8_t yf, uint8_t yt) { for(int y=yf;y<=yt;y++) sdraw_hpart(y); }

        void sdraw_u() { sdraw_vrange(0, sng_sy()); }
        void sdraw_d() { sdraw_vrange(sng_sy(), 7); }
        void sdraw_l() { sdraw_hrange(0, sng_sx()); }
        void sdraw_r() { sdraw_hrange(sng_sx(), 7); }

        // Отрисовка: горизонталь
        void sdraw_horizontal() { sdraw_hrange(0, 7); }

        // Отрисовка: вертикаль
        void sdraw_vertical()   { sdraw_vrange(0, 7); }

        // ---------------------------
        // Dbl pxls / Draw Double part
        // ---------------------------

        // dbl:  -*-*-
        void ddraw_hpl(uint8_t y)   { setpix(dbl_sx1()+0, y, 1); if(dl.wdtx) setpix(dbl_sx1()+1, y, 1); }
        void ddraw_hpr(uint8_t y)   { setpix(dbl_sx2()+0, y, 1); if(dl.wdtx) setpix(dbl_sx2()+1, y, 1); }
        void ddraw_hpart(uint8_t y) { ddraw_hpl(y); ddraw_hpr(y); }

        // dbl: *
        //      |
        //      *
        void ddraw_vpu(uint8_t x)   { setpix(x, dbl_sy1(), 1); if(dl.wdty) setpix(x, dbl_sy1()+1, 1); }
        void ddraw_vpd(uint8_t x)   { setpix(x, dbl_sy2(), 1); if(dl.wdty) setpix(x, dbl_sy2()+1, 1); }
        void ddraw_vpart(uint8_t x) { ddraw_vpu(x); ddraw_vpd(x); }

        // Горизонтальная отрисовка двойного фрагмента
        void ddraw_hrange(uint8_t xf, uint8_t xt) { for(int x=xf;x<=xt;x++) ddraw_vpart(x); }
        void ddraw_hrangu(uint8_t xf, uint8_t xt) { for(int x=xf;x<=xt;x++) ddraw_vpu(x);   }
        void ddraw_hrangd(uint8_t xf, uint8_t xt) { for(int x=xf;x<=xt;x++) ddraw_vpd(x);   }

        // Вертикальная отрисовка фрагмента
        void ddraw_vrange(uint8_t yf, uint8_t yt) { for(int y=yf;y<=yt;y++) ddraw_hpart(y); }
        void ddraw_vrangl(uint8_t yf, uint8_t yt) { for(int y=yf;y<=yt;y++) ddraw_hpl(y);   } // Левая часть
        void ddraw_vrangr(uint8_t yf, uint8_t yt) { for(int y=yf;y<=yt;y++) ddraw_hpr(y);   } // Правая часть

        // Отрисовка: горизонталь
        void ddraw_horizontal() { ddraw_hrange(0, 7); }
        void ddraw_horzu()      { for(int x=0;x<=7;x++) ddraw_vpu(x); }
        void ddraw_horzd()      { for(int x=0;x<=7;x++) ddraw_vpd(x); }

        // Отрисовка: вертикаль
        void ddraw_vertical()   { ddraw_vrange(0, 7); }
        void ddraw_vertl()      { for(int y=0;y<=7;y++) ddraw_hpl(y); }  // corners
        void ddraw_vertr()      { for(int y=0;y<=7;y++) ddraw_hpr(y); }  // corners

        // отрисовка: крест
        void draw_cross(const PGT & pgt) { }

        //------------------------------------------------------------------------------
        //
        //------------------------------------------------------------------------------
        void subcreateSingle(const PGT & pgt) {

            // HLine
            if(pgt.ishline()) { sdraw_horizontal(); return; }

            // VLine
            if(pgt.isvline()) { sdraw_vertical(); return; }

            // Draw parts
            if(pgt.validu()) { sdraw_u(); }
            if(pgt.validd()) { sdraw_d(); }
            if(pgt.validl()) { sdraw_l(); }
            if(pgt.validr()) { sdraw_r(); } }


        //------------------------------------------------------------------------------
        // 3-Edges Double
        //------------------------------------------------------------------------------
        void subcreate3e(const PGT & pgt) {

            if(pgt.contn_h()) {

                if(pgt.dblu()) {
                    ddraw_hrangu(0, dbl_ex1());
                    ddraw_hrangu(dbl_sx2(), 7);
                    ddraw_horzd();
                    ddraw_vrange(0, dbl_sy1());
                } else {
                    ddraw_hrangd(0, dbl_ex1());
                    ddraw_hrangd(dbl_sx2(), 7);
                    ddraw_horzu();
                    ddraw_vrange(dbl_ey2(), 7);
                }

                return; }

            if(pgt.contn_v()) {

                if(pgt.dbll()) {
                    ddraw_vertr();
                    ddraw_vrangl(0, dbl_sy1());
                    ddraw_vrangl(dbl_sy2(), 7);
                    ddraw_hrange(0, dbl_sx1());
                } else {
                    ddraw_vertl();
                    ddraw_vrangr(0, dbl_sy1());
                    ddraw_vrangr(dbl_sy2(), 7);
                    ddraw_hrange(dbl_sx2(), 7);
                }

                return; }
        }

        //------------------------------------------------------------------------------
        //
        //------------------------------------------------------------------------------
        void subcreateDouble(const PGT & pgt) {

            // HLine
            if(pgt.ishline()) { ddraw_horizontal(); return; }

            // VLine
            if(pgt.isvline()) { ddraw_vertical(); return; }

            // 3-edges
            if(pgt.is3edgs()) { subcreate3e(pgt); return; }

            // -------
            // Corners
            // -------

            if(pgt.iscorn1()) {
                ddraw_hrangu(0, dbl_ex1());
                ddraw_hrangd(0, dbl_ex2());
                ddraw_vrangl(0, dbl_sy1());
                ddraw_vrangr(0, dbl_sy2());
                return; }

            if(pgt.iscorn3()) {
                ddraw_hrangu(dbl_sx2(), 7);
                ddraw_hrangd(dbl_sx1(), 7);
                ddraw_vrangl(0, dbl_sy2());
                ddraw_vrangr(0, dbl_sy1());
                return; }

            if(pgt.iscorn7()) {
                ddraw_hrangu(0, dbl_ex2());
                ddraw_hrangd(0, dbl_ex1());
                ddraw_vrangl(dbl_ey2(), 7);
                ddraw_vrangr(dbl_ey1(), 7);
                return; }

            if(pgt.iscorn9()) {
                ddraw_hrangu(dbl_sx1(), 7);
                ddraw_hrangd(dbl_sx2(), 7);
                ddraw_vrangl(dbl_ey1(), 7);
                ddraw_vrangr(dbl_ey2(), 7);
                return; }

            // HV Cross
            if(pgt.ishvcrs()) {
                ddraw_hrange(0, dbl_ex1()); ddraw_hrange(dbl_sx2(), 7);
                ddraw_vrange(0, dbl_ey1()); ddraw_vrange(dbl_sy2(), 7);
                return; }

        }

        //------------------------------------------------------------------------------
        //
        //------------------------------------------------------------------------------
        void subcreateMixed(const PGT & pgt) {

            if(pgt.ishvcrs()) {
                if(pgt.sngu())  { ddraw_horizontal(); sdraw_vertical(); }
                else            { sdraw_horizontal(); ddraw_vertical(); }
                return; }

            // H-Line ?
            if(pgt.contn_h()) {
                if(pgt.sngl()) sdraw_horizontal(); else ddraw_horizontal();
                if(pgt.validu())    { if(pgt.sngu()) sdraw_vrange(0, dbl_sy1()); else ddraw_vrange(0, sng_sy()); }
                else                { if(pgt.sngd()) sdraw_vrange(dbl_sy2(), 7); else ddraw_vrange(sng_sy(), 7); }
                return; }

            // V-Line ?
            if(pgt.contn_v()) {
                if(pgt.sngu()) sdraw_vertical(); else ddraw_vertical();
                if(pgt.validl())    { if(pgt.sngl()) sdraw_hrange(0, dbl_sx1()); else ddraw_hrange(0, sng_sx()); }
                else                { if(pgt.sngr()) sdraw_hrange(dbl_ex2(), 7); else ddraw_hrange(sng_sx(), 7); }
                return; }

            if(pgt.iscorn1()) {
                if(pgt.sngl())  { sdraw_hrange(0, dbl_ex2()); ddraw_vrange(0, sng_sy());  }
                else            { ddraw_hrange(0, sng_sx() ); sdraw_vrange(0, dbl_sy2()); }
                return; }
            
            if(pgt.iscorn3()) {
                if(pgt.sngr())  { sdraw_hrange(dbl_sx1(), 7); ddraw_vrange(0, sng_sy());  }
                else            { ddraw_hrange(sng_sx() , 7); sdraw_vrange(0, dbl_ey2()); }
                return; }

            if(pgt.iscorn7()) {
                if(pgt.sngl())  { sdraw_hrange(0, dbl_ex2()); ddraw_vrange(sng_sy(),  7); }
                else            { ddraw_hrange(0, sng_sx() ); sdraw_vrange(dbl_sy1(), 7); }
                return; }

            if(pgt.iscorn9()) {
                if(pgt.sngr())  { sdraw_hrange(dbl_sx1(), 7); ddraw_vrange(sng_sy(),  7); }
                else            { ddraw_hrange(sng_sx() , 7); sdraw_vrange(dbl_sy1(), 7); }
                return; }

        }

        //------------------------------------------------------------------------------
        //
        //------------------------------------------------------------------------------
        void subcreate(const PGT & pgt) {

            //
            PGT::entype t = pgt.detecttype();
            switch(t) {
                case PGT::eTypeSingle:  subcreateSingle(pgt); break;
                case PGT::eTypeDouble:  subcreateDouble(pgt); break;
                default:                subcreateMixed (pgt); break; }
        }


};