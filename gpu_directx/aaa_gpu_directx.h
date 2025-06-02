// Created by camilo on 2023-04-27 17:06 <3ThomasBorregaardSorensen!!
#pragma once


#undef USUAL_OPERATING_SYSTEM_SUPPRESSIONS
#include "acme/_operating_system.h"


//#include <directx/directx.h>


namespace directx
{

//
//
//   /** @brief Returns an error code as a string */
//   CLASS_DECL_GPU_DIRECTX ::string errorString(HRESULT errorCode);
//
//   /** @brief Returns the device type as a string */
//   CLASS_DECL_GPU_DIRECTX ::string physicalDeviceTypeString(VkPhysicalDeviceType type);
//
//   // Selected a suitable supported depth format starting with 32 bit down to 16 bit
//   // Returns false if none of the depth formats in the list is supported by the device
//   CLASS_DECL_GPU_DIRECTX VkBool32 getSupportedDepthFormat(VkPhysicalDevice physicalDevice, VkFormat * depthFormat);
//   // Same as getSupportedDepthFormat but will only select formats that also have stencil
//   CLASS_DECL_GPU_DIRECTX VkBool32 getSupportedDepthStencilFormat(VkPhysicalDevice physicalDevice, VkFormat * depthStencilFormat);
//
//   // Returns true a given format support LINEAR filtering
//   CLASS_DECL_GPU_DIRECTX VkBool32 formatIsFilterable(VkPhysicalDevice physicalDevice, VkFormat format, VkImageTiling tiling);
//   // Returns true if a given format has a stencil part
//   CLASS_DECL_GPU_DIRECTX VkBool32 formatHasStencil(VkFormat format);
//
//   // Put an image memory barrier for setting an image layout on the sub resource into the given command buffer
//   CLASS_DECL_GPU_DIRECTX void setImageLayout(
//      VkCommandBuffer cmdbuffer,
//      VkImage image,
//      VkImageLayout oldImageLayout,
//      VkImageLayout newImageLayout,
//      VkImageSubresourceRange subresourceRange,
//      VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
//      VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
//   // Uses a fixed sub resource layout with first mip level and layer
//   CLASS_DECL_GPU_DIRECTX void setImageLayout(
//      VkCommandBuffer cmdbuffer,
//      VkImage image,
//      VkImageAspectFlags aspectMask,
//      VkImageLayout oldImageLayout,
//      VkImageLayout newImageLayout,
//      VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
//      VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
//
//   /** @brief Insert an image memory barrier into the command buffer */
//   CLASS_DECL_GPU_DIRECTX void insertImageMemoryBarrier(
//      VkCommandBuffer cmdbuffer,
//      VkImage image,
//      VkAccessFlags srcAccessMask,
//      VkAccessFlags dstAccessMask,
//      VkImageLayout oldImageLayout,
//      VkImageLayout newImageLayout,
//      VkPipelineStageFlags srcStageMask,
//      VkPipelineStageFlags dstStageMask,
//      VkImageSubresourceRange subresourceRange);
//
//   // Display error message and exit on fatal error
//   CLASS_DECL_GPU_DIRECTX void exitFatal(const ::string & message, int32_t exitCode);
//   CLASS_DECL_GPU_DIRECTX void exitFatal(const ::string & message, HRESULT resultCode);
//
////   // Load a SPIR-V shader (binary)
////#if defined(__ANDROID__)
////   VkShaderModule loadShader(AAssetManager * assetManager, const char * fileName, VkDevice device);
////#else
////   VkShaderModule loadShader(const char * fileName, VkDevice device);
////#endif
//
//   /** @brief Checks if a file exists */
//   CLASS_DECL_GPU_DIRECTX bool fileExists(const ::string & filename);
//
//   CLASS_DECL_GPU_DIRECTX uint32_t alignedSize(uint32_t value, uint32_t alignment);
//   CLASS_DECL_GPU_DIRECTX VkDeviceSize alignedVkSize(VkDeviceSize value, VkDeviceSize alignment);
//
//
//   //CLASS_DECL_GPU_DIRECTX ::array<VkVertexInputBindingDescription> _001GetVertexBindingDescriptions(const ::gpu::property* pproperties);
//   //CLASS_DECL_GPU_DIRECTX ::array<VkVertexInputAttributeDescription> _001GetVertexAttributeDescriptions(const ::gpu::property* pproperties);
//   CLASS_DECL_GPU_DIRECTX::array<VkVertexInputBindingDescription> _001GetVertexBindingDescriptions();
//   CLASS_DECL_GPU_DIRECTX::array<VkVertexInputAttributeDescription> _001GetVertexAttributeDescriptions();


} // namespace directx



