
#include "background_template.h"

using namespace gui;

background_template::background_template(): imgback_(NULL)
{
  type_ = NOTHING;
}


void background_template::load( igzstream & is)
{
  if (imgback_) 
    {
      delete imgback_;
      imgback_ = NULL;
    }
  
  name_ << is;
  type_ << is;
  
  switch (type_)
    {
    case NOTHING:
      break;
    case COLOR:
      r_ << is;
      g_ << is;
      b_ << is;
      break;
    case IMAGE:
      imgback_ = new gfx::image();
      imgback_->get (is);
      break;
    }
}

void background_template::save (ogzstream & os)
{
  name_ >> os;
  type_ >> os;

  if (type_ == COLOR) 
    {
      r_ >> os;
      g_ >> os;
      b_ >> os;
    }
  else imgback_->put (os);
}


std::string background_template::get_name () const
{
  return name_;
}


void background_template::set_name (const std::string & name)
{
  name_ = name;
}

gfx::image * background_template::get_image()
{
  return imgback_;
}

/*
u_int32 background_template::get_color()
{
  return colorback_;
}
*/

u_int8 background_template::get_r()
{
  return r_;
}

u_int8 background_template::get_g()
{
  return g_;
}

u_int8 background_template::get_b()
{
  return b_;
}


void background_template::set_image (gfx::image * tmp)
{
  if ( imgback_) delete imgback_;
  imgback_ = tmp;
  if (tmp ) type_= IMAGE;
  else type_ = COLOR;
}


void background_template::set_color (u_int8 r, u_int8 g, u_int8 b)
{
  type_ = COLOR;
  r_ = r;
  g_ = g;
  b_ = b;
}


u_int8 background_template::get_type()
{
  return type_;
}

