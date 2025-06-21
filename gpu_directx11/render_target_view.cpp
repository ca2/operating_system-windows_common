// From vk_swapchain by camilo on 2025-05-09 02:01 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "physical_device.h"
#include "render_target_view.h"
#include "renderer.h"


namespace gpu_directx11
{

   
   render_target_view::render_target_view()
   {


   }


   render_target_view::~render_target_view()
   {
      

   }


   void render_target_view::on_init()
   {

      ::gpu::render_target::on_init();

      createRenderPassImpl();
      createImageViews();
      createRenderPass();
      //createDepthResources();
      createFramebuffers();
      createSyncObjects();

      m_prendertargetviewOld = nullptr;

   }


   void render_target_view::on_before_begin_render(frame* pframe)
   {


   }


   void render_target_view::initialize_render_target(::gpu::renderer* pgpurenderer, const ::int_size& size, ::pointer <::gpu::render_target>previous)
   {

      ::gpu::render_target::initialize_render_target(pgpurenderer, size, previous);

   }


   //int render_target_view::get_image_index() const
   //{
   //   
   //   return m_pgpurenderer->get_frame_index(); 
   //
   //}


   HRESULT render_target_view::acquireNextImage() 
   {

      return E_FAIL;

   }


   void render_target_view::createRenderPassImpl() 
   {

   }

   
   void render_target_view::createImageViews() 
   {


   }


   void render_target_view::createRenderPass() 
   {


   }


   void render_target_view::createFramebuffers() 
   {


   }




   void render_target_view::createSyncObjects() 
   {

      //imageAvailable.resize(MAX_FRAMES_IN_FLIGHT);
      //imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
      //renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
      //inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
      //imagesInFlight.resize(imageCount(), VK_NULL_HANDLE);


      //VkSemaphoreCreateInfo semaphoreInfo = {};
      //semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

      //VkFenceCreateInfo fenceInfo = {};
      //fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
      //fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

      //for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
      //{
      //   imageAvailable[i] = 0;
      //   if (vkCreateSemaphore(m_pgpucontext->logicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) !=
      //      VK_SUCCESS ||
      //      vkCreateSemaphore(m_pgpucontext->logicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) !=
      //      VK_SUCCESS 
      //      ||        vkCreateFence(m_pgpucontext->logicalDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS
      //       ) 
      //   {
      //      throw ::exception(error_failed,"failed to create synchronization objects for a frame!");
      //   }
      //}
   }

   
   //VkSurfaceFormatKHR render_target_view::chooseSwapSurfaceFormat(
   //   const ::array<VkSurfaceFormatKHR>& availableFormats) {
   //   for (const auto& availableFormat : availableFormats) {
   //      // SRGB can be changed to "UNORM" instead
   //      if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
   //         availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
   //         return availableFormat;
   //      }
   //   }

   //   return availableFormats[0];
   //}

   //VkPresentModeKHR render_target_view::chooseSwapPresentMode(
   //   const ::array<VkPresentModeKHR>& availablePresentModes) {
   //   for (const auto& availablePresentMode : availablePresentModes) {
   //      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
   //         debug() << "Present mode: Mailbox";
   //         return availablePresentMode;
   //      }
   //   }

   //   // for (const auto &availablePresentMode : availablePresentModes) {
   //   //   if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
   //   //     debug() << "Present mode: Immediate";
   //   //     return availablePresentMode;
   //   //   }
   //   // }

   //   debug() << "Present mode: V-Sync";
   //   return VK_PRESENT_MODE_FIFO_KHR;
   //}

   //VkExtent2D render_target_view::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
   //   if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
   //      return capabilities.currentExtent;
   //   }
   //   else {
   //      VkExtent2D actualExtent = windowExtent;
   //      actualExtent.width = std::max(
   //         capabilities.minImageExtent.width,
   //         std::min(capabilities.maxImageExtent.width, actualExtent.width));
   //      actualExtent.height = std::max(
   //         capabilities.minImageExtent.height,
   //         std::min(capabilities.maxImageExtent.height, actualExtent.height));

   //      return actualExtent;
   //   }
   //}

   //VkFormat render_target_view::findDepthFormat() 
   //{

   //   return m_pgpucontext->m_pgpudevice->m_pphysicaldevice->findSupportedFormat(
   //      { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
   //      VK_IMAGE_TILING_OPTIMAL,
   //      VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

   //}

} // namespace gpu_directx11



