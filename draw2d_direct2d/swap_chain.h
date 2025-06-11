// Created by camilo on 2025-06-10 14:25 <3ThomasBorregaardSørensen!!
#pragma once


#include "direct2d/swap_chain.h"


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D swap_chain :
      virtual public ::direct2d::swap_chain
   {
   public:

      D3D11_TEXTURE2D_DESC texDesc = {};
      //::pointer < ::draw2d_gpu::graphics > m_pgraphics;
      //::pointer < ::gpu::context > m_pgpucontext;

      //::comptr<ID3D11Device> m_pd3d11device;
      //::comptr<ID3D11DeviceContext> m_pd3d11context;


      ////::comptr<ID2D1DeviceContext> m_pd2d1devicecontext;
      //::comptr <IDXGIDevice> m_pdxgidevice;

      //::comptr<ID2D1Device> m_pd2d1device;

      ////::comptr<ID2D1DeviceContext> m_pd2d1context;

      //::comptr<IDXGISurface> m_pdxgisurface;

      ////::comptr<ID2D1Bitmap1> m_pd2d1bitmap;

      ////::pointer < ::gpu_directx11::shader > m_pshader;

      swap_chain();
      ~swap_chain() override;


      void initialize_gpu_swap_chain(::gpu::device* pdevice, ::windowing::window* pwindow) override;


      virtual void initialize_direct2d_draw2d_gpu_swap_chain(::gpu::device* pdevice, ::windowing::window* pwindow);

      IDXGIDevice * _get_dxgi_device() override;

      void endDraw(::draw2d_gpu::graphics* pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc) override;


   };


} // namespace draw2d_direct2d



