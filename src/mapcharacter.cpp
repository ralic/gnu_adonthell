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

#include "mapcharacter.h"

void mapcharacter::init()
{
  length=height=submap=posx=posy=offx=offy=0;
  refmap=NULL;
  anim.resize(NBR_MOVES);
  for(u_int16 i=0; i<NBR_MOVES; i++)
    anim[i]=new animation_off;
  current_move=STAND_NORTH;
  ask_move=NO_MOVE;
}

void mapcharacter::clear()
{
  for(u_int16 i=0;i<NBR_MOVES;i++)
    delete(anim[i]);
  anim.clear();
}

mapcharacter::mapcharacter() : maptpl(0,0,1,1,9,9)
{
  init();
}

mapcharacter::~mapcharacter()
{
  clear();
}

s_int8 mapcharacter::get(gzFile file)
{
  int i;
  for(i=0;i<NBR_MOVES;i++)
    {
      anim[i]->get(file);
      anim[i]->play();
    }
  maptpl::get(file);
  calculate_dimensions();
  return 0;
}

s_int8 mapcharacter::load(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"rb"); 
  if(!file) return(-1);
  retvalue=get(file);
  gzclose(file);
  return 0;
}

void mapcharacter::set_pos(u_int16 smap,u_int16 x,u_int16 y)
{
  submap=smap; 
  posx=x; posy=y; 
  refmap->put_mapchar(this,smap,x,y);
}

void mapcharacter::stand_north() 
{
  anim[current_move]->stop();
  anim[current_move]->rewind();
  current_move=STAND_NORTH;anim[current_move]->play();
}

void mapcharacter::stand_south() 
{
  anim[current_move]->stop();
  anim[current_move]->rewind();
  current_move=STAND_SOUTH;anim[current_move]->play();
}

void mapcharacter::stand_east() 
{
  anim[current_move]->stop();
  anim[current_move]->rewind();
  current_move=STAND_EAST;
  anim[current_move]->play();
}

void mapcharacter::stand_west() 
{
  anim[current_move]->stop();
  anim[current_move]->rewind();
  current_move=STAND_WEST;
  anim[current_move]->play();
}

void mapcharacter::go_north() 
{
  if(!posy) 
    {
      if(current_move>=WALK_NORTH && current_move!=WALK_NORTH) return;
      stand_north(); 
      return;
    }
  ask_move=WALK_NORTH;
  if(current_move>=WALK_NORTH) return;
  anim[current_move]->stop();
  anim[current_move]->rewind();
  current_move=WALK_NORTH;
  anim[current_move]->play();
}

void mapcharacter::go_south() 
{
  if(posy==refmap->submap[submap]->height-1 && !offy)
    {
      if(current_move>=WALK_NORTH && current_move!=WALK_SOUTH) return;
      stand_south();
      return;
    }
  ask_move=WALK_SOUTH;
  if(current_move>=WALK_NORTH) return;
  anim[current_move]->stop();
  anim[current_move]->rewind();
  current_move=WALK_SOUTH;
  anim[current_move]->play();
}

void mapcharacter::go_east() 
{
  if(posx==refmap->submap[submap]->length-1 && !offx)
    {
      if(current_move>=WALK_NORTH && current_move!=WALK_EAST) return;
      stand_east();
      return;
    }
  ask_move=WALK_EAST;
  if(current_move>=WALK_NORTH) return;
  anim[current_move]->stop();
  anim[current_move]->rewind();
  current_move=WALK_EAST;
  anim[current_move]->play();
}

void mapcharacter::go_west() 
{
  if(!posx) 
    {
      if(current_move>=WALK_NORTH && current_move!=WALK_WEST) return;
      stand_west(); 
      return;
    }
  ask_move=WALK_WEST;
  if(current_move>=WALK_NORTH) return;
  anim[current_move]->stop();
  anim[current_move]->rewind();
  current_move=WALK_WEST;
  anim[current_move]->play();
}

void mapcharacter::update()
{
  if(refmap) switch(current_move)
    {
    case WALK_NORTH:
      if(!offy) refmap->mapchar_occupy(this,submap,posx,posy-1);
      offy--;
      if(offy==-MAPSQUARE_SIZE)
	{
	  refmap->remove_mapchar(this,submap,posx,posy);
	  refmap->remove_mapchar(this,submap,posx,posy-1);
	  set_pos(submap,posx,posy-1);
	  if(ask_move!=WALK_NORTH) stand_north();
	  offy=0;
	}
      break;
    case WALK_SOUTH:
      if(!offy)
	{
	  refmap->mapchar_occupy(this,submap,posx,posy);
	  refmap->remove_mapchar(this,submap,posx,posy);
	  set_pos(submap,posx,posy+1);
	  set_offset(0,-(MAPSQUARE_SIZE-1));
	}
      else
	{
	  offy++;
	  if(!offy)
	    {
	      refmap->remove_mapchar(this,submap,posx,posy-1);
	      if(ask_move!=WALK_SOUTH) stand_south();
	    }
	}
      break;
    case WALK_WEST:
      if(!offx) refmap->mapchar_occupy(this,submap,posx-1,posy);
      offx--;
      if(offx==-MAPSQUARE_SIZE)
	{
	  refmap->remove_mapchar(this,submap,posx,posy);
	  refmap->remove_mapchar(this,submap,posx-1,posy);
	  set_pos(submap,posx-1,posy);
	  if(ask_move!=WALK_WEST) stand_west();
	  offx=0;
	}
      break;
    case WALK_EAST:
      if(!offx)
	{
	  refmap->mapchar_occupy(this,submap,posx,posy);
	  refmap->remove_mapchar(this,submap,posx,posy);
	  set_pos(submap,posx+1,posy);
	  set_offset(-(MAPSQUARE_SIZE-1),0);
	}
      else
	{
	  offx++;
	  if(!offx)
	    { 
	      refmap->remove_mapchar(this,submap,posx-1,posy);
	      if(ask_move!=WALK_EAST) stand_east();
	    }
	}
      break;
    }
  anim[current_move]->update();
  ask_move=NO_MOVE;
}

void mapcharacter::draw(s_int16 x, s_int16 y, drawing_area * da_opt=NULL)
{
  anim[current_move]->draw(x,y,da_opt);
}

void mapcharacter::draw(mapview * mv)
{
  u_int16 xdraw=((posx-(mv->posx-mv->ctrx)-basex)*MAPSQUARE_SIZE)+offx-mv->offx+mv->x;
  u_int16 ydraw=((posy-(mv->posy-mv->ctry)-basey)*MAPSQUARE_SIZE)+offy-mv->offy+mv->y;
  draw(xdraw,ydraw,mv->da);
}

mapcharacter& mapcharacter::operator =(mapcharacter &m)
{
  u_int16 i,j;
  for (i=0;i<NBR_MOVES;i++)
    (*anim[i])=(*m.anim[i]);
  for(i=0;i<maptpl::length;i++)
    delete[] placetpl[i];
  delete[] placetpl;
  maptpl::length=m.maptpl::length;
  maptpl::height=m.maptpl::height;
  maptpl::basex=m.maptpl::basex;
  maptpl::basey=m.maptpl::basey;
  placetpl=new (mapsquaretpl*)[maptpl::length];
  for(i=0;i<maptpl::length;i++)
    { 
      placetpl[i]=new mapsquaretpl[maptpl::height];
      for(j=0;j<maptpl::height;j++)
	placetpl[i][j].walkable=m.placetpl[i][j].walkable;
    }  
  return *this;
}

void mapcharacter::calculate_dimensions()
{
  u_int16 i;
  length=0;
  height=0;
  for(i=0;i<NBR_MOVES;i++)
    {
      u_int16 tl,th;
      if((tl=anim[i]->get_length()+anim[i]->xoffset)>length)
	length=tl;
      
      if((th=anim[i]->get_height()+anim[i]->yoffset)>height)
	height=th;
    }
}

#ifdef _EDIT_
s_int8 mapcharacter::put(gzFile file)
{
  int i;
  for(i=0;i<NBR_MOVES;i++)
    {
      anim[i]->put(file);
    }
  maptpl::put(file);
  return 0;
}

s_int8 mapcharacter::save(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"wb6"); 
  if(!file) return(-1);
  retvalue=put(file);
  gzclose(file);
  return 0;
}

// New inserting method, much faster: the animation pointed by an must
// however be kept in memory, and will be deleted by destructor.

void mapcharacter::insert_anim(animation_off * an, u_int16 pos)
{
  delete anim[pos];
  anim[pos]=an;
  calculate_dimensions();
  must_upt_label_char=true;
}

void mapcharacter::load_anim()
{
  win_query * qw=new win_query(70,40,th,font,"Load animation:");
  char * s=qw->wait_for_text(makeFunctor(*this,
					 &mapcharacter::update_editor),
			     makeFunctor(*this,
					 &mapcharacter::draw_editor));
  if(!s) return;
  animation a;
  if(a.load(s))
    {
      win_info * wi=new win_info(70,40,th,font,"Error loading!");
      wi->wait_for_keypress(makeFunctor(*this,&mapcharacter::update_editor),
			    makeFunctor(*this,&mapcharacter::draw_editor));
      delete wi;
    }
  else 
    {
      animation_off * ao=new animation_off;
      *ao=a;
      insert_anim(ao,current_move);
      anim[current_move]->play();
    }
  delete qw;
}

void mapcharacter::save()
{
  win_query * qw=new win_query(70,40,th,font,"Save character as:");
  char * s=qw->wait_for_text(makeFunctor(*this,
					 &mapcharacter::update_editor),
			     makeFunctor(*this,
					 &mapcharacter::draw_editor));
  if(!s) return;
  if(save(s))
    {
      win_info * wi=new win_info(70,40,th,font,"Error saving!");
      wi->wait_for_keypress(makeFunctor(*this,&mapcharacter::update_editor),
			    makeFunctor(*this,&mapcharacter::draw_editor));
      delete wi;
    }
  delete qw;
}

void mapcharacter::load()
{
  mapcharacter * t=new mapcharacter;
  win_query * qw=new win_query(70,40,th,font,"Load character:");
  char * s=qw->wait_for_text(makeFunctor(*this,
					 &mapcharacter::update_editor),
			     makeFunctor(*this,
					 &mapcharacter::draw_editor));
  if(!s)
    {
      delete t;
      delete qw;
      return;
    }
  if(t->load(s)) 
    {
      win_info * wi=new win_info(70,40,th,font,"Error loading!");
      wi->wait_for_keypress(makeFunctor(*this,&mapcharacter::update_editor),
			    makeFunctor(*this,&mapcharacter::draw_editor));
      delete wi;
    }
  else 
    {
      *(mapcharacter*)this=*t;
      must_upt_label_frame=true;
      must_upt_label_char=true;
    }
  delete t;
  delete qw;
}

void mapcharacter::update_editor()
{
  update();
  maptpl::update();
  container->update();
}

void mapcharacter::set_anim_xoffset(u_int16 p, s_int16 xoff)
{
  if(xoff<0) return;
  anim[p]->xoffset=xoff;
  must_upt_label_char=true;
}

void mapcharacter::set_anim_yoffset(u_int16 p, s_int16 yoff)
{
  if(yoff<0) return;
  anim[p]->yoffset=yoff;
  must_upt_label_char=true;
}

void mapcharacter::update_label_frame()
{
  label_frame->set_text(current_move==STAND_NORTH?"Looking North":
			current_move==STAND_SOUTH?"Looking South":
			current_move==STAND_WEST?"Looking West":
			current_move==STAND_EAST?"Looking East":
			current_move==WALK_NORTH?"Walking to North":
			current_move==WALK_SOUTH?"Walking to South":
			current_move==WALK_WEST?"Walking to West":
			current_move==WALK_EAST?"Walking to East":"");
  must_upt_label_char=false;
}

void mapcharacter::update_label_char()
{
  sprintf(label_txt,"Character:\nLength: %d\nHeight:%d",length,height);
  label_char->set_text(label_txt);
  must_upt_label_char=false;
}

void mapcharacter::draw_editor()
{
  bg->draw(0,0);
  calculate_dimensions();
  draw(0,0);
  if(show_grid) maptpl::draw();
  update_label_frame();
  update_label_char();
  container->draw();
}

void mapcharacter::update_and_draw()
{
  static u_int16 i;
  for(i=0;i<screen::frames_to_do();i++) update_editor();
  draw_editor();
}

inline bool testkey(SDLKey k)
{
  if(SDL_GetModState()&KMOD_LCTRL)
    return((input::is_pushed(k)));
  else return ((input::has_been_pushed(k)));
}

void mapcharacter::update_editor_keys()
{
  if(input::has_been_pushed(SDLK_F5))
    save();

  if(input::has_been_pushed(SDLK_F6))
    load();
  
  if(testkey(SDLK_KP_PLUS))
    {
      anim[current_move]->stop();
      anim[current_move]->rewind();
      current_move=(current_move==NBR_MOVES-1)?0:current_move+1;
      anim[current_move]->play();
      must_upt_label_frame=true;
    }
  if(testkey(SDLK_KP_MINUS))
    {
      anim[current_move]->stop();
      anim[current_move]->rewind();
      current_move=(current_move==0)?NBR_MOVES-1:current_move-1;
      anim[current_move]->play();
      must_upt_label_frame=true;
    }

  if(testkey(SDLK_LEFT) && show_grid)
    {
      if(SDL_GetModState()&KMOD_SHIFT)
	{ 
	  if(anim[current_move]->xoffset)
	    set_anim_xoffset(current_move,anim[current_move]->xoffset-1); 
	}
      else if(SDL_GetModState() & (KMOD_META | KMOD_ALT))
	{ 
	  if((maptpl::get_length()-1)*MAPSQUARE_SIZE>=
	     anim[current_move]->get_length()+anim[current_move]->xoffset)
	    maptpl::resize(maptpl::get_length()-1,maptpl::get_height()); 
	}
      else
	{ move_cursor_left(); }
    }
  if(testkey(SDLK_RIGHT) && show_grid)
    {
      if(SDL_GetModState()&KMOD_SHIFT)
	{ 
	  if(anim[current_move]->xoffset+anim[current_move]->get_length()<
	     maptpl::length*MAPSQUARE_SIZE)
	    set_anim_xoffset(current_move,anim[current_move]->xoffset+1); 
	}
      else if(SDL_GetModState() & (KMOD_META | KMOD_ALT))
	{ maptpl::resize(maptpl::get_length()+1,maptpl::get_height()); }
      else
	{ move_cursor_right(); }
    }
  if(testkey(SDLK_UP) && show_grid)
    {
      if(SDL_GetModState()&KMOD_SHIFT)
	{ 
	  if(anim[current_move]->yoffset)
	    set_anim_yoffset(current_move,anim[current_move]->yoffset-1);
	}
      else if(SDL_GetModState() & (KMOD_META | KMOD_ALT))
	{ 
	  if((maptpl::get_height()-1)*MAPSQUARE_SIZE>=
	     anim[current_move]->get_height()+anim[current_move]->yoffset)
	    maptpl::resize(maptpl::get_length(),maptpl::get_height()-1); 
	}
      else
	{ move_cursor_up(); }
    }
  if(testkey(SDLK_DOWN) && show_grid)
    {
      if(SDL_GetModState()&KMOD_SHIFT)
	{ 
	  if(anim[current_move]->yoffset+anim[current_move]->get_height()<
	     maptpl::height*MAPSQUARE_SIZE)
	    set_anim_yoffset(current_move,anim[current_move]->yoffset+1); 
	}
      else if(SDL_GetModState() & (KMOD_META | KMOD_ALT))
	{ maptpl::resize(maptpl::get_length(),maptpl::get_height()+1); }
      else
	{ move_cursor_down(); }
    }

  if(testkey(SDLK_g))
     show_grid=show_grid?false:true;

  if(input::has_been_pushed(SDLK_SPACE))
      toggle_walkable();
  if(input::has_been_pushed(SDLK_b))
    if(show_grid)
      set_base_tile(posx,posy);

  if(input::has_been_pushed(SDLK_F1)) load_anim();
}

void mapcharacter::editor()
{
  u_int16 i;
  image temp;
  string t;
  t=WIN_DIRECTORY;
  t+=WIN_BACKGROUND_DIRECTORY;
  t+=WIN_THEME_ORIGINAL;
  t+=WIN_BACKGROUND_FILE;
  temp.load_pnm(t.data());
  bg=new image(320,240);
  bg->putbox_tile_img(&temp);
  font=new win_font(WIN_THEME_ORIGINAL);
  th=new win_theme(WIN_THEME_ORIGINAL);  
  container=new win_container(200,12,110,216,th);
  label_frame=new win_label(5,5,100,30,th,font);
  label_char=new win_label(5,70,100,120,th,font);
  container->add(label_frame);
  container->add(label_char);
  container->set_visible_all(true);
  container->set_border_visible(true);
  must_upt_label_frame=true;
  must_upt_label_char=true;
  show_grid=false;
  while(!input::has_been_pushed(SDLK_ESCAPE))
    {
      input::update();
      for(i=0;i<screen::frames_to_do();i++) update_editor_keys();
      update_and_draw();
      screen::show();
    }
  delete container;
  delete bg;
  delete font;
  delete th;
}
#endif

