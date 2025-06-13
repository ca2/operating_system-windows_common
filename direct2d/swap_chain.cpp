// Created by camilo on 2025-06-04 23:11 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "swap_chain.h"
#include "direct2d.h"
#include "aura/windowing/window.h"
#include "bred/gpu/context.h"
#include "bred/gpu/renderer.h"
#include "bred/gpu/graphics.h"


namespace direct2d
{


   swap_chain::swap_chain()
   {


   }


   swap_chain::~swap_chain()
   {


   }


   void swap_chain::initialize_gpu_swap_chain(::gpu::device * pdevice, ::windowing::window * pwindow)
   {

      ::directx11::swap_chain::initialize_gpu_swap_chain(pdevice, pwindow);

   }

   
   ID3D11Device* swap_chain::_get_d3d11_device()
   {

      return m_pd3d11device;

   }



   IDXGIDevice* swap_chain::_get_dxgi_device()
   {

      return m_pdxgidevice_2;

   }


   IDXGISurface* swap_chain::_get_dxgi_surface()
   {

      auto r = m_pwindow->get_window_rectangle();

      if (texDesc.Width != r.width()
         || texDesc.Height != r.height())
      {


         texDesc.Width = r.width();
         texDesc.Height = r.height();
         texDesc.MipLevels = 1;
         texDesc.ArraySize = 1;
         texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
         texDesc.SampleDesc.Count = 1;
         texDesc.Usage = D3D11_USAGE_DEFAULT;
         texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
         texDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

         m_pd3d11device->CreateTexture2D(&texDesc, nullptr, &m_ptextureShared);

         m_ptextureShared.as(m_pdxgisurface_2);

      }



      return m_pdxgisurface_2;

   }

   
   ID2D1Device* swap_chain::_get_d2d1_device()
   {

      return ::direct2d::get()->d2d1_device(m_pwindow, m_pwindow->get_window_rectangle());

   }



   void swap_chain::endDraw(::draw2d_gpu::graphics * pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc)
   {



//      ::cast < ::draw2d_direct2d::graphics > pgraphicsDirect2d = pgraphics;
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
//      // Save original transform
//      D2D1_MATRIX_3X2_F originalTransform;
//      m_pd2d1devicecontext->GetTransform(&originalTransform);
//
//      // Flip vertically: scale Y by -1 and translate down
//      D2D1_MATRIX_3X2_F flipY =
//         D2D1::Matrix3x2F::Scale(1.0f, -1.0f) *
//         D2D1::Matrix3x2F::Translation(0.0f, m_pgpucontext->m_rectangle.height());
//
//      m_pd2d1devicecontext->SetTransform(flipY * originalTransform);
//
//
//      rectfDib1.left = 0;
//      rectfDib1.top = 0;
//      rectfDib1.right = m_pgpucontext->m_rectangle.width();
//      rectfDib1.bottom = m_pgpucontext->m_rectangle.height();
//      D2D1_COLOR_F color;
//
//      copy(color, ::rgba(0.75, 0.75, 0.40, 0.5));
//
//      comptr< ID2D1SolidColorBrush> pd2d1solidcolorbrush1;
//
//      m_pd2d1devicecontext->CreateSolidColorBrush(color, &pd2d1solidcolorbrush1);
//      //D2D1_RECT_F r1;
//
//      //r1.left = 500;
//      //r1.top = 300;
//      //r1.right = 600;
//      //r1.bottom = 400;
//
//      //m_pd2d1devicecontext->FillRectangle(r1, pd2d1solidcolorbrush1);
//
//
//
//      m_pd2d1devicecontext->DrawImage(
//         pd2d1bitmap,
//         D2D1::Point2F(0.f, 0.f),
//         rectfDib1,
//         D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
//         D2D1_COMPOSITE_MODE_SOURCE_COPY);
//
//      // Restore transform
//      m_pd2d1devicecontext->SetTransform(originalTransform);
//      //D2D1_RECT_F r2;
//
//      //r2.left = 100;
//      //r2.top = 100;
//      //r2.right = 200;
//      //r2.bottom = 500;
//
//      //comptr< ID2D1SolidColorBrush> pd2d1solidcolorbrush;
//
//      //D2D1_COLOR_F color1;
//
//      //copy(color1, ::rgba(0.5, 0.75, 0.90, 0.5));
//
//      //m_pd2d1devicecontext->CreateSolidColorBrush(color1, &pd2d1solidcolorbrush);
//
//      //m_pd2d1devicecontext->DrawRectangle(r2, pd2d1solidcolorbrush);
//
//
//      m_pd2d1devicecontext->EndDraw();
//
//      pgraphicsDirect2d->m_pdevicecontext->SetTarget(pd2d1bitmap);
//
//      ::cast < ::gpu::renderer > pgpurendererOutput = m_pgraphics->end_draw_renderer_output();
//
//      pgpurendererOutput->endDraw();
//
//      ///::cast < gpu_directx11::swap_chain_render_target_view > pswapchainrendertargetview = pgpurendererOutput->m_prendertargetview;
//
//      //pswapchainrendertargetview->endDraw();
//
//
//      //d2dDeviceContext->BeginDraw();
////// ... Direct2D drawing calls ...
//      pgraphicsDirect2d->m_pdevicecontext->EndDraw();
//
//      //m_pd3d11on12->ReleaseWrappedResources(resources, _countof(resources));
//      m_pd3d11context->Flush(); // Ensure rendering is completed
//
//
//      //::cast < ::draw2d_direct2d::graphics > pgraphicsDirect2d = pgraphics;
//
//      //comptr < ID2D1Bitmap > pd2d1bitmap;
//
//      //pgraphicsDirect2d->m_pbitmaprendertarget->GetBitmap(&pd2d1bitmap);
//
//      //pgraphicsDirect2d->m_pdevicecontext->SetTarget(nullptr);
//
//      //m_pd2d1devicecontext->BeginDraw();
//
//      //D2D1_RECT_F rectfDib1;
//
//      //// Save original transform
//      //D2D1_MATRIX_3X2_F originalTransform;
//      //m_pd2d1devicecontext->GetTransform(&originalTransform);
//
//      //// Flip vertically: scale Y by -1 and translate down
//      //D2D1_MATRIX_3X2_F flipY =
//      //   D2D1::Matrix3x2F::Scale(1.0f, -1.0f) *
//      //   D2D1::Matrix3x2F::Translation(0.0f, m_pgpucontext->m_rectangle.height());
//
//      //m_pd2d1devicecontext->SetTransform(flipY * originalTransform);
//
//
//      //rectfDib1.left = 0;
//      //rectfDib1.top = 0;
//      //rectfDib1.right = m_pgpucontext->m_rectangle.width();
//      //rectfDib1.bottom = m_pgpucontext->m_rectangle.height();
//      //D2D1_COLOR_F color;
//
//      //copy(color, ::rgba(0.75, 0.75, 0.40, 0.5));
//
//      //comptr< ID2D1SolidColorBrush> pd2d1solidcolorbrush1;
//
//      //m_pd2d1devicecontext->CreateSolidColorBrush(color, &pd2d1solidcolorbrush1);
//      ////D2D1_RECT_F r1;
//
//      ////r1.left = 500;
//      ////r1.top = 300;
//      ////r1.right = 600;
//      ////r1.bottom = 400;
//
//      ////m_pd2d1devicecontext->FillRectangle(r1, pd2d1solidcolorbrush1);
//
//
//
//      //m_pd2d1devicecontext->DrawImage(
//      //   pd2d1bitmap,
//      //   D2D1::Point2F(0.f, 0.f),
//      //   rectfDib1,
//      //   D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
//      //   D2D1_COMPOSITE_MODE_SOURCE_COPY);
//
//      //// Restore transform
//      //m_pd2d1devicecontext->SetTransform(originalTransform);
//      ////D2D1_RECT_F r2;
//
//      ////r2.left = 100;
//      ////r2.top = 100;
//      ////r2.right = 200;
//      ////r2.bottom = 500;
//
//      ////comptr< ID2D1SolidColorBrush> pd2d1solidcolorbrush;
//
//      ////D2D1_COLOR_F color1;
//
//      ////copy(color1, ::rgba(0.5, 0.75, 0.90, 0.5));
//
//      ////m_pd2d1devicecontext->CreateSolidColorBrush(color1, &pd2d1solidcolorbrush);
//
//      ////m_pd2d1devicecontext->DrawRectangle(r2, pd2d1solidcolorbrush);
//
//
//      //m_pd2d1devicecontext->EndDraw();
//
//      //m_pgraphics->m_pdevicecontext->SetTarget(pd2d1bitmap);
//
//      //::cast < gpu_directx11::renderer > pgpurendererOutput = m_pgraphics->end_draw_renderer_output();
//
//      //::cast < gpu_directx11::swap_chain_render_target_view > pswapchainrendertargetview = pgpurendererOutput->m_prendertargetview;
//
//      //pswapchainrendertargetview->endDraw();

   }


   void swap_chain::present()
   {

      m_pd3d11context->OMSetRenderTargets(1, m_prendertargetviewBackBuffer.pp(), nullptr);

      // 2. Set viewport
      D3D11_VIEWPORT vp = {};
      vp.TopLeftX = 0;
      vp.TopLeftY = 0;
      vp.Width = static_cast<float>(texDesc.Width);
      vp.Height = static_cast<float>(texDesc.Height);
      vp.MinDepth = 0.0f;
      vp.MaxDepth = 1.0f;
      m_pd3d11context->RSSetViewports(1, &vp);

      if (!m_pvertexshaderFullscreen
         || !m_ppixelshaderFullscreen
         || !m_pshaderresourceviewShader
         || !m_psamplerstateLinear)
      {

         _update_swap_chain();

      }

      m_pd3d11context->VSSetShader(m_pvertexshaderFullscreen, nullptr, 0);
      m_pd3d11context->PSSetShader(m_ppixelshaderFullscreen, nullptr, 0);

      m_pd3d11context->PSSetShaderResources(0, 1, m_pshaderresourceviewShader.pp());
      m_pd3d11context->PSSetSamplers(0, 1, m_psamplerstateLinear.pp());

      m_pd3d11context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      m_pd3d11context->Draw(3, 0);

      m_pdxgiswapchain1->Present(1, 0);

   }


} // namespace direct2d



