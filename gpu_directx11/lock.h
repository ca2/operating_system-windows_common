// Created by camilo on 2025-06-25 14:36 <3ThomasBorregaardSørensen!!
#pragma once


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 directx11_lock
   {
   public:


      context* m_pcontext;
      directx11_lock* m_plockUpper;
      ::particle_pointer m_pparticleSynchronization;


      directx11_lock(::gpu::context* pcontext);
      ~directx11_lock();




   };



} // namespace gpu_directx11



