/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "preset.h"
#include "pset_interface.h"

preset_dlg::preset_dlg (MainFrame *w) : wnd(w)
{
    dlg = create_preset_dlg (this);
}


void preset_dlg::run ()
{
}

int preset_dlg::on_ok (char *v)
{
    // everything turned out fine :)
    wnd->pset_vars = v;
    return 1;
}

