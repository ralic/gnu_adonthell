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

/**
 * @file   map_moving.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the map_moving class.
 * 
 * 
 */


#ifndef MAP_MOVING_H
#define MAP_MOVING_H

#include "map_placeable.h"

/**
 * Implements a map moving object.
 *
 * While map_placeable can occupy space on a map, it has not the ability to move.
 * Objects inheriting from this class can be placed on a map and be given a certain
 * velocity, that they will follow each time they are updated.
 * 
 */
class map_moving : public map_placeable, public map_coordinates
{
private:
    float fox, foy; 
    float Vx, Vy;
    bool Has_moved; 

    u_int16 Lx, Ly;
    
public:

    map_moving (landmap & mymap); 

    float vx () 
    {
        return Vx; 
    }

    float vy () 
    {
        return Vy; 
    }

    u_int16 lx () 
    {
        return Lx;
    }

    u_int16 ly () 
    {
        return Ly; 
    }
    
    void set_offset (u_int16 ox, u_int16 oy); 
    
    void set_limits (u_int16 mx, u_int16 my); 

    void set_velocity (float vx, float vy); 

    void update_pos();

    bool should_fall();

    bool update(); 
}; 

#endif

