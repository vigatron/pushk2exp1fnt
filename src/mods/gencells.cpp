#include "gencells.hpp"
#include "colorize.hpp"
#include "method.hpp"

//------------------------------------------------------------------------------
// .c16 to mono .cells
//------------------------------------------------------------------------------
void c16tocells( const c16cell & c16, CELLS256 & c256) {
    for(int symn=0; symn<256;symn++) {
        for(int y=0;y<8;y++) {
            for(int x=0;x<8;x++) {
                int ci = c16.getpxl(symn, x, y);
                c256.setPixel(symn, x, y, ci ? 1:0); } } } }

//------------------------------------------------------------------------------
// Генерируем оригинальный .cells из .c16
//------------------------------------------------------------------------------
verr c256original(
    const c16cell & c16orig,
    CELLS256 & c256,
    const std::string fname_orig_cells) {

    c16tocells(c16orig, c256);

    // Сохраняем результат: original as cells with highlight
    if(!c256.saveToFile(fname_orig_cells))
        return verror("Save results " + fname_orig_cells, 4);

    return vok; }

//------------------------------------------------------------------------------
// Генерируем .cells с подсветкой симметрии
//------------------------------------------------------------------------------
verr c256enlight(
    const c16cell & c16orig,
    CELLS256 & c256enl,
    const CELLS256 & c256out,
    const std::vector<uint8_t> & arrflts,
    const std::string fname) {

    // .c16 to .cells + highlight
    for(int symn=0; symn<256;symn++) {
        uint8_t m = getmethod(symn);
        enattr attr = (enattr)arrflts[symn];
        for(int y=0;y<8;y++) {
            for(int x=0;x<8;x++) {
                uint8_t ci = c16orig.getpxl(symn, x, y)?1:0;
                uint8_t ca = c256out.getPixel(symn,x,y);

                uint8_t cb;

                if( m == opG ) { cb = 81; }
                else {

                    switch(attr) {
                        case noattr: cb = COLOR_BN; break;
                        
                        case hsplit: 
                        case hattr404:
                        case hattr313:
                        case hattr303: cb = COLOR_BH; break;

                        case vattr313:
                        case vattr413:
                        case vattr404: cb = COLOR_BV; break;

                        default: cb = 0; break; }

                }

                uint8_t co;
                if(ca) { if(ca==ci) co = 63; else co = 56; }
                else   { if(ci) co = 42; else co = cb; }

                c256enl.setPixel(symn, x, y, co);
            }
        }
    }

    // Сохраняем результат: original as cells with highlight
    if(!c256enl.saveToFile(fname))
        return verror("Save results " + fname, 4);

    return vok; }

//------------------------------------------------------------------------------
verr c256optim(
    const c16cell & c16xor,
    CELLS256 & c256,
    const std::vector<uint8_t> & arrflts,
    const std::string & fname_mirr_cells ) {

    for(int symn=0; symn<256;symn++) {
        enattr attr = (enattr)arrflts[symn];
        for(int y=0;y<8;y++) {
            for(int x=0;x<8;x++) {
                int ci = c16xor.getpxl(symn, x, y);
                c256.setPixel(symn, x, y, ci ? 1:0); } } }

    if(!c256.saveToFile(fname_mirr_cells))
        return verror("Save results " + fname_mirr_cells, 4);
    
    return vok; }
