// Created by camilo 2021-02-10 <3TBS_!!
#pragma once


#include "aura/graphics/write_text/write_text.h"
#include "directx11/object.h"


namespace write_text_directx11
{


   class CLASS_DECL_WRITE_TEXT_DIRECTX11 write_text :
      virtual public ::write_text::write_text,
      virtual public ::directx11::object
   {
   public:


      write_text();
      ~write_text() override;


      virtual ::write_text::font_pointer font(const ::write_text::font_family_pointer& pfontfamily, const ::write_text::font_size& fontsize, int iFontWeight = e_font_weight_normal) override;


   };


} // namespace write_text_directx11



