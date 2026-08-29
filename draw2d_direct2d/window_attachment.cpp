// Created by camilo on 2026-07-31 17:19 <3ThomasBorregaardSørensen!! Mummi!! Bilbo!!
#include "platform.h"
#include "draw2d.h"
#include "window_attachment.h"
#include "aura/graphics/draw2d/draw2d.h"
#include "aura/graphics/graphics/graphics.h"
#include "aura/windowing/window.h"
#include "operating_system-windows_common/direct2d/direct2d.h"
#ifdef WINDOWS_DESKTOP
#include "operating_system-windows/aura_windows/layered_window_buffer.h"
#include "operating_system-windows_common/acme_windows_common/dxgi_device_source.h"
#endif


namespace draw2d_direct2d
{


   window_attachment::window_attachment() 
   {
   
   }


   window_attachment::~window_attachment() 
   {
   
   
   }

   UINT GetWindowDPI(HWND hwnd) {
      UINT dpi = GetDpiForWindow(hwnd);

      // If the HWND is invalid, it returns 0.
      if (dpi == 0) {
         dpi = 96; // Default to 100% scaling standard DPI
      }

      return dpi;
   }


   void window_attachment::initialize_window_attachment(::windowing::window *pwindow) 
   {

      ::draw2d::window_attachment::initialize_window_attachment(pwindow);
   
      if (m_papplication->m_gpu.m_bUseSwapChainWindow)
      {
         //::direct2d::get() -> d2d1_factory1()->CreateHwndRenderTarget(rendertargetproperties,
           // hwndrendertargetproperties)
      }
//#ifdef WINDOWS_DESKTOP
//      else
//      {
//
//         auto pgraphicsgraphics = pwindow->get_graphics_graphics();
//
//         ::cast < ::windowing_win32::layered_window_buffer > playeredwindowbuffer = pgraphicsgraphics->m_pwindowbuffer;
//
//         construct_newø(m_pgdioffscreen);
//
//         HWND hwnd = ::as_HWND(pwindow->operating_system_window());
//
//         auto dpi = GetWindowDPI(hwnd);
//
//         auto rendertargetproperties = D2D1::RenderTargetProperties(
//            D2D1_RENDER_TARGET_TYPE_DEFAULT,
//            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
//            (FLOAT) dpi, (FLOAT) dpi);
//
//         auto hrCreateDCRenderTarget =
//            ::direct2d::get()->d2d1_factory1()->CreateDCRenderTarget(
//               &rendertargetproperties, 
//               &m_pgdioffscreen->m_pd2d1dcrendertarget);
//
//         if (FAILED(hrCreateDCRenderTarget))
//         {
//
//            throw ::hresult_exception(hrCreateDCRenderTarget, "Failed to create DC render target");
//
//         }
//
//         m_pgdioffscreen->m_pd2d1dcrendertarget.as(m_pd2d1devicecontext);
//
//      }
//
//#endif
   
   }



   mutex *window_attachment::_d2d1_device_context_mutex() 
   {
   
      return m_pmutexDeviceContext;

   
   }



   ID2D1DeviceContext *window_attachment::_d2d1_device_context()
   {

      if (!m_pd2d1devicecontext)
      {

         ::cast < ::draw2d_direct2d::draw2d > pdraw2ddirect2ddraw2d = draw2d();

         auto pdirect2d = pdraw2ddirect2ddraw2d->direct2d();

         defer_constructø(pdirect2d->m_pdxgidevicesource);

         ::cast<::dxgi_device_source> pdxgidevicesource = pdirect2d->m_pdxgidevicesource;

         m_pd2d1devicecontext = pdirect2d->create_d2d1_device_context(pdxgidevicesource);

      }

      return m_pd2d1devicecontext;

   }


} // namespace draw2d_direct2d
