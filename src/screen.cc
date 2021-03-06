/*
   Copyright (C) 1999/2000/2001/2004  Alexandre Courbot
   Copyright (C) 2016  Kai Sterker
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file   screen.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @author Kai Sterker
 * 
 * @brief  Defines the screen class.
 * 
 * 
 */

#include <config.h>
#include "screen.h"
#include "game.h"
#include <iostream>
#include <sstream> 

using namespace std; 

#if !defined(HAVE_DECL_SDL_WINDOW_ALLOW_HIGHDPI) || HAVE_DECL_SDL_WINDOW_ALLOW_HIGHDPI == 0
#define SDL_WINDOW_ALLOW_HIGHDPI 0
#endif

#ifndef HAVE_SDL_GETDISPLAYUSABLEBOUNDS
#define SDL_GetDisplayUsableBounds SDL_GetDisplayBounds
#endif

surface screen::display; 
u_int8 screen::bytes_per_pixel_ = 0;
u_int32 screen::trans = 0;
SDL_Window *screen::Window = NULL;
SDL_Renderer *screen::Renderer = NULL;

u_int8 screen::mode_ = 0;
u_int8 screen::scale_;
SDL_Rect screen::clip_rect_ = {};

void screen::cleanup()
{
    if (Renderer) SDL_DestroyRenderer(Renderer);
    if (Window) SDL_DestroyWindow(Window);

    Renderer = NULL;
    Window = NULL;
}

bool screen::init (u_int16 nl, u_int16 nh, u_int8 depth, const config & myconfig)
{
	u_int8 screen = myconfig.display;
	u_int8 screen_mode = myconfig.screen_mode;

#if defined(SDL_VIDEO_DRIVER_X11) || defined(SDL_VIDEO_DRIVER_WAYLAND)
	static std::string wm_class = "SDL_VIDEO_X11_WMCLASS=" + myconfig.game_name;
	putenv ((char *) wm_class.c_str ());
#endif

    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
    	std::cout << "Couldn't init SDL: " << SDL_GetError () << std::endl;
    	return false;
    }

    SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

	int availableDisplays = SDL_GetNumVideoDisplays();
	if (availableDisplays < 1)
	{
    	std::cout << "Couldn't init screen: " << SDL_GetError () << std::endl;
    	return false;
	}

	if (screen >= availableDisplays)
	{
		// if the requested display does not exist, pick the first one
		screen = 0;
	}

	if (screen_mode > 2)
	{
		// if the requested screen mode is invalid, fallback to window mode
		screen_mode = 0;
	}

	mode_ = screen_mode;
	scale_ = get_scale_for_display(screen, nl, nh);

	// set window flags
	unsigned int flags = SDL_WINDOW_ALLOW_HIGHDPI;
	switch (screen_mode)
	{
		case 1:
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			break;
		}
		case 2:
		{
			flags |= SDL_WINDOW_FULLSCREEN;
			break;
		}
	}

    SDL_DisplayMode fullscreen_mode;
    memset(&fullscreen_mode, 0, sizeof(SDL_DisplayMode));
    fullscreen_mode.format = SDL_PIXELFORMAT_RGB888;

    nl *= scale_;
    nh *= scale_;

	SDL_ShowCursor(SDL_DISABLE);

    Window = SDL_CreateWindow ("Adonthell", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, nl, nh, flags);
    if (!Window)
    {
        std::cout << "Failed creating window: " << SDL_GetError() << std::endl;
    	return false;
    }
#ifdef WIN32
    const string icon_name = game::find_file("gfx/icon32.bmp");
    if (!icon_name.empty())
    {
    	SDL_Surface* icon = SDL_LoadBMP(icon_name.c_str());
		if (icon != NULL)
		{
			SDL_SetWindowIcon(Window, icon);
			SDL_FreeSurface(icon);
		}
    }
#endif
    if (SDL_SetWindowDisplayMode(Window, &fullscreen_mode) < 0)
    {
        std::cout << "Failed setting display mode: " << SDL_GetError() << std::endl;
    	return false;
    }
    SDL_ShowWindow(Window);

    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!Renderer)
    {
    	std::cout << "Failed creating accelerated renderer: " << SDL_GetError() << std::endl;
        Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_SOFTWARE);
        if (!Renderer)
        {
        	std::cout << "Failed creating renderer: " << SDL_GetError() << std::endl;
        	return false;
        }
    }

    display.set_length(nl/scale_);
    display.set_height(nh/scale_);

    // check if we have a HIGH_DPI window, in which case we need to update our scale
	update_scale();

    // Setting up transparency color
    trans = display.map_color(255, 0, 255, SDL_ALPHA_OPAQUE);

	return true;
}

string screen::info ()
{
    SDL_version version_info;
    SDL_RendererInfo render_info;
    std::ostringstream temp;

    SDL_GetVersion(&version_info);

    if (Renderer)
    {
    	SDL_GetRendererInfo(Renderer, &render_info);
    }
    else
    {
    	render_info.name = "not yet initialized";
    	render_info.flags = 0;
    }

    temp << "Video information: " << std::endl
         << "Platform:          " << SDL_GetPlatform() << std::endl
         << "Version:           " << "SDL " <<(int) version_info.major << "." << (int) version_info.minor << "." << (int) version_info.patch << " " << SDL_GetRevision() << std::endl
         << "Video driver used: " << SDL_GetCurrentVideoDriver() << std::endl
         << "Renderer used:     " << render_info.name << std::endl
         << "HW Accelerated:    " << ((render_info.flags & SDL_RENDERER_ACCELERATED) == SDL_RENDERER_ACCELERATED ? "Yes" : "No") << std::endl
         << "Display Format:    " << SDL_GetPixelFormatName (format()) << std::endl
		 << "Screen Size        " << (int)length()*scale() << "x" << (int)height()*scale() << std::endl
         << "Fullscreen:        " << (mode() ? "Yes" : "No") << std::endl
         << std::ends;

    return temp.str ();
}

bool screen::set_fullscreen (const u_int8 & m)
{
	bool r = false;
    if (mode_ != m)
    {
#ifdef DEBUG
    	std::cout << "Switching from " << (int)mode_ << " to " << (int)m << std::endl;
#endif
    	if (mode_ != 0)
    	{
			r = SDL_SetWindowFullscreen(Window, SDL_FALSE) == 0;
			if (!r)
			{
				return false;
			}
    	}

    	// update mode before getting new scale
    	mode_ = m;

    	u_int8 screen = SDL_GetWindowDisplayIndex(Window);
    	u_int8 new_scale = get_scale_for_display(screen, length(), height());
    	if (new_scale != scale())
    	{
#ifdef DEBUG
        	std::cout << "Scale changed from " << (int)scale_ << " to " << (int)new_scale << std::endl;
#endif
    		SDL_SetWindowSize(Window, length()*new_scale, height()*new_scale);
    	}

    	switch(mode_)
    	{
			case 0:
			{
				SDL_SetWindowPosition(Window, SDL_WINDOWPOS_CENTERED_DISPLAY(screen), SDL_WINDOWPOS_CENTERED_DISPLAY(screen));
				break;
			}
			case 1:
			{
				r = SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN_DESKTOP) == 0;
				break;
			}
			case 2:
			{
				SDL_Rect bounds;
				SDL_GetDisplayBounds(screen, &bounds);
				SDL_SetWindowPosition(Window, bounds.x, bounds.y);
				r = SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN) == 0;
				break;
			}
    	}

        if (r)
		{
        	update_scale();
		}

        return r; 
    }
    return 0; 
}

u_int8 screen::get_scale_for_display(u_int8 screen, u_int16 nl, u_int16 nh)
{
	SDL_Rect bounds;
	switch (mode_)
	{
		case 0:
		{
			// window mode
			SDL_Delay(250);
			SDL_GetDisplayUsableBounds(screen, &bounds);
			break;
		}
		case 1:
		{
			// letterbox mode with aspect ratio preserved
			SDL_GetDisplayBounds(screen, &bounds);
			break;
		}
		case 2:
		{
			// fullscreen mode at 640x480
			bounds.x = 0;
			bounds.y = 0;
			bounds.w = nl * 2;
			bounds.h = nh * 2;
			break;
		}
	}

	int scale_x = (bounds.w - bounds.x) / nl;
	int scale_y = (bounds.h - bounds.y) / nh;

	return scale_x > scale_y ? scale_y : scale_x;
}

void screen::update_scale()
{
	int w, h;

	SDL_GetRendererOutputSize(Renderer, &w, &h);

	int scale_x = w / length();
	int scale_y = h / height();

	scale_ = scale_x > scale_y ? scale_y : scale_x;

	if (mode_ == 1)
	{
		// center viewport in letterbox mode
		clip_rect_.x = (w - length() * scale_) / 2;
		clip_rect_.y = (h - height() * scale_) / 2;
		clip_rect_.w = length() * scale_;
		clip_rect_.h = height() * scale_;

		SDL_RenderSetClipRect(Renderer, &clip_rect_);
	}
	else
	{
		// no rendering offset required when running in window or fullscreen modes
		clip_rect_.x = 0;
		clip_rect_.y = 0;
		clip_rect_.w = length() * scale_;
		clip_rect_.h = height() * scale_;

		SDL_RenderSetClipRect(Renderer, NULL);
	}

#ifdef DEBUG
	std::cout << "Mode = " << (int) mode_ << ", X = " << offset_x_ << ", Y = " << offset_y_
			<< ", Width = " << w << ", Height = " << h << ", Scale = "
			<< (int) (scale_) << std::endl;
#endif
}

void screen::transition (u_int16 i) 
{
    display.fillrect (0, 0, i, screen::height (), 0); 
    display.fillrect (screen::length () - i, 0, i, screen::height (), 0); 
    display.fillrect (0, 0, screen::length (), i, 0); 
    display.fillrect (0, screen::height () - i, screen::length (), i, 0); 
}
