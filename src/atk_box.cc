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

#include "atk_box.h"



atk_box::atk_box () : geometry_ (atk_box::HORIZONTAL), homogene_ (false), spacing_ (0)
{
    
}


void atk_box::set_homogene(const bool b)
{
    homogene_ = b; 
}


bool atk_box::get_homogene () const
{
    return homogene_; 
}


void atk_box::set_spacing(const u_int16 b)
{
    spacing_ = b; 
}


u_int16 atk_box::get_spacing () const
{
    return spacing_; 
}


void atk_box::add (atk_widget * w)
{
    add_end (w); 
    
}


void atk_box::remove (atk_widget * w)
{
    std::deque <atk_box_struct* >::iterator it = v_widget_.begin ();

    while (it != v_widget_.end () && (*it)->widget_ != w) it++;

    if (it != v_widget_.end ()) 
    {
        v_widget_.erase (it);

        on_remove (); 
    }
}


void atk_box::clear ()
{
    for (std::deque <atk_box_struct* >::iterator it = v_widget_.begin (); it != v_widget_.end (); it++)
    {
        delete (*it)->widget_;
        delete *it; 
    }
    v_widget_.clear (); 
}
    
    
void atk_box::add_start (atk_widget * w, bool expand = true, bool fill = true, u_int16 padding = 0)
{
    atk_box_struct * tmp =  new atk_box_struct;

    tmp->widget_ = w;
    tmp->fill_ = fill;
    tmp->expand_ = expand;
    tmp->padding_ = padding;

    v_widget_.push_front (tmp);  
    w->set_parent (this); 
    
    update_homogene (); 
    
    on_add (); 
}


void atk_box::realize ()
{
    if (geometry_ == HORIZONTAL) realize_horizontal ();
    else realize_vertical (); 
}


void atk_box::realize_horizontal ()
{
    if (v_widget_.size () == 0) return; 

    /* init value with border width */
    s_int32 x_tmp = border_width_;
    s_int32 y_tmp = border_width_;

    /* calcul the max length for each widget*/
    s_int32 max_length = (get_length () - ((v_widget_.size () - 1)  * spacing_) - (border_width_ << 1) ) / v_widget_.size (); 
    
    for (u_int16 i = 0;i < v_widget_.size (); i++)  
    {
        if (v_widget_[i]->expand_ == true)
        {
            if (v_widget_[i]->fill_ == true)
            {
                v_widget_[i]->widget_->set_size (max_length , get_height () - (border_width_ << 1)); 
                v_widget_[i]->widget_->set_position (x_tmp, y_tmp);
                x_tmp += max_length; 
            }
            else
            {
                // before fill just add padding
                v_widget_[i]->widget_->set_size (max_length - (v_widget_[i]->padding_ << 1) , get_height () - (border_width_ << 1) - (v_widget_[i]->padding_ << 1));
                v_widget_[i]->widget_->set_position (x_tmp + v_widget_[i]->padding_, y_tmp + v_widget_[i]->padding_);
                x_tmp += max_length; 
            } 
        }
        else
        {
            /* move */
            v_widget_[i]->widget_->set_position (x_tmp, y_tmp); 

            v_widget_[i]->widget_->set_size ((v_widget_[i]->widget_->get_length () > max_length) ? max_length : v_widget_[i]->widget_->get_length (),
                                             get_height () - (border_width_ << 1) );   
            
            x_tmp += v_widget_[i]->widget_->get_length ();
            
            /* calcul new max_length */
            if ((v_widget_.size () - i - 1) != 0) 
                max_length = ((get_length () - ((v_widget_.size () - 1)  * spacing_) - border_width_ ) - x_tmp )
                / ( v_widget_.size () - i - 1)   ;
            
        }  
        /*add space between each widget */
        x_tmp += spacing_; 
    }
}


void atk_box::realize_vertical ()
{

}


void atk_box::add_end (atk_widget * w, bool expand = true, bool fill = true, u_int16 padding = 0)
{
    atk_box_struct * tmp =  new atk_box_struct;

    tmp->widget_ = w;
    tmp->fill_ = fill;
    tmp->expand_ = expand;
    tmp->padding_ = padding;

    v_widget_.push_back (tmp);  
    w->set_parent (this);
    
    update_homogene (); 
    
    on_add (); 
}


atk_box::~atk_box ()
{
    clear (); 
}


void atk_box::update_homogene ()
{
    if (!homogene_) return;

    // WARNING : should be improve with keep in this max_length and max_height
    // find max length and max height
    u_int16 max_length = 0;
    u_int16 max_height = 0;
    
    for (std::deque <atk_box_struct* >::iterator it = v_widget_.begin (); it != v_widget_.end (); it++)
    {
        if (max_length < (*it)->widget_->get_length ()) max_length = (*it)->widget_->get_length ();
        if (max_length < (*it)->widget_->get_height ()) max_height = (*it)->widget_->get_height ();
    }

    // now resize
    for (std::deque <atk_box_struct* >::iterator it = v_widget_.begin (); it != v_widget_.end (); it++)
        (*it)->widget_->set_size (max_length, max_height);    
}




void atk_box::draw (drawing_area * da = NULL, surface * sf = NULL)
{
    if (!is_visible ()) return; 
    
    atk_widget::draw (da, sf);

    for (std::deque <atk_box_struct* >::iterator it = v_widget_.begin (); it != v_widget_.end (); it++)
        (*it)->widget_->draw (da, sf);  
}