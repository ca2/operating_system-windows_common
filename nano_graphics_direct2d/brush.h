//
// Created by camilo on 31/01/2022 14:40 <3ThomasBorregaardSorensen!!
//
#pragma once


#include "acme/nano/graphics/brush.h"
#include "object.h"


namespace universal_windows
{



   namespace nano
   {


      namespace graphics
      {


         class CLASS_DECL_NANO_GRAPHICS_DIRECT2D brush :
            virtual public ::nano::graphics::brush,
            virtual public object
         {
         public:


            //CreatableFromBase(brush, brush);

            comptr<ID2D1Brush>                 m_pbrush;
            comptr<ID2D1SolidColorBrush>       m_psolidbrush;

            brush();
            ~brush() override;


            void update(::nano::graphics::device* pnanodevice) override;


         };



      } // namespace graphics


   } // namespace nano


} // namespace universal_windows



