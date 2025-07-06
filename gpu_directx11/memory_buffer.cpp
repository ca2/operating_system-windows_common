#include "framework.h"
#include "gpu_directx11/context.h"
/*
 * Encapsulates a directx11 memory_buffer
 *
 * Initially based off DirectX11Buffer by Sascha Willems -
 * https://github.com/SaschaWillems/DirectX11/blob/master/base/DirectX11Buffer.h
 */

#include "memory_buffer.h"


namespace gpu_directx11
{

   /**
    * Returns the minimum m_vkinstance size required to be compatible with devices minOffsetAlignment
    *
    * @param instanceSize The size of an m_vkinstance
    * @param minOffsetAlignment The minimum required alignment, in bytes, for the offset member (eg
    * minUniformBufferOffsetAlignment)
    *
    * @return HRESULT of the memory_buffer mapping call
    */
   //VkDeviceSize memory_buffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
   //   if (minOffsetAlignment > 0) {
   //      return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
   //   }
   //   return instanceSize;
   //}

   memory_buffer::memory_buffer()
   {

   }


   //void memory_buffer::initialize_buffer(
   //   ::gpu::context* pgpucontext,
   //   VkDeviceSize instanceSize,
   //   uint32_t instanceCount,
   //   VkBufferUsageFlags usageFlags,
   //   VkMemoryPropertyFlags memoryPropertyFlags,
   //   VkDeviceSize minOffsetAlignment)
   //{
   //   m_pgpucontext = dynamic_cast <::gpu_directx11::context*>(pgpucontext);
   //   m_instanceSize = instanceSize;
   //   m_instanceCount = instanceCount;
   //   m_usageFlags = usageFlags;
   //   m_memoryPropertyFlags = memoryPropertyFlags;

   //   m_alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
   //   m_bufferSize = m_alignmentSize * instanceCount;
   //   m_pgpucontext->createBuffer(m_bufferSize, m_usageFlags, m_memoryPropertyFlags, m_buffer, m_memory);
   //}

   memory_buffer::~memory_buffer() {
      unmap();

 /*     vkDestroyBuffer(m_pgpucontext->logicalDevice(), m_buffer, nullptr);
      vkFreeMemory(m_pgpucontext->logicalDevice(), m_memory, nullptr);*/

   }


   void memory_buffer::initialize_memory_buffer(::gpu::context* pcontext, memsize size, ::gpu::memory_buffer::enum_type etype)
   {

      ::gpu::memory_buffer::initialize_memory_buffer(pcontext, size, etype);

      int iCount = (int)size;
      //     //int iTypeSize = sizeof(::graphics3d::Vertex);
      //int iMyCalculatedTotalSizeInBytes = iTypeSize * iCount;
      int iFrameworkCalculatedTotalSizeInBytes = (int)size;
      D3D11_BUFFER_DESC bd =
      { (UINT)
         iFrameworkCalculatedTotalSizeInBytes,
         D3D11_USAGE_DYNAMIC,
         D3D11_BIND_VERTEX_BUFFER,
         D3D11_CPU_ACCESS_WRITE
      };
      //D3D11_SUBRESOURCE_DATA initData =
      //{
      //   vertices.data()
      //};

      ::cast < ::gpu_directx11::context > pgpucontext = m_pcontext;

      ::cast < ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;

      auto pd3d11device = pgpudevice->m_pdevice;



      auto hresult = pd3d11device->CreateBuffer(&bd, nullptr, &m_pbuffer);

      if (FAILED(hresult))
      {

         throw ::hresult_exception(hresult);

      }

   }

   
   bool memory_buffer::is_initialized() const
   { 

      return m_pbuffer;
   
   }
   


   ///**
   // * Map a memory range of this memory_buffer. If successful, mapped points to the specified memory_buffer range.
   // *
   // * @param size (Optional) Size of the memory range to map. Pass VK_WHOLE_SIZE to map the complete
   // * memory_buffer range.
   // * @param offset (Optional) Byte offset from beginning
   // *
   // * @return HRESULT of the memory_buffer mapping call
   // */
   //HRESULT memory_buffer::map(VkDeviceSize size, VkDeviceSize offset)
   //{

   //   assert(m_buffer && m_memory && "Called map on memory_buffer before create");

   //   ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

   //   return vkMapMemory(m_pgpucontext->logicalDevice(), m_memory, offset, size, 0, &m_mapped);

   //}

   /**
    * Unmap a mapped memory range
    *
    * @note Does not return a result as vkUnmapMemory can't fail
    */
   //void memory_buffer::unmap() {
   //   if (m_pMap) {

   //      ::cast < device > pgpudevice = m_pcontext->m_pgpudevice;
   //      //vkUnmapMemory(m_pgpucontext->logicalDevice(), m_memory);
   //      m_pMap = nullptr;
   //   }
   //}

   /**
    * Copies the specified data to the mapped memory_buffer. Default value writes whole memory_buffer range
    *
    * @param data Pointer to the data to copy
    * @param size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete memory_buffer
    * range.
    * @param offset (Optional) Byte offset from beginning of mapped region
    *
    */
   //void memory_buffer::writeToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset) {
   //   assert(m_mapped && "Cannot copy to unmapped memory_buffer");

   //   if (size == VK_WHOLE_SIZE) {
   //      memcpy(m_mapped, data, m_bufferSize);
   //   }
   //   else {
   //      char* memOffset = (char*)m_mapped;
   //      memOffset += offset;
   //      memcpy(memOffset, data, size);
   //   }
   //}

   ///**
   // * Flush a memory range of the memory_buffer to make it visible to the pgpucontext
   // *
   // * @note Only required for non-coherent memory
   // *
   // * @param size (Optional) Size of the memory range to flush. Pass VK_WHOLE_SIZE to flush the
   // * complete memory_buffer range.
   // * @param offset (Optional) Byte offset from beginning
   // *
   // * @return HRESULT of the flush call
   // */
   //HRESULT memory_buffer::flush(VkDeviceSize size, VkDeviceSize offset)
   //{
   //   VkMappedMemoryRange mappedRange = {};
   //   mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
   //   mappedRange.memory = m_memory;
   //   mappedRange.offset = offset;
   //   mappedRange.size = size;
   //   ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;
   //   return vkFlushMappedMemoryRanges(m_pgpucontext->logicalDevice(), 1, &mappedRange);
   //}

   ///**
   // * Invalidate a memory range of the memory_buffer to make it visible to the host
   // *
   // * @note Only required for non-coherent memory
   // *
   // * @param size (Optional) Size of the memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate
   // * the complete memory_buffer range.
   // * @param offset (Optional) Byte offset from beginning
   // *
   // * @return HRESULT of the invalidate call
   // */
   //HRESULT memory_buffer::invalidate(VkDeviceSize size, VkDeviceSize offset) 
   //{
   //   VkMappedMemoryRange mappedRange = {};
   //   mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
   //   mappedRange.memory = m_memory;
   //   mappedRange.offset = offset;
   //   mappedRange.size = size;
   //   ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;
   //   return vkInvalidateMappedMemoryRanges(m_pgpucontext->logicalDevice(), 1, &mappedRange);
   //}

   ///**
   // * Create a memory_buffer info descriptor
   // *
   // * @param size (Optional) Size of the memory range of the descriptor
   // * @param offset (Optional) Byte offset from beginning
   // *
   // * @return VkDescriptorBufferInfo of specified offset and range
   // */
   //VkDescriptorBufferInfo memory_buffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
   //   return VkDescriptorBufferInfo{
   //       m_buffer,
   //       offset,
   //       size,
   //   };
   //}

   /**
    * Copies "instanceSize" bytes of data to the mapped memory_buffer at an offset of index * alignmentSize
    *
    * @param data Pointer to the data to copy
    * @param index Used in offset calculation
    *
    */
   //void memory_buffer::writeToIndex(void* data, int index) {
   //   //writeToBuffer(data, m_instanceSize, index * m_alignmentSize);
   //}

   /**
    *  Flush the memory range at index * alignmentSize of the memory_buffer to make it visible to the pgpucontext
    *
    * @param index Used in offset calculation
    *
    */
   //HRESULT memory_buffer::flushIndex(int index) 
   //{
   //   
   ////   return flush(m_alignmentSize, index * m_alignmentSize); 
   //   return E_FAIL;
   //
   //}

   ///**
   // * Create a memory_buffer info descriptor
   // *
   // * @param index Specifies the region given by index * alignmentSize
   // *
   // * @return VkDescriptorBufferInfo for m_vkinstance at index
   // */
   //VkDescriptorBufferInfo memory_buffer::descriptorInfoForIndex(int index) {
   //   return descriptorInfo(m_alignmentSize, index * m_alignmentSize);
   //}

   /**
    * Invalidate a memory range of the memory_buffer to make it visible to the host
    *
    * @note Only required for non-coherent memory
    *
    * @param index Specifies the region to invalidate: index * alignmentSize
    *
    * @return HRESULT of the invalidate call
    */
   //HRESULT memory_buffer::invalidateIndex(int index) {
   //   //return invalidate(m_alignmentSize, index * m_alignmentSize);
   //   return E_FAIL;
   //}


   void* memory_buffer::__map(memsize start, memsize count)
   {
      ::cast < ::gpu_directx11::context > pcontext = m_pcontext;

      D3D11_MAPPED_SUBRESOURCE mapped;
      pcontext->m_pcontext->Map(
         m_pbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
//      memcpy(mapped.pData, &myData, sizeof(MyConstants));

      m_pMap = mapped.pData;

      return m_pMap;
  
   }


   void memory_buffer::__unmap()
   {
      
      ::cast < ::gpu_directx11::context > pcontext = m_pcontext;
      
      pcontext->m_pcontext->Unmap(m_pbuffer, 0);

   }


   void memory_buffer::bind()
   {

      if (m_etype == e_type_vertex_buffer)
      {

         // Bind vertex buffer
         UINT stride = m_iStrideSize;
         UINT offset = 0;

         ::cast < ::gpu_directx11::context > pcontext = m_pcontext;

         pcontext->m_pcontext->IASetVertexBuffers(0, 1, m_pbuffer.pp(), &stride, &offset);

      }
      else if (m_etype == e_type_index_buffer)
      {

         ::cast < ::gpu_directx11::context > pcontext = m_pcontext;
         
         pcontext->m_pcontext->IASetIndexBuffer(m_pbuffer, DXGI_FORMAT_R32_UINT, 0);

      }

   }


   void memory_buffer::unbind()
   {


   }


}  // namespace graphics3d_directx11

