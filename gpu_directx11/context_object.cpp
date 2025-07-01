// From bred/gpu/context_object.cpp by 
// camilo on 2025-06-29 16:07 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "context.h"
#include "context_object.h"


namespace gpu_directx11
{


   context_object::context_object()
   {

   }


   context_object::~context_object()
   {

   }


   void context_object::on_initialize_gpu_context_object()
   {

      m_pcontext = dynamic_cast <::gpu_directx11::context*> (m_pgpucontext.m_p);

   }


} // namespace gpu_directx11



