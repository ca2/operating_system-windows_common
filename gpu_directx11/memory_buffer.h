#pragma once

#include "bred/gpu/memory_buffer.h"
#include "context.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 memory_buffer :
      virtual public ::gpu::memory_buffer
   {
   public:


      //context* m_pgpucontext;
      //void* m_mapped = nullptr;
      //VkBuffer m_buffer = VK_NULL_HANDLE;
      //VkDeviceMemory m_memory = VK_NULL_HANDLE;

      //VkDeviceSize m_bufferSize;
      uint32_t m_instanceCount;
      //VkDeviceSize m_instanceSize;
      //VkDeviceSize m_alignmentSize;
      //VkBufferUsageFlags m_usageFlags;
      //VkMemoryPropertyFlags m_memoryPropertyFlags;

      ::comptr<ID3D11Buffer> m_pbuffer;

      memory_buffer();
      ~memory_buffer();



      void on_initialize_memory_buffer() override;


      bool is_initialized() const override;




      uint32_t getInstanceCount() const { return m_instanceCount; }

      void* __map(memsize start, memsize count) override;
      void __unmap() override;

      void bind() override;
      void unbind()override;


   };


}  // namespace gpu_directx11



