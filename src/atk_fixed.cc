/*
   $Id$

   (C) Copyright 2002 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "atk_fixed.h"


atk_fixed::atk_fixed ()
{ 
}


void atk_fixed::add (atk_widget * w)
{
    w->set_parent (this);

    v_widget_.push_back (w);

    on_add ();
}


void atk_fixed::remove (atk_widget * w)
{ 
    std::vector <atk_widget * >::iterator it = v_widget_.begin ();
    
    while (it != v_widget_.end () && * it != w)it++;

    if (it != v_widget_.end ())
    {
        v_widget_.erase (it);
        w->set_parent (NULL); 
    }

    on_remove (); 
}


void atk_fixed::clear ()
{
    for ( std::vector <atk_widget * >::iterator it = v_widget_.begin (); it != v_widget_.end (); it++) 
        delete *it;
    clear ();  
}



void atk_fixed::realize ()
{ 
  atk_container::realize();
}


atk_fixed::~atk_fixed ()
{
    // free memory
    clear (); 
}


bool atk_fixed::draw (gfx::drawing_area * da, gfx::surface * sf)
{    
  if (atk_container::draw (da, sf))
    {
      assign_drawing_area (da);
      for (std::vector <atk_widget* >::iterator it = v_widget_.begin (); it != v_widget_.end (); it++)
        (*it)->draw (this, sf);  
      detach_drawing_area();
      return true;
    }
  return false;
}




