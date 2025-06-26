// Created by camilo on 2025-06-04 23:08 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/end_draw.h"
//#include "directx11/graphics.h"


namespace draw2d_directx11
{


   class CLASS_DECL_DRAW2D_DIRECTX11 swap_chain_end_draw :
      virtual public ::draw2d_gpu::end_draw
   {
   public:


      ::pointer < ::draw2d_directx11::graphics > m_pgraphics;
      ::pointer < ::gpu_directx11::context > m_pgpucontext;
      ::comptr<ID2D1DeviceContext> m_pd2d1devicecontext;
      ::comptr <IDXGIDevice> m_pdxgidevice;

      ::comptr<ID2D1Device> m_pd2d1device;

      //::comptr<ID2D1DeviceContext> m_pd2d1context;

      ::comptr<IDXGISurface> m_pdxgisurface;

      ::comptr<ID2D1Bitmap1> m_pd2d1bitmap;

      //::pointer < ::gpu_directx11::shader > m_pshader;

      swap_chain_end_draw();
      ~swap_chain_end_draw() override;


      void initialize_end_draw(::draw2d_gpu::graphics * pgraphics) override;



      void endDraw(::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc) override;


   };


} // namespace draw2d_directx11



