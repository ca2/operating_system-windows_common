// From gpu_vulkan/block.h by camilo on 2025-12-11 17:24 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/block.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 block :
      virtual public ::gpu::block
   {
   public:

      //comptr < ID3D11Buffer>              m_pbufferGlobalUbo;
      comptr < ID3D11Buffer>              m_pbuffer;

      //::pointer_array<::gpu_vulkan::memory_buffer> m_uboBuffers;

      //VkDescriptorSet m_vkdescriptorset;

      block();
      ~block() override;


      void create_gpu_block(::gpu::context *pgpucontext) override;


      ID3D11Buffer * buffer();
      //virtual VkDescriptorSet descriptor_set(::gpu::binding_set_pointer pgpubindingset, ::gpu::command_buffer *pgpucommandbuffer);


   };


} // namespace gpu_directx11


