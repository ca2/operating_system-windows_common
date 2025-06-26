// Created by camilo on 2025-06-10 14:28 <3ThomasBorregaardSørensen!!
#include "framework.h"
//#include "graphics.h"
//#include "swap_chain.h"
//#include "directx11/directx11.h"
//#include "aura/windowing/window.h"
//#include "bred/gpu/context.h"
//#include "bred/gpu/device.h"
//#include "bred/gpu/renderer.h"
////#include "bred/gpu/swap_chain_render_target_view.h"
/////#include "bred/gpu/graphics.h"
////#include "draw2d_directx11/graphics.h"
//
//
//namespace draw2d_directx11
//{
//
//   swap_chain::swap_chain()
//   {
//
//   }
//
//
//   swap_chain::~swap_chain()
//   {
//
//
//   }
//
//
//   IDXGIDevice * swap_chain::_get_dxgi_device()
//   {
//
//      return directx11::swap_chain::_get_dxgi_device();
//
//   }
//
//
//   //void swap_chain::initialize_gpu_swap_chain(::gpu::device* pgpudevice, ::windowing::window* pwindow)
//   //{
//
//   //   ::directx11::swap_chain::initialize_gpu_swap_chain(pgpudevice, pwindow);
//
//   //   D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
//   //      D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
//   //      D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
//   //   );
//
//   //   auto pdxgisurface = _get_dxgi_surface();
//
//   //   m_pd2d1devicecontext = ::directx11::get()->create_d2d1_device_context(nullptr, {});
//
//   //   m_pd2d1devicecontext->CreateBitmapFromDxgiSurface(pdxgisurface, bitmapProps, &m_pd2d1bitmap);
//   //   
//   //   m_pd2d1devicecontext->SetTarget(m_pd2d1bitmap);
//
//   //}
//
//
//   //void swap_chain::initialize_directx11_draw2d_gpu_swap_chain(::gpu::device* pdevice, ::windowing::window* pwindow)
//   //{
//
//
//   //}
//
//
//   void swap_chain::endDraw(::draw2d_gpu::graphics* pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc)
//   {
//
//      ::cast < ::draw2d_directx11::graphics > pgraphicsDirect2d = pgraphics;
//
//      comptr < ID2D1Bitmap > pd2d1bitmap;
//
//      pgraphicsDirect2d->m_pbitmaprendertarget->GetBitmap(&pd2d1bitmap);
//
//      pgraphicsDirect2d->m_pdevicecontext->SetTarget(nullptr);
//
//      m_pd2d1devicecontext->BeginDraw();
//
//      D2D1_RECT_F rectfDib1;
//
//      D2D1_MATRIX_3X2_F originalTransform;
//      m_pd2d1devicecontext->GetTransform(&originalTransform);
//
//      D2D1_MATRIX_3X2_F flipY =
//         D2D1::Matrix3x2F::Scale(1.0f, -1.0f) *
//         D2D1::Matrix3x2F::Translation(0.0f, m_pgpudevice->current_context()->m_rectangle.height());
//
//      m_pd2d1devicecontext->SetTransform(flipY * originalTransform);
//
//      rectfDib1.left = 0;
//      rectfDib1.top = 0;
//      rectfDib1.right = m_pgpudevice->current_context()->m_rectangle.width();
//      rectfDib1.bottom = m_pgpudevice->current_context()->m_rectangle.height();
//      D2D1_COLOR_F color;
//
//      copy(color, ::rgba(0.75, 0.75, 0.40, 0.5));
//
//      comptr< ID2D1SolidColorBrush> pd2d1solidcolorbrush1;
//
//      m_pd2d1devicecontext->CreateSolidColorBrush(color, &pd2d1solidcolorbrush1);
//
//      D2D1_RECT_F r1;
//
//      r1.left = 500;
//      r1.top = 300;
//      r1.right = 600;
//      r1.bottom = 400;
//
//      m_pd2d1devicecontext->FillRectangle(r1, pd2d1solidcolorbrush1);
//
//      if (1)
//      {
//
//         m_pd2d1devicecontext->DrawImage(
//            pd2d1bitmap,
//            D2D1::Point2F(0.f, 0.f),
//            rectfDib1,
//            D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
//            D2D1_COMPOSITE_MODE_SOURCE_COPY);
//
//      }
//
//      m_pd2d1devicecontext->SetTransform(originalTransform);
//      
//      m_pd2d1devicecontext->EndDraw();
//
//      pgraphicsDirect2d->m_pdevicecontext->SetTarget(pd2d1bitmap);
//
//      m_pd3d11context->Flush();
//
//      present();
//
//   }
//
//
//} // namespace draw2d_directx11
//
//
//
