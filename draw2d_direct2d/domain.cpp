//
//  domain.cpp
//  aura
//
//  Created by camilo on 2026-09-25 00:45 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
//  Copyright © 2026 ca2 Software Development. All rights reserved.
//
#include "platform.h"
#include "domain.h"
#include "draw2d.h"
#include "operating_system/operating_system-windows_common/direct2d/direct2d.h"


namespace draw2d_direct2d
{


   domain::domain()
   {


   }


   domain::~domain()
   {


   }


   mutex * domain::_d2d1_device_context_mutex()
   {

      return m_pmutexDeviceContext;

   }



   ID2D1DeviceContext * domain::_d2d1_device_context()
   {

      if (!m_pd2d1devicecontext)
      {

         ::cast < ::draw2d_direct2d::draw2d > pdraw2ddirect2ddraw2d = draw2d();

         auto pdxgidevicesource = pdraw2ddirect2ddraw2d->_dxgi_device_source(this);

         auto pdirect2d = pdraw2ddirect2ddraw2d->direct2d();

         m_pd2d1devicecontext = pdirect2d->create_d2d1_device_context(pdxgidevicesource);

      }

      return m_pd2d1devicecontext;

   }

   void domain::destroy()
   {


      m_pd2d1devicecontext.release();
      m_pmutexDeviceContext.release();
      //m_pgdioffscreen.defer_destroy_and_release();

      ::draw2d::domain::destroy();

   }



} // namespace draw2d_direct2d



