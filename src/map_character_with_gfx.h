/*
   $Id:

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef MAP_CHARACTER_WITH_GFX_H
#define MAP_CHARACTER_WITH_GFX_H

#include "map_character.h"
#include "map_character_gfx.h"

class map_character_with_gfx : public map_character, public map_character_gfx
{
private:
    animation * make_anim (const string & s) 
    {
        animation * an = new animation ();
        image * im = new image ();
        animationframe af; 
        im->load_pnm (s);
        
        an->insert_image (im, 0);
        af.set_image_nbr (0);
        af.set_mask (true); 
        an->insert_frame (af, 0); 
        an->set_offset (0, 20);
        return an; 
    }

public:
    map_character_with_gfx (); 
    
    bool update () 
    {
        map_character_gfx::update ();
        return map_character::update ();
    }

    void set_state (const string & st) 
    {
        map_character::set_state (st);
        map_character_gfx::set_state (st); 
    }

    void update_state () 
    {
        map_character::update_state ();
        map_character_gfx::set_state (map_character::current_state_name ()); 
    }
}; 

#endif