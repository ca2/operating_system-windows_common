// Created by camilo on 2025-06-10 14:25 <3ThomasBorregaardSørensen!!
#pragma once


#include "direct2d/swap_chain.h"


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D swap_chain :
      virtual public ::direct2d::swap_chain
   {
   public:


      swap_chain();
      ~swap_chain() override;


      void initialize_gpu_swap_chain(::gpu::device* pdevice, ::windowing::window* pwindow) override;


      virtual void initialize_direct2d_draw2d_gpu_swap_chain(::gpu::device* pdevice, ::windowing::window* pwindow);

      IDXGIDevice * _get_dxgi_device() override;

      void endDraw(::draw2d_gpu::graphics* pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc) override;


   };


} // namespace draw2d_direct2d



