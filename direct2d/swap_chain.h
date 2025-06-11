// Created by camilo on 2025-06-04 23:08 <3ThomasBorregaardSørensen!!
#pragma once


#include "aura/graphics/gpu/swap_chain.h"
#include <d3d11.h>

namespace direct2d
{


   class CLASS_DECL_DIRECT2D swap_chain :
      virtual public ::gpu::swap_chain
   {
   public:


      //::pointer < ::draw2d_gpu::graphics >   m_pgraphics;
      //::pointer < ::gpu::context >           m_pgpucontext;

      ::comptr<ID3D11Device>                 m_pd3d11device;
      ::comptr<ID3D11DeviceContext>          m_pd3d11context;


      ::comptr<ID2D1Device>                  m_pd2d1device;

      //::comptr<ID2D1DeviceContext> m_pd2d1context;

      ::comptr <IDXGIDevice>                 m_pdxgidevice_2;
      ::comptr<IDXGISurface>                 m_pdxgisurface_2;
      ::comptr<ID2D1DeviceContext>           m_pd2d1devicecontext;
      ::comptr<ID2D1Bitmap1>                 m_pd2d1bitmap;

      //::pointer < ::gpu_directx11::shader > m_pshader;

      swap_chain();
      ~swap_chain() override;


      void initialize_gpu_swap_chain(::gpu::device* pdevice, ::windowing::window* pwindow) override;

      virtual IDXGIDevice* _get_dxgi_device();

      virtual IDXGISurface* _get_dxgi_surface();

      void endDraw(::draw2d_gpu::graphics * pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc) override;


   };


} // namespace direct2d



