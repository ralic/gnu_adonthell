/*
   Copyright (C) 2001/2002 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/


/**
 * @file   py_callback.cc
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Defines the py_callback class.
 * 
 * 
 */


#include "py_callback.h"
#include "python_class.h"

// 'hack' to aid restoring of callbacks from file
PyObject *py_callback::instance = NULL;

// default constructor
py_callback::py_callback ()
{
    function = NULL;
    arguments = NULL;
}

// preferred constructor
py_callback::py_callback (PyObject *func, PyObject *args)
{
    function = func;
    arguments = args == Py_None ? NULL : args;
    Py_XINCREF (function);
    Py_XINCREF (arguments);
}

// dtor
py_callback::~py_callback ()
{
    Py_XDECREF (function);
    Py_XDECREF (arguments);
}

// calls the python function without argument
void py_callback::callback_func0 ()
{
    PyObject *py_arg = arguments ? Py_BuildValue ("(O)",arguments) : NULL;
    PyObject* val = make_call (py_arg);
    Py_XDECREF (val);
}

// calls the python function returning a boolean
bool py_callback::callback_func0ret ()
{
    int retval = 1;
    
    PyObject *py_arg = arguments ? Py_BuildValue ("(O)",arguments) : NULL;
    PyObject* val = make_call (py_arg);

    if (val) retval = PyInt_AsLong (val);
    Py_XDECREF (val);

    return retval != 0;
}

// calls the python function with an integer as argument
void py_callback::callback_func1 (int arg)
{
    PyObject *py_arg;

    if (arguments) py_arg = Py_BuildValue ("(i,O)", arg, arguments);
    else py_arg = Py_BuildValue ("(i)", arg);

    PyObject * val = make_call (py_arg);
    Py_XDECREF (val);
}

// save callback to a file
void py_callback::put_state (ogzstream & file) const
{
    std::string name = "";
    
    // get name of callback function
    if (function) {
	PyObject *p_name = PyObject_GetAttrString (function, "__name__");
    	if (PyString_Check (p_name)) name = python::as_string (p_name);
    	else fprintf (stderr, "*** error: py_callback::put_state: Failed to retrieve callback name!");
     
        // cleanup
        Py_XDECREF (p_name);
    }

    name >> file;

    // NOTE: extra arguments need to be a tuple containing only ints or strings.
    if (arguments != NULL)
    {
        true >> file; 
        python::put_tuple (arguments, file);
    }
    else false >> file; 
}

// restore callback from a file
bool py_callback::get_state (igzstream & file)
{
    std::string name;
    bool has_args;
    
    name << file;
    has_args << file;
    
    // load arguments. No need to INCREF as get_tuple returns new instance.
    if (has_args) arguments = python::get_tuple (file); 

    // check that we have a valid instance that contains our callback
    if (instance == NULL)
    {
        fprintf (stderr, "*** error: py_callback::get_state: Invalid instance!\n");
        return false;
    }
     
    // get our callback from the class or module. No need to INCREF
    // as GetAttrString returns a new instance.
    function = PyObject_GetAttrString (instance, (char *) name.c_str ());
    
    // sanity check
    if (!PyCallable_Check (function))
    {
        fprintf (stderr, "*** error: py_callback::get_state: Setting callback '%s' failed!\n", name.c_str ());
        return false;
    }
    
    return true;
}

// the actual python callback call
PyObject *py_callback::make_call (PyObject *args)
{
    if (function == NULL) return NULL;

    PyObject * val = PyObject_CallObject (function, args);
    Py_XDECREF (args);

#ifdef PY_DEBUG
    python::show_traceback ();
#endif

    return val;
}
