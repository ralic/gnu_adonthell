// $Id$
/*
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
#include "pnm.h"

u_int16 screen::screenwidth;
u_int16 screen::screenheight;
u_int8 screen::frames_to_do;
u_int8 screen::bytes_per_pixel;
u_int8 screen::sizefactor;
u_int32 screen::trans;

u_int8 image::bytes_per_pixel, image::sizefactor;
SDL_Rect image::sr;
SDL_Rect image::dr;

SDL_Surface * screen::vis;
SDL_Surface * screen::getbuffer;

image::image ()
{
  data = NULL;
  lenght=height=0;
  mask_on=false;
  trans_on=false;
}

image::~image()
{
  SDL_FreeSurface(data);
  data=NULL;
}

u_int16 image::get_lenght()
{
  return(lenght);
}

u_int16 image::get_height()
{
  return(height);
}

void image::set_sizefactor(u_int8 x)
{
  sizefactor=x;
}

void image::putbox (u_int16 x, u_int16 y)
{
  x*=sizefactor;
  y*=sizefactor;
  dr.x=x;
  dr.y=y;
  dr.w=lenght;
  dr.h=height;
  if(mask_on) { SDL_SetColorKey(data,0,0); mask_on=false; }
  if(trans_on) { SDL_SetAlpha(data,0,0); trans_on=false; }
  SDL_BlitSurface(data, NULL, screen::vis, &dr);
}

void image::putbox_mask (u_int16 x, u_int16 y)
{
  x*=sizefactor;
  y*=sizefactor;
  dr.x=x;
  dr.y=y;
  dr.w=lenght;
  dr.h=height;
  if(trans_on) { SDL_SetAlpha(data,0,0); trans_on=false; }
  if(!mask_on) 
    {
      SDL_SetColorKey(data, SDL_SRCCOLORKEY|SDL_RLEACCEL, screen::get_trans());
      mask_on=true;
    }
  SDL_BlitSurface(data, NULL, screen::vis, &dr);
}

void image::putbox_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
			 u_int16 xo, u_int16 yo)
{
  if(sizefactor!=1)
    {
      x*=sizefactor;
      y*=sizefactor;
      bw*=sizefactor;
      bh*=sizefactor;
      xo*=sizefactor;
      yo*=sizefactor;
    }
  sr.x=xo;
  sr.y=yo;
  sr.w=bw;
  sr.h=bh;
  dr.x=x;
  dr.y=y;
  dr.w=bw;
  dr.h=bh;
  SDL_BlitSurface(data, &sr, screen::vis, &dr);
}

void image::putbox_trans (u_int16 x, u_int16 y, u_int8 alpha)
{
  x*=sizefactor;
  y*=sizefactor;
  dr.x=x;
  dr.y=y;
  dr.w=lenght;
  dr.h=height;

  if(mask_on) { SDL_SetColorKey(data,0,0); mask_on=false; }
  SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
  trans_on=true;
  SDL_BlitSurface(data, NULL, screen::vis, &dr);  
}

void image::putbox_mask_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
			      u_int16 xo, u_int16 yo)
{
  if(sizefactor!=1)
    {
      x*=sizefactor;
      y*=sizefactor;
      bw*=sizefactor;
      bh*=sizefactor;
      xo*=sizefactor;
      yo*=sizefactor;
    }
  sr.x=xo;
  sr.y=yo;
  sr.w=bw;
  sr.h=bh;
  dr.x=x;
  dr.y=y;
  dr.w=bw;
  dr.h=bh;
  if(trans_on)
    {
      SDL_SetAlpha(data,0,0);
      trans_on=false;
    }
  if(!mask_on)
    {
      SDL_SetColorKey(data, SDL_SRCCOLORKEY|SDL_RLEACCEL, screen::get_trans());
      mask_on=true;
    }
  SDL_BlitSurface(data, &sr, screen::vis, &dr);
}

void image::putbox_mask_trans (u_int16 x, u_int16 y, u_int8 alpha)
{
  x*=sizefactor;
  y*=sizefactor;
  dr.x=x;
  dr.y=y;
  dr.w=lenght;
  dr.h=height;
  if(!mask_on)
    {
      SDL_SetColorKey(data, SDL_SRCCOLORKEY|SDL_RLEACCEL, screen::get_trans());
      mask_on=true;
    }
  SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
  trans_on=true;
  SDL_BlitSurface(data, NULL, screen::vis, &dr);
}

void image::putbox_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
			       u_int16 bh, u_int16 xo, u_int16 yo, 
			       u_int8 alpha)
{
  if(sizefactor!=1)
    {
      x*=sizefactor;
      y*=sizefactor;
      bw*=sizefactor;
      bh*=sizefactor;
      xo*=sizefactor;
      yo*=sizefactor;
    }
  sr.x=xo;
  sr.y=yo;
  sr.w=bw;
  sr.h=bh;
  dr.x=x;
  dr.y=y;
  dr.w=bw;
  dr.h=bh;

  SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
  trans_on=true;
  SDL_BlitSurface(data, &sr, screen::vis, &dr);
}


void image::putbox_mask_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
				    u_int16 bh, u_int16 xo, u_int16 yo, 
				    u_int8 alpha)
{
  if(sizefactor!=1)
    {
      x*=sizefactor;
      y*=sizefactor;
      bw*=sizefactor;
      bh*=sizefactor;
      xo*=sizefactor;
      yo*=sizefactor;
    }
  sr.x=xo;
  sr.y=yo;
  sr.w=bw;
  sr.h=bh;
  dr.x=x;
  dr.y=y;
  dr.w=bw;
  dr.h=bh;
  if(!mask_on)
    {
      SDL_SetColorKey(data, SDL_SRCCOLORKEY|SDL_RLEACCEL, screen::get_trans());
      mask_on=true;
     }
  SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
  trans_on=true;
  SDL_BlitSurface(data, &sr, screen::vis, &dr);
}

void image::gfxrealloc(u_int32 l)
{
 // void * tmp;
//  tmp=calloc(bytes_per_pixel,sizefactor*sizefactor*lenght*height*l);
//  data->pixels=(void*)realloc(data,l*bytes_per_pixel*sizefactor*sizefactor);
 
}

void image::size(u_int16 l, u_int16 h)
{
  if(!data)
    {  
      lenght=l;
      height=h;
    }

}

void image::resize(u_int16 l, u_int16 h)
{
  if(!data)
    {  
      lenght=l*sizefactor;
      height=h*sizefactor;
      data=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,screen::get_bytes_per_pixel()*8,0,0,0,0);
    }

}

void image::putbox_tile_img(image * source)
{
  u_int16 posx;
  u_int16 posy;
  for(posy=0; posy<height; posy+=source->height)
    for(posx=0; posx<lenght; posx+=source->lenght)
      putbox_img(source,posx,posy);

}

void image::putbox_img(image * source, u_int16 x, u_int16 y)
{
 
  sr.x=0;
  sr.y=0;
  sr.w=lenght;
  sr.h=height;
  dr.x=x;
  dr.y=y;
  dr.w=lenght;
  dr.h=height;
  SDL_BlitSurface(source->data, &sr, data, &dr);

}

void image::putbox_mask_img (image * source, u_int16 x, u_int16 y)
{
  u_int32 i, j;
  u_int8 mltp=sizefactor*sizefactor;
  u_int32 l=lenght*mltp, h=height*mltp;
  u_int32 sl=source->lenght*mltp, sh=source->height*mltp;
  x*=mltp;y*=mltp;
  switch (screen::get_bytes_per_pixel())
    {
    case 2:
      for(j=0;(j<sh)&&(j+y<h);j++)
	for(i=0;(i<sl)&&(i+x<l);i++)
	  if((*((u_int16*)source->data->pixels+(j*sl)+i))!=screen::get_trans())
      break;
    case 3:
      // Think about a good method
      break;
    }
}

void image::putbox_part_img (image * source, u_int16 x, u_int16 y, u_int16 bw, 
			     u_int16 bh,u_int16 xo, u_int16 yo)
{
  if(sizefactor!=1)
    {
      x*=sizefactor;
      y*=sizefactor;
      bw*=sizefactor;
      bh*=sizefactor;
      xo*=sizefactor;
      yo*=sizefactor;
    }
  sr.x=xo;
  sr.y=yo;
  sr.w=bw;
  sr.h=bh;
  dr.x=x;
  dr.y=y;
  dr.w=bw;
  dr.h=bh;
  SDL_BlitSurface(source->data, &sr, data, &dr);
}

void image::putbox_font_img (void * source, u_int16 xo, u_int16 totlen)
{
  // Looks like we didn't need this after all !!
}

void image::putbox_trans_img (image * source, u_int16 x, u_int16 y, 
			      u_int8 alpha)
{
}

void image::putbox_mask_part_img (image * source, u_int16 x, u_int16 y, 
				  u_int16 bw, u_int16 bh,u_int16 xo, 
				  u_int16 yo)
{
}

void image::putbox_mask_trans_img (image * source, u_int16 x, u_int16 y, 
				   u_int8 alpha)
{
}

void image::putbox_part_trans_img (image * source, u_int16 x, u_int16 y, 
				   u_int16 bw, u_int16 bh, u_int16 xo, 
				   u_int16 yo, u_int8 alpha)
{
}

void image::putbox_mask_part_trans_img(image * source, u_int16 x, u_int16 y, 
				       u_int16 bw, u_int16 bh,u_int16 xo, 
				       u_int16 yo, u_int8 alpha)
{
}

s_int8 image::get(FILE * file)
{
  u_int16 i,j;
  void * tmp;
  SDL_Surface * tmp2;

  tmp=read_pnm(file,&lenght,&height);
 
  switch(screen::get_bytes_per_pixel())
    {
    case 3:
      if(sizefactor==1) 
	{
	  tmp2=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,24,0,0,0,0);
	}else{
	  tmp2=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght*2,height*2,24,0,0,0,0);
	}

      tmp2->format->Rmask=0x0000FF;
      tmp2->format->Bmask=0xFF0000;
      tmp2->format->Gmask=0x00FF00;
      
      for(i=0;i<height;i++)
	{
	  for(j=0;j<lenght;j++)
	    {
	      u_int8 R,G,B;
	      R=*((u_int8*)tmp+(j*3)+(lenght*3*i));
	      G=*((u_int8*)tmp+(j*3)+(lenght*3*i)+1);
	      B=*((u_int8*)tmp+(j*3)+(lenght*3*i)+2);
		      
	      if(sizefactor==1) 
		{		       
		  *((u_int8*)tmp2->pixels+(i*tmp2->pitch)+(j*3))=R;
		  *((u_int8*)tmp2->pixels+(i*tmp2->pitch)+(j*3)+1)=G;
		  *((u_int8*)tmp2->pixels+(i*tmp2->pitch)+(j*3)+2)=B;
		}else{
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6))=R;
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6)+1)=G;
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6)+2)=B;

		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6)+3)=R;
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6)+4)=G;
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6)+5)=B;

		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6))=R;
		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6)+1)=G;
		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6)+2)=B;

		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6)+3)=R;
		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6)+4)=G;
		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6)+5)=B;
		}
	    }
	}
    
      lenght*=sizefactor;
      height*=sizefactor;
      data=SDL_ConvertSurface(tmp2, tmp2->format, SDL_SWSURFACE);
      break;
      
    case 2:
      if(sizefactor==1) 
	{
	  tmp2=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,16,0,0,0,0);
	}else{
	  tmp2=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght*2,height*2,16,0,0,0,0);
	}
      
      u_int8 R,G,B;
      for(i=0;i<height;i++)
	{      
	  for(j=0;j<lenght;j++)
	    {
	      R=*((u_int8*)tmp+(j*3)+(lenght*3*i));
	      G=*((u_int8*)tmp+(j*3)+(lenght*3*i)+1);
	      B=*((u_int8*)tmp+(j*3)+(lenght*3*i)+2);
	      u_int32 colour =  SDL_MapRGB(tmp2->format, R, G, B);
	      if(sizefactor==1) 
		{
		  *((u_int16 *)tmp2->pixels+(i*tmp2->pitch/2)+j) = colour;
		}else{
		  *((u_int16 *)tmp2->pixels+((i*2)*tmp2->pitch/2)+(j*2)) = colour;
		  *((u_int16 *)tmp2->pixels+((i*2)*tmp2->pitch/2)+(j*2)+1) = colour;
		  *((u_int16 *)tmp2->pixels+((i*2+1)*tmp2->pitch/2)+(j*2)) = colour;
		  *((u_int16 *)tmp2->pixels+((i*2+1)*tmp2->pitch/2)+(j*2)+1) = colour;
		}
	    }
	}
      lenght*=sizefactor;
      height*=sizefactor;
      data=SDL_ConvertSurface(tmp2, tmp2->format, SDL_SWSURFACE);
      //      SDL_SetColorKey(data, SDL_SRCCOLORKEY, 0xF81F);
      break;
    }
  
  free(tmp);
  
  if (!data) return(-1);
  return(0);
}

s_int8 image::get(char * file)
{
  u_int16 i,j;
  u_int32 colour;
  u_int8 R,G,B;
  
  SDL_Surface * tmp;
  SDL_Surface * tmp2=NULL;

  tmp=IMG_Load(file);
  lenght=tmp->w;
  height=tmp->h;

  switch(screen::get_bytes_per_pixel())
    {
    case 3:
      if(sizefactor==1) 
	{
	  tmp2=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,24,0,0,0,0);
	}else{
	  tmp2=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght*2,height*2,24,0,0,0,0);
	}
      for(i=0;i<height;i++)
	{
	  for(j=0;j<lenght;j++)
	    {
	      switch(tmp->format->BitsPerPixel)
		{
		case 32:
		  colour = *((u_int32*)tmp->pixels+(i*tmp->pitch/4)+(j));
		  SDL_GetRGB(colour, tmp->format, &R, &G, &B);
		 
		  break;
		case 24:
		  R=*((u_int8 *)tmp->pixels+i*tmp->pitch+j*3);
		  G=*((u_int8 *)tmp->pixels+i*tmp->pitch+j*3+1);
		  B=*((u_int8 *)tmp->pixels+i*tmp->pitch+j*3+2);
		 		 
		  break;
		default:
		  fprintf(stderr,"Whoops: tried to load graphic in %dbpp!\n",tmp->format->BitsPerPixel);
		  colour = 0;
		  break;
		}
	      
	      if(sizefactor==1) 
		{
		  *((u_int8*)tmp2->pixels+(i*tmp2->pitch)+(j*3))=R;
		  *((u_int8*)tmp2->pixels+(i*tmp2->pitch)+(j*3)+1)=G;
		  *((u_int8*)tmp2->pixels+(i*tmp2->pitch)+(j*3)+2)=B;
		}else{
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6))=R;
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6)+1)=G;
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6)+2)=B;
		  
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6)+3)=R;
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6)+4)=G;
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6)+5)=B;
		  
		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6))=R;
		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6)+1)=G;
		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6)+2)=B;
		  
		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6)+3)=R;
		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6)+4)=G;
		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6)+5)=B;
		}
	    }
	}
      lenght*=sizefactor;
      height*=sizefactor;
      tmp2->format->Rmask=0x0000FF;
      tmp2->format->Bmask=0xFF0000;
      tmp2->format->Gmask=0x00FF00;
      data=SDL_ConvertSurface(tmp2, tmp2->format, SDL_SWSURFACE);
      break;
      
    case 2:
      if(sizefactor==1) 
	{
	  data=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,16,0,0,0,0);
	  SDL_BlitSurface(tmp, NULL, data, NULL);
	}else{
	  tmp2=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght*2,height*2,16,0,0,0,0);
	  for(i=0;i<height;i++)
	    {      
	      for(j=0;j<lenght;j++)
		{
		  switch(tmp->format->BitsPerPixel)
		    {
		    case 32:
		      colour = *((u_int32*)tmp->pixels+(i*tmp->pitch/4)+(j));
		      SDL_GetRGB(colour, tmp->format, &R, &G, &B);
		      colour =  SDL_MapRGB(tmp2->format, R, G, B);
		      break;
		    case 24:
		      R=*((u_int8 *)tmp->pixels+i*tmp->pitch+j*3);
		      G=*((u_int8 *)tmp->pixels+i*tmp->pitch+j*3+1);
		      B=*((u_int8 *)tmp->pixels+i*tmp->pitch+j*3+2);

		      colour =  SDL_MapRGB(tmp2->format, R, G, B);
		      break;
		    default:
		      fprintf(stderr,"Whoops: tried to load graphic in %dbpp!\n",tmp->format->BitsPerPixel);
		      colour = 0;
		      break;
		    }

		  *((u_int16 *)tmp2->pixels+((i*2)*tmp2->pitch/2)+(j*2)) = colour;
		  *((u_int16 *)tmp2->pixels+((i*2)*tmp2->pitch/2)+(j*2)+1) = colour;
		  *((u_int16 *)tmp2->pixels+((i*2+1)*tmp2->pitch/2)+(j*2)) = colour;
		  *((u_int16 *)tmp2->pixels+((i*2+1)*tmp2->pitch/2)+(j*2)+1) = colour;
		
		}
	    }	
	  lenght*=sizefactor;
	  height*=sizefactor;
	  data=SDL_ConvertSurface(tmp2, tmp2->format, SDL_SWSURFACE);
	}

      break;
    }
  
  if (!data) return(-1);
  return(0);
}

s_int8 image::load(char * fname)
{
  s_int8 res;
  res=get(fname);
  return(res);
}

u_int8 image::get_sizefactor()
{
  return(sizefactor);
}

s_int8 image::simpleget(FILE * file)
{
  u_int8 bpp = screen::get_bytes_per_pixel() * 8;
  if(data) { SDL_FreeSurface(data); }
  data=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,bpp,0,0,0,0);
  data->pixels=read_pnm(file,&lenght,&height);
  if (data) return(0);
  return(-1);
}

s_int8 image::simpleload(char * fname)
{
  s_int8 res;
  FILE * file=fopen(fname,"r");
  if(!file) return(-1);
  res=simpleget(file);
  fclose(file);
  return(res);
}

void image::adapttodepth()
{
  // nothing needs to be done here, as SDL will 
  // always convert to the display surface format..
}

void screen::set_video_mode(u_int16 w, u_int16 h, u_int8 sf=1)
{
  u_int8 bpp;

  image::set_sizefactor(sf);
  if (SDL_Init (SDL_INIT_VIDEO) < 0) {
    fprintf (stderr, "couldn't init SDL: %s\n", SDL_GetError ());
    exit (1);
  }
  
  atexit (SDL_Quit);
  bpp=SDL_VideoModeOK(w, h, 16, SDL_HWSURFACE);

  switch (bpp)
    {
    case 0:
      fprintf(stderr, "Video mode %dx%d unavailable. Exiting.. \n",w, h);
      exit (1);
      break;
    case 16:
      printf ("Using 16bpp depth: %dx%d.\n",w,h);
      bytes_per_pixel = 2;
      trans=0xF81F;  
      break;
    case 24:
      printf ("Using 24bpp depth: %dx%d.\n",w,h);
      bytes_per_pixel = 3;
      trans=0xFF00FF;  
      break;
    default:
      printf ("Emulating 16bpp depth in %dbpp mode: %dx%d.\n",bpp,w,h);
      bpp=16;
      bytes_per_pixel = 2;
      trans=0xF81F;
      break;
    }

  screenwidth=w;
  screenheight=h;
  sizefactor=sf;
  vis=SDL_SetVideoMode(w, h, bpp, SDL_HWSURFACE);
  if (vis == NULL) 
    {
      fprintf (stderr, "error: %s\n", SDL_GetError ());
      exit (1);
    }
  // Turning off mouse cursor
  SDL_ShowCursor(0);
  
  SDL_WM_SetCaption ("Adonthell", NULL);
  
  init_gfx_buffers();
  frames_to_do=1;
}

void screen::init_display(u_int8 vidmode = 0)
{  
  switch (vidmode)
    {
    case 0:
      screenwidth = 320;
      screenheight = 200;
      sizefactor = 1;
      break;
    case 1:
      screenwidth = 640;
      screenheight = 400;
      sizefactor = 2;
      break;
    default:                   /*printdisclaimer(); */
      exit (1);
    }
  set_video_mode(screenwidth, screenheight, sizefactor);
}

void screen::show()
{
  // Lenght of a game cycle. Decreasing it will increase the speed.
  const u_int32 cycle_length=13;
  static Uint32 timer1, timer2, timer3;

  // Waiting for being sync with a game cycle
  while(1)
    {
      timer2=SDL_GetTicks();
      timer3=timer2-timer1;
      if(timer3>=cycle_length) break;
    }
  timer1=SDL_GetTicks();
  timer1-=(timer3%cycle_length);

  SDL_UpdateRect (vis, 0, 0, 0, 0);
  // How slow is our machine? :)
  frames_to_do=timer3/cycle_length;
  if(frames_to_do>20) frames_to_do=20;
}

inline u_int8 screen::get_bytes_per_pixel()
{
  return(bytes_per_pixel);
}

inline u_int32 screen::get_trans()
{
  return(trans);
}

inline u_int8 screen::get_sizefactor()
{
  return(sizefactor);
}

u_int8 screen::get_frames_to_do()
{
  return(frames_to_do);
}

u_int8 screen::get_screenheight()
{
  return(screenheight);
}

u_int8 screen::get_screenwidth()
{
  return(screenwidth);
}

void screen::init_gfx_buffers()
{
 getbuffer=SDL_ConvertSurface(vis, vis->format, SDL_HWSURFACE);
}


void screen::drawbox(u_int16 x, u_int16 y, u_int16 w, u_int16 h, u_int32 color)
{
  x*=sizefactor;
  y*=sizefactor;
  w*=sizefactor;
  h*=sizefactor;

  static SDL_Rect dr;
  dr.x=x;
  dr.y=y;
  dr.w=w;
  dr.h=h;
  SDL_FillRect(vis, &dr, color);
}


void screen::makesquare(u_int16 px,u_int16 py, u_int16 fact)
{
  drawbox(0,py,320,fact,0);
  drawbox(0,200-fact-py,320,fact,0);
  drawbox(px,0,fact,200,0);
  drawbox(320-fact-px,0,fact,200,0);
}

void screen::mouse_cursor_off()
{
  SDL_ShowCursor(0);
}

void screen::mouse_cursor_on()
{
  SDL_ShowCursor(1);
}

sprite::sprite()
{
  frame=NULL;
  nbr_of_frames=0;
}

sprite::~sprite()
{
  u_int16 i;
  for(i=0;i<nbr_of_frames;i++)
    frame[i].~image();
  delete [] frame;
}

void sprite::get(FILE * file)
{
  fread(&nbr_of_frames,sizeof(nbr_of_frames),1,file);
  frame= new image [nbr_of_frames];
  fread(&counterlimit,sizeof(counterlimit),1,file);
  fread(&alpha,sizeof(alpha),1,file);
  fread(&nbr_of_frames,sizeof(nbr_of_frames),1,file);  
}