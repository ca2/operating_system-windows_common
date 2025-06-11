// Created by camilo on 2025-06-04 23:08 <3ThomasBorregaardSørensen!!
#pragma once


#include "direct2d/_.h"
#include "direct2d/swap_chain.h"
#include "draw2d_direct2d/_.h"
#include "draw2d_direct2d/swap_chain.h"
#include "device.h"
#include "swap_chain.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 direct2d_draw2d_swap_chain :
      virtual public ::draw2d_direct2d::swap_chain,
      virtual public ::gpu_directx11::swap_chain
   {
   public:


      D3D_DRIVER_TYPE m_edrivertype;
      D3D_FEATURE_LEVEL m_efeaturelevel;

#ifdef _DEBUG

      ::comptr<ID3D11Debug> m_pd3d11debug;

#endif


      
      ::pointer < ::gpu_directx11::device > m_pgpudevice;
      ::comptr<IDXGISwapChain1> m_pdxgiswapchain1;
      //::comptr<ID3D11Device> m_pd3d11device;
      //::comptr<ID3D11DeviceContext> m_pd3d11context;
      //::comptr <ID3D11Resource > m_presourceWrappedRTV;
      //::comptr<IDXGISurface> m_pdxgisurface;
      //::comptr<ID3D12CommandQueue>                                m_pcommandqueue;
      //::pointer < ::draw2d_direct2d::graphics > m_pgraphics;
      ::pointer < ::gpu_directx11::context > m_pgpucontext;
      //::comptr<ID2D1DeviceContext> m_pd2d1devicecontext;
      //::comptr <IDXGIDevice> m_pdxgidevice;
      //::comptr <IDXGIDevice> m_pdxgidevice;
      //ID3D11Resource* resources[1];
      //::comptr<ID2D1Device> m_pd2d1device;

      //::comptr<ID2D1DeviceContext> m_pd2d1context;

      //::comptr<IDXGISurface> m_pdxgisurface;

      //::comptr<ID2D1Bitmap1> m_pd2d1bitmap;

      //::pointer < ::gpu_directx12::shader > m_pshader;


      direct2d_draw2d_swap_chain();
      ~direct2d_draw2d_swap_chain() override;


      void initialize_direct2d_draw2d_gpu_swap_chain(::gpu::device* pgpudevice, ::windowing::window* pwindow) override;

      virtual IDXGISurface* _get_dxgi_surface();

      void endDraw(::draw2d_gpu::graphics* pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc) override;
      void present();

   };


} // namespace gpu_directx12


