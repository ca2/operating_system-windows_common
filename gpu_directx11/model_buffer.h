// From gpu_directx11/model_buffer.h by
// camilo on 2025-07-06 01:08 <3ThomasBorregaardSørensen!!
// From gpu_vulkan/model_buffer.h by
// camilo on 2025-07-01 20:34 <3ThomasBorregaardSørensen!!
// Created by camilo on 2025-06-11 01:06 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/model_buffer.h"
#include "acme/prototype/prototype/poolable.h"
#include "bred/graphics3d/types.h"
#include "gpu_directx11/context.h"
#include "gpu_directx11/memory_buffer.h"



namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 model_buffer :
      virtual public ::gpu::model_buffer
   {
   public:


      model_buffer();
      ~model_buffer();


      void on_initialize_gpu_context_object() override;


      void bind(::gpu::command_buffer* pcommandbuffer) override;

      void draw(::gpu::command_buffer* pcommandbuffer) override;

      void draw_lines(::gpu::command_buffer* pcommandbuffer) override;

      void unbind(::gpu::command_buffer* pcommandbuffer) override;


      void apply_input_layout() override;


   };


} // namespace gpu_directx11



