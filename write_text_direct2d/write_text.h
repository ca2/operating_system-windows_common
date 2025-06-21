// Created by camilo 2021-02-10 <3TBS_!!
#pragma once


#include "aura/graphics/write_text/write_text.h"
#include "direct2d/object.h"


namespace write_text_direct2d
{


   class CLASS_DECL_WRITE_TEXT_DIRECT2D write_text :
      virtual public ::write_text::write_text,
      virtual public ::direct2d::object
   {
   public:


      write_text();
      ~write_text() override;


      virtual ::write_text::font_pointer font(const ::write_text::font_family_pointer& pfontfamily, const ::write_text::font_size& fontsize, int iFontWeight = e_font_weight_normal) override;


   };


} // namespace write_text_direct2d



