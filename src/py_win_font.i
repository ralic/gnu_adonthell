%module win_font

%{

#include "win_types.h"
#include "win_base.h"
#include "win_font.h"

%}

#define u_int8 unsigned char
#define s_int8 signed char
#define u_int16 unsigned short
#define s_int16 short
#define u_int32 unsigned long
#define s_int32 signed long

%include "win_types.h"
%include "win_font.h"