/*
   $Id$

   (C) Copyright 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "ps_dlg.h"
#include "ps_callbacks.h"
#include "ps_interface.h"
#include "callbacks.h"

GtkWidget * create_ps_window (ps_dlg *dlg, const char* name, int race, int gender)
{
    GtkWidget *window1;
    GtkWidget *vbox1;
    GtkWidget *frame1;
    GtkWidget *alignment1;
    GtkWidget *ps_name;
    GtkWidget *frame2;
    GtkWidget *table1;
    GSList *race_group = NULL;
    GtkWidget *ps_elf;
    GtkWidget *ps_half;
    GtkWidget *ps_human;
    GtkWidget *ps_dwarf;
    GtkWidget *frame3;
    GtkWidget *hbox1;
    GSList *gender_group = NULL;
    GtkWidget *ps_female;
    GtkWidget *ps_male;
    GtkWidget *hbuttonbox1;
    GtkWidget *ps_ok;
    GtkWidget *ps_cancel;

    window1 = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_object_set_data (GTK_OBJECT (window1), "window1", window1);
    gtk_widget_set_usize (window1, 400, 220);
    gtk_window_set_title (GTK_WINDOW (window1), "Player Settings");
    gtk_window_set_position (GTK_WINDOW (window1), GTK_WIN_POS_MOUSE);
    gtk_window_set_modal (GTK_WINDOW (window1), TRUE);
    gtk_window_set_policy (GTK_WINDOW (window1), FALSE, FALSE, FALSE);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox1);
    gtk_object_set_data_full (GTK_OBJECT (window1), "vbox1", vbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (window1), vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 8);

    frame1 = gtk_frame_new ("Name");
    gtk_widget_ref (frame1);
    gtk_object_set_data_full (GTK_OBJECT (window1), "frame1", frame1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (frame1);
    gtk_box_pack_start (GTK_BOX (vbox1), frame1, FALSE, TRUE, 0);

    alignment1 = gtk_alignment_new (0, 0, 1, 1);
    gtk_widget_ref (alignment1);
    gtk_object_set_data_full (GTK_OBJECT (window1), "alignment1", alignment1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (alignment1);
    gtk_container_add (GTK_CONTAINER (frame1), alignment1);
    gtk_container_set_border_width (GTK_CONTAINER (alignment1), 10);

    ps_name = gtk_entry_new ();
    gtk_widget_ref (ps_name);
    gtk_object_set_data_full (GTK_OBJECT (window1), "ps_name", ps_name, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (ps_name);
    gtk_container_add (GTK_CONTAINER (alignment1), ps_name);
    
    // set the right text
    gtk_entry_set_text ((GtkEntry *) ps_name, name);
    dlg->name = (GtkEntry *) ps_name;
    
    frame2 = gtk_frame_new ("Race");
    gtk_widget_ref (frame2);
    gtk_object_set_data_full (GTK_OBJECT (window1), "frame2", frame2, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (frame2);
    gtk_box_pack_start (GTK_BOX (vbox1), frame2, FALSE, TRUE, 0);

    table1 = gtk_table_new (2, 2, TRUE);
    gtk_widget_ref (table1);
    gtk_object_set_data_full (GTK_OBJECT (window1), "table1", table1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (table1);
    gtk_container_add (GTK_CONTAINER (frame2), table1);

    ps_human = gtk_radio_button_new_with_label (race_group, "Human");
    race_group = gtk_radio_button_group (GTK_RADIO_BUTTON (ps_human));
    gtk_widget_ref (ps_human);
    gtk_object_set_data_full (GTK_OBJECT (window1), "ps_human", ps_human, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (ps_human);
    gtk_table_attach (GTK_TABLE (table1), ps_human, 1, 2, 1, 2, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), (GtkAttachOptions) (0), 0, 0);

    ps_half = gtk_radio_button_new_with_label (race_group, "Half-Elf");
    race_group = gtk_radio_button_group (GTK_RADIO_BUTTON (ps_half));
    gtk_widget_ref (ps_half);
    gtk_object_set_data_full (GTK_OBJECT (window1), "ps_half", ps_half, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (ps_half);
    gtk_table_attach (GTK_TABLE (table1), ps_half, 0, 1, 1, 2, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), (GtkAttachOptions) (0), 0, 0);

    ps_elf = gtk_radio_button_new_with_label (race_group, "Elf");
    race_group = gtk_radio_button_group (GTK_RADIO_BUTTON (ps_elf));
    gtk_widget_ref (ps_elf);
    gtk_object_set_data_full (GTK_OBJECT (window1), "ps_elf", ps_elf, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (ps_elf);
    gtk_table_attach (GTK_TABLE (table1), ps_elf, 1, 2, 0, 1, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), (GtkAttachOptions) (0), 0, 0);

    ps_dwarf = gtk_radio_button_new_with_label (race_group, "Dwarf");
    race_group = gtk_radio_button_group (GTK_RADIO_BUTTON (ps_dwarf));
    gtk_widget_ref (ps_dwarf);
    gtk_object_set_data_full (GTK_OBJECT (window1), "ps_dwarf", ps_dwarf, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (ps_dwarf);
    gtk_table_attach (GTK_TABLE (table1), ps_dwarf, 0, 1, 0, 1, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), (GtkAttachOptions) (0), 0, 0);
    
    // activate the correct radiobutton
    gtk_toggle_button_set_active ((GtkToggleButton *) g_slist_nth_data (race_group, race), TRUE);
    dlg->race = (GtkRadioButton *) ps_elf;
    
    frame3 = gtk_frame_new ("Gender");
    gtk_widget_ref (frame3);
    gtk_object_set_data_full (GTK_OBJECT (window1), "frame3", frame3, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (frame3);
    gtk_box_pack_start (GTK_BOX (vbox1), frame3, FALSE, FALSE, 0);

    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_widget_ref (hbox1);
    gtk_object_set_data_full (GTK_OBJECT (window1), "hbox1", hbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbox1);
    gtk_container_add (GTK_CONTAINER (frame3), hbox1);

    ps_male = gtk_radio_button_new_with_label (gender_group, "Male");
    gender_group = gtk_radio_button_group (GTK_RADIO_BUTTON (ps_male));
    gtk_widget_ref (ps_male);
    gtk_object_set_data_full (GTK_OBJECT (window1), "ps_male", ps_male, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (ps_male);
    gtk_box_pack_start (GTK_BOX (hbox1), ps_male, TRUE, TRUE, 0);

    ps_female = gtk_radio_button_new_with_label (gender_group, "Female");
    gender_group = gtk_radio_button_group (GTK_RADIO_BUTTON (ps_female));
    gtk_widget_ref (ps_female);
    gtk_object_set_data_full (GTK_OBJECT (window1), "ps_female", ps_female, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (ps_female);
    gtk_box_pack_start (GTK_BOX (hbox1), ps_female, TRUE, TRUE, 0);

    // activate the correct radiobutton
    gtk_toggle_button_set_active ((GtkToggleButton *) g_slist_nth_data (gender_group, gender), TRUE);
    dlg->gender = (GtkRadioButton *) ps_female;

    hbuttonbox1 = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox1);
    gtk_object_set_data_full (GTK_OBJECT (window1), "hbuttonbox1", hbuttonbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox1);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox1, TRUE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_END);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox1), 0);

    ps_ok = gtk_button_new_with_label ("OK");
    gtk_widget_ref (ps_ok);
    gtk_object_set_data_full (GTK_OBJECT (window1), "ps_ok", ps_ok, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (ps_ok);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), ps_ok);
    GTK_WIDGET_SET_FLAGS (ps_ok, GTK_CAN_DEFAULT);

    ps_cancel = gtk_button_new_with_label ("Cancel");
    gtk_widget_ref (ps_cancel);
    gtk_object_set_data_full (GTK_OBJECT (window1), "ps_cancel", ps_cancel, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (ps_cancel);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), ps_cancel);
    GTK_WIDGET_SET_FLAGS (ps_cancel, GTK_CAN_DEFAULT);

    gtk_signal_connect (GTK_OBJECT (ps_ok), "clicked", GTK_SIGNAL_FUNC (on_ps_ok_pressed), dlg);
    gtk_signal_connect (GTK_OBJECT (ps_cancel), "clicked", GTK_SIGNAL_FUNC (on_ps_cancel_pressed), window1);
    gtk_signal_connect (GTK_OBJECT (window1), "delete_event", GTK_SIGNAL_FUNC (on_widget_destroy), NULL);

    gtk_widget_show (window1);
    
    return window1;
}