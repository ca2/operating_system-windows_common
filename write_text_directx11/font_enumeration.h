// Created by camilo 2021-06-05 22:59 BRT <3ThomasBS_!!
#pragma once


#include "aura/graphics/write_text/font_enumeration.h"
#include "directx11/object.h"


namespace write_text_directx11
{


   class CLASS_DECL_WRITE_TEXT_DIRECTX11 font_enumeration :
      virtual public ::write_text::font_enumeration,
      virtual public ::directx11::object
   {
   public:


      font_enumeration();
      ~font_enumeration() override;


      void on_initialize_particle() override;


      void on_enumerate_fonts() override;


   };


} // namespace write_text_directx11



