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

#ifndef _animation_h
#define _animation_h

#include "types.h"
#include "image.h"
#include <vector>

#ifdef _EDIT_
#include <list>
#include <string>
#include "input.h"
#include "prefs.h"
#include "win_types.h"
#include "win_base.h"
#include "win_font.h"
#include "win_label.h"
#include "win_write.h"
#include "win_border.h"
#include "win_image.h"
#include "win_anim.h"
#include "win_container.h"
#include "win_select.h"
#include "win_theme.h"
#include "win_background.h"
#include "win_query.h"
#include "win_info.h"
#include "win_file_select.h"

#ifdef _EDIT_
typedef enum anim_editor_mode {IMAGE, FRAME};
#endif

#endif

class animationframe
{
 public:
  animationframe();
  void clear();
  ~animationframe();
  u_int8 get_alpha();
  void set_alpha(u_int8 a);
  bool get_mask();
  void set_mask(bool mask);
  u_int16 get_image();
  void set_image(u_int16 imnbr);
  u_int16 get_delay();
  void set_delay(u_int16 d);
  u_int16 get_nextframe();
  void set_nextframe(u_int16 nf);
  s_int8 get(gzFile file);
  s_int8 load(const char * fname);

  u_int16 get_offx() { return gapx;}
  void set_offx(u_int16 ox) { gapx=ox;}
  u_int16 get_offy() { return gapy;}
  void set_offy(u_int16 oy) { gapy=oy;}
 private:
  u_int16 imagenbr;
  bool is_masked;
  u_int8 alpha;
  s_int16 gapx;
  s_int16 gapy;
  u_int16 delay;
  u_int16 nextframe;
#ifdef _DEBUG_
  static u_int16 a_d_diff;
#endif

  void init();

 public:
#ifdef _EDIT_
  s_int8 put(gzFile file);
  s_int8 save(const char * fname);
#endif

#ifndef SWIG
  friend class animation;
#endif
#ifndef SWIG
  friend class win_anim;
  friend class animation_off;
#endif
};

class animation
{
 public:
  animation();
  void clear();
  ~animation();
  bool is_empty();
  u_int16 get_length();
  u_int16 get_height();

  void play();
  void stop();
  void rewind();

  void update();
  void draw(s_int16 x, s_int16 y, drawing_area * da_opt=NULL);

  s_int8 get(gzFile file);
  s_int8 load(const char * fname);

  u_int16 nbr_of_frames() { return frame.size(); }
  u_int16 nbr_of_images() { return t_frame.size(); }

  u_int16 get_currentframe() { return currentframe; };

  void next_frame();
  void set_currentframe(u_int16 framenbr) { currentframe=framenbr; }
  
  animationframe * get_frame(u_int16 nbr) { return &(frame[nbr]); }
  animationframe * get_current_frame() 
    { return get_frame(get_currentframe()); }
  image * get_image(u_int16 nbr) { return t_frame[nbr]; }

  u_int16 add_image(image * im) { insert_image(im, nbr_of_images()); return nbr_of_images()-1;}
  u_int16 add_frame(animationframe af) { insert_frame(af, nbr_of_frames()); return nbr_of_frames()-1;}

  void zoom(u_int16 sx, u_int16 sy, animation * src);

 private:
#ifdef _EDIT_
  char file_name[500];
  win_label * info_win_label;
  win_container * info_win;
  u_int8 info_win_count;

  image * clipboard;
  animationframe f_clipboard;
  bool mode;
  char frame_txt[500];
  win_font * font;
  win_theme * th;

  win_container * container;
  win_label * label_mode;
  win_label * label_frame_nbr;
  win_label * label_frame_info;
  win_label * label_anim_info;
  bool must_upt_label_mode;
  bool must_upt_label_frame_nbr;
  bool must_upt_label_frame_info;
  bool must_upt_label_status;

  u_int16 currentimage;
  bool in_editor;
#endif

  void init();

 protected:
#ifdef _DEBUG_
  static u_int16 a_d_diff;
#endif
  vector<image *> t_frame;
  vector<animationframe> frame;
  u_int16 length, height;
  u_int16 currentframe;
  u_int16 speedcounter;
  bool play_flag;

  s_int8 insert_image(image * im, u_int16 pos);
  s_int8 insert_frame(animationframe af, u_int16 pos);
  s_int8 delete_image(u_int16 pos);
  s_int8 delete_frame(u_int16 pos);

 public:
#ifdef _EDIT_
  s_int8 put(gzFile file);
  s_int8 save(const char * fname);

  void select_image(u_int16 nbr);
  void select_frame(u_int16 nbr);

  void set_frame_gapx(u_int16 nbr, s_int16 gx);
  void set_frame_gapy(u_int16 nbr, s_int16 gy);
  void set_frame_alpha(u_int16 nbr, u_int8 a);
  void set_frame_delay(u_int16 nbr, u_int16 d);
  void set_frame_nextframe(u_int16 nbr, u_int16 nf);
  void set_frame_mask(u_int16 nbr, bool m);
  void set_frame_imagenbr(u_int16 nbr, u_int16 imnbr);
  u_int16 increase_frame(u_int16 c);
  u_int16 decrease_frame(u_int16 c);
  u_int16 increase_image(u_int16 c);
  u_int16 decrease_image(u_int16 c);

  void ed_add_image();
  void ed_add_raw_image();
  void ed_add_frame();

  void quick_save();
  void quick_load();
  void save();
  void load();

  void save_image();
  void save_image_raw();

  void set_mode(anim_editor_mode m);

  void update_label_mode();
  void update_label_frame_nbr();
  void update_label_frame_info();
  void update_label_status();
  void update_editor_keys();
  void update_editor();

  void draw_editor();
  void update_and_draw();
  void set_info_win(char * text);
  void editor();
#endif

#ifndef SWIG
  friend class win_anim;
  friend class animation_off;
  animation &operator =(animation &a);
#endif
};

#endif
