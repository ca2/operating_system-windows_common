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
      createDepthResources();
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


   void render_target_view::createDepthResources()
   {

      if (m_bDepthStencil)
      {

         D3D11_TEXTURE2D_DESC depthDesc = {};
         depthDesc.Width = m_size.cx();
         depthDesc.Height = m_size.cy();
         depthDesc.MipLevels = 1;
         depthDesc.ArraySize = 1;
         int MorePrecisionNoStencil = 0;
         if (MorePrecisionNoStencil)
         {
            depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
         }
         else
         {
            depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
         }
         depthDesc.SampleDesc.Count = 1;
         depthDesc.Usage = D3D11_USAGE_DEFAULT;
         depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
         ::cast < ::gpu_directx11::device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

         auto pdevice = pgpudevice->m_pdevice;

         HRESULT hrCreateTexture = pdevice->CreateTexture2D(&depthDesc, nullptr, &m_ptextureDepthStencil);

         if (FAILED(hrCreateTexture))
         {

            throw ::hresult_exception(hrCreateTexture);

         }
         D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

         if (MorePrecisionNoStencil)
         {

            dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
            dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
         }
         HRESULT hrCreateDepthStencilView = pdevice->CreateDepthStencilView(
            m_ptextureDepthStencil,
            MorePrecisionNoStencil ? &dsvDesc : nullptr, &m_pdepthstencilview);

         if (FAILED(hrCreateDepthStencilView))
         {

            throw ::hresult_exception(hrCreateDepthStencilView);

         }

         //ID3D11DepthStencilState* depthStencilState = nullptr;

         D3D11_DEPTH_STENCIL_DESC dsDesc = {};
         dsDesc.DepthEnable = TRUE;
         dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
         dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

         HRESULT hrCreateDepthStencilState = pdevice->CreateDepthStencilState(&dsDesc, &m_pdepthstencilstate);

         if (FAILED(hrCreateDepthStencilState))
         {

            throw ::hresult_exception(hrCreateDepthStencilState);

         }



         //VkFormat depthFormat = findDepthFormat();

         //m_formatDepth = depthFormat;

         //VkExtent2D extent = getExtent();

         //depthImages.resize(imageCount());
         //depthImageMemorys.resize(imageCount());
         //depthImageViews.resize(imageCount());

         //for (int i = 0; i < depthImages.size(); i++) 
         //{

         //   VkImageCreateInfo imageInfo{};
         //   imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
         //   imageInfo.imageType = VK_IMAGE_TYPE_2D;
         //   imageInfo.extent.width = extent.width;
         //   imageInfo.extent.height = extent.height;
         //   imageInfo.extent.depth = 1;
         //   imageInfo.mipLevels = 1;
         //   imageInfo.arrayLayers = 1;
         //   imageInfo.format = depthFormat;
         //   imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
         //   imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         //   imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
         //   imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
         //   imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
         //   imageInfo.flags = 0;

         //   m_pgpucontext->createImageWithInfo(
         //      imageInfo,
         //      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
         //      depthImages[i],
         //      depthImageMemorys[i]);

         //   VkImageViewCreateInfo viewInfo{};
         //   viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
         //   viewInfo.image = depthImages[i];
         //   viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
         //   viewInfo.format = depthFormat;
         //   viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
         //   viewInfo.subresourceRange.baseMipLevel = 0;
         //   viewInfo.subresourceRange.levelCount = 1;
         //   viewInfo.subresourceRange.baseArrayLayer = 0;
         //   viewInfo.subresourceRange.layerCount = 1;

         //   if (vkCreateImageView(m_pgpucontext->logicalDevice(), &viewInfo, nullptr, &depthImageViews[i]) != VK_SUCCESS) 
         //   {
         //      throw ::exception(error_failed,"failed to create texture image view!");
         //   }

         //}

      }

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



