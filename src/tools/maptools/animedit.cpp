/*
   $Id$
 
   Copyright (C) 2001 by Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "animation.h"
#include "game.h"

int main(int argc, char * argv[])
{
    game the_game (argc, argv);
    if (!the_game.init ()) 
        return 1;

    animation anim;
    if(argc==2) anim.load(argv[1]);
    anim.editor();

    return 0;
}
