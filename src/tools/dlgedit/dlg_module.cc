/*
   $Id$

   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/** 
 * @file dlg_module.cc
 *
 * @author Kai Sterker
 * @brief One complete dialogue or sub-dialogue.
 */

#include <gtk/gtk.h>
#include <algorithm>
#include "dlg_module.h"
#include "dlg_circle.h"
#include "dlg_arrow.h"
#include "gui_dlgedit.h"

// ctor
DlgModule::DlgModule (std::string p, std::string n, std::string s, std::string d)
{
    entry_.setDescription (d);
    serial_ = s;
    path_ = p + "/";
    name_ = n;
    
    init ();
}

// initialize a newly constructed DlgModule
void DlgModule::init ()
{
    type_ = MODULE;
    mode_ = IDLE;
    selected_ = NULL;
    highlighted_ = NULL;
    parent_ = NULL;
    changed_ = false;
}

// reset dialogue to initial state
void DlgModule::clear ()
{
    // delete all nodes
    while (!nodes.empty ()) deleteNode (nodes.front ());
    
    // clear custom code and such
    entry_.clear ();
    
    // reset all variables
    init ();
}

// calculate shape of sub-dialogue
void DlgModule::initShape (DlgPoint &center)
{
    // calculate width of the module icon
    GdkFont *font = GuiDlgedit::window->font ();
    int width = gdk_string_width (font, name ().c_str ()) + 10;
    
    // align module to the (imaginary) grid and set shape
    top_left = center.offset (-width/2 - (center.x () % CIRCLE_DIAMETER), -(center.y () % CIRCLE_DIAMETER));
    resize (width, 20); 
}

// select a given node
bool DlgModule::selectNode (DlgNode *node)
{
    // try to find the given node in our array of nodes
    std::vector<DlgNode*>::iterator i = find (nodes.begin(), nodes.end(), node);
    
    // if it is not there, return
    if (i == nodes.end ()) return false;
    
    // see if a node is already selected
    if (selected_ != NULL) return false;
    
    // remember which node is selected
    selected_ = node;
    
    // set the mode of dialogue and node
    mode_ = NODE_SELECTED;
    node->setMode (NODE_SELECTED);

    return true;
}

// highlight the given node
DlgNode* DlgModule::highlightNode (DlgNode *node)
{
    // mark the new node as highlighted ...
    DlgNode *prev = highlighted_;
    highlighted_ = node;

    // ... and return the node highlighted before
    return prev;
}

// select the dialogue's root node.
bool DlgModule::selectRoot ()
{
    for (std::vector<DlgNode*>::iterator i=nodes.begin(); i != nodes.end(); i++)
        if ((*i)->type () != LINK)
            if ((*i)->prev (FIRST) == NULL)
                return selectNode (*i);
    
    return false;
}

// get the node at the given postion
DlgNode* DlgModule::getNode (DlgPoint &position)
{
    // find the node with the given pos in our array of nodes
    std::vector<DlgNode*>::iterator i;
    
    for (i = nodes.begin (); i != nodes.end (); i++)
        if (*(*i) == position) return *i;
    
    return NULL;
}

// add a node to the dialogue
void DlgModule::addNode (DlgNode *node)
{
    nodes.push_back (node);    
}

// delete a node from the dialogue
bool DlgModule::deleteNode ()
{
    // if no node is selected, there's nothing to delete
    if (selected_ == NULL) return false;
    
    // otherwise, first deselect it
    DlgNode *node = deselectNode ();
    
    // actually delete it
    deleteNode (node);
    
    return true;
}

// delete the given node
void DlgModule::deleteNode (DlgNode *node)
{
    // if the node is a circle, also delete the attached arrows
    if (node->type () != LINK)
    {
        // delete all preceding arrows
        for (DlgNode *i = node->prev (FIRST); i != NULL; i = node->prev (FIRST))
        {
            nodes.erase (remove (nodes.begin (), nodes.end (), i), nodes.end ());       
            if (highlighted_ == i) highlighted_ = NULL;
            delete i;
        }
        
        // delete all following arrows
        for (DlgNode *i = node->next (FIRST); i != NULL; i = node->next (FIRST))
        {
            nodes.erase (remove (nodes.begin (), nodes.end (), i), nodes.end ());     
            if (highlighted_ == i) highlighted_ = NULL;
            delete i;
        }
    }

    // remove the node itself from the vector
    nodes.erase (remove (nodes.begin (), nodes.end (), node), nodes.end ());       
    if (highlighted_ == node) highlighted_ = NULL;
    delete node;
}

// deselect the selected node
DlgNode* DlgModule::deselectNode ()
{
    DlgNode *retval = selected_;
    
    // see if a node is selected at all
    if (selected_ == NULL) return NULL;

    // unselect node
    mode_ = IDLE;
    selected_->setMode (IDLE);
    
    // nothing selected
    selected_ = NULL;
    
    // return the node for drawing
    return retval;
}

// draw the module
void DlgModule::draw (GdkPixmap *surface, DlgPoint &offset)
{
    // get the color for drawing the circle
    GdkGC *gc = GuiDlgedit::window->getColor (mode_, type_);

    // offset circle
    DlgPoint position = topLeft ().offset (offset);
    DlgRect area (position, width () + 1, height () + 1);

    // draw everything to the surface
    gdk_draw_rectangle (surface, GuiDlgedit::window->getColor (GC_WHITE), TRUE, position.x (), position.y (), width (), height ());
    gdk_draw_rectangle (surface, gc, FALSE, position.x (), position.y (), width (), height ());

    // get the font to draw name
    GdkFont *font = GuiDlgedit::window->font ();

    // place text in module's center
    int x = position.x () + 5;
    int y = position.y () + (height () + gdk_string_height (font, name ().c_str ())) / 2;
    gdk_draw_string (surface, font, gc, x, y, name ().c_str ());

    // Update the drawing area
    GuiDlgedit::window->graph ()->update (area);
}

// Get extension of the graph for proper displaying
void DlgModule::extension (int &min_x, int &max_x, int &y)
{
    y = 0;
    min_x = 0;
    max_x = 0;
    
    for (std::vector<DlgNode*>::iterator i=nodes.begin(); i != nodes.end(); i++)
        if ((*i)->type () != LINK)
        {
            if ((*i)->y () < y) y = (*i)->y ();

            if ((*i)->x () < min_x) min_x = (*i)->x ();
            else if ((*i)->x () > max_x) max_x = (*i)->x ();
        }
}

// load a dialogue from file
bool DlgModule::load ()
{
    int i = 1, n;
    DlgCircle *circle;
    DlgArrow *arrow;
    std::string s;

    // load all nodes and toplevel items
    while (i)
    {
        switch (i = parse_dlgfile (s, n))
        {
            case LOAD_PROJECT:
            {
                if (parse_dlgfile (s, n) == LOAD_STR) entry_.setProject (s);
                break;                
            }

            case LOAD_NOTE:
            {
                if (parse_dlgfile (s, n) == LOAD_STR) entry_.setDescription (s);
                break;
            }

            case LOAD_FUNC:
            {
                if (parse_dlgfile (s, n) == LOAD_STR) entry_.setMethods (s);
                break;
            }
            
            case LOAD_CTOR:
            {
                if (parse_dlgfile (s, n) == LOAD_STR) entry_.setCtor (s);
                break;
            }
            
            case LOAD_DTOR:
            {
                if (parse_dlgfile (s, n) == LOAD_STR) entry_.setDtor (s);
                break;
            }
            
            case LOAD_IMPORTS:
            {
                if (parse_dlgfile (s, n) == LOAD_STR) entry_.setImports (s);
                break;
            }

            case LOAD_RACE:
            {
                if (parse_dlgfile (s, n) == LOAD_NUM); //->myplayer->set_val ("race", n);
                break;
            }

            case LOAD_GENDER:
            {
                if (parse_dlgfile (s, n) == LOAD_NUM); //->myplayer->set_val ("gender", n);
                break;
            }

            case LOAD_NPC:
            {
                if (parse_dlgfile (s, n) == LOAD_STR);
                break;
            }

            case LOAD_CIRCLE:
            {
                circle = new DlgCircle;
                circle->load ();

                nodes.push_back (circle);

                break;
            }

            case LOAD_ARROW:
            {
                arrow = new DlgArrow;
                arrow->load (nodes);

                nodes.push_back (arrow);
                break;
            }

            default: break;
        }
    }
    
    fclose (loadlgin);
    return true;
}

// save dialogue to file
bool DlgModule::save (std::string &path, std::string &name)
{
    // update path and name
    path_ = path + "/";
    name_ = name;
    
    // open file
    std::ofstream out (fullName ().c_str ());
    int index = 0;
    
    // opening failed for some reasons    
    if (!out) return false;

    // Write Header: Adonthell Dialogue System file version 1
    out << "# Dlgedit File Format 1\n#\n"
        << "# Produced by Adonthell Dlgedit v" << _VERSION_ << "\n"
        << "# (C) 2000/2001/2002 Kai Sterker\n#\n"
        << "# $I" << "d$\n\n"
        << "Note �" << entry_.description () << "�\n\n";

    // Node ID
    out << "Id " << nid_ << "\n";
    
    // Save settings and stuff
    if (entry_.project () != "")
        out << "Proj �" << entry_.project () << "�\n";
    
    if (entry_.imports () != "")
        out << "Inc  �" << entry_.imports () << "�\n";
    
    if (entry_.ctor () != "")
        out << "Ctor �" << entry_.ctor () << "�\n";

    if (entry_.dtor () != "")
        out << "Dtor �" << entry_.dtor () << "�\n";
    
    if (entry_.methods () != "")
        out << "Func �" << entry_.methods () << "�\n";
    
    // Save Circles first, as arrows depend on them when loading later on
    for (std::vector<DlgNode*>::iterator i = nodes.begin (); i != nodes.end (); i++)
        if ((*i)->type () != LINK)
        {
            (*i)->setIndex (index++);
            (*i)->save (out);
        }
                
    // Save Arrows
    for (std::vector<DlgNode*>::iterator i = nodes.begin (); i != nodes.end (); i++)
        if ((*i)->type () == LINK)
            (*i)->save (out);

    // mark dialogue as unchanged
    changed_ = false;
    
    return true;    
}
