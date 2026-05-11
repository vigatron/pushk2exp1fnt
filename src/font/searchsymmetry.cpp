#include "sym8x8.hpp"
#include "c16cell.hpp"

//------------------------------------------------------------------------------
// Ищем параметры симметрии для каждого символа в наборе
//------------------------------------------------------------------------------
std::vector<uint8_t> SearchForSymmetry(const c16cell & c16) {

    sym8x8                  singlecell;
    std::vector<uint8_t>    arrpxls;
    std::vector<uint8_t>    r;

    for(int i=0;i<256;i++) {
        arrpxls = c16.getsinglecell(i);
        singlecell.set(arrpxls);
        enattr optimalfilter = singlecell.FindOptimalFilter(i);
        r.push_back( optimalfilter ); }

    return r; }
