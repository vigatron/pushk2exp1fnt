#pragma once

#include "vhplatform.hpp"
#include "sym8x8.hpp"
#include "pgt.hpp"
#include "gmeta.hpp"
#include "dbg.hpp"


// --------------
class PGlyph {

    public:

        //------------------------------------------------------------------------------
        //
        //------------------------------------------------------------------------------
        const std::vector<uint8_t> & recreate(uint8_t symn) {
            
            arr.assign(W*H, 0);

            if(symn < SYM_PSEUDO_FIRST) return arr;
            if(symn > SYM_PSEUDO_LAST ) return arr;

            PGT pgt(symn);
            subcreate(pgt);

            dbg_i1t("\nPS-Symn #%d", symn);
            dbg_8x8(arr);
            return arr; }

        void FindCenterPointSingle(const sym8x8 & objsym) {
            msng.FindCenterPointSingle(objsym); }

        void FindCenterPointDouble(const sym8x8 & objsym) {
            mdbl.FindCenterPointDouble(objsym); }

    private:

        // Структура + координаты центральной точки (single)
        GlyphMeta msng;

        // Структура + координаты центральной точки (double top-left)
        GlyphMeta mdbl;

        static constexpr uint8_t W = 8, H = 8;
        std::vector<uint8_t>    arr{W*H, 0};

        void setpix(uint8_t x, uint8_t y, uint8_t v) {
            if(x<W && y<H) arr[y*W+x] = v;
        }

        uint8_t sng_sx()  { return msng.cpx(); }
        uint8_t sng_sy()  { return msng.cpy(); }

        uint8_t dbl_sx1() { return mdbl.cpx() - mdbl.wx();     }
        uint8_t dbl_ex1() { return dbl_sx1()  + mdbl.wx() - 1; }
        uint8_t dbl_sx2() { return mdbl.cpx() + mdbl.spx();    }
        uint8_t dbl_ex2() { return dbl_sx2()  + mdbl.wx() - 1; }

        uint8_t dbl_sy1() { return mdbl.cpy() - mdbl.wy();     }
        uint8_t dbl_ey1() { return dbl_sy1()  + mdbl.wy() - 1; }
        uint8_t dbl_sy2() { return mdbl.cpy() + mdbl.spy();    }
        uint8_t dbl_ey2() { return dbl_sy2()  + mdbl.wy() - 1; }

        // ----------------------------
        // Sng pxls / Draw Single parts
        // ----------------------------

        // sng:  -**--
        void sdraw_hpart(uint8_t y) {
            setpix(msng.cpx(), y, 1);
            if(msng.wx2()) setpix(msng.cpx() + 1, y, 1); }

        // sng: |
        //      *
        //      |
        void sdraw_vpart(uint8_t x) {
            setpix(x, msng.cpy(), 1);
            if(msng.wy2()) setpix(x, msng.cpy() + 1, 1); }

        // Горизонтальная отрисовка одиночного фрагмента
        void sdraw_hrange(uint8_t xf, uint8_t xt) { for(int x=xf;x<=xt;x++) sdraw_vpart(x); }

        // Вертикальная отрисовка одиночного фрагмента
        void sdraw_vrange(uint8_t yf, uint8_t yt) { for(int y=yf;y<=yt;y++) sdraw_hpart(y); }

        void sdraw_u() { sdraw_vrange(0, sng_sy()); }
        void sdraw_d() { sdraw_vrange(sng_sy(), H-1); }
        void sdraw_l() { sdraw_hrange(0, sng_sx()); }
        void sdraw_r() { sdraw_hrange(sng_sx(), W-1); }

        // Отрисовка: горизонталь
        void sdraw_horizontal() { sdraw_hrange(0, W-1); }

        // Отрисовка: вертикаль
        void sdraw_vertical()   { sdraw_vrange(0, H-1); }

        // ---------------------------
        // Dbl pxls / Draw Double part
        // ---------------------------

        // dbl:  -*-*-
        void ddraw_hpl(uint8_t y)   { setpix(dbl_sx1()+0, y, 1); if(mdbl.wx2()) setpix(dbl_sx1()+1, y, 1); }
        void ddraw_hpr(uint8_t y)   { setpix(dbl_sx2()+0, y, 1); if(mdbl.wx2()) setpix(dbl_sx2()+1, y, 1); }
        void ddraw_hpart(uint8_t y) { ddraw_hpl(y); ddraw_hpr(y); }

        // dbl: *
        //      |
        //      *
        void ddraw_vpu(uint8_t x)   { setpix(x, dbl_sy1(), 1); if(mdbl.wy2()) setpix(x, dbl_sy1()+1, 1); }
        void ddraw_vpd(uint8_t x)   { setpix(x, dbl_sy2(), 1); if(mdbl.wy2()) setpix(x, dbl_sy2()+1, 1); }
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
        void ddraw_horizontal() { ddraw_hrange(0, W-1); }
        void ddraw_horzu()      { for(int x=0;x<W;x++) ddraw_vpu(x); }
        void ddraw_horzd()      { for(int x=0;x<W;x++) ddraw_vpd(x); }

        // Отрисовка: вертикаль
        void ddraw_vertical()   { ddraw_vrange(0, H-1); }
        void ddraw_vertl()      { for(int y=0;y<H;y++) ddraw_hpl(y); }
        void ddraw_vertr()      { for(int y=0;y<H;y++) ddraw_hpr(y); }

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
                    ddraw_hrangu(dbl_sx2(), W-1);
                    ddraw_horzd();
                    ddraw_vrange(0, dbl_sy1());
                } else {
                    ddraw_hrangd(0, dbl_ex1());
                    ddraw_hrangd(dbl_sx2(), W-1);
                    ddraw_horzu();
                    ddraw_vrange(dbl_ey2(), H-1);
                }

                return; }

            if(pgt.contn_v()) {

                if(pgt.dbll()) {
                    ddraw_vertr();
                    ddraw_vrangl(0, dbl_sy1());
                    ddraw_vrangl(dbl_sy2(), H-1);
                    ddraw_hrange(0, dbl_sx1());
                } else {
                    ddraw_vertl();
                    ddraw_vrangr(0, dbl_sy1());
                    ddraw_vrangr(dbl_sy2(), H-1);
                    ddraw_hrange(dbl_sx2(), W-1);
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
                ddraw_hrangu(dbl_sx2(), W-1);
                ddraw_hrangd(dbl_sx1(), W-1);
                ddraw_vrangl(0, dbl_sy2());
                ddraw_vrangr(0, dbl_sy1());
                return; }

            if(pgt.iscorn7()) {
                ddraw_hrangu(0, dbl_ex2());
                ddraw_hrangd(0, dbl_ex1());
                ddraw_vrangl(dbl_ey2(), H-1);
                ddraw_vrangr(dbl_ey1(), H-1);
                return; }

            if(pgt.iscorn9()) {
                ddraw_hrangu(dbl_sx1(), W-1);
                ddraw_hrangd(dbl_sx2(), W-1);
                ddraw_vrangl(dbl_ey1(), H-1);
                ddraw_vrangr(dbl_ey2(), H-1);
                return; }

            // HV Cross
            if(pgt.ishvcrs()) {
                ddraw_hrange(0, dbl_ex1()); ddraw_hrange(dbl_sx2(), W-1);
                ddraw_vrange(0, dbl_ey1()); ddraw_vrange(dbl_sy2(), H-1);
                return; }

        }

        //------------------------------------------------------------------------------
        //
        //------------------------------------------------------------------------------
        void subcreateMixed(const PGT & pgt) {

            // отрисовка: крест
            if(pgt.ishvcrs()) {
                if(pgt.sngu())  { ddraw_horizontal(); sdraw_vertical(); }
                else            { sdraw_horizontal(); ddraw_vertical(); }
                return; }

            // H-Line ?
            if(pgt.contn_h()) {
                if(pgt.sngl()) sdraw_horizontal(); else ddraw_horizontal();
                if(pgt.validu())
                    { if(pgt.sngu()) sdraw_vrange(0, dbl_sy1()); else ddraw_vrange(0, sng_sy()); }
                else
                    { if(pgt.sngd()) sdraw_vrange(dbl_sy2(), H-1); else ddraw_vrange(sng_sy(), H-1); }
                return; }

            // V-Line ?
            if(pgt.contn_v()) {
                if(pgt.sngu()) sdraw_vertical(); else ddraw_vertical();
                if(pgt.validl())
                    { if(pgt.sngl()) sdraw_hrange(0, dbl_sx1()); else ddraw_hrange(0, sng_sx()); }
                else
                    { if(pgt.sngr()) sdraw_hrange(dbl_ex2(), W-1); else ddraw_hrange(sng_sx(), W-1); }
                return; }

            if(pgt.iscorn1()) {
                if(pgt.sngl())  { sdraw_hrange(0, dbl_ex2()); ddraw_vrange(0, sng_sy());  }
                else            { ddraw_hrange(0, sng_sx() ); sdraw_vrange(0, dbl_sy2()); }
                return; }
            
            if(pgt.iscorn3()) {
                if(pgt.sngr())  { sdraw_hrange(dbl_sx1(), W-1); ddraw_vrange(0, sng_sy());  }
                else            { ddraw_hrange(sng_sx() , W-1); sdraw_vrange(0, dbl_ey2()); }
                return; }

            if(pgt.iscorn7()) {
                if(pgt.sngl())  { sdraw_hrange(0, dbl_ex2()); ddraw_vrange(sng_sy(),  H-1); }
                else            { ddraw_hrange(0, sng_sx() ); sdraw_vrange(dbl_sy1(), H-1); }
                return; }

            if(pgt.iscorn9()) {
                if(pgt.sngr())  { sdraw_hrange(dbl_sx1(), W-1); ddraw_vrange(sng_sy(),  H-1); }
                else            { ddraw_hrange(sng_sx() , W-1); sdraw_vrange(dbl_sy1(), H-1); }
                return; }

        }

        //------------------------------------------------------------------------------
        //
        //------------------------------------------------------------------------------
        void subcreate(const PGT & pgt) {

            PGT::entype t = pgt.detecttype();
            switch(t) {
                case PGT::eTypeSingle:  subcreateSingle(pgt); break;
                case PGT::eTypeDouble:  subcreateDouble(pgt); break;
                default:                subcreateMixed (pgt); break; }
        }


};