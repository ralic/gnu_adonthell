/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "gfx.h"
#include "keyboard.h"
#include "SDL.h"

bool keyboard::charkeys[65536];

u_int16 keyboard::last_key;

int keyboard_init(void *) {

  SDL_Event event;
  SDL_EnableUNICODE(1);

  // All key events are saved in the "charkeys" variable in the
  // keyboard namespace.
  while ( SDL_WaitEvent(&event) ) {

    switch (event.type) {
      case SDL_KEYDOWN:
         keyboard::charkeys[event.key.keysym.sym]=1;
	 keyboard::last_key = event.key.keysym.sym;
         break;
      case SDL_KEYUP:
         keyboard::charkeys[event.key.keysym.sym]=0;
         break;
    }
  }
  return(0);
}

void keyboard::update_keyboard() {}
u_int16 keyboard::getkeypressed() { return(last_key); }


bool keyboard::is_pushed(u_int16 key)
{
  return(charkeys[key]);
}
