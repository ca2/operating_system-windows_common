// From bred/gpu/context_object.h by 
// camilo on 2025-06-29 15:57 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/context_object.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 context_object :
      virtual public ::gpu::context_object
   {
   public:


      ::gpu_directx11::context * m_pcontext;


      context_object();
      ~context_object();


      void on_initialize_gpu_context_object() override;


      ::gpu_directx11::context* context() { return m_pcontext; }


   };



} // namespace gpu_directx11



