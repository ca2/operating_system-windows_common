// Created by camilo on 2025-06-04 23:08 <3ThomasBorregaardSørensen!!
#pragma once


#include "directx11/swap_chain.h"
//#include <d3d11.h>

namespace direct2d
{


   class CLASS_DECL_DIRECT2D swap_chain :
      virtual public ::directx11::swap_chain
   {
   public:


      ::comptr<ID3D11Device>                 m_pd3d11device;
      ::comptr<ID3D11DeviceContext>          m_pd3d11context;


      

      ::comptr<ID2D1DeviceContext>           m_pd2d1devicecontext;
      ::comptr<ID2D1Bitmap1>                 m_pd2d1bitmap;


      swap_chain();
      ~swap_chain() override;


      void initialize_gpu_swap_chain(::gpu::device* pdevice, ::windowing::window* pwindow) override;


      ID3D11Device* _get_d3d11_device() override;

      IDXGIDevice* _get_dxgi_device() override;

      IDXGISurface* _get_dxgi_surface() override;

      virtual ID2D1Device* _get_d2d1_device();


      void endDraw(::draw2d_gpu::graphics * pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc) override;

      void present() override;


   };


} // namespace direct2d



