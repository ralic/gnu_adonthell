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

#include <algorithm>

#include "py_inc.h"
#include "game.h"
#include "compile.h"
#include "eval.h"
#include "event.h"
#include "character.h"

// "Automatic" unregistering of events no longer needed
event_list::~event_list ()
{
    event *e;

    while (!events.empty ())
    {
        e = events.back ();
        events.pop_back ();
        event_handler::remove_event (e);
        delete e;    
    }    
}

event::~event ()
{
    if (script_file) delete script_file;
    if (script) delete script;
}

// Array with the registered events; each type of event is kept in
// a vector of its own for faster access
vector<event*> event_handler::handlers[MAX_EVENT];

// See whether a matching event is registered and execute the
// according script(s) 
void event_handler::raise_event (event *e)
{
    vector<event*> reg_evs = handlers[e->type];
    vector<event*>::iterator i;

    // Search through all registered events with the type of the raised event
    for (i = reg_evs.begin (); i != reg_evs.end (); i++)
        // Execute the script; pass recieved event on to get event data
        if (e->equals (*i)) (*i)->execute (e); 
}

// Unregister an event
void event_handler::remove_event (event *e)
{
    vector<event*> reg_evs = handlers[e->type];
    vector<event*>::iterator i;

    // Search for the event we want to remove
    i = find (reg_evs.begin (), reg_evs.end (), e);

    // found? -> get rid of it :)
    if (i != reg_evs.end ()) reg_evs.erase(i); 
}

// Register a event with it's script
void event_handler::register_event (event *e, char *file)
{
    FILE *f = fopen (file, "r");
    // See whether the script exists at all
    if (f)
    {
        // Compile the script into a PyCodeObject for quicker execution
        _node *n = PyParser_SimpleParseFile (f, file, Py_file_input);
        if (n)
        {
            e->script = PyNode_Compile (n, file);       
            handlers[e->type].push_back (e);
        }
        else
        {
            cout << "\n*** Cannot register event: Error in" << flush;
            show_traceback ();
        }
        fclose (f);
    }
    else cout << "\n*** Cannot register event: file \"" << file << "\" not found!" << flush;
}

// Load (and register an event)
event* event_handler::load_event (FILE* f, bool reg)
{
    u_int8 type;
    char *script_file;
    event *e = NULL;

    fread (&type, sizeof (type), 1, f);

    switch (type)
    {
        case ENTER_EVENT:
        {
            e = new enter_event;
            break;
        }
        
        default:
        {
            cout << "\n*** Cannot load event. Unknown event tyoe!" << flush;
            return e;
        }
    }
    
    e->load (f);

    // should we register the event?
    if (reg)
    {
        script_file = new char[strlen (e->script_file) + 20];
        strcpy (script_file, "scripts/events/");
        strcat (script_file, e->script_file);
        strcat (script_file, ".py");
        
        register_event (e, script_file);
        delete script_file;
    }
    
    return e;
}

// =======================================================================
// HERE COME ALL THE DIFFERENT EVENTS:

enter_event::enter_event ()
{ 
    type = ENTER_EVENT; 
    x = y = dir = map = -1;
    c = NULL;
    script_file = NULL;
    script = NULL;
}

// compare two enter events
bool enter_event::equals (event *e)
{
    // we know that we've got an enter_event :)
    enter_event *tmp = (enter_event *) e;

    if (tmp->x != -1 && tmp->x != x) return false;
    if (tmp->y != -1 && tmp->y != y) return false;
    if (tmp->dir != -1 && tmp->dir != dir) return false;
    if (tmp->map != -1 && tmp->map != map) return false;
    if (tmp->c && tmp->c != c) return false;

    return true;
}

// Execute enter event's script
void enter_event::execute (event *e)
{
    enter_event *t = (enter_event *) e;

    // Build the event script's local namespace
    PyObject *locals = Py_BuildValue ("{s:i,s:i,s:i,s:i,s:s}", "posx", t->x, 
        "posy", t->y, "dir", t->dir, "map", t->map, "name", t->c->name);
    // Execute script
    PyEval_EvalCode (script, game::globals, locals);
    // Cleanup
    Py_DECREF (locals);
#ifdef _DEBUG_
    show_traceback ();
#endif // _DEBUG_
}

// Load a enter event from file
void enter_event::load (FILE *f)
{
    u_int16 len;
    char* name;
        
    fread (&x, sizeof (x), 1, f);
    fread (&y, sizeof (y), 1, f);
    fread (&dir, sizeof (dir), 1, f);
    fread (&map, sizeof (map), 1, f);

    fread (&len, sizeof (len), 1, f);
    name = new char[len];
    fread (name, len, 1, f);
    c = (character*) game::characters.get (name);

    fread (&len, sizeof (len), 1, f);
    script_file = new char[len];
    fread (script_file, len, 1, f);

    delete name;
}

// Save enter_event to file
void enter_event::save (FILE *out)
{
    u_int16 len = strlen (c->name) + 1;

    fwrite (&type, sizeof (type), 1, out);
    fwrite (&x, sizeof (x), 1, out);
    fwrite (&y, sizeof (y), 1, out);
    fwrite (&dir, sizeof (dir), 1, out);
    fwrite (&map, sizeof (map), 1, out);    
    fwrite (&len, sizeof (len), 1, out);
    fwrite (c->name, len, 1, out);    

    len = strlen (script_file) + 1;
    fwrite (&len, sizeof (len), 1, out);
    fwrite (script_file, len, 1, out);    
}