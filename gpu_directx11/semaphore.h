//
// Created by camilo on 2026-06-08.
//
#pragma once


#include "bred/gpu/semaphore.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 semaphore :
      virtual public ::gpu::semaphore
   {
   public:


      semaphore();
      ~semaphore() override;


      void initialize_gpu_semaphore(::gpu::context * pgpucontext) override;


   };


} // namespace gpu_directx11
