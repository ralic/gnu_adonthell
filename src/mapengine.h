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

class mapengine
{
  
 public:
  static void map_engine(map*amap);
  static void update_and_show(map * amap);
  static void fade_out(map*amap); 
  static void fade_in(map*amap); 
};
