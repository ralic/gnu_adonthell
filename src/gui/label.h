/*
   $Id$

   (C) Copyright 2002 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/


/**
 * @file   label.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the misc base class.
 * 
 * 
 */

#ifndef LABEL_H_
#define LABEL_H_

#include "font.h"
#include "misc.h"

#include <string>  
#include <vector> 

namespace gui
{

  class label : public misc
    {
      public : 

	/**Constructor initialise some parameters
	 */
	label (); 


      /**Set text
       * @param text
       */
      void set_text (const std::string & text); 

    
      /** get text
       * @return text of this label
       */
      std::string & get_text ();  


      /** set font
       * @param the font to used
       */
      void set_font (font & font);


      /**get font used by this label
       * @return the font used
       */
      font * get_font () const;

    
      /* clear the vector*/
      ~label (); 


      /**Realize this object,  not draw but adapt this object with his contains
       */
      void realize (); 


      /**
       * draw the label
       */
      virtual bool draw (gfx::drawing_area * da = NULL, gfx::surface * sf = NULL); 

      protected : 

    
    
	private : 
    
	/** transform text in line, if the text contain some line
	 */
	void transform_in_line (); 
      
      /* a line vector */
      std::vector <std::string> line_;   
    
      /* text of the label */
      std::string text_; 
    
      /* font used */
      font * font_;  
    }; 

};
#endif

