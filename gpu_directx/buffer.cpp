#include "framework.h"
#include "gpu_directx/context.h"
/*
 * Encapsulates a directx buffer
 *
 * Initially based off DirectXBuffer by Sascha Willems -
 * https://github.com/SaschaWillems/DirectX/blob/master/base/DirectXBuffer.h
 */

#include "buffer.h"


namespace gpu_directx
{

   /**
    * Returns the minimum m_vkinstance size required to be compatible with devices minOffsetAlignment
    *
    * @param instanceSize The size of an m_vkinstance
    * @param minOffsetAlignment The minimum required alignment, in bytes, for the offset member (eg
    * minUniformBufferOffsetAlignment)
    *
    * @return HRESULT of the buffer mapping call
    */
   //VkDeviceSize buffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
   //   if (minOffsetAlignment > 0) {
   //      return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
   //   }
   //   return instanceSize;
   //}

   buffer::buffer()
   {

   }


   //void buffer::initialize_buffer(
   //   ::gpu::context* pgpucontext,
   //   VkDeviceSize instanceSize,
   //   uint32_t instanceCount,
   //   VkBufferUsageFlags usageFlags,
   //   VkMemoryPropertyFlags memoryPropertyFlags,
   //   VkDeviceSize minOffsetAlignment)
   //{
   //   m_pgpucontext = dynamic_cast <::gpu_directx::context*>(pgpucontext);
   //   m_instanceSize = instanceSize;
   //   m_instanceCount = instanceCount;
   //   m_usageFlags = usageFlags;
   //   m_memoryPropertyFlags = memoryPropertyFlags;

   //   m_alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
   //   m_bufferSize = m_alignmentSize * instanceCount;
   //   m_pgpucontext->createBuffer(m_bufferSize, m_usageFlags, m_memoryPropertyFlags, m_buffer, m_memory);
   //}

   buffer::~buffer() {
      unmap();

 /*     vkDestroyBuffer(m_pgpucontext->logicalDevice(), m_buffer, nullptr);
      vkFreeMemory(m_pgpucontext->logicalDevice(), m_memory, nullptr);*/

   }

   ///**
   // * Map a memory range of this buffer. If successful, mapped points to the specified buffer range.
   // *
   // * @param size (Optional) Size of the memory range to map. Pass VK_WHOLE_SIZE to map the complete
   // * buffer range.
   // * @param offset (Optional) Byte offset from beginning
   // *
   // * @return HRESULT of the buffer mapping call
   // */
   //HRESULT buffer::map(VkDeviceSize size, VkDeviceSize offset)
   //{

   //   assert(m_buffer && m_memory && "Called map on buffer before create");

   //   ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

   //   return vkMapMemory(m_pgpucontext->logicalDevice(), m_memory, offset, size, 0, &m_mapped);

   //}

   /**
    * Unmap a mapped memory range
    *
    * @note Does not return a result as vkUnmapMemory can't fail
    */
   void buffer::unmap() {
      if (m_mapped) {

         ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;
         //vkUnmapMemory(m_pgpucontext->logicalDevice(), m_memory);
         m_mapped = nullptr;
      }
   }

   /**
    * Copies the specified data to the mapped buffer. Default value writes whole buffer range
    *
    * @param data Pointer to the data to copy
    * @param size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete buffer
    * range.
    * @param offset (Optional) Byte offset from beginning of mapped region
    *
    */
   //void buffer::writeToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset) {
   //   assert(m_mapped && "Cannot copy to unmapped buffer");

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
   // * Flush a memory range of the buffer to make it visible to the pgpucontext
   // *
   // * @note Only required for non-coherent memory
   // *
   // * @param size (Optional) Size of the memory range to flush. Pass VK_WHOLE_SIZE to flush the
   // * complete buffer range.
   // * @param offset (Optional) Byte offset from beginning
   // *
   // * @return HRESULT of the flush call
   // */
   //HRESULT buffer::flush(VkDeviceSize size, VkDeviceSize offset)
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
   // * Invalidate a memory range of the buffer to make it visible to the host
   // *
   // * @note Only required for non-coherent memory
   // *
   // * @param size (Optional) Size of the memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate
   // * the complete buffer range.
   // * @param offset (Optional) Byte offset from beginning
   // *
   // * @return HRESULT of the invalidate call
   // */
   //HRESULT buffer::invalidate(VkDeviceSize size, VkDeviceSize offset) 
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
   // * Create a buffer info descriptor
   // *
   // * @param size (Optional) Size of the memory range of the descriptor
   // * @param offset (Optional) Byte offset from beginning
   // *
   // * @return VkDescriptorBufferInfo of specified offset and range
   // */
   //VkDescriptorBufferInfo buffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
   //   return VkDescriptorBufferInfo{
   //       m_buffer,
   //       offset,
   //       size,
   //   };
   //}

   /**
    * Copies "instanceSize" bytes of data to the mapped buffer at an offset of index * alignmentSize
    *
    * @param data Pointer to the data to copy
    * @param index Used in offset calculation
    *
    */
   void buffer::writeToIndex(void* data, int index) {
      //writeToBuffer(data, m_instanceSize, index * m_alignmentSize);
   }

   /**
    *  Flush the memory range at index * alignmentSize of the buffer to make it visible to the pgpucontext
    *
    * @param index Used in offset calculation
    *
    */
   HRESULT buffer::flushIndex(int index) 
   {
      
   //   return flush(m_alignmentSize, index * m_alignmentSize); 
      return E_FAIL;
   
   }

   ///**
   // * Create a buffer info descriptor
   // *
   // * @param index Specifies the region given by index * alignmentSize
   // *
   // * @return VkDescriptorBufferInfo for m_vkinstance at index
   // */
   //VkDescriptorBufferInfo buffer::descriptorInfoForIndex(int index) {
   //   return descriptorInfo(m_alignmentSize, index * m_alignmentSize);
   //}

   /**
    * Invalidate a memory range of the buffer to make it visible to the host
    *
    * @note Only required for non-coherent memory
    *
    * @param index Specifies the region to invalidate: index * alignmentSize
    *
    * @return HRESULT of the invalidate call
    */
   HRESULT buffer::invalidateIndex(int index) {
      //return invalidate(m_alignmentSize, index * m_alignmentSize);
      return E_FAIL;
   }

}  // namespace graphics3d_directx

