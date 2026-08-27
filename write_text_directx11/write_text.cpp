// Created by camilo 2021-02-10 <3TBS_!!
#include "platform.h"
#include "write_text.h"


namespace write_text_directx11
{

   
   write_text::write_text()
   {


   }


   write_text::~write_text()
   {


   }



   ::write_text::font_pointer write_text::font(const ::write_text::font_family_pointer& pfontfamily, const ::write_text::font_size& fontsize, ::i32 iFontWeight)
   {

      auto pwritetextfont = ::write_text::write_text::font(pfontfamily, fontsize, iFontWeight);

      ::cast < ::directx11::object > pdirectx11object = pwritetextfont;

      if (pdirectx11object)
      {

         pdirectx11object->initialize_directx11_object(m_pdirectx11);

      }

      return pwritetextfont;

   }



} // namespace write_text_directx11



