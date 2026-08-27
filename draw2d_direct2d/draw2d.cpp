#include "platform.h"
#include "draw2d.h"
#include "direct2d/direct2d.h"
#include "image.h"
#include "window_attachment.h"
#include "acme/platform/node.h"
#include "aura/windowing/window.h"


namespace draw2d_direct2d
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

      ////return estatus;
      //m_pmutexDeviceContext = node()->create_mutex();


      //m_pimage32Raw = nullptr;
      //m_hdcMemory = nullptr;
      //m_hbitmap = nullptr;
      //m_hbitmapOld = nullptr;


   
   }


   ::pointer < ::draw2d::window_attachment > draw2d::allocate_draw2d_window_attachment(::acme::windowing::window * pacmewindowingwindow)
   {

      return create_newø<::draw2d_direct2d::window_attachment >();

   }



   //::mutex * draw2d::_generic_d2d1_device_context_mutex()
   //{

   //   return m_pmutexDeviceContext;

   //}


   //ID2D1DeviceContext * draw2d::_generic_d2d1_device_context()
   //{

   //   _synchronous_lock synchronouslock(this->m_pmutexDeviceContext);

   //   if (!m_pd2d1devicecontext)
   //   {

   //      auto properties = D2D1::RenderTargetProperties(
   //         D2D1_RENDER_TARGET_TYPE_DEFAULT,
   //         D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), 
   //         96.0f, 96.0f);

   //      auto hrCreateDCRenderTarget = direct2d()->d2d1_factory1()->CreateDCRenderTarget(&properties, &m_pd2d1dcrendertarget);

   //      if (FAILED(hrCreateDCRenderTarget))
   //      {

   //         throw hresult_exception(hrCreateDCRenderTarget);

   //      }

   //      ::i32_size sizeRaw{1920, 1080};

   //      BITMAPINFO info{};
   //      info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   //      info.bmiHeader.biWidth = sizeRaw.cx;
   //      info.bmiHeader.biHeight = -sizeRaw.cy; // Top-down.
   //      info.bmiHeader.biPlanes = 1;
   //      info.bmiHeader.biBitCount = 32; // Memory order is BGRA.
   //      info.bmiHeader.biCompression = BI_RGB;

   //      m_pimage32Raw = nullptr;

   //      m_hdcMemory = CreateCompatibleDC(nullptr);

   //      m_hbitmap = CreateDIBSection(m_hdcMemory, &info, DIB_RGB_COLORS, (void **)&m_pimage32Raw, nullptr, 0);

   //      if (!m_hdcMemory || !m_hbitmap || !m_pimage32Raw)
   //      {
   //         // return false;
   //         throw ::interface_only();

   //      }

   //      RECT bounds{0, 0, sizeRaw.cx, sizeRaw.cy};

   //      m_hbitmapOld = (HBITMAP)SelectObject(m_hdcMemory, m_hbitmap);

   //      auto hrBindDC = m_pd2d1dcrendertarget->BindDC(m_hdcMemory, &bounds);

   //      if (FAILED(hrBindDC))
   //      {

   //         throw hresult_exception(hrBindDC);

   //      }

   //      m_pd2d1dcrendertarget.as(m_pd2d1devicecontext);

   //   }

   //   return m_pd2d1devicecontext;

   //}


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


   ::draw2d::graphics_pointer draw2d::do_allocation_strategy(::acme::user::interaction * pacmeuserinteractionAffinity, ::image::image *pimage,
                                                             const ::i32_size &size)
   {

      return allocate_graphics(pacmeuserinteractionAffinity);

      

      //return ::draw2d::draw2d::do_allocation_strategy(pdraw2dhost, pimage, size);

      //auto pdraw2dgraphics = create_memory_graphics(pdraw2dhost, size);

      //if (::is_set(pimage))
      //{

      //   pimage->create_from_graphics(pdraw2dgraphics);

      //}

      //return pdraw2dgraphics;

   }

   
   void draw2d::do_release_to_pool_strategy(::draw2d::graphics_pointer &pdraw2dgraphics, ::image::image *pimage)
   {

      ::draw2d::draw2d::do_release_to_pool_strategy(pdraw2dgraphics, pimage);

      //if (::is_set(pimage))
      //{

      //   auto pgraphicsOwned = pimage->m_pgraphicsOwned;

      //   if (pgraphicsOwned && pgraphicsOwned == pdraw2dgraphics)
      //   {

      //      return;

      //   }

      //}

      //m_graphicsaMemoryPoolIdle.add(pdraw2dgraphics);

      //pdraw2dgraphics.release();

   }

} // namespace draw2d_direct2d



