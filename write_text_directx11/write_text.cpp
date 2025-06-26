// Created by camilo 2021-02-10 <3TBS_!!
#include "framework.h"
#include "write_text.h"


namespace write_text_directx11
{

   
   write_text::write_text()
   {


   }


   write_text::~write_text()
   {


   }



   ::write_text::font_pointer write_text::font(const ::write_text::font_family_pointer& pfontfamily, const ::write_text::font_size& fontsize, int iFontWeight)
   {

      auto pfont = ::write_text::write_text::font(pfontfamily, fontsize, iFontWeight);

      ::cast < ::directx11::object > pdirectx11object = pfont;

      if (pdirectx11object)
      {

         pdirectx11object->initialize_directx11_object(m_pdirectx11);

      }

      return pfont;

   }



} // namespace write_text_directx11



