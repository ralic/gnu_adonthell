/*
   $Id$

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   input_listener.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the input_listener class.
 * 
 * 
 */


#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include "input_event.h"
#include "callback.h"
#include "types.h"

/**
 * Class that has the ability to get notified of input events by the input_manager
 * and launch callback functions when such events are triggered.
 * 
 */
class input_listener
{    
public:
    /**
     * Constructor.
     * 
     */
    input_listener ();

    /**
     * Destructor.
     * 
     */
    ~input_listener ();    

    /** 
     * Returns whether this listener will react to a certain kind of events.
     * 
     * @param t event type to test
     * 
     * @return \e true if this listener listens to events of type t, \e false otherwise.
     */
    bool is_listening_to (input_event::input_type t) 
    {
        return Listen_to[t]; 
    }

    /** 
     * Connect a callback function to a type of event for this listener.
     * The listener will automatically listen to this kind of events.
     * 
     * @param t type of event to listen to
     * @param f callback function to call when an event of type \e t is raised
     */
    void connect_function(input_event::input_type t, Functor1wRet<input_event *, int> f);

    /** 
     * Stops listening to events of type \e t. 
     * 
     * @param t type of events to stop listening to.
     */
    void disconnect_function(input_event::input_type t);

    /** 
     * Raise the \e ev event with the listener. This will call the
     * appropriate callback function with the appropriate parameters
     * if this listener listens to events of the type of \e ev.
     * 
     * @warning You should never have to call this function by yourself. If
     *          you want to manually raise an input_event, use 
     *          input_manager::raise_event() instead.
     * 
     * @param ev event to raise
     * 
     * @return value returned by the callback function
     */
    int raise_event (input_event * ev);

private:
    bool Listen_to[input_event::NBR_INPUT_TYPES];
    bool Callback_set[input_event::NBR_INPUT_TYPES];
    Functor1wRet<input_event *, int> Callbacks[input_event::NBR_INPUT_TYPES];
};

#endif
