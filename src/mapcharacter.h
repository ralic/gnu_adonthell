#ifndef _mapcharacter_h
#define _mapcharacter_h

#include "gfx.h"

class map;
class mapcharacter
{
  image * frame;
  u_int16 posx;
  u_int16 posy;
  s_int8 addx;
  s_int8 addy;
  u_int8 prevmovtype;
  u_int8 speeddelay;
  u_int8 speedcounter;
  u_int8 movtype;
  u_int16 framefactor;      /* u_int8 */
  u_int8 walkcounter;
  u_int8 moveframe;
  u_int8 scridx;
  u_int8 waitcounter;
  u_int16 nbr_of_frames;
  u_int16 currentframe;
  u_int8 is_event;
  u_int8 mapcharnbr;

 public:
  mapcharacter();
  ~mapcharacter();
  s_int8 get(FILE * file);
  s_int8 load(char * fname);
  void get_heroe_stat(FILE * file);
  void get_NPC_stat(FILE * file, u_int16 nbr);
  void init_moveframe();
  u_int16 get_nbr();
  u_int16 get_posx();
  u_int16 get_posy();
  u_int8 get_addx();
  u_int8 get_addy();
  u_int8 get_scridx();
  u_int8 get_movtype();
  u_int8 get_speeddelay();
  void draw(u_int16 x, u_int16 y);
  void draw_part(u_int16 x, u_int16 y, u_int16 bw, u_int16 bh, 
		 u_int16 xo, u_int16 yo);
  bool is_ready();
  bool is_speed_ready();
  void set_speeddelay(u_int8 sd);
  void set_movtype(u_int8 mt);
  void set_framefactor(u_int8 ff);
  void set_posx(u_int16 x);
  void set_posy(u_int16 y);
  void update_frame();
  void update_NPC(map * amap);
  void update_dir();
  void update_move(map*amap);
  void update_main_move(map*amap);
  void update_pos(map*amap);
  void update_eventleave(map*amap);
  void update_eventcome(map*amap);
};

#define MAPCHAR_EVENT_COME 1
#define MAPCHAR_EVENT_LEFT 2

#endif
