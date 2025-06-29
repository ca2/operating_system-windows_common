// From gpu_vulkan/input_layout.h by camilo on 2025-06-29 03:49 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/input_layout.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 input_layout :
      virtual public ::gpu::input_layout
   {
   public:


      input_layout();
      ~input_layout();

      ::comptr <ID3D11InputLayout > m_pd3d11inputlayout;


   };


} // namespace gpu_directx11



