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

#ifndef _screen_h
#define _screen_h

#include "types.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

class screen
{
  static u_int16 width;
  static u_int16 height;

  static void init_gfx_buffers();
  // Called by init_display to init the drawable fx buffers.
 public:

  static u_int8 bytes_per_pixel;
  static u_int8 frames_to_do;
  static u_int32 trans;

  static SDL_Surface * vis;
  static SDL_Surface * getbuffer;

  static void set_video_mode(u_int16 w, u_int16 h);
  static void init_display();
  static void show();
  // update the screen.

  static void drawbox(u_int16 x, u_int16 y, u_int16 w, u_int16 h, 
		      u_int32 color);
  // Draw a box of a given color.

  static void makesquare(u_int16 px, u_int16 py, u_int16 factor);
  // FIXME: Used in mapeditor only, move it somewhere else

  static void mouse_cursor_off();
  static void mouse_cursor_on();
  // Mouse cursor display functions.
};

#endif