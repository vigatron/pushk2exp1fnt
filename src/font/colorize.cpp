#include "vhplatform.hpp"

#include "enumsymm.hpp"

static uint8_t caxsh    = 15;   // Цвет оси H
static uint8_t caxsv    = 28;   // Цвет оси V

static uint8_t cdef     = 63;

static uint8_t cecoh     = 23;
static uint8_t cecov     = 8;

static uint8_t cred     = 96;
static uint8_t cgry     = 255;

static uint8_t cbackh  = 2;
static uint8_t cbackhu = 1;

static uint8_t cbackv = 4;

//------------------------------------------------------------------------------
uint8_t axiscolors( enattr attr, uint8_t x, uint8_t y, uint8_t ci ) {
    uint8_t r;
    uint8_t colf;

    switch(attr) {

        case hattr404: {
        if(x < 4)   { r = ci ? cecoh : cbackh; }
        else        { r = ci ? cdef : cbackh; } }
        break;

        case hattr303: {
        if      (x<=2)  { r = ci ? cecoh : cbackh; }
        else if (x <6)  { r = ci ? cdef : cbackh; }
        else            { r = ci ? cgry : cbackhu; } }
        break;

        case hattr313: {
        if      (x<=2)  { r = ci ? cecoh  : cbackh; }
        else if (x==3)  { r = ci ? caxsh : cbackh; }
        else if (x <7)  { r = ci ? cdef  : cbackh; }
        else            { r = ci ? cgry  : cbackhu; } }
        break;

        case vattr313: {
            if(y==3) colf = caxsv; else if(y<3) colf = cecov;
            else if(y<7) colf = cdef; else colf = cgry;
            r = ci ? colf : cbackv; }
        break;

        case vattr413: {
            if(y==4) colf = caxsv; else if(y>4) colf = cdef;
            else if(y>0) colf = cecov; else colf = cgry;
            r = ci ? colf : cbackv; }
        break;

        case vattr404: {
            r = ci ? cdef  : cbackv;
        }
        break;

        default: r = ci ? cred : 0; break;
    }

    return r;
}
