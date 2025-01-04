//
// Created by camilo on 31/01/2022 16:06 <3ThomasBorregaardSorensen!!
//
#include "framework.h"
#include "brush.h"
#include "device.h"
//#include "_nano.h"


namespace universal_windows
{



   namespace nano
   {


      namespace graphics
      {

         brush::brush()
         {

            m_pthis = this;

         }


         brush::~brush()
         {

         }


         void brush::update(::nano::graphics::device* pnanodevice)
         {

            if (m_bModified)
            {

               destroy();

               D2D1_COLOR_F color;

               copy(color, m_color);

               ::cast < ::universal_windows::nano::graphics::device > pdevice = pnanodevice;

               pdevice->m_prendertarget->CreateSolidColorBrush(color, &m_psolidbrush);

               m_bModified = false;

            }

         }



      } // namespace graphics


   } // namespace nano


} // namespace universal_windows




