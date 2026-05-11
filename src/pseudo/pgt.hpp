#pragma once

#include "vhplatform.hpp"

#define SYM_PSEUDO_FIRST    179
#define SYM_PSEUDO_LAST     218

#define SYM_DBG_ON    1

#define SYM_DBG_ONCE  0
#define SYM_DBG_ONCEN 208

// #define SYM_DBG_FROM SYM_PSEUDO_FIRST

#if(SYM_DBG_ONCE==1)
#define SYM_DBG_FROM  SYM_DBG_ONCEN
#define SYM_DBG_TO    SYM_DBG_ONCEN
#else
#define SYM_DBG_FROM  SYM_PSEUDO_FIRST
#define SYM_DBG_TO    SYM_PSEUDO_LAST
#endif


class PGT {

    public:

        enum entype { eTypeSingle=0, eTypeDouble, eTypeMix };

        PGT(uint8_t symn) { tbln = symn - SYM_PSEUDO_FIRST; }

        bool validu() const { return rco()[0]; }
        bool validd() const { return rco()[1]; }
        bool validl() const { return rco()[2]; }
        bool validr() const { return rco()[3]; }

        bool sngu() const { return rco()[0] == 1; }
        bool sngd() const { return rco()[1] == 1; }
        bool sngl() const { return rco()[2] == 1; }
        bool sngr() const { return rco()[3] == 1; }

        bool dblu() const { return rco()[0] == 2; }
        bool dbll() const { return rco()[2] == 2; }

        //
        bool contn_h() const { return  rco()[2] && rco()[3]; }
        bool contn_v() const { return  rco()[0] && rco()[1]; }
        bool ishvcrs() const { return contn_h() && contn_v(); } // крест ?
        bool ishline() const { return !rco()[0] && !rco()[1] &&  rco()[2] &&  rco()[3]; }  // Горизонталь ?
        bool isvline() const { return  rco()[0] &&  rco()[1] && !rco()[2] && !rco()[3]; }  // Вертикаль ?
        bool iscorn1() const { return  rco()[0] && !rco()[1] &&  rco()[2] && !rco()[3]; }
        bool iscorn3() const { return  rco()[0] && !rco()[1] && !rco()[2] &&  rco()[3]; }
        bool iscorn7() const { return !rco()[0] &&  rco()[1] &&  rco()[2] && !rco()[3]; }
        bool iscorn9() const { return !rco()[0] &&  rco()[1] && !rco()[2] &&  rco()[3]; }

        bool is3edgs() const {
            uint8_t c = 0;
            for(int i=0;i<4;i++) if(rco()[i]) c++;
            return c==3; }

        // Распознать тип глифа  eTypeSingle / eTypeDouble / eTypeMix
        entype detecttype() const {

            bool sen = false;
            bool den = false;

            for(int i=0; i<4; i++) {
                if(rco()[i] == 1) sen = true;
                if(rco()[i] == 2) den = true; }

            if( sen && !den) return eTypeSingle;
            if(!sen &&  den) return eTypeDouble;

            return eTypeMix; }

        // расчитать тип пересечения по боковым элементам
        // uint8_t crosstype(uint8_t p1, uint8_t p2) {
        //     uint8_t r;
        //     if((p1==1)||(p2==1)) r=1; else if((p1==2)||(p2==2)) r=2; else r=0;
        //     return r; }

    private:

        static const uint8_t    tblreco[][4];
        uint8_t                 tbln;
        const uint8_t *         rco() const { return tblreco[tbln]; }

};
