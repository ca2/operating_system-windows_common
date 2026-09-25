//
//  domain.h
//  aura
//
//  Created by camilo on 2026-09-25 00:42 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
//  Copyright © 2026 ca2 Software Development. All rights reserved.
//
#pragma once


#include "aura/graphics/draw2d/domain.h"


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D domain :
      virtual public ::draw2d::domain
   {
   public:


      comptr<ID2D1DeviceContext> m_pd2d1devicecontext;
      ::pointer<::mutex> m_pmutexDeviceContext;


      domain();
      ~domain() override;



      virtual mutex * _d2d1_device_context_mutex();
      virtual ID2D1DeviceContext * _d2d1_device_context();


      void destroy() override;


   };


} // namespace draw2d_direct2d



