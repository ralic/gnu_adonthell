/*
   $Id$

   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include <string.h>
#include <list>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_friend.h"
#include "win_base.h"
#include "win_font.h"
#include "win_container.h"
#include "win_select.h"
#include "win_label.h"


win_label::win_label(u_int16 x,u_int16 y,u_int16 l,u_int16 h,win_font *fo,win_container * tmpwc):win_base(x,y,l,h,tmpwc,tmpwc->get_drawing_area())
{
  font=fo;
  text.lenght=0;
  text.pos=0;
  text.pos_tmp=0;
  ch_page=false;
}

win_label::~win_label()
{
    if(wselect)
     {
       wselect->remove(this);
       wselect=NULL;
     }
}

void win_label::set_font(win_font * fo)
{
  font=fo;
}

void win_label::set_text(char t[])
{
  strcpy(text.text,t);
  text.lenght=strlen(text.text);
  text.pos=0;
  text.pos_tmp=0;
}

void win_label::erase_text()
{
  text.lenght=0;
  text.pos_tmp=0;
}

void win_label::draw()
{
  if(visible && wc) 
    {
       draw_background();
       if(wselect && !selected && select_mode==WIN_SELECT_MODE_BRIGHTNESS) draw_text(real_x,real_y,real_x+length,real_y+height,font,text,da,true);
       else draw_text(real_x,real_y,real_x+length,real_y+height,font,text,da,false);
       draw_border();
     }
}

void win_label::update()
{
  if(ch_page)
    {ch_page=false;text.pos=text.pos_tmp;}
}

void win_label::next_page()
{
  ch_page=true;
}

bool win_label::end_text()
{
  return(text.pos==text.lenght);
}