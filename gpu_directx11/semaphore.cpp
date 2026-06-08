//
// Created by camilo on 2026-06-08.
//
#include "framework.h"
#include "semaphore.h"


namespace gpu_directx11
{


   semaphore::semaphore()
   {


   }


   semaphore::~semaphore()
   {


   }


   void semaphore::initialize_gpu_semaphore(::gpu::context * pgpucontext)
   {

      ::gpu::semaphore::initialize_gpu_semaphore(pgpucontext);

   }


} // namespace gpu_directx11
