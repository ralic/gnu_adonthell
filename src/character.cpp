/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "character.h"

// Returns the active dialogue
const char* npc::talk ()
{
    return dialogues[active_dialogue];
}

// Set the active dialogue
void npc::set_dialogue (u_int32 dlg)
{
    active_dialogue = dlg;
}