/*
   $Id$

   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file map_event.cc
 *
 * @author Kai Sterker
 * @brief Implements the different map events.
 */
 
#include "map_event.h"

// constructor
map_event::map_event () : event ()
{
    submap = x = y = dir = map = -1;
    c = NULL;
}

// compare two map events
bool map_event::equals (const event* e)
{
    // if our event is paused, this will prevent it from getting executed
    if (Paused) return false;
    
    // we know that we've got a map_event :)
    map_event *t = (map_event *) e;

    if (submap != -1 && t->submap != submap) return false;
    if (x != -1 && t->x != x) return false;
    if (y != -1 && t->y != y) return false;
    if (dir != -1 && t->dir != dir) return false;
    if (map != -1 && t->map != map) return false;
    if (c && t->c != c) return false;
    
    return true;
}

// Execute map event's script
s_int32 map_event::execute (const event* e)
{
    switch (Action)
    {
        case ACTION_SCRIPT:
        {
            map_event *t = (map_event *) e; 
    
            PyObject *args = Py_BuildValue ("(i, i, i, i, s)", 
                t->submap, t->x, t->y, t->dir, t->c->get_id ().c_str ());  
    
            Script->run (args);
            
            Py_DECREF (args);
            break;
        }
        
        case ACTION_PYFUNC:
        {
            PyFunc->callback_func0 ();
            break;
        }
        
        case ACTION_CPPFUNC:
        {
            Callback ();
            break;
        }
        
        default: break;
    }
    
    return do_repeat ();
}

// Load a map event from file
bool map_event::get_state (igzstream& f)
{
    event::get_state (f); 

    string name;
    string s; 
    
    submap << f; 
    x << f;
    y << f;

    dir << f;
    map << f;

    s << f;
    if (s != "") c = (mapcharacter*) data::characters[s.c_str ()];
    else c = NULL; 

    return true;
}

// Save map event to file
void map_event::put_state (ogzstream& out) const
{
    event::put_state (out);
    
    submap >> out; 
    x >> out;
    y >> out;
    dir >> out;
    map >> out;
    
    if (c) c->get_id () >> out;
    else 
    {
        string s = ""; 
        s >> out;
    }
}  

// constructor
enter_event::enter_event () : map_event ()
{
    Type = ENTER_EVENT; 
}

// constructor
leave_event::leave_event () : map_event ()
{
    Type = LEAVE_EVENT; 
}

// constructor
action_event::action_event () : map_event ()
{
    Type = ACTION_EVENT; 
}