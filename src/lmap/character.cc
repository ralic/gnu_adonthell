/*
   $Id$k

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   character.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the character class.
 * 
 * 
 */

#include <math.h>

#include "character.h"

using namespace lmap;

character::character (landmap & mymap) : moving (mymap)
{
    Type = CHARACTER;
    Speed = 2;
    VSpeed = 0;
    Is_running = false;
    Current_dir = NONE;
}

void character::jump()
{
    VSpeed = 4.5;
}

void character::update()
{
    set_vertical_velocity(VSpeed);
    moving::update();
    if (!Is_falling) VSpeed = 0.0;
    else VSpeed = vz() - 0.2;
}

void character::set_direction(int ndir)
{
  float vx = 0.0, vy = 0.0;
  
  Current_dir = ndir;
  
  if (current_dir() & WEST) vx = -speed() * (1 + is_running());
  if (current_dir() & EAST) vx = speed() * (1 + is_running());
  if (current_dir() & NORTH) vy = -speed() * (1 + is_running());
  if (current_dir() & SOUTH) vy = speed() * (1 + is_running());
  
  if (vx && vy)
  {
      float s = sqrt (vx*vx + vy*vy);
      vx = (vx * fabs (vx))/s;
      vy = (vy * fabs (vy))/s;
  }
  
  set_velocity(vx, vy);

  update_state();
}

void character::add_direction(direction ndir)
{
    int tstdir = current_dir();
    switch (ndir)
    {
        case WEST:
            if (tstdir && EAST) tstdir &= ~EAST;
            break;
        case EAST:
            if (tstdir && WEST) tstdir &= ~WEST;
            break;
        case SOUTH:
            if (tstdir && NORTH) tstdir &= ~NORTH;
            break;
        case NORTH:
            if (tstdir && SOUTH) tstdir &= ~SOUTH;
            break;
        default:
            break;
    }
    
    set_direction(tstdir | ndir);
}

void character::update_state()
{
    std::string state;
    float xvel = vx () > 0 ? vx () : -vx (); 
    float yvel = vy () > 0 ? vy () : -vy ();
    
    if (xvel || yvel) 
    {
        if (xvel > yvel) 
        {
            if (vx () > 0) state = "e";
            else if (vx () < 0) state = "w"; 
        }
        else if (yvel > xvel) 
        {
            if (vy () > 0) state = "s";
            else if (vy () < 0) state = "n"; 
        }
        else 
        {
            if ((vx() > 0) && (current_state_name()[0] == 'w'))
                state = "e";
            else if ((vx() < 0) && (current_state_name()[0] == 'e'))
                state = "w";
            else if ((vy() > 0) && (current_state_name()[0] == 'n'))
                state = "s";
            else if ((vy() < 0) && (current_state_name()[0] == 's'))
                state = "n";
            else state = current_state_name()[0];
        }
        state += is_running() ? "_run" : "_walk";
    }
    else if (!xvel)
    {
        state = current_state_name()[0];
        state += "_stand";
    }
    set_state(state);

}

void character::put(ogzstream & file) const
{
    placeable_model::put(file);
}

void character::get(igzstream & file)
{
    placeable_model::get(file);
    set_state ("s_stand"); 
}

s_int8 character::save(const std::string fname) const
{
    ogzstream file (fname);
    s_int8 ret = 0; 
    
    if (!file.is_open ())
        return 1;
    put (file);
    file.close (); 
    return ret;
}

s_int8 character::load(const std::string fname)
{
    igzstream file (fname);
    s_int8 ret = 0; 
    
    if (!file.is_open ())
        return 1;
    get (file);
    file.close (); 
    return ret;
}