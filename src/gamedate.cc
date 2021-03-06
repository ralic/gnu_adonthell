/*
   Copyright (C) 2002 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file gamedate.cc
 *
 * @author Kai Sterker
 * @brief Implements the gamedate class.
 */

#include <ctype.h>
#include <stdlib.h>
#include "gamedate.h"
#include "gametime.h"
#include "time_event.h"
#include "event_handler.h"

// gametime minutes spent in the gameworld so far
u_int32 gamedate::Time = 0;

// number of game cycles since the last gametime minute passed
double gamedate::Ticks = 0.0;

// Increase gametime 
void gamedate::update ()
{
    static double tenth_minute = gametime::minute () / 10.0;
    
    // fts contains the number of cycles that passed since the last
    // call to gamedate::update
    Ticks += gametime::frames_to_skip ();

    // check whether an in-game minute has passed
    while (Ticks >= tenth_minute)
    {
        Ticks -= tenth_minute;
        Time++;
        
        // raise time event
        time_event evt (Time);
        event_handler::raise_event (&evt);
    }
}

// load state from disk
bool gamedate::get_state (igzstream &in)
{
    // read the current date as (gametime) minutes since start of the game
    Time << in;
    
    return true;
}

// save state to disk
void gamedate::put_state (ogzstream &out)
{
    // write the time to disk
    Time >> out;
}

// calculate the current weekday
u_int16 gamedate::weekday ()
{
    return day () % DAYS_PER_WEEK;
}

// calculate the current day
u_int16 gamedate::day ()
{
    // how many minutes make one day
    static u_int16 day_in_minutes = 600 * HOURS_PER_DAY;

    return Time / day_in_minutes;
}

// calculate the hour of the current day
u_int16 gamedate::hour ()
{
    return (Time / 600) % HOURS_PER_DAY;
}

// calculate minute of the hour
u_int16 gamedate::minute ()
{
    return (Time / 10) % 60; 
}

// convert the time string to gametime minutes
u_int32 gamedate::parse_time (const std::string & time)
{
    u_int32 t_minutes = 0, number = 0;
    char num[2] = "0";

    for (u_int32 i = 0; i < time.length (); i++)
    {
        // got a number
        if (isdigit (time[i]))
        {
            num[0] = time[i];
            number = 10 * number + atoi (num);
        }
        // got a letter
        else if (isalpha (time[i]))
        {
            switch (time[i])
            {
                // weeks
                case 'w':
                {
                    t_minutes += number * DAYS_PER_WEEK * HOURS_PER_DAY * 600;
                    break;
                }
                // days
                case 'd':
                {
                    t_minutes += number * HOURS_PER_DAY * 600;
                    break;
                }
                // hours
                case 'h':
                {
                    t_minutes += number * 600;
                    break;
                }
                // minutes
                case 'm':
                {
                    t_minutes += number * 10;
                    break;
                }
                // 1/10 minutes
                case 't':
                {
                    t_minutes += number;
                    break;
                }
                // error
                default:
                {
                    fprintf (stderr, "*** gamedate::parse_time: Unknown time specifier '%c'\n", time[i]);
                    break;
                }
            }

            number = 0;
        }
    }

    return t_minutes;
}
