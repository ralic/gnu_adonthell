/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

void * read_pnm(FILE * file, u_int16 * lenght, u_int16 * height);
void put_pnm(FILE * file, void * image, u_int16 lenght, u_int16 height);
int write_pnm(char * filename, void * image, u_int16 lenght, u_int16 height);
