//
// Created by camilo on 2026-06-08.
//
#pragma once


#include "bred/gpu/fence.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 fence :
      virtual public ::gpu::fence
   {
   public:


      ::comptr < ID3D11Query > m_pquery;
      bool                     m_bPending;


      fence();
      ~fence() override;


      void initialize_gpu_fence(::gpu::device * pgpudevice, bool bCreateSignaled) override;

      void reset_gpu_fence() override;

      void wait_gpu_fence() override;


   };


} // namespace gpu_directx11
