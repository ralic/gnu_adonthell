/*
   $Id$

   Copyright (C) 2001/2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   callback.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the callback class.
 * 
 * 
 */

#include <iostream>

#include "callback.h"
#include "base.h"

using namespace python;

callback::callback (PyObject *func, PyObject *args)
{
    function = func;
    arguments = args;
    Py_XINCREF (function);
    Py_XINCREF (arguments);
}

callback::~callback ()
{
    Py_XDECREF (function);
    Py_XDECREF (arguments);
}

// calls the python function without argument
void callback::callback_func0 ()
{
    PyObject *py_arg = arguments ? Py_BuildValue ("(O)",arguments) : NULL;
    PyObject* val = make_call (py_arg);
    Py_XDECREF (val);
}

// calls the python function returning a boolean
bool callback::callback_func0ret ()
{
    int retval = 1;
    
    PyObject *py_arg = arguments ? Py_BuildValue ("(O)",arguments) : NULL;
    PyObject* val = make_call (py_arg);

    if (val) retval = PyInt_AsLong (val);
    Py_XDECREF (val);

    return retval != 0;
}

// calls the python function with an integer as argument
void callback::callback_func1 (int arg)
{
    PyObject *py_arg;

    if (arguments) py_arg = Py_BuildValue ("(i,O)", arg, arguments);
    else py_arg = Py_BuildValue ("(i)", arg);

    PyObject * val = make_call (py_arg);
    Py_XDECREF (val);
}

PyObject *callback::make_call (PyObject *arg)
{
    PyObject * val = PyObject_CallObject (function, arg);
    Py_XDECREF (arg);

#ifdef PY_DEBUG
    python::show_traceback ();
#endif

    return val;
}