/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.#include <sys/types.h>
*/

#include <sys/stat.h>
#include <zlib.h>
#include <glib.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include <unistd.h>

#include "../../character_base.h"
#include "../../fileops.h"

#define main main
 
// read the character source file and append it to the character data file
void process_character (char *input, ogzstream& output)
{
    ifstream in (input);
    gchar str[256], **vals;
    int mode = 0;
//    gchar **params, **pair;
//    event *myevent = NULL;
//    int event_type = -1;
//    int i = 0;
    
    if (!in)
    {
        cout << " failed\n";
        return;
    }

    character_base mynpc;
        
    // read character data from source file
    while (!in.eof ())
    {
        in.getline (str, 255);
        g_strstrip (str);

        // ignore comments or blank lines
        if (str != NULL && str[0] != '#' && str[0] != '\0')
        {
            if (strcmp (str, "basic:") == 0)
            {
                mode = 1;
                continue;
            }
            if (strcmp (str, "attributes:") == 0)
            {
                mode = 2;
                continue;
            }
            if (strcmp (str, "events:") == 0)
            {
                mode = 3;
                continue;
            }
            if (strcmp (str, "actions:") == 0)
            {
                mode = 4;
                continue;
            }

            vals = g_strsplit (str, "=", 1);
            g_strstrip (vals[0]);
            g_strstrip (vals[1]);

            switch (mode)
            {
                // read basic attributes
                case 1:
                {
                    if (strcmp (vals[0], "name") == 0) mynpc.set_name(strdup (vals[1]));
                    else if (strcmp (vals[0], "color") == 0) mynpc.set_color(atoi (vals[1]));
                    else if (strcmp (vals[0], "race") == 0)
                    {
                        if (strcmp (vals[1], "Elf") == 0) mynpc.set_val ("race", ELF);
                        else if (strcmp (vals[1], "Half-Elf") == 0) mynpc.set_val ("race", HALFELF);
                        else if (strcmp (vals[1], "Human") == 0) mynpc.set_val ("race", HUMAN);
                        else if (strcmp (vals[1], "Dwarf") == 0)  mynpc.set_val ("race", DWARF);
                    }
                    else if (strcmp (vals[0], "gender") == 0)
                    {
                        if (strcmp (vals[1], "Male") == 0) mynpc.set_val ("gender", MALE);
                        else if (strcmp (vals[1], "Female") == 0) mynpc.set_val ("gender", FEMALE);
                    }
                    break;
                }

                // read further attributes and flags
                case 2:
                {
                    mynpc.set_val (strdup (vals[0]), atoi (vals[1]));

                    break;
                }

                // read events
                case 3:
                {
                    // create new event and attach it to the npc
		  /*                    if (strcmp (vals[0], "type") == 0)
                    {
                        if (strcmp (vals[1], "Enter") == 0)
                        {
                            myevent = new enter_event;
                            ((enter_event *) myevent)->c = &mynpc;
                            mynpc.events.push_back (myevent);
                            event_type = ENTER_EVENT;
                        }
                        else if (strcmp (vals[1], "Leave") == 0)
                        {
                            myevent = new leave_event;
                            ((leave_event *) myevent)->c = &mynpc;
                            mynpc.events.push_back (myevent);
                            event_type = LEAVE_EVENT;
                        }
                    }

                    // set event script
                    else if (strcmp (vals[0], "script") == 0)
                        if (myevent) myevent->script_file = strdup (vals[1]);

                    // set event parameters
                    else if (strcmp (vals[0], "parameters") == 0);
                    {
                        params = g_strsplit (vals[1], ", ", 0);

                        while (params[i] != NULL)
                        {
                            pair = g_strsplit (params[i++], "=", 0);

                            switch (event_type)
                            {
                                case LEAVE_EVENT:
                                case ENTER_EVENT:
                                {
                                    if (strcmp (pair[0], "x coordinate") == 0) 
                                        ((base_map_event *) myevent)->x = atoi (pair[1]);
                                    else if (strcmp (pair[0], "y coordinate") == 0) 
                                        ((base_map_event *) myevent)->y = atoi (pair[1]);
                                    else if (strcmp (pair[0], "direction") == 0) 
                                        ((base_map_event *) myevent)->dir = atoi (pair[1]);
                                    else if (strcmp (pair[0], "map") == 0) 
                                        ((base_map_event *) myevent)->map = atoi (pair[1]);

                                    break;
                                }
                            }

                            g_strfreev (pair);
                        }

                        g_strfreev (params);
                        i = 0;
                    }
		  */
                    break;
                }

                // read schedule/dialogue script
                case 4:
                {
                    if (strcmp (vals[0], "dialogue") == 0) mynpc.set_dialogue (vals[1]);
		  /*
                    else if (strcmp (vals[0], "schedule") == 0) mynpc.set_schedule (strdup (vals[1]), false);
		  */       
                    break;
                }
            }

            g_strfreev (vals);
        }
    }

    // tell the character.data loader that another entry follows
    char tc = 1; 
    tc >> output; 
//     gzputc (output, 1);

    // append the character data
    mynpc.put_state (output);

    cout << " done\n";
}

int main (int argc, char* argv[])
{
	struct dirent *dirent;
	struct stat statbuf;
	char *path = NULL, *cwd = NULL;
    ogzstream outfile;
	DIR *dir;

    if (argc < 2 || argc > 3)
    {
        cout << "\nusage: " << argv[0] << " <input directory> [<output file>]\n\n";
        return 1;
    }

//     if (argc == 2) outfile = gzopen ("character.data", "w6");
    if (argc == 2) outfile.open ("character.data");
//     else outfile = gzopen (argv[2], "w6");
    else outfile.open(argv[2]);

    if (!outfile.is_open ())
    {
        cout << "\ncannot open \"" << (argc == 3 ? argv[2] : "character.data") 
             << "\" for writing.\n\n";
        return 1;
    }

    // write version
    fileops::put_version (outfile, 2);

    // get absolute pathname
    chdir (argv[1]);
    cwd = getcwd (cwd, 0);

    // process all ".character" files in the given directory:
    if ((dir = opendir (cwd)) != NULL)
    {
        // grab the next file
        while ((dirent = readdir (dir)) != NULL)
        {
            if (path != NULL) delete path;

            path = new char[strlen (cwd) + strlen (dirent->d_name) + 2];
            strcpy (path, cwd);
            strcat (path, "/");
            strcat (path, dirent->d_name);
            
            // look if we get a file
			if (stat (path, &statbuf) != -1 && S_ISREG(statbuf.st_mode))
                // make sure none of strcmp()'s arguments are NULL
                if (strrchr (path, '.') != NULL)
                    // do we have a ",character" file at all? 
                    if (strcmp ( ".character", strrchr (path, '.')) == 0)
                    {
                        cout << "Adding " << strrchr (path, '/')+1 << " ..." << flush;
                        process_character (path, outfile);
                    }
        }
    }
    else
    {
        cout << "\nCannot open directory \"" << cwd << "\"\n\n";
        return 1;
    }

    // tell the character.data loader that the EOF has been reached
    char tc = 0;
    tc >> outfile;
    outfile.close (); 
//     gzputc (outfile, 0);
//     gzclose (outfile);
    
    return 0;
}
