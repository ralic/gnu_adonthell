#include <iostream.h>
#include <list>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_border.h"
#include "win_container.h"
#include "win_select.h"

void win_base::init_base(u_int16 tx,u_int16 ty, u_int16 tl,u_int16 th,win_container *twc,drawing_area * tda)
{
   //set position
   win_base::x=tx;
   win_base::y=ty;
   win_base::length=tl;
   win_base::height=th;
   
   
   //set flags
   selected=false;
   visible=false;
   
   
   wc=twc;
   da=tda;

   //update position
   win_base::update_da();

   //initialize image
   h_border=NULL;
   v_border=NULL;
   corner=NULL;
   background=NULL;
   wborder=NULL;
   wback=NULL;
   
   //initialize transluency of background
   level_trans_back=180;

   //initialize select
   wselect=NULL;

   //init_var
   act_function=NULL;

}

win_base::win_base(u_int16 tx,u_int16 ty, u_int16 tl,u_int16 th,win_container * twc=NULL,drawing_area * tda=NULL)
{
   init_base(tx,ty,tl,th,twc,tda);
}

win_base::~win_base()
{
   wc=NULL;
   da=NULL;
   wborder=NULL;
   wback=NULL; 
 
   if(h_border) delete h_border;
   if(v_border) delete v_border;
   if(corner) delete corner;
   if(background) delete background;
}


void win_base::select()
{
   selected=true;
}

void win_base::unselect()
{
   selected=false;
}

void win_base::show()
{
  visible=true;
}

void win_base::hide()
{
  visible=false;
}

u_int16 win_base::get_x()
{
   return(x);
}

u_int16 win_base::get_y()
{
   return(y);
}

u_int16 win_base::get_length()
{
   return(length);
}

u_int16 win_base::get_height()
{
   return(height);
}

void win_base::resize(u_int16 tl, u_int16 th)
{
   length=tl;
   height=th;
   resize_border();
   resize_background();
}

void win_base::move(u_int16 tx, u_int16 ty)
{
   x=tx;
   y=ty;
   win_base::update_da();
}

drawing_area * win_base::get_drawing_area()
{
   return(da);
}

win_border * win_base::get_border()
{
  return(wborder);
}

void win_base::set_border(win_border * twb)
{
   wborder=twb;
   if(wborder)
     {
	if(!h_border) h_border=new image();
	if(!v_border) v_border=new image();
	if(!corner) corner=new image();
	*corner=*(twb->corner);
	resize_border();
     }
}


void win_base::resize_border()
{
   if(wborder)
     {
	v_border->resize(wborder->v_border_template->length,height);
	v_border->putbox_tile_img(wborder->v_border_template);
	h_border->resize(length,wborder->h_border_template->height);
	h_border->putbox_tile_img(wborder->h_border_template); 
     }
}




void win_base::draw_border()
{
  if(wborder)
    {
#define WIN_CORNER_RELX (corner->length>>1)
#define WIN_CORNER_RELY (corner->height>>1)
#define WIN_HBRELX (wborder->h_border_template->length>>1)
#define WIN_HBRELY (wborder->h_border_template->height>>1)
#define WIN_VBRELX (wborder->v_border_template->length>>1)
#define WIN_VBRELY (wborder->v_border_template->height>>1)
      if(wselect && select_mode==WIN_SELECT_MODE_BRIGHTNESS)
	{
	  if(selected)
	    {
	  
	       
		  if(wc)
		    { 
		      h_border->putbox(real_x,real_y-h_border->height,wc->get_drawing_area());
		      h_border->putbox(real_x,real_y+height,wc->get_drawing_area());
		      v_border->putbox(real_x-v_border->length,real_y,wc->get_drawing_area());
		      v_border->putbox(real_x+length,real_y,wc->get_drawing_area());
		      
		      corner->putbox_mask(real_x-WIN_CORNER_RELX-((v_border->length)>>1),
					  real_y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());      
		      corner->putbox_mask(real_x+length-WIN_CORNER_RELX+(v_border->length>>1),
				      real_y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());      
		      corner->putbox_mask(real_x-WIN_CORNER_RELX-((v_border->length)>>1),
					  real_y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area());     
		      corner->putbox_mask(real_x+length-WIN_CORNER_RELX+(v_border->length>>1),
				      real_y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area());
		    }
		  else
		    {
		      //image tmp;
		      //tmp.brightness(h_border,120);
		      h_border->putbox(x,y-h_border->height);
		      h_border->putbox(x,y+height);
		      
		      //tmp.brightness(v_border,120);
		      v_border->putbox(x-v_border->length,y);
		      v_border->putbox(x+length,y);
		      
		      //tmp.brightness(corner,120);
		      corner->putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
					  y-WIN_CORNER_RELY-(h_border->height>>1));      
		      corner->putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
					  y-WIN_CORNER_RELY-(h_border->height>>1));      
		      corner->putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
					  y+height-WIN_CORNER_RELY+(h_border->height>>1));     
		      corner->putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
					  y+height-WIN_CORNER_RELY+(h_border->height>>1));
		    }
		}
	      else
		{//if wselect but not select
		  image tmp;
		  if(wc)
		    { 
		      tmp.brightness(h_border,120);
		      tmp.putbox(real_x,real_y-h_border->height,wc->get_drawing_area());
		      tmp.putbox(real_x,real_y+height,wc->get_drawing_area());
		      tmp.brightness(v_border,120);
		      tmp.putbox(real_x-v_border->length,real_y,wc->get_drawing_area());
		      tmp.putbox(real_x+length,real_y,wc->get_drawing_area());
		      
		      tmp.brightness(corner,120);
		      tmp.putbox_mask(real_x-WIN_CORNER_RELX-((v_border->length)>>1),
				      real_y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());      
		      tmp.putbox_mask(real_x+length-WIN_CORNER_RELX+(v_border->length>>1),
				      real_y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());      
		      tmp.putbox_mask(real_x-WIN_CORNER_RELX-((v_border->length)>>1),
				      real_y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area());     
		      tmp.putbox_mask(real_x+length-WIN_CORNER_RELX+(v_border->length>>1),
				      real_y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area());
		    }
		  else
		    {
		      tmp.brightness(h_border,120);
		      tmp.putbox(x,y-h_border->height);
		      tmp.putbox(x,y+height);
		      
		      tmp.brightness(v_border,120);
		      tmp.putbox(x-v_border->length,y);
		      tmp.putbox(x+length,y);
		      
		      tmp.brightness(corner,120);
		      tmp.putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
				      y-WIN_CORNER_RELY-(h_border->height>>1));      
		      tmp.putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
				      y-WIN_CORNER_RELY-(h_border->height>>1));      
		      tmp.putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
				      y+height-WIN_CORNER_RELY+(h_border->height>>1));     
		      tmp.putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
				      y+height-WIN_CORNER_RELY+(h_border->height>>1));
		    }
	      
	      
	    }
	}
      else
	{//if not wselect
	  if(wc)
	    { 
	      h_border->putbox(real_x,real_y-h_border->height,wc->get_drawing_area());
	      h_border->putbox(real_x,real_y+height,wc->get_drawing_area());
	      v_border->putbox(real_x-v_border->length,real_y,wc->get_drawing_area());
	      v_border->putbox(real_x+length,real_y,wc->get_drawing_area());
	      
	      corner->putbox_mask(real_x-WIN_CORNER_RELX-((v_border->length)>>1),
				  real_y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());      
	      corner->putbox_mask(real_x+length-WIN_CORNER_RELX+(v_border->length>>1),
				  real_y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());      
	      corner->putbox_mask(real_x-WIN_CORNER_RELX-((v_border->length)>>1),
				  real_y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area());     
	      corner->putbox_mask(real_x+length-WIN_CORNER_RELX+(v_border->length>>1),
				  real_y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area());
	    }
	  else
	    {
	      h_border->putbox(x,y-h_border->height);
	      h_border->putbox(x,y+height);
	      v_border->putbox(x-v_border->length,y);
	      v_border->putbox(x+length,y);
	      
	      corner->putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
				  y-WIN_CORNER_RELY-(h_border->height>>1));      
	      corner->putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
				  y-WIN_CORNER_RELY-(h_border->height>>1));      
	      corner->putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
				  y+height-WIN_CORNER_RELY+(h_border->height>>1));     
	      corner->putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
				  y+height-WIN_CORNER_RELY+(h_border->height>>1));
	    }
	}
    }
}


void win_base::set_background(win_background * twb)
{
  wback=twb;
  if(!background) background = new image();
  resize_background();  
}


void win_base::draw_background()
{
  if(wback)
    {
      if(wselect && select_mode==WIN_SELECT_MODE_BRIGHTNESS)
	{
	  if(selected)
	    {
	      if(wc) background->putbox_trans(real_x,real_y,level_trans_back,wc->get_drawing_area());
	      else background->putbox_trans(real_x,real_y,level_trans_back);
	    } 
	  else
	    {
	      image tmp;
	      tmp.brightness(background,120);
	      if(wc) tmp.putbox_trans(real_x,real_y,level_trans_back,wc->get_drawing_area());
	      else tmp.putbox_trans(real_x,real_y,level_trans_back);
	    }
	}
      else
	{
	  if(wc) background->putbox_trans(real_x,real_y,level_trans_back,wc->get_drawing_area());
	  else background->putbox_trans(real_x,real_y,level_trans_back);
	}
    }
}


void win_base::resize_background()
{
  if(wback) 
    {
      background->resize(length,height);
      background->putbox_tile_img(wback->background_template);
    }
}

void win_base::update_da()
{
  if(wc)
    {
      real_x=x+wc->real_x;
      real_y=y+wc->real_y;
    }
  else 
    {
      real_x=x;
      real_y=y;
    }
}


void win_base::attach_activate_function(void (*f)(void*,void*),void * d1,void * d2)
{
  act_function=f;
  param_func1=d1;
  param_func2=d2;
}

void win_base::detach_activate_function()
{
  act_function=NULL;
  param_func1=NULL;
  param_func2=NULL;
}
void win_base::execute_activate_function()
{
  if(act_function) 
    {
      act_function(param_func1,param_func2);
    }
}

void win_base::activate()
{
  execute_activate_function();
}










