//
// Created by camilo on 31/01/2022 14:43 <3ThomasBorregaardSorensen!!
//
#pragma once


#include "acme/nano/graphics/font.h"
#include "object.h"


namespace universal_windows
{

   namespace nano
   {


      namespace graphics
      {


         class CLASS_DECL_NANO_GRAPHICS_DIRECT2D font :
            virtual public ::nano::graphics::font,
            virtual public object
         {
         public:

            //CreatableFromBase(::nano::graphics::font, ::nano::graphics::font);

            comptr < IDWriteTextFormat >    m_pformat;

            font();
            ~font() override;


            void update(::nano::graphics::device* pnanodevice) override;


            //static HFONT _create_point_font(int nPointSize, const ::scoped_string& scopedstrFaceName, bool bBold, HDC hdc, LOGFONTW* plf);
            //static HFONT _create_point_font_indirect(LOGFONTW* pLogFont, HDC hdc);

         };



      } // namespace graphics


   } // namespace nano



} // namespace universal_windows



