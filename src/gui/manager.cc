/*
   $Id$

   (C) Copyright 2002 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/


#include "gfx/screen.h"
#include "manager.h"

using namespace gui;

//  window *  manager::cur_window_ = NULL;
//  bool manager::alive_ = true; 


manager::manager ()
{
    init (); 
}



void manager::init ()   
{
    
    cur_window_ = NULL;
    alive_ = true; 
    
    
    // connect keyboard function callback
    il_.connect_function (input::event::KEYBOARD_EVENT,
                          MemberTranslator1wRet<input::event *,int,manager,int (manager::*)(input::event *)>
                          (*this, &manager::input_update));  
    

    
    //connect mouse function callback 
    il_.connect_function (input::event::MOUSE_EVENT,
                          MemberTranslator1wRet<input::event *,int,manager,int (manager::*)(input::event *)>
                          (*this, &manager::input_update));  
   
    
       
    input::manager::add(&il_);
}


void manager::add (window *w)
{ 
    v_window_.push_back (w); 
}



void manager::remove (window *w)
{
    std::deque <window * >::iterator it = v_window_.begin ();
    
    while (it != v_window_.end () && * it != w)it++;
    
    if (it != v_window_.end ()) 
        v_window_.erase (it);     
}


void manager::clear () 
{
    for (std::deque <window* >::iterator it = v_window_.begin (); it != v_window_.end (); it++)
        delete *it;
    
    v_window_.clear (); 
}



void manager::draw () 
{
    for (std::deque <window* >::iterator it = v_window_.begin (); it != v_window_.end (); it++)
        (*it)->draw ();


    /* draw cursor */
    // WARNING IMPROVE IT !
    if (mouse_x_ <=  0 || mouse_y_ <=  0 || mouse_x_ >=  gfx::screen::length () - 5 || mouse_y_ >=  gfx::screen::height () - 10  ) return; 
    gfx::screen::display.draw_line (mouse_x_, mouse_y_, mouse_x_ + 5, mouse_y_, 0x000000); 
    gfx::screen::display.draw_line (mouse_x_, mouse_y_, mouse_x_, mouse_y_ + 5, 0x000000);
    gfx::screen::display.draw_line (mouse_x_, mouse_y_, mouse_x_ + 10, mouse_y_ + 10, 0x000000); 
    
}


void manager::shutdown ()
{
    alive_ = false; 
}

/*
int manager::keyboard_event (input_event * ev)
{
    cout << "key event\n"; 
    return 0; 
}


int manager::mouse_event (input_event * ev)
{
    cout << "Mouse event\n"; 
    return 1; 
}
*/

int manager::input_update (input::event * ev)
{

    // get mouse position for the cursor
    input::mouse_event * mev = (input::mouse_event*) ev; 
    mouse_x_ = mev->x ();
    mouse_y_ = mev->y (); 
    
    
    // if the window is a modal window,  only modal window receive input event.
    // if it is not a modal window all the event are dispatch for all window in the deque.
    // In this way if a window catch an event, the event is not longer dispatched 
    if (cur_window_ && cur_window_->is_modal ())
    {
        cur_window_->input_update (ev);  
    }
    else
    {
        // std::deque <window* >::iterator it = v_window_.rend ();
        // while (it != v_window_.rbegin () && (*it)->input_update (ev) == 0)it--;
        int i = v_window_.size () - 1;
        while ( i > -1 && v_window_[i]->input_update (ev) == 0) i--;  
        
    } 
    return 1; 
}


void manager::set_focus (window * w)
{
    // WARNING :  call lost focus for the window
    cur_window_ = w; 
    remove (w);
    add (w); 
}


bool manager::update ()
{
    if (cur_window_ && cur_window_->is_modal ())
    {
        if (cur_window_->update () == false) 
        {
            remove (cur_window_); 
            delete cur_window_;
            cur_window_ = NULL;
        }  
    }
    else
    {
        int i = v_window_.size () - 1;
        window * tmp;
        
        while ( i > -1)
        {
            if (v_window_[i]->update () == false)
            {
                remove (tmp = v_window_[i]);
                delete tmp; 
            } 
            i--; 
        } 
    } 
    return alive_; 
}

manager::~manager ()
{
    clear (); 
}
