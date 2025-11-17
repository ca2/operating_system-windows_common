// Created by camilo on 2025-06-10 <3ThomasBorregaardSørensen!!
#include "framework.h"
//#include "directx11_swap_chain.h"
//#include "graphics.h"
//#include "directx11/directx11.h"
//#include "aura/windowing/window.h"
//#include "gpu_directx11/context.h"
//#include "gpu_directx11/renderer.h"
////#include "gpu_directx11/swap_chain_render_target_view.h"
//#include "bred/gpu/graphics.h"
//#include "windowing_win32/window.h"
//namespace draw2d_directx11
//{
//
//
//   directx11_swap_chain::directx11_swap_chain()
//   {
//
//
//   }
//
//
//   directx11_swap_chain::~directx11_swap_chain()
//   {
//
//
//   }
//
//
//   //IDXGISurface* directx11_swap_chain::_get_dxgi_surface()
//   //{
//
//   //   return draw2d_directx11::swap_chain::_get_dxgi_surface();
//
//   //}
//
//
//   void directx11_swap_chain::initialize_gpu_swap_chain(::gpu::device* pgpudevice, ::windowing::window* pwindow)
//   {
//
//      ::cast < ::gpu_directx11::device > pdx11gpudevice = pgpudevice;
//
//      HRESULT hrD3d11Device = pdx11gpudevice->m_pdevice.as(m_pd3d11device);
//
//      ::defer_throw_hresult(hrD3d11Device);
//
//      HRESULT hrDxgiDevice2 = m_pd3d11device.as(m_pdxgidevice_2);
//
//      ::defer_throw_hresult(hrDxgiDevice2);
//
//      HRESULT hrD3d11DeviceContext = pdx11gpudevice->m_pdevicecontext.as(m_pd3d11context);
//
//      ::defer_throw_hresult(hrD3d11DeviceContext);
//
//      //HRESULT hrGetBuffer = m_pdxgiswapchain1->GetBuffer(
//        // 0, __interface_of(m_ptextureBackBuffer));
//
//      //::defer_throw_hresult(hrGetBuffer);
//
//      //auto pd3d11device = _get_d3d11_device();
//
//      //HRESULT hrCreateRenderTargetView = pd3d11device->CreateRenderTargetView(
//      //   m_ptextureBackBuffer, nullptr, &m_prendertargetviewBackBuffer);
//
//      //::defer_throw_hresult(hrCreateRenderTargetView);
//
//      ::draw2d_directx11::swap_chain::initialize_gpu_swap_chain(pgpudevice, pwindow);
//
//      ::gpu_directx11::swap_chain::initialize_gpu_swap_chain(pgpudevice, pwindow);
//
//   }
//      
//
//   //void directx11_swap_chain::initialize_directx11_draw2d_gpu_swap_chain(::gpu::device* pgpudevice, ::windowing::window* pwindow)
//   //{
//
//   //   ::draw2d_directx11::swap_chain::initialize_directx11_draw2d_gpu_swap_chain(pgpudevice, pwindow);
//
//   //}
//
//
//   void directx11_swap_chain::endDraw(::draw2d_gpu::graphics* pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc)
//   {
//
//      ::draw2d_directx11::swap_chain::endDraw(pgraphics, puserinteraction, prendererSrc);
//
//   }
//
//
//   ID3D11Device* directx11_swap_chain::_get_d3d11_device()
//   {
//
//      return m_pd3d11device;
//
//   }
//
//
//   void directx11_swap_chain::present()
//   {
//      
//      ::draw2d_directx11::swap_chain::present();
//
//   }
//
//
//} // namespace draw2d_directx11
//
//
//
