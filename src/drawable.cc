// $Id$
/*
   Copyright (C) 1999/2000/2001   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   drawable.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief Defines the drawable class. 
 */


#include "drawable.h"
 
using namespace std; 

// Public methods.


drawable::drawable () 
{
    set_length (0);
    set_height (0); 
}

drawable::~drawable ()
{ 
}

void drawable::update ()
{
}

void drawable::input_update ()
{
}
