/*
   $Id$
   
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __MAIN_H__
#define __MAIN_H__

#include <gtk/gtk.h>

class main_wnd
{
public:
    main_wnd ();

    void load_default_attributes (void);
    void colorify_list (GtkCList*);
    gchar* get_option (GtkOptionMenu*);

    GtkWidget *name_entry;
    GtkWidget *race_choice;
    GtkWidget *gender_choice;
    GtkWidget *attrib_entry;
    GtkWidget *val_entry;
    GtkWidget *attribute_list;
    int attribute_list_sel;

    GtkWidget *event_choice;
    GtkWidget *event_list;
    int event_list_sel;

    GtkWidget *dlg_list;

    static gchar *events[5]; // = { "Enter", "Leave", "Pickup", "Drop", "Kill" };
    static gchar *races[4];  // = { "Dwarf", "Elf", "Half-Elf", "Human" };
    static gchar *gender[2]; // = { "Female", "Male" }; 
};

#endif // __MAIN_H__