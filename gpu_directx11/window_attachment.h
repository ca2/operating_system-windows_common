// Created by camilo on 2026-08-04 01:33 <3ThomasBorregaardSørensen!! Mummi!! Bilbo!!
#pragma once


#include "bred/gpu/window_attachment.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 window_attachment :
      virtual public ::gpu::window_attachment
   {
   public:


      window_attachment();
      ~window_attachment() override;


      void on_start_frame() override;

      void on_end_frame() override;




   };


} // namespace gpu_directx11



