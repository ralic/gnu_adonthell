/*
   $Id$

   Copyright (C) 1999/2000   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef _MAPVIEW_H_
#define _MAPVIEW_H_

#include "mapobject.h"
class mapsquare_tile;
class landmap;

class mapview
#ifdef _EDIT_
: public mapselect
#endif
{
  u_int16 length,height;      // size of the view in pixels
  u_int16 d_length,d_height;  // size of the view in map squares

  u_int16 currentsubmap;
  u_int16 x,y;
  u_int16 posx, posy;
  u_int16 offx, offy;

  landmap * m_map;

  u_int16 ctrx,ctry;
  drawing_area * da;

  list<mapsquare_tile> critical_draw;

#ifdef _EDIT_
  win_font * font;
  win_theme * th;
  win_container * container;
  win_label * label_pos;
  win_label * label_object;
  win_label * label_square;

  bool must_upt_label_pos;
  bool must_upt_label_object;
  bool must_upt_label_square;

  char tmps[500];

  list<mapsquare_tile>::iterator current_tile;

  void update_label_pos();
  void update_label_object();
  void update_label_square();

  void decrease_obj_here();
  void increase_obj_here();
  void decrease_currentobj();
  void increase_currentobj();
  u_int16 currentobj;
#endif

 public:
  mapview();
  ~mapview();

  void attach_map(landmap * m);
  void detach_map();
  s_int8 set_current_submap(u_int16 sm);
  s_int8 set_pos(u_int16 x, u_int16 y);
  void scroll_right();
  void scroll_left();
  void scroll_down();
  void scroll_up();
  u_int16 get_current_submap() { return currentsubmap; }
  u_int16 get_posx() { return posx; }
  u_int16 get_posy() { return posy; }
  void resize(u_int16 d_l, u_int16 d_h);
  void update();
  void draw_cursor();
  void draw(u_int16 x, u_int16 y, drawing_area * da_opt=NULL);

#ifdef _EDIT_
  void move_cursor_left();
  void move_cursor_right();
  void move_cursor_up();
  void move_cursor_down();

  void resize_map();
  void add_mapobject();

  void update_current_tile(mapsquare_tile t);

  void update_editor();
  void draw_editor();
  void update_editor_keys();
  void update_and_draw();
  void editor();
#endif
  friend class mapsquare_tile;
  friend class mapsquare;
};

#endif