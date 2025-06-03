// Changed by camilo on 2021-12-10 19:36 PM <3ThomasBorregaardSorensen!!
#pragma once


#include "aura/_.h"


#if defined(_gpu_directx_project)
#define CLASS_DECL_GPU_DIRECTX  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_GPU_DIRECTX  CLASS_DECL_IMPORT
#endif


#include "_gpu_directx.h"


namespace gpu_directx
{

   
   class program;
   class approach;


   class swap_chain;

   class benchmark;

   class physical_device;

   class context;

   class device;

   class pipeline;

   class renderer;

   class descriptor_pool;

   class buffer;

   class set_descriptor_layout;

   class frame;


} // namespace gpu_directx




 