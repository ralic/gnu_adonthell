/*
   $Id$
   
   (C) Copyright 2004 Kai Sterker
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef WIN_TTF_H
#define WIN_TTF_H

#include "SDL_ttf.h"
#include "win_font.h"

class win_ttf : public win_font
{
public:
    win_ttf (const char *color, const string & file);
    ~win_ttf ();
    
    bool load (const string & file);
  	bool in_table (u_int16 tmp);
    image & operator[] (u_int16);

private:
    SDL_Color Color;
    static TTF_Font *ttf;
    static u_int32 refcount;
};

#endif WIN_TTF_H