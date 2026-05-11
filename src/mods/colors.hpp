#pragma once

#include <stdint.h>

#define COL2B(X, SHL)   ( ( (X) >> SHL ) & 3 )
#define COLDBL(C2B)     ( ( (C2B) << 2 ) | (C2B) )
#define COLQRT(C4B)     ( ( (C4B) << 4 ) | (C4B) )
#define COLXOR(X)       ( ( ( (X) >> 5) & 4 ) | ( ( (X) >> 5) & 2) )
#define COLF8(X,N)      COLQRT( ( COLDBL( COL2B(X,N) )  ^ COLXOR(X) ) )
#define CLRR8(X)        COLF8((X), 0)
#define CLRG8(X)        COLF8((X), 2)
#define CLRB8(X)        COLF8((X), 4)
#define CLRSH(X,N)      ((X)<<N)
#define CLR32(X)        (CLRSH(CLRR8(X), 0) | CLRSH(CLRG8(X), 8) | CLRSH(CLRB8(X), 16))
#define SWAPRB(X)       ( ( ( X & 0xFF ) << 16) | ( X & 0xFF00) | ( (X>>16) & 0xFF ) )
