/*
   $Id$

   Copyright (C) 2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file gametime.cc
 *
 * @author Kai Sterker
 * @brief Implements the gametime class.
 */

#include <string>
// #include "event.h"
#include "gametime.h"
#include <SDL/SDL.h>


u_int32 gametime::timer1;
u_int32 gametime::timer2;
u_int8 gametime::fts = 0;
bool gametime::running = false; 

// NEW_EVENT(time_event)

gametime::gametime (u_int32 start, float ratio)
{     
    ticks = 0;
    time = start;
    
    fts = 0; 
    timer1 = 0;
    timer2 = 0;
    
    // This decides how many realtime milliseconds make one gametime minute
    minute = (int) (60000 * ratio);
    
//     REGISTER_EVENT (TIME_EVENT, time_event)
}

// Increase gametime 
// void gametime::tick (u_int32 val)
// {
//     ticks += val;

//     // notify the eventhandler whenever a minute has passed
//     if (ticks >= minute)
//     {
//         ticks -= minute;
//         time++;

//         time_event t;
//         t.time = time;
//         event_handler::raise_event (t);
//     }
// }

// Synchronize the game's speed to the machine it's running on.
void gametime::update () 
{
    // We declare this variable as static to avoid having to
    // perform the division every time.
    // This value correspond to the minimum delay before between
    // Two displayed frames (see FRAME_RATE).
    static u_int16 gfx_cycle_length = 1000 / FRAME_RATE;
    
    while (1)
    {
        timer2 = SDL_GetTicks () - timer1;
        
        // if the mainloop was performed faster than one frame
        // should take, we sleep for the time remaining
        if (timer2 >= gfx_cycle_length) break;
        else SDL_Delay (3);
    }
    
    timer1 = SDL_GetTicks () - (timer2 % CYCLE_LENGTH);
    
    // Calculate the number of frames to skip (if the mainloop
    // takes longer than allowed, we drop frames (up to a certain
    // limit) to keep the game's speed constant.)
    fts = timer2 / CYCLE_LENGTH;
    if (fts > FTS_LIMIT) fts = FTS_LIMIT;
}

/* 
time_event::time_event ()
{
    time = minute = hour = day =  0;
    m_step = h_step = d_step = 1;
    type = TIME_EVENT;
}

// Save a time_event to file
void time_event::save (ogzstream& out) const
{
    type >> out;
    minute >> out;
    m_step >> out;
    hour >> out;
    h_step >> out;
    day >> out;
    d_step >> out;
    script_file () >> out;
}

// Load a time event from file
bool time_event::load (igzstream& f)
{
    string s; 
    minute << f;
    m_step << f;
    hour << f;
    h_step << f;
    day << f;
    d_step << f;
    s << f;
    set_script (s);
    
    return true;
}

// Execute time event's script
void time_event::execute (event &e)
{
    time_event t = (time_event&) e; 

    // Build the event script's local namespace
    PyObject *locals = Py_BuildValue ("{s:i,s:i,s:i}", "minute", (int) t.minute, 
        "hour", (int) t.hour, "day", (int) t.day);
//     script.set_locals (locals);
    script.run ();
//     script.set_locals (NULL); 
    
    // Cleanup
    Py_DECREF (locals);
#ifdef PY_DEBUG
    python::show_traceback ();
#endif // PY_DEBUG
}

// Check whether this time_even matches a given gametime
bool time_event::equals (event &e)
{
    time_event t = (time_event &) e;
    u_int32 time = t.time % 40320;
    u_int32 d, h, m = time % 60;
    
    if (m_step != 0) {
        if ((m - minute) % m_step != 0 || m < minute) return 0;
    }
    else
        if (m != minute) return 0;

    // this is the current hour
    h = ((time - m) % 1440) / 60;

    if (h_step != 0) {
        if ((h - hour) % h_step != 0 || h < hour) return 0;
    }
    else
        if (h != hour) return 0;
         
    // this is the current day
    d = (time - (m + h * 60)) / 1440;
    
    if (d_step != 0) {
        if ((d - day) % d_step != 0 || d < day) return 0; 
    }
    else
        if (d != day) return 0;

    // if the event matches, we set the actual minute, hour and day,
    // in case it is needed in the script
    t.minute = m;
    t.hour = h;
    t.day = d;
    
    return 1;
}
*/ 
