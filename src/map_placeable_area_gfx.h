/*
   $Id$

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef MAP_PLACEABLE_AREA_GFX
#define MAP_PLACEABLE_AREA_GFX

#include "animation.h"
#include "map_placeable_area.h"
#include "fileops.h"

class map_placeable_area_gfx : public drawable
{
private:
    animation * anim;

public:
    map_placeable_area_gfx();

    ~map_placeable_area_gfx();

    void play()
    {
        anim->play();
    }

    void stop()
    {
        anim->stop();
    }

    void rewind()
    {
        anim->rewind();
    }

    bool update() 
    {
        if (anim) anim->update ();
        return true; 
    }

    void draw(s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
              surface * target = NULL) const;
    
    void set_animation(animation * an) 
    {
        anim = an; 
    }

    void set_area_size(u_int16 nx, u_int16 ny);

    void put(ogzstream & file) const
    {
        anim->put(file);
    }

    void get(igzstream & file)
    {
        if (anim) delete anim;
        anim = new animation;
        anim->get(file);
    }
}; 


#endif
