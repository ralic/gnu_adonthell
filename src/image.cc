/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   image.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the image class.
 * 
 * 
 */
 
#include "image.h"
#include "pnm.h"


using namespace std; 

image::image () : surface ()
{
}

image::image (u_int16 l, u_int16 h) : surface () 
{
    resize (l, h); 
}
 
image::~image () 
{
}

void image::resize (u_int16 l, u_int16 h)
{
    surface::resize (l, h); 
}

void image::clear ()
{
    surface::clear ();  
}

s_int8 image::get (igzstream& file)
{
    s_int8 ret;

    bool m;
    u_int8 a;
    
    m << file;
    a << file;
    
    ret = get_raw (file);
    if (!ret)
    {
        set_mask (m); 
        set_alpha (a);
    }
    return ret;
}
 
s_int8 image::load (string fname)
{
    igzstream file (fname);
    s_int8 ret = 0; 

    if (!file.is_open ())
        return 1;
    ret = get (file);
    file.close (); 
    return ret;
}

s_int8 image::get_raw (igzstream& file)
{
    void * rawdata; 

    u_int16 l, h; 

    clear (); 
    
    l << file;
    h << file; 
    
    rawdata = new char[l * h * 3];
    file.get_block (rawdata, l * h * 3); 
    
    raw2display (rawdata, l, h); 

    delete[] (char *) rawdata;
    
    if (!vis) return -1;

    changed = true; 
    return 0;
}


s_int8 image::load_raw (string fname)
{
    igzstream file (fname);
    s_int8 ret = 0; 

    if (!file.is_open ())
        return 1;
    ret = get_raw (file);
    file.close (); 
    return ret;
}


s_int8 image::get_pnm (SDL_RWops * file)
{
    void *rawdata;
    u_int16 l, h; 

    clear (); 

    rawdata = pnm::get (file, &l, &h);
    
    raw2display (rawdata, l, h); 

    free (rawdata);

    if (!vis) return -1;

    changed = true; 
    return 0;
}


s_int8 image::load_pnm (string fname)
{
    SDL_RWops *file;
    s_int8 ret = 0; 

    file = SDL_RWFromFile (fname.c_str (), "r"); 
    if (!file)
        return 1;
    ret = get_pnm (file);
    SDL_RWclose (file);
    return ret;
}

s_int8 image::put (ogzstream& file) const
{
    bool m = is_masked (); 
    s_int8 a = alpha ();
    
    m >> file;
    a >> file;
    
    put_raw (file); 
    
    return 0; 
}

s_int8 image::save (string fname) const
{
    ogzstream file (fname);
    s_int8 ret = 0; 

    if (!file.is_open ())
        return 1;
    ret = put (file);
    file.close (); 
    return ret;
}

s_int8 image::put_raw (ogzstream& file) const
{
    SDL_Surface *tmp2 = SDL_CreateRGBSurface (0, length (),
                                              height (), 24, 
                                              0x0000FF, 0x00FF00,
                                              0xFF0000, 0);
    
    SDL_Surface * temp = SDL_ConvertSurface (vis, tmp2->format, 0);

    length () >> file;
    height () >> file; 
    
    file.put_block (temp->pixels, length () * height () * 3); 

    SDL_FreeSurface (temp);
    SDL_FreeSurface (tmp2); 
    return 0;
}

s_int8 image::save_raw (string fname) const
{
    ogzstream file (fname);
    s_int8 ret = 0; 

    if (!file.is_open ())
        return 1;
    ret = put_raw (file);
    file.close (); 
    return ret;
}

s_int8 image::put_pnm (SDL_RWops * file) const
{
    SDL_Surface *tmp2 = SDL_CreateRGBSurface (0, length (),
                                              height (), 24, 
                                              0x0000FF, 0x00FF00,
                                              0xFF0000, 0);
    
    SDL_Surface * temp = SDL_ConvertSurface (vis, tmp2->format, 0);
    pnm::put (file, temp->pixels, length (), height ()); 

    SDL_FreeSurface (temp);
    SDL_FreeSurface (tmp2); 

    return 0; 
}

s_int8 image::save_pnm (string fname) const
{
    SDL_RWops *file;
    s_int8 ret = 0; 

    file = SDL_RWFromFile (fname.c_str (), "w"); 
    if (!file)
        return 1;
    ret = put_pnm (file);
    SDL_RWclose (file);
    return ret;
}
 
void image::zoom (const surface& src, u_int16 l, u_int16 h, u_int16 x = 0, u_int16 y = 0)
{ 
    // Calculate the step per pixel.
    // While the surface positions are u_int16s, we use u_int32s for step
    // and position during zoom, that we'll divide by 65535 ( >> 16). That
    // way,  we can perform our zoom without having to divide two times per
    // pixel we proceed (we can replace the divides with shift, much much
    // faster.
    u_int32 xstep = (u_int32) (((float) src.length () / (float) l) * 65535); 
    u_int32 ystep = (u_int32) (((float) src.height () / (float) h) * 65535); 
    u_int32 xcur;
    u_int32 ycur;

    u_int32 col;  
         
    lock ();
    src.lock (); 
    ycur = 0;
    u_int16 i, j;
    for (j = y; j < h + y; j++)
    {
        xcur = 0; 
        for (i = x; i < l + x; i++)
        {
            src.get_pix (xcur >> 16, ycur >> 16, col);
            put_pix (i, j, col); 
            xcur += xstep; 
        }
        ycur += ystep;  
    }
    src.unlock ();
    unlock (); 
}

void image::tile (const surface& src, u_int16 l, u_int16 h, u_int16 x = 0, u_int16 y = 0)
{
    u_int16 posx;
    u_int16 posy;

    drawing_area da (x, y, l, h); 
    
    for (posy = 0; posy < h; posy += src.height ())
        for (posx = 0; posx < l; posx += src.length ())
            src.draw (x + posx, y + posy, &da, this); 
}

void image::brightness (const surface& src, u_int8 cont, bool proceed_mask = false)
{
    u_int16 i, j;
    u_int8 ir, ig, ib;
    u_int32 temp = 0;
    
    resize (src.length (), src.height ());
    lock ();
    src.lock (); 
    for (j = 0; j < height (); j++)
        for (i = 0; i < length (); i++)
        {
            src.get_pix (i, j, temp);
            if ((proceed_mask) || temp != screen::trans_col ())
            {
                src.get_pix (i, j, ir, ig, ib);
                ir = (ir * cont) >> 8;
                ig = (ig * cont) >> 8;
                ib = (ib * cont) >> 8;
                put_pix (i, j, ir, ig, ib);
            }
            else put_pix (i, j, temp); 
        }
    src.unlock ();
    unlock ();

    set_mask (false);
    set_alpha (255); 
}  

image& image::operator = (const image& src)
{
    (surface&) (*this) = (surface&) src;    
    return *this; 
}




// Private methods



void image::raw2display (void * rawdata, u_int16 l, u_int16 h)
{
    set_length (l);
    set_height (h); 
    SDL_Surface *tmp2 = SDL_CreateRGBSurfaceFrom (rawdata, length (),
                                                  height (), 24,
                                                  length () * 3,
                                                  0x0000FF, 0x00FF00,
                                                  0xFF0000, 0);
    vis = SDL_DisplayFormat (tmp2);
    SDL_FreeSurface (tmp2); 
}
