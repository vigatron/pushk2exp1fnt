#pragma once

#define CENTER_POINT_SYM_S  197
#define CENTER_POINT_SYM_D  206


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
