#pragma once

#include "vhplatform.hpp"
#include "globaldefs.hpp"
#include <array>
#include <cassert>

namespace PGTTBL {

    // Упаковка четырёх 2‑битных полей в один байт:
    // U: bits 7-6, D: bits 5-4, L: bits 3-2, R: bits 1-0
    constexpr uint8_t pack2bits(uint8_t U, uint8_t D, uint8_t L, uint8_t R) noexcept {
        return static_cast<uint8_t>( ((U & 0x3u) << 6) | ((D & 0x3u) << 4) | ((L & 0x3u) << 2) | (R & 0x3u) );
    }

     // CP866 pseudograph syms metaprops
    inline constexpr std::array<uint8_t, 40> tblreco = {
        pack2bits(1,1,0,0), // 179
        pack2bits(1,1,1,0), // 180
        pack2bits(1,1,2,0), // 181
        pack2bits(2,2,1,0), // 182
        pack2bits(0,2,1,0), // 183
        pack2bits(0,1,2,0), // 184
        pack2bits(2,2,2,0), // 185
        pack2bits(2,2,0,0), // 186
        pack2bits(0,2,2,0), // 187
        pack2bits(2,0,2,0), // 188
        pack2bits(2,0,1,0), // 189
        pack2bits(1,0,2,0), // 190
        pack2bits(0,1,1,0), // 191
        pack2bits(1,0,0,1), // 192
        pack2bits(1,0,1,1), // 193
        pack2bits(0,1,1,1), // 194
        pack2bits(1,1,0,1), // 195
        pack2bits(0,0,1,1), // 196
        pack2bits(1,1,1,1), // 197
        pack2bits(1,1,0,2), // 198
        pack2bits(2,2,0,1), // 199
        pack2bits(2,0,0,2), // 200
        pack2bits(0,2,0,2), // 201
        pack2bits(2,0,2,2), // 202
        pack2bits(0,2,2,2), // 203
        pack2bits(2,2,0,2), // 204
        pack2bits(0,0,2,2), // 205
        pack2bits(2,2,2,2), // 206
        pack2bits(1,0,2,2), // 207
        pack2bits(2,0,1,1), // 208
        pack2bits(0,1,2,2), // 209
        pack2bits(0,2,1,1), // 210
        pack2bits(2,0,0,1), // 211
        pack2bits(1,0,0,2), // 212
        pack2bits(0,1,0,2), // 213
        pack2bits(0,2,0,1), // 214
        pack2bits(2,2,1,1), // 215
        pack2bits(1,1,2,2), // 216
        pack2bits(1,0,1,0), // 217
        pack2bits(0,1,0,1)  // 218
    };

    static_assert(tblreco.size() == 40, "tblreco size mismatch");

};

class PGT {

    public:

        enum Edge   { None = 0, Single = 1, Double = 2 };
        enum entype { eTypeSingle, eTypeDouble, eTypeMix };

        PGT(uint8_t symn) {
            if(symn >= SYM_PSEUDO_FIRST && symn <= SYM_PSEUDO_LAST) {
                uint8_t tbln = symn - SYM_PSEUDO_FIRST;
                v = PGTTBL::tblreco[tbln];
            } else {
                v = 0;
            }
        }

        uint8_t typeu() const { return (v>>6) & 3; }
        uint8_t typed() const { return (v>>4) & 3; }
        uint8_t typel() const { return (v>>2) & 3; }
        uint8_t typer() const { return (v>>0) & 3; }

        // Avail ?
        bool au()       const { return typeu(); }
        bool ad()       const { return typed(); }
        bool al()       const { return typel(); }
        bool ar()       const { return typer(); }

        bool nu()       const { return !typeu(); }
        bool nd()       const { return !typed(); }
        bool nl()       const { return !typel(); }
        bool nr()       const { return !typer(); }

        // Single ?
        bool sngu()     const { return typeu() == Single; }
        bool sngd()     const { return typed() == Single; }
        bool sngl()     const { return typel() == Single; }
        bool sngr()     const { return typer() == Single; }

        // Double ?
        bool dblu()     const { return typeu() == Double; }
        bool dbld()     const { return typed() == Double; }
        bool dbll()     const { return typel() == Double; }
        bool dblr()     const { return typer() == Double; }

        bool contn_h()  const { return al() && ar(); }
        bool contn_v()  const { return au() && ad(); }

        // крест ?
        bool ishvcrs()  const { return contn_h() && contn_v(); }

        // Горизонталь ?
        bool ishline() const { return nu() && nd() && al() && ar(); }

        // Вертикаль ?
        bool isvline() const { return au() && ad() && nl() && nr(); }

        // Corners ?
        bool iscorn1() const { return au() && nd() && al() && nr(); }
        bool iscorn3() const { return au() && nd() && nl() && ar(); }
        bool iscorn7() const { return nu() && ad() && al() && nr(); }
        bool iscorn9() const { return nu() && ad() && nl() && ar(); }

        // полукрест ?
        bool is3edgs() const {
            uint8_t c = 0;
            if(au()) c++;
            if(ad()) c++;
            if(al()) c++;
            if(ar()) c++;
            return c==3; }

        // Распознать тип глифа:  eTypeSingle / eTypeDouble / eTypeMix
        entype detecttype() const {

            bool sen = false;
            bool den = false;
            uint8_t arr[4] = { typeu(), typed(), typel(), typer() };

            for(int i=0; i<4; i++) {
                if(arr[i] == 1) sen = true;
                if(arr[i] == 2) den = true; }

            if( sen && !den) return eTypeSingle;
            if(!sen &&  den) return eTypeDouble;

            return eTypeMix; }

    private:

        uint8_t v;

};
