/*
   $Id$

   Copyright (C) 2001 by Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __DATA_SCREEN_H__
#define __DATA_SCREEN_H__

#include "win_types.h"
#include "win_font.h"
#include "win_select.h"
#include "win_theme.h"
#include "win_container.h"

class win_write;

enum
{
    LOAD_SCREEN = 0,
    SAVE_SCREEN = 1
};

// The gui for loading/saving games
class data_screen : public win_container
{
public:
    data_screen (int);                      // open load/save window
    ~data_screen ();                        // destructor
    bool update ();                         // react to input

private:
    void init ();                           // gui initialisation
    void on_select ();                      // callback for selecting a game 
    void on_save ();                        // callback for entering a description
    void save_preview (char*);              // write the thumbnail to disk

    win_font *font;                         // the font
    win_theme *theme;                       // the theme
    win_write *entry;                       // for entering a description of the game
    win_select *image_list;                 // list of thumbnails

    vector<win_write*> entry_list;          // list of game descriptions
    int mode;                               // Whether we're saving or loading
    bool quit;                              // Tells the window to close
};

#endif // __DATA_SCREEN_H__
