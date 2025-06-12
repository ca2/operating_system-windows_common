// Created by camilo on 2025-06-10 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "direct2d_draw2d_swap_chain.h"
#include "draw2d_direct2d/graphics.h"
#include "direct2d/direct2d.h"
#include "aura/windowing/window.h"
#include "gpu_directx11/context.h"
#include "gpu_directx11/renderer.h"
#include "gpu_directx11/swap_chain_render_target_view.h"
#include "aura/graphics/gpu/graphics.h"
#include "windowing_win32/window.h"


namespace gpu_directx11
{


   direct2d_draw2d_swap_chain::direct2d_draw2d_swap_chain()
   {


   }


   direct2d_draw2d_swap_chain::~direct2d_draw2d_swap_chain()
   {


   }


   IDXGISurface* direct2d_draw2d_swap_chain::_get_dxgi_surface()
   {

      return draw2d_direct2d::swap_chain::_get_dxgi_surface();

   }


   void direct2d_draw2d_swap_chain::initialize_gpu_swap_chain(::gpu::device* pgpudevice, ::windowing::window* pwindow)
   {

      ::cast < ::gpu_directx11::device > pdx11gpudevice = pgpudevice;

      HRESULT hrD3d11Device = pdx11gpudevice->m_pdevice.as(m_pd3d11device);

      ::defer_throw_hresult(hrD3d11Device);

      HRESULT hrDxgiDevice2 = m_pd3d11device.as(m_pdxgidevice_2);

      ::defer_throw_hresult(hrDxgiDevice2);

      HRESULT hrD3d11DeviceContext = pdx11gpudevice->m_pdevicecontext.as(m_pd3d11context);

      ::defer_throw_hresult(hrD3d11DeviceContext);

      ::draw2d_direct2d::swap_chain::initialize_gpu_swap_chain(pgpudevice, pwindow);

      ::gpu_directx11::swap_chain::initialize_gpu_swap_chain(pgpudevice, pwindow);

   }
      

   void direct2d_draw2d_swap_chain::initialize_direct2d_draw2d_gpu_swap_chain(::gpu::device* pgpudevice, ::windowing::window* pwindow)
   {

      ::draw2d_direct2d::swap_chain::initialize_direct2d_draw2d_gpu_swap_chain(pgpudevice, pwindow);

   }


   void direct2d_draw2d_swap_chain::endDraw(::draw2d_gpu::graphics* pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc)
   {

      ::draw2d_direct2d::swap_chain::endDraw(pgraphics, puserinteraction, prendererSrc);

   }


   ID3D11Device* direct2d_draw2d_swap_chain::_get_d3d11_device()
   {

      return m_pd3d11device;

   }


   void direct2d_draw2d_swap_chain::present()
   {
      
      ::draw2d_direct2d::swap_chain::present();

   }


} // namespace gpu_directx11



