#include "platform.h"
#include "draw2d.h"
#include "direct2d/direct2d.h"
#include "image.h"
#include "bred/gpu/bred_approach.h"
#include "bred/gpu/device.h"
#include "aura/windowing/window.h"
#include "operating_system-windows_common/acme_windows_common/dxgi_device_source.h"


namespace draw2d_direct2d_for_directx11
{


   draw2d::draw2d()
   {

      defer_create_synchronization();

   }


   draw2d::~draw2d()
   {

      //::direct2d::finalize();

   }


   void draw2d::initialize(::particle * pparticle)
   {

      //auto estatus = 
      
      ::draw2d::draw2d::initialize(pparticle);

      //if (!estatus)
      //{

      //   return estatus;

      //}
   
      /*::direct2d::defer_initialize(this);*/

      //estatus = initialize_gdiplus();

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //return estatus;
   
   }



   string draw2d::write_text_get_default_implementation_name()
   {

      return system()->implementation_name("write_text", "direct2d");

   }


   bool draw2d::lock_device()
   {

      auto pdirect2d = direct2d();

      if (!pdirect2d)
      {

         return false;

      }

      auto pmultithread = pdirect2d->m_pd2d1multithread.m_p;

      if (!pmultithread)
      {

         return false;

      }

      pmultithread->Enter();

      return true;

   }


   void draw2d::unlock_device()
   {
      
      auto pdirect2d = direct2d();

      auto pmultithread = pdirect2d->m_pd2d1multithread.m_p;

      pmultithread->Leave();

   }


   ::dxgi_device_source * draw2d::_dxgi_device_source(::acme::windowing::window * pacmewindowingwindow)
   {

      ::cast < ::gpu::approach > pgpuapproach = m_papplication->get_gpu_approach();

      ::cast < ::dxgi_device_source > pdxgidevicesource = pgpuapproach->get_gpu_device(pacmewindowingwindow);

      return pdxgidevicesource;

   }


   //void draw2d::on_create_window(::windowing::window* pwindow)
   //{

   //   ::gpu::draw2d::on_create_window(pwindow);

   //   ///auto rectangleWindow = pwindow->get_window_rectangle();
   //   
   //   //::direct2d::defer_initialize(pwindow, rectangleWindow);

   //}


   void draw2d::adjust_composited_window_styles(::u32& nExStyle, ::u32& nStyle)
   {

      nExStyle |= WS_EX_LAYERED;

      if (m_papplication->m_gpu.m_bUseSwapChainWindow)
      {

         nExStyle |= WS_EX_NOREDIRECTIONBITMAP;

      }

   }


   ::draw2d::graphics_pointer draw2d::do_allocation_strategy(::acme::user::interaction * pacmeuserinteractionAffinity, ::image::image * pimage,
                                                                const ::i32_size & size)
   {

      // The acquisition phase binds the caller's image. Taking a snapshot
      // here would manufacture a different target before that binding.
      return create_memory_graphics(size, pacmeuserinteractionAffinity);

   }


} // namespace draw2d_direct2d_for_directx11



