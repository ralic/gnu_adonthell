/*
   Copyright (C) 1999/2000/2001   Alexandre Courbot
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
 * @file mapcharacter.h
 *
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @brief Declares the mapcharacter class.
 */



#ifndef MAPCHARACTER_H_
#define MAPCHARACTER_H_

/**
 * Where mapcharacter files resides.
 * 
 */ 
#define MAPCHAR_DIR "gfx/mapcharacters/"

#include "animation.h"
#include "character_base.h"
#include "path.h"
#include "text_bubble.h"
#include "event_list.h"

class landmap; 
class path; 

/**
 * @name mapcharacter moves.
 * 
 */ 
//@{
 
/**
 * Standing North.
 * 
 */ 
#define STAND_NORTH 0

/**
 * Standing South.
 * 
 */ 
#define STAND_SOUTH 1

/**
 * Standing West.
 * 
 */ 
#define STAND_WEST 2

/**
 * Standing East.
 * 
 */ 
#define STAND_EAST 3

/**
 * Walking North.
 * 
 */ 
#define WALK_NORTH 4

/**
 * Walking South.
 * 
 */ 
#define WALK_SOUTH 5

/**
 * Walking West.
 * 
 */ 
#define WALK_WEST 6

/**
 * Walking East.
 * 
 */ 
#define WALK_EAST 7

/**
 * Total number of moves.
 * 
 */
#define NBR_MOVES 8

/**
 * No move.
 * 
 */
#define NO_MOVE 65535

//@}

class mapview;



/**
 * Representation of characters on a landmap.
 *
 * Like mapobjects, mapcharacters are a set of animations (one for every movment)
 * and a grid of mapsquare_walkables. This grid represents the map area the mapcharacter
 * physically occupies, which means that a mapcharacter can occupies several tiles.
 *
 * During the execution of Python scripts, some mapcharacter-local variables are
 * available:
 * @li myself is a pointer to the character holding this mapcharacter (can of course
 * serve as a mapcharacter pointer, as character inheritates from mapcharacter).
 * @li mymap, if defined, points to the landmap the mapcharacter is on.
 *
 * These Python variables are available both for schedules and actions.  
 *
 * In supplement, actions have an extra variable available:
 * @li requester, which points to the mapcharacter that requested the action.
 * 
 */ 
class mapcharacter : public mapsquare_walkable_area, public character_base, public event_list
{
public:
    
    /** 
     * Default constructor.
     * 
     */ 
     mapcharacter ();

    /**
     * Destructor.
     * 
     */ 
    ~mapcharacter ();

    /**
     * Puts the mapcharacter back to it's post-constructor state.
     * 
     */ 
    void clear ();
    
    /** 
     * Returns the current file name of the mapcharacter.
     * 
     * 
     * @return filename of the mapcharacter.
     */
    string filename () const
    {
        return filename_;
    }

    /**
     * @name State updating
     * 
     */ 
    //@{ 
         
    /** 
     * Updates the mapcharacter's state and launchs his schedule.
     * 
     */
    bool update ();

    //@}


    /**
     * @name Drawing methods
     * 
     */ 
    //@{ 

    void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL, surface * target = NULL) const;
    void draw_bubble (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL, surface * target = NULL) const;
    
    //@}
    
    
    /**
     * @name Loading/Saving methods
     *
     * @note You can't save mapcharacters with this class.
     */
    //@{
        
    /**
     * Loads a mapcharacter from an opened file.
     * @param file the opened file from which to load.
     * @return 0 in case of success, error code otherwise.
     *
     */ 
    s_int8 get (igzstream& file);

    /**
     * Loads a mapcharacter from it's filename.
     * 
     * @param fname the name of the file to load.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 load (string fname);

    /** Saves an mapcharacter into an opened file, in %game format, with
     *  alpha and mask values. 
     *  @warning as the mapcharacter which is saved comes from a %screen's depth
     *           surface, it will be slightly altered during the save.
     *           If you want a class capable of saving mapcharacters with full
     *           truecolor quality, use mapcharacter_edit instead.
     *  @param file opened file where to save into.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa save ()
     */
    s_int8 put (ogzstream& file) const;

    /** Saves an mapcharacter into an file, in %game format, with
     *  alpha and mask values.
     *  @warning as the mapcharacter which is saved comes from a %screen's depth
     *           surface, it will be slightly altered during the save.
     *           If you want a class capable of saving mapcharacters with full
     *           truecolor quality, use mapcharacter_edit instead.
     *  @param fname file name where to save into.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa put ()
     */
    s_int8 save (string fname) const;
    //@}
    

    /**
     * @name State loading/saving methods
     *
     */
    //@{

    /** 
     * Restore the mapcharacter's state from an opened file.
     * 
     * @param file the opened file from which to load the state.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 get_state (igzstream& file);

    /** 
     * Saves the mapcharacter's state into an opened file.
     * 
     * @param file the opened file where to the state.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 put_state (ogzstream& file) const;

    //@}



    /**
     * @name Landmap assignment
     * 
     */ 
    //@{
    
    /** 
     * Puts the mapcharacter on a landmap.
     * This methods can only be applied if the mapcharacter isn't on any landmap
     * when it is called, otherwise nothing will occur.
     *
     * @warning Be aware that once this methods is called, the mapcharacter has NO
     * position on the landmap. You MUST call jump_to () after this method to actually
     * have placed the character on the map.
     * 
     * @param m pointer to the landmap the mapcharacter should be on.
     */
    void set_map (landmap * m); 
    
    /** 
     * Removes the mapcharacter from the landmap he was on (if any).
     * 
     */
    void remove_from_map ();
    
    /** 
     * Returns a pointer to the landmap the mapcharacter is on.
     * 
     * 
     * @return pointer to the landmap the mapcharacter is on (NULL if none).
     */
    landmap * mymap () const
    {
        return refmap; 
    }
    
    //@}
    
    /**
     * @name High-level control
     *
     * These methods provide a simple way to control the mapcharacter on the map he's on.
     * They cover "normal" moves like walking or looking into a direction, plus tests to
     * know whether a move is possible or not.
     *
     */
    //@{

    /** 
     * Look to North.
     * 
     */
    void stand_north ();

    /** 
     * Look to South.
     * 
     */
    void stand_south ();

    /** 
     * Look to East.
     * 
     */
    void stand_east ();

    /** 
     * Look to West.
     * 
     */
    void stand_west ();

    /** 
     * Stand to the current direction.
     *
     * @note This method only serves to abord an expected waking movment.
     * 
     */
    void stand ();
    
    /** 
     * Walk to North (if possible).
     *
     * This method asks the mapcharacter to walk one square to North. If the
     * movment isn't possible (non-walkable mapsquare or map limit), the
     * character will stand_north () instead.
     *
     * @note Each time update () is called, the mapcharacter will continue advancing,
     * until he reaches the next mapsquare.
     *
     */
    bool go_north ();

    /** 
     * Walk to South (if possible).
     *
     * This method asks the mapcharacter to walk one square to South. If the
     * movment isn't possible (non-walkable mapsquare or map limit), the
     * character will stand_south () instead.
     *
     * @note Each time update () is called, the mapcharacter will continue advancing,
     * until he reaches the next mapsquare.
     *
     */
    bool go_south ();

    /** 
     * Walk to East (if possible).
     *
     * This method asks the mapcharacter to walk one square to East. If the
     * movment isn't possible (non-walkable mapsquare or map limit), the
     * character will stand_east () instead.
     *
     * @note Each time update () is called, the mapcharacter will continue advancing,
     * until he reaches the next mapsquare.
     *
     */
    bool go_east ();
    
    /** 
     * Walk to West (if possible).
     *
     * This method asks the mapcharacter to walk one square to West. If the
     * movment isn't possible (non-walkable mapsquare or map limit), the
     * character will stand_west () instead.
     *
     * @note Each time update () is called, the mapcharacter will continue advancing,
     * until he reaches the next mapsquare.
     *
     */
    bool go_west ();

    /** 
     * Returns whether it is possible or not to go to North from
     * the current mapcharacter's position.
     * 
     * 
     * @return \c true if it is possible to go to North, \c false otherwise.
     */
    bool can_go_north () const;
    
    /** 
     * Returns whether it is possible or not to go to South from
     * the current mapcharacter's position.
     * 
     * 
     * @return \c true if it is possible to go to South, \c false otherwise.
     */
    bool can_go_south () const;

    /** 
     * Returns whether it is possible or not to go to East from
     * the current mapcharacter's position.
     * 
     * 
     * @return \c true if it is possible to go to East, \c false otherwise.
     */
    bool can_go_east ()const;
    
    /** 
     * Returns whether it is possible or not to go to West from
     * the current mapcharacter's position.
     * 
     * 
     * @return \c true if it is possible to go to West, \c false otherwise.
     */
    bool can_go_west () const; 

    /** 
     * Look at the opposite position of p.
     * 
     * This method is usefull for dialogues, when we want two
     * characters to face each other.
     *
     * 
     * @param p opposite position of the position to look at. Can be
     * \c STAND_NORTH, \c STAND_SOUTH, \c STAND_EAST or \c STAND_WEST.
     */
    void look_invert (u_int16 p);

    /** 
     * Return a pointer to the mapcharacter that is right next to this
     * mapcharacter, i.e the mapcharacter that is on the square this
     * mapcharacter is looking at.
     *
     * If no mapcharacter is next to this one, NULL will be returned.
     *
     * 
     * @return pointer to the mapcharacter next to this mapcharacter.
     */
    mapcharacter *whosnext () const;

    void speak (const string & text);

    bool is_speaking ()
    {
        return (saying != NULL); 
    }

    text_bubble * get_bubble () 
    {
        return saying; 
    }
    //@}
      

    /**
     * @name Low-level controls
     *
     * If you need to do non-conventionnal or special things (like
     * teleport a character from a position to another), or need
     * to override the walkable mechanism, use these methods.
     *
     * You are also provided with various informative methods.
     *
     */ 
    //@{

    /** 
     * Sets the offset of the mapcharacter on it's current mapsquare.
     * 
     * @param x X offset.
     * @param y Y offset.
     */
    void set_offset (s_int8 x, s_int8 y)
    {
        offx_ = x;
        offy_ = y;
    }

    /** 
     * Removes the mapcharacter from the place he was on the map.
     * 
     */
    void remove_from_pos ();
    
    /** 
     * Remove the mapcharacter from it's current place and put him to a new one.
     * 
     * @param smap index of the submap to jump to.
     * @param x X offset to to.
     * @param y Y offset to to.
     * @param pos Position to adopt once placed.
     */
    void jump_to (u_int16 smap, u_int16 x, u_int16 y, u_int16 pos = NO_MOVE);

    /** 
     * Returns the index of the submap where the mapcharacter is.
     * 
     * 
     * @return the index of the submap where the mapcharacter is.
     */
    u_int16 submap () const
    {
        return submap_;
    }
    
    /** 
     * Returns the X position of the mapcharacter.
     * 
     * 
     * @return the X position of the mapcharacter on his map.
     */
    u_int16 posx () const
    {
        return posx_;
    }
    
    /** 
     * Returns the Y position of the mapcharacter.
     * 
     * 
     * @return the Y position of the mapcharacter on his map.
     */
    u_int16 posy () const
    {
        return posy_;
    }
    
    /** 
     * Returns the X offset of the mapcharacter.
     * 
     * 
     * @return the X offset of the mapcharacter on his map.
     */
    s_int8 offx () const
    {
        return offx_;
    }
    
    /** 
     * Returns the Y offset of the mapcharacter.
     * 
     * 
     * @return the Y offset of the mapcharacter on his map.
     */
    s_int8 offy () const
    {
        return offy_;
    }

    /** 
     * Returns the current move of the mapcharacter.
     * 
     * 
     * @return current mapcharacter's move (STAND_NORTH, WALK_SOUTH, etc...).
     */
    u_int16 currentmove () const
    {
        return current_move;
    }
 
    bool set_goal (u_int16 x, u_int16 y, u_int16 dir = NO_MOVE);
    void set_callback (PyObject *callback, PyObject *args = NULL);
    bool follow_path (); 
    bool goal_reached ();
    void stop_moving ();
    
    void time_callback (string delay, PyObject *cb, PyObject *args = NULL);
    void time_callback_string (string delay, string cb, PyObject *args = NULL);
    //@}
    

    /**
     * Schedule control.
     * 
     */

    //@{

    /** 
     * Assign a schedule to the mapcharacter.
     * 
     * The schedule's filename will be \e "scripts/schedules/mapcharacters/<file>.py".
     * 
     * @param file name of the schedule to use.
     * @param args Python tuple containing extra arguments passed to the class constructor.
     *
     * @warning the args tuple argument MUST ONLY contain strings or integers, as it will
     * be saved with the mapcharacter state by python::put_tuple ().
     *
     */
    void set_schedule (string file, PyObject * args = NULL);

    /** 
     * Returns the name of the mapcharacter's current schedule.
     * 
     * 
     * @return name of the mapcharacter's current schedule.
     */
    string schedule_file () const
    {
        return schedule_file_;
    }
    
    /** 
     * Returns whether the schedule is activated or not.
     * 
     * 
     * @return \c true if the schedule is activated, \c false otherwise.
     */
    bool is_schedule_activated () const
    {
        return schedule_activated; 
    }
    
    /** 
     * Sets whether the schedule is active or not.
     * 
     * @param a \c true if the schedule should be activated, \c false otherwise.
     */
    void set_schedule_active (bool a)
    {
        if (a && !schedule.has_attribute ("run")) return;
        schedule_activated = a; 
    }

    /**
     * Tell the character to do something. Will execute the given method
     * of the current schedule with the given arguments.
     *
     * @param method The method of the schedule to call.
     * @param args The arguments to pass to the method.
     *
     * @return \c true if the method has been called, \c false otherwise.
     */
    bool do_stuff (string method, PyObject *args = NULL); 
    //@}


    /**
     * Action control.
     * 
     */

    //@{

    /** 
     * Assign a action to the mapcharacter.
     * 
     * The action's filename will be \e "scripts/actions/<file>.py".
     * 
     * @param file name of the action to use.
     * @param args Python tuple containing extra arguments passed to the class constructor.
     *
     * @warning the args tuple argument MUST ONLY contain strings or integers, as it will
     * be saved with the mapcharacter state by python::put_tuple ().
     *
     */
    void set_action (string file, PyObject * args = NULL);

    /** 
     * Returns the name of the mapcharacter's current action.
     * 
     * 
     * @return name of the mapcharacter's current action.
     */
    string action_file () const
    {
        return action_file_;
    }
    
    /** 
     * Returns whether the action is activated or not.
     * 
     * 
     * @return \c true if the action is activated, \c false otherwise.
     */
    bool is_action_activated () const
    {
        return action_activated; 
    }
    
    /** 
     * Sets whether the action is active or not.
     * 
     * @param a \c true if the action should be activated, \c false otherwise.
     */
    void set_action_active (bool a)
    {
        action_activated = a; 
    }

    /** 
     * Run the mapcharacter's action, passing requester as the "requester" parameter
     * for the action's Python script.
     * 
     * @param requester pointer to the mapcharacter that requested the action, which
     * is passed to the Python run () method. 
     */
    void launch_action (mapcharacter * requester);

    //@}


    /** 
     * Returns a pointer to an animation corresponding
     * to a movment.
     * 
     * @param nbr index of the animation to get.
     * 
     * @return pointer to the \e nbr animation.
     */
    animation * get_animation (u_int16 nbr) 
    {
        return anim[nbr]; 
    }
    
#ifndef SWIG
    /**
     * Mapcharacter copy (similar to copy ()).
     *
     * @attention Not available from Python. Use copy () from Python instead.
     * @sa copy ()
     */ 
    mapcharacter & operator = (const mapcharacter & m);
#endif							// SWIG

    /**
     * Synonym of operator = to guarantee its access from Python.
     *
     * @sa operator = 
     */
    void copy (const mapcharacter& src) 
    {
        *this = src; 
    }
      
private:
    /**
     * Forbid value passing.
     * 
     */ 
    mapcharacter (const mapcharacter & src); 
    
    /** 
     * Makes the mapcharacter physically occupy an area.
     *
     * The given parameters are considered to be where
     * the mapcharacter's base square will be.
     * 
     * @param smap submap where to occupy.
     * @param px X position where to occupy.
     * @param py Y position where to occupy.
     */
    void occupy (u_int16 smap, u_int16 px, u_int16 py);

    /** 
     * Makes the mapcharacter physically leave an area previously occupied
     * with occupy ().
     *
     * The given parameters are considered to be where
     * the mapcharacter's base square were be.
     * 
     * @param smap submap where to leave.
     * @param px X position where to leave.
     * @param py Y position where to leave.
     */ 
    void leave (u_int16 smap, u_int16 px, u_int16 py); 

    void leave_position (); 
    
    /** 
     * Sets the position of the mapcharacter on the map.
     *
     * This sets the mapcharacter's position to the parameters,
     * and occupy () the corresponding region.
     *
     * @warning Don't forget to leave () the region when moving!
     * 
     * @param smap index of the submap where the mapcharacter should be.
     * @param x X position on the submap.
     * @param y Y position on the submap.
     */
    void set_pos (u_int16 smap, u_int16 x, u_int16 y);

    /** 
     * Updates the movment of the mapcharacter.
     * 
     */
    void update_move ();
    

    /**
     * Path used for the mapcharacter to have realistic movments.
     * 
     */ 
    path mypath; 

    /**
     * Used to count the position on the path
     * 
     */ 
    u_int16 pathindex; 

    u_int16 current_move;
    u_int16 previous_move;
    u_int16 submap_;
    u_int16 posx_, posy_;
    s_int8 offx_, offy_;
    vector <animation *> anim;
    landmap *refmap;

    py_object schedule; 
    py_object action; 
    
    string filename_; 

    text_bubble * saying; 

    bool schedule_activated;
    bool action_activated;
    bool goal_reached_;

    PyObject * schedule_args;
    PyObject * action_args; 

    string schedule_file_;
    string action_file_; 
    
    py_callback *callback;
    
#ifndef SWIG
    friend class landmap; 
#endif
};

#endif
