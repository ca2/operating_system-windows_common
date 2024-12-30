//
// Created by camilo on 31/01/2022 16:00 <3ThomasBorregaardSorensen!!
//
#pragma once


#include "acme/nano/graphics/object.h"
#include "direct2d/object.h"
#include "acme/_win32_gdi.h"


namespace universal_windows
{




   namespace nano
   {


      namespace graphics
      {


         class CLASS_DECL_NANO_GRAPHICS_DIRECT2D object :
            virtual public ::nano::graphics::object,
            virtual public ::direct2d::object
         {
         public:



            object();
            ~object();


            void* operating_system_data() override;


            void destroy() override;


         };



      } // namespace graphics


   } // namespace nano


} // namespace universal_windows



