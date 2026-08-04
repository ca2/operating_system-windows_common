// Created by camilo on 2026-08-04 01:33 <3ThomasBorregaardSørensen!! Mummi!! Bilbo!!
#pragma once


#include "bred/gpu/draw2d_window_attachment.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 draw2d_window_attachment :
      virtual public ::gpu::draw2d_window_attachment
   {
   public:


      draw2d_window_attachment();
      ~draw2d_window_attachment() override;


      void on_start_frame() override;

      void on_end_frame() override;




   };


} // namespace gpu_directx11



