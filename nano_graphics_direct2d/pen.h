//
// Created by camilo on 31/01/2022 14:38 <3ThomasBorregaardSorensen!!
//
#pragma once


#include "acme/nano/graphics/pen.h"
#include "object.h"


namespace universal_windows
{


   namespace nano
   {


      namespace graphics
      {


         class CLASS_DECL_NANO_GRAPHICS_DIRECT2D pen :
            virtual public ::nano::graphics::pen,
            virtual public object
         {
         public:


            comptr<ID2D1SolidColorBrush>                 m_pbrush;
            comptr<ID2D1StrokeStyle1>                    m_pstrokestyle;
            bool                                         m_bMetroColor;
            ::color::color                               m_colorMetro;


            pen();
            ~pen() override;


            void update(::nano::graphics::device* pnanodevice) override;


         };



      } // namespace graphics


   } // namespace nano

} // namespace universal_windows



