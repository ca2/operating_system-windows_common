// Created by camilo 2021-02-10 <3TBS_!!
#include "framework.h"
#include "write_text.h"


namespace write_text_direct2d
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

      ::cast < ::direct2d::object > pdirect2dobject = pfont;

      if (pdirect2dobject)
      {

         pdirect2dobject->initialize_direct2d_object(m_pdirect2d);

      }

      return pfont;

   }



} // namespace write_text_direct2d



