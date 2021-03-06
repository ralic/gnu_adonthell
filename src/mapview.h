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
 * @file   mapview.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the mapview class.
 * 
 * 
 */

 

#ifndef MAPVIEW_H_
#define MAPVIEW_H_

#include "landmap.h"
#include "python_class.h"


/**
 * Allows you to display a landmap on a specified area of a surface.
 *
 * This class just acts as a "camera" which takes snapshots of a landmap.
 * It's size can be specified, a schedule can be set to
 * update it's movment. Nothing prevents you from having several mapviews
 * on the same map that displays each a different (or similar) part of the
 * landmap.
 * 
 */ 
class mapview : public drawable
{

public:

    /**
     * Default constructor.
     * 
     */ 
    mapview ();

    /**
     * Destructor.
     * 
     */ 
    ~mapview ();
 
    /**
     * Attach/Detach a map
     * 
     */

    //@{


    /** 
     * Sets which map this mapview will display.
     * 
     * @param m pointer to the map that will be displayed by the mapview.
     */
    void attach_map (landmap * m);

    /** 
     * Stops displaying a map.
     * 
     */
    void detach_map ();
 
    //@}


    /**
     * Position settings
     * 
     */

    //@{
    
    /** 
     * Sets the position of the top-left corner of the mapview on the map.
     *
     * You'll probably don't want to use this method. To center the mapview
     * on a precise position, see center_on () instead.
     *
     * @param sm submap.
     * @param x X position.
     * @param y Y position.
     * @param ox X offset.
     * @param oy Y offset.
     *
     * @sa center_on () 
     * 
     */
    s_int8 set_pos (u_int16 sm, u_int16 x, u_int16 y, s_int16 ox = 0, s_int16 oy = 0);

    /** 
     * Sets the position of the center of the mapview on the map.
     * 
     * @param sm submap.
     * @param x X position.
     * @param y Y position.
     * @param ox X offset.
     * @param oy Y offset.
     * 
     */ 
    s_int8 center_on (u_int16 sm, u_int16 x, u_int16 y, s_int16 ox = 0, s_int16 oy = 0);

    //@}


    /**
     * Position information
     * 
     */

    //@{
    
    /** 
     * Returns the submap this mapview is displaying.
     * 
     * 
     * @return current submap this mapview is displaying.
     */
    u_int16 currentsubmap ()
    {
        return currentsubmap_;
    }
    
    /** 
     * Returns the X position of the mapview.
     * 
     * 
     * @return X position of the mapview.
     */
    u_int16 posx () const
    {
        return posx_;
    }
    
    /** 
     * Returns the Y position of the mapview.
     * 
     * 
     * @return Y position of the mapview.
     */
    u_int16 posy () const
    {
        return posy_;
    }

    /** 
     * Returns the X offset of the mapview.
     * 
     * 
     * @return X offset of the mapview.
     */
    u_int16 offx () const
    {
        return offx_;
    }
    
    /** 
     * Returns the Y offset of the mapview.
     * 
     * 
     * @return Y offset of the mapview.
     */
    u_int16 offy () const
    {
        return offy_;
    }

    //@}
    
    /**
     * Basic movment
     * 
     */

    //@{
    
    /** 
     * Returns whether it is possible to scroll to right. A scrolling is impossible
     * if the mapview is at the map limits.
     * 
     * 
     * @return true if a right scrolling is possible, false otherwise.
     */
    bool can_scroll_right ()
    {
        s_int32 tpx = posx () * MAPSQUARE_SIZE + offx () + length ();
        return (tpx < m_map->submap[currentsubmap_]->area_length () * MAPSQUARE_SIZE);
    }
    
    /** 
     * Returns whether it is possible to scroll to left. A scrolling is impossible
     * if the mapview is at the map limits.
     * 
     * 
     * @return true if a left scrolling is possible, false otherwise.
     */
    bool can_scroll_left ()
    {
        return (posx_ || offx_);
    }
    
    /** 
     * Returns whether it is possible to scroll to up. A scrolling is impossible
     * if the mapview is at the map limits.
     * 
     * 
     * @return true if a up scrolling is possible, false otherwise.
     */
    bool can_scroll_up ()
    {
        return (posy_ || offy_);
    }

    /** 
     * Returns whether it is possible to scroll to down. A scrolling is impossible
     * if the mapview is at the map limits.
     * 
     * 
     * @return true if a down scrolling is possible, false otherwise.
     */
    bool can_scroll_down ()
    {
        s_int32 tpy = posy () * MAPSQUARE_SIZE + offy () + height ();
        return (tpy < m_map->submap[currentsubmap_]->area_height () * MAPSQUARE_SIZE);
    }

    /** 
     * Scrolls 1 pixel to right, if possible.
     * 
     */
    void scroll_right ();

    /** 
     * Scrolls 1 pixel to left, if possible.
     * 
     */
    void scroll_left ();

    /** 
     * Scrolls 1 pixel to down, if possible.
     * 
     */
    void scroll_down ();

    /** 
     * Scrolls 1 pixel to up, if possible.
     * 
     */
    void scroll_up ();

    //@}
    

    /**
     * State saving/loading
     * 
     */ 
    
    //@{
    
    /** 
     * Restore the mapview's state from an opened file.
     * 
     * @param file the opened file from which to load the state.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 get_state (igzstream& file);

    /** 
     * Saves the mapview's state into an opened file.
     * 
     * @param file the opened file where to the state.
     * 
     * @return 0 in case of success, error code otherwise.
     */ 
    s_int8 put_state (ogzstream& file);

    //@}
    
    /** 
     * Resize the mapview. The parameters are given in pixels.
     * 
     * @param l new length.
     * @param h new height.
     */
    void resize (u_int16 l, u_int16 h); 
    
    /** 
     * Assign a schedule to the mapview.
     * 
     * The schedule's filename will be \e "scripts/schedules/mapviews/<file>.py".
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
     * Returns the name of the mapview's current schedule.
     * 
     * 
     * @return name of the mapview's current schedule.
     */
    string schedule_file () 
    {
        return schedule_file_; 
    }

    /** 
     * Updates the mapview's state and launchs his schedule.
     * 
     */
    bool update ();

    void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
               surface *target = NULL) const;
  
private:
    /**
     * Forbid value passing.
     */
    mapview (const mapview& src);  
    
#ifndef SWIG
    void draw_tile (s_int16 x, s_int16 y, const drawing_area * da_opt, surface * target,
                    list<mapsquare_tile>::iterator it) const;
    
    void draw_mapchar (s_int16 x, s_int16 y, const drawing_area * da_opt,
                       surface * target, list<mapsquare_char>::iterator itc) const;  
    void draw_bubble (s_int16 x, s_int16 y, const drawing_area * da_opt,
                      surface * target, list<mapcharacter *>::iterator itc) const; 
#endif
    
    landmap *m_map;

    u_int16 d_length, d_height;	// size of the view in map squares

    u_int16 currentsubmap_;
    u_int16 posx_, posy_;
    u_int16 offx_, offy_;
    

    mutable drawing_area da; 
     
    py_object schedule;

    PyObject * schedule_args; 

    string schedule_file_; 
};

#endif
