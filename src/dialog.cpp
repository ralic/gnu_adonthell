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

#include <string.h>
#include <algorithm>

#include "dialog.h"
#include "py_inc.h"
#include "yarg.h"


// Load and instanciate the dialogue object
bool dialog::init (char *fpath, char *name)
{
	PyObject *module;
	PyObject *classobj;

	// First, test if the module has already been imported

	// Seems not, so import
	module = import_module(fpath);

	if (!module)
		return false;

    module = PyImport_ReloadModule (module);

	// Extract the class from the dialogue module
	classobj = PyObject_GetAttrString(module, name);

	if (!classobj)
		return false;

	Py_DECREF(module);

	// Instantiate! Will we ever need to pass args to class
	// constructor here?
	instance = PyObject_CallObject(classobj, NULL);

	Py_DECREF(classobj);

    // extract the dialogue's strings
    extract_strings ();

    // Init the first answer
    answers.push_back(0);

    text = NULL;

	return true;
}

// extract the dialogue's strings
void dialog::extract_strings ()
{
    PyObject *list = PyObject_GetAttrString (instance, "strings");
    PyObject *s;
    u_int32 i, index = PyList_Size (list);

    strings = new char*[index];

    for (i = 0; i < index; i++)
    {
        s = PyList_GetItem (list, i);
        if (s) strings[i] = PyString_AsString (s);
    }
}

dialog::dialog ()
{
    instance = NULL;
    strings = NULL;
}

dialog::~dialog ()
{
    Py_XDECREF (instance);
    if (strings) delete strings;
}

// Gets the index of either the player or npc array
void dialog::run (u_int32 index)
{
    u_int32 nsz, psz, i, j = 0, k = 0, l = 1;
    yarg randgen;
    s_int32 s;
    
    // Is it sufficient to get those objects only once???
    PyObject *npc, *player, *cont;

    // Execute the next part of the dialogue
    PyObject_CallMethod (instance, "run", "i", answers[index]);
#ifdef _DEBUG_
    show_traceback ();
#endif // _DEBUG_

    // Mark the Player's text (if any) as used unless loops allowed
    if (index != 0)
    {
        s = choices[index-1];
        if (!PySequence_In (PyObject_GetAttrString (instance, "loop"), PyInt_FromLong (s)))
            used.push_back (s);
    }
    
    // Empty helper arrays
    answers.clear ();
    choices.clear ();

    // Now fill in the NPC's and Player's responses:
    // 1. Get the neccesary attributes of the dialogue class
    npc = PyObject_GetAttrString (instance, "npc");
    player = PyObject_GetAttrString (instance, "player");
    cont = PyObject_GetAttrString (instance, "cont");

    // 2. Search the NPC part for used text
    for (i = 0; (int)i < PyList_Size (npc); i++)
    {
        s = PyInt_AsLong (PyList_GetItem (npc, i));

        // Remove NPC text that was already used and isn't allowed to loop
        if (find (used.begin (), used.end (), s) != used.end ())
            PySequence_DelItem (npc, i--);
    }

    // 3. (Re)Init dialog::text
    if (text)
    {
        for (i = 0; i < text_size; i++) delete text[i];
        delete text;
    }
    
    nsz = PyList_Size (npc);
    psz = PyList_Size (player);

    if (nsz != 0)
    {
        text = new char*[nsz+psz];
    
        // 4. Randomly chose between possible NPC replies
        randgen.init (" ", 0, nsz-1);
    }
    else 
    {
        // End of dialogue
        text = NULL;
        return;
    }
    
    randgen.randomize ();
    i = randgen.get (5);

    // The first value of text is the NPC Part
    s = PyInt_AsLong (PyList_GetItem (npc, i));

    // scan the string for { python code }
    text[0] = scan_string (strings[s]);
    answers.push_back (0);

    // Mark the NPC text as used unless it's allowed to loop
    if (!PySequence_In (PyObject_GetAttrString (instance, "loop"), PyInt_FromLong (s)))
        used.push_back (s);

    // 5. Extract the matching player strings
    while (j <= i)
    {
        s = PyInt_AsLong (PyList_GetItem (player, k));
        
        if (s == -1) j++;

        // These are the strings belonging to the chosen NPC text
        if (j == i)
        {
            // Only display unused text
            if (find (used.begin (), used.end (), s) == used.end ())
            {
                // add string to current text list
                text[l++] = scan_string (strings[s]);
                
                // Remember Player's possible replies to avoid loops
                choices.push_back (s);               
                answers.push_back (PyInt_AsLong (PyList_GetItem (cont, k+1)));
            }
        }
        
        k++;
    }

    // Insert the target of the NPC text
    answers[0] = PyInt_AsLong (PyList_GetItem (cont, k-1));
    text_size = l;

    // Free the three lists
    Py_XDECREF (npc);
    Py_XDECREF (player);
    Py_XDECREF (cont);
}

char* dialog::scan_string (const char *s)
{
    u_int32 begin, end, len;
    PyObject *result;
    char *start, *mid, *string = NULL;
    char *tmp, *newstr = strdup (s);

    /*
    // replace $... shortcuts
    while (1)
    {
        // check wether the string contains shortcut code at all
        start = strchr (strings[index], '$');
        if (start == NULL) break;
    
    }
    */
    
    // execute python functions
    while (1)
    {
        // check wether the string contains python code at all
        start = strchr (newstr, '{');
        if (start == NULL) break;

        end = strcspn (start, "}");

        string = new char[end];
        string[end-1] = 0;        

        // extract the code without the brackets
        strncpy (string, start+1, end-1);

        // run the string
        result = PyObject_CallMethod (instance, string, NULL);

        mid = NULL;

        if (result)
            if (PyString_Check (result))
                mid = PyString_AS_STRING (result);    
        
        // Replace existing with new, changed string
        // 1. Calculate string's length
        len = strlen (newstr);
        begin = len - strlen (start);
        tmp = new char[(mid ? strlen(mid) : 0) + len - strlen(string)];

        // 2. Merge prefix, resulting string and postfix into new string
        strncpy (tmp, newstr, begin);
        tmp[begin] = 0;
        if (mid) strcat (tmp, mid);
        strcat (tmp, start+end+1);

        // 3. Exchange strings
        delete newstr;
        newstr = tmp;

        // Cleanup
        Py_XDECREF (result);
        delete string;
    }

    return newstr;
}

/*
// Init the dialogue engine
dialog_engine::dialog_engine ()
{
	// For the test, the only dialogue is created here
	dlg = new dialog;
	dlg->answer = 0;

	// Import module
	dlg->class_object = import_module( "dialog/tomas_hterin.py" );
}

void dialog_engine::update_keyboard ()
{
	static u_int16 lastkeypressed=0;
	
	if(input::is_pushed(lastkeypressed)) return;
	lastkeypressed=0;

	if (input::is_pushed (97)) 
	{dlg->answer = 0; lastkeypressed=97;}
	if (input::is_pushed (98)) 
	{dlg->answer = 1; lastkeypressed=98;}
	if (input::is_pushed (99)) 
	{dlg->answer = 2; lastkeypressed=99;}
	if (input::is_pushed (100)) 
	{dlg->answer = 3; lastkeypressed=100;}
}

void dialog_engine::update (window &win)
{

    if (dlg->answer == 0xFFFF) return;

    run (win);
    dlg->answer = 0xFFFF;
}

void dialog_engine::run (window &win)
{
    u_int32 i;
    s_int32 result;
    char *abc[8] = { "a", "b", "c", "d", "e", "f", "g", "h" };
    string str;

    result = engine->run ();

    switch (result)
    {
        case -1:
        {
            str = "Interpreter returned error";
            break;
        }

        case 0:
        {
            // the end
            str = "";
            break;
        }
        
        case 1:
        {
            // Player part 
            str = dlg->npc_text;
            
            for (i = 0; i < dlg->player_text.length () ; i++)
            {
                str += " -- ";
                str += abc[i];
                str += "> ";
                str += dlg->player_text.get_element (i);
            }
            
            break;
        }

        case 2:
        {
            // NPC only
            str = dlg->npc_text;
        }
    }
    win.set_text (str.c_str ());   
}

void dialog_engine::insert_plugin ()
{
    // We have to do the following:
    // - Add the plugin to the end of the main dialogue
    // - Insert a JMP <length_of_main_dlg - 2> after the main dlg's IMPORT
    //   (That means, the plugged in subjects appear before the main ones,
    //   but else, we'd have to check the main dlg for JMP's to the first
    //   block)
    // - Offset the TEXT's string_id to point after the main strings
    // - Remove the RETURN 1 of the first plugin block (fpb)
    // - Replace the CLEAR of the fpb with JMP <-length_of_main_dlg + 2>
    // - Replace all RETURN 0's of the plugin with JMP's to the first main block
}
*/
