// Created by camilo 2021-06-05 22:59 BRT <3ThomasBS_!!
#pragma once


namespace write_text_direct2d
{


   class CLASS_DECL_WRITE_TEXT_DIRECT2D font_enumeration :
      virtual public ::write_text::font_enumeration
   {
   public:


      font_enumeration();
      ~font_enumeration() override;


      ::e_status on_enumerate_fonts() override;


   };


} // namespace write_text_direct2d



