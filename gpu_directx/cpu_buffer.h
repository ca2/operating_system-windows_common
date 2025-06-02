#pragma once


#include "cube/gpu/cpu_buffer.h"


namespace gpu_directx
{


   class CLASS_DECL_GPU_DIRECTX cpu_buffer :
      virtual public ::gpu::cpu_buffer
   {
   public:


      //__creatable_from_base(buffer, ::gpu::buffer);


      cpu_buffer();
      ~cpu_buffer() override;


      void gpu_read() override;
      void gpu_write() override;
      


   };


} // namespace gpu_directx



