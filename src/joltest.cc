#include <iostream>

#include "input.h"
#include "gui/gui.h"
#include "gametime.h"

using namespace gui;

int main (int argc, char * argv[]) 
{
  
  gfx::screen::init ();
  gfx::screen::set_video_mode (640, 480, 16); 
  gfx::screen::clear (); 
  
  input::init ();
  
  if (argc != 2) 
    {
      std::cout << "Please need TTF font as argument\n";
      exit (1);
    }
    
  //define the gui manager
  gui::manager manage;

  //define and load a font
  gui::ttf font;
  if ( !font.load (argv[1]))
    {
      std::cout << "Error loading font!\n";
      exit (1);
    }
  font.set_size (32);
  font.set_color (255, 255, 255);
  font.build ();
  font.info ();

//   gui::glyph_info & gl = font[L'f'];

//   gfx::image myimage(500, 100);
//   myimage.fillrect(0, 0, 500, 100, 0x0000FF);

//   myimage.set_mask(true);

//   gl.my_glyph->draw(0, 0, NULL, &myimage);
//   gl.my_glyph->draw(gl.my_advance + gl.my_minx, 0, NULL, &myimage);
  
//   myimage.draw(0, 0);

//   gfx::screen::show();

//   getchar();

  // define the label !
  label_input l;
  l.resize (300, 300);
  l.set_ttf (font);

  //  l.set_text (L"�@��");
  
  while ( !input::is_pushed (SDLK_ESCAPE))
    {
      l.input_update ();
      
      //    c = input::get_next_unicode();
      input::update(); 
      
      gametime::update (); 
      
      gfx::screen::display.fillrect (0, 0, 640, 480, 127, 127, 127);
      
      manage.draw (); 
      
      //      font[c].draw (40,40);

      l.draw (50, 50);
      
      gfx::screen::show ();
      
      gfx::screen::clear ();     
    }
  
  input::shutdown();
  
  return 0; 
}



