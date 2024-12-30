//
// Created by camilo on 31/01/2022 16:06 <3ThomasBorregaardSorensen!!
//
#include "framework.h"
#include "pen.h"
#include "device.h"


namespace universal_windows
{



   namespace nano
   {


      namespace graphics
      {

         pen::pen()
         {

            m_pthis = this;

         }


         pen::~pen()
         {

         }


         void pen::update(::nano::graphics::device* pnanodevice)
         {

            if (m_bModified)
            {

               destroy();

               ::cast < ::universal_windows::nano::graphics::device > pdevice = pnanodevice;

               D2D1_COLOR_F color;

               copy(color, m_color);

               pdevice->m_prendertarget->CreateSolidColorBrush(color, &m_pbrush);

               if (m_pbrush != nullptr)
               {

                  m_colorMetro = m_color;

                  m_bMetroColor = true;

               }

               m_pstrokestyle = _create_stroke_style(pdevice, 
                  ::draw2d::e_line_cap_flat,
                  ::draw2d::e_line_cap_flat);

               //if (m_pstrokestyle != nullptr)
               //{

               //   m_osdata[1] = (ID2D1StrokeStyle1 *)m_pstrokestyle;

               //}

            }

         }



      } // namespace graphics


   } // namespace nano


} // namespace universal_windows



