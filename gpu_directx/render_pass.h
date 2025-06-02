// From vk_swapchain by camilo on 2025-05-09 01:40 <3ThomasBorregaardSorensen!!
#pragma once


#include "context.h"

//// directx headers
//#include "_gpu_directx.h"



namespace gpu_directx
{


   class CLASS_DECL_GPU_DIRECTX render_pass :
      virtual public ::particle
   {
   public:


      //VkFormat m_formatImage;
      //VkFormat m_formatDepth;
      //VkExtent2D m_extent;

      //::array<VkFramebuffer> m_framebuffers;
      //VkRenderPass m_vkrenderpass;

      ::pointer < renderer >  m_pgpurenderer;

      //::array < VkSemaphore> m_semaphoreaSignalOnSubmit;
      //::array < VkSemaphore> m_semaphoreaWaitToSubmit;
      //::array<VkImage> depthImages;
      //::array<VkDeviceMemory> depthImageMemorys;
      //::array<VkImageView> depthImageViews;
      //::array<VkImage> m_images;
      //::array<VkImageView> m_imageviews;

      context* m_pgpucontext;
      //VkExtent2D windowExtent;

      //VkSwapchainKHR swapChain;
      ::pointer<render_pass> m_pvkcrenderpassOld;

      ::int_array imageAvailable;
      //::array<VkSemaphore> imageAvailableSemaphores;
      //::array<VkSemaphore> renderFinishedSemaphores;
      //::array<VkFence>     inFlightFences;
      //::array<VkFence>     imagesInFlight;
      bool                 m_bNeedRebuild;

      static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

      render_pass();
      ////render_pass(renderer* prenderer, VkExtent2D windowExtent);
      ////render_pass(renderer* prenderer, VkExtent2D windowExtent, ::pointer <render_pass>previous);
      ~render_pass();

      virtual void on_before_begin_render(frame* pframe);

      //VkFramebuffer getFrameBuffer(int index) { return m_framebuffers[index]; }
      //VkFramebuffer getCurrentFrameBuffer() { return m_framebuffers[get_image_index()]; }
      virtual int get_image_index() const;
      //VkRenderPass getRenderPass() { return m_vkrenderpass; }
      //VkImageView getImageView(int index) { return m_imageviews[index]; }
      //size_t imageCount() { return m_images.size(); }
      size_t imageCount() { return 0; }
      //VkFormat getImageFormat() { return m_formatImage; }
      //VkExtent2D getExtent() { return m_extent; }
      uint32_t width() { return 16; }
      uint32_t height() { return 9; }

      float extentAspectRatio() {
         return (float) width() / (float) height();
         //return static_cast<float>(m_extent.width) / static_cast<float>(m_extent.height);
      }
      //virtual VkFormat findDepthFormat();

      //virtual HRESULT acquireNextImage(uint32_t* imageIndex);
      //virtual HRESULT submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);
      virtual HRESULT acquireNextImage();
      // virtual HRESULT submitCommandBuffers(const VkCommandBuffer* buffers);

      //bool compareFormats(const render_pass& renderpass) const {
      //   return renderpass.m_formatDepth == m_formatDepth &&
      //      renderpass.m_formatImage == m_formatImage;
      //}


      virtual void init();
      virtual void createRenderPassImpl();
      virtual void createImageViews();
      virtual void createDepthResources();
      virtual void createRenderPass();
      virtual void createFramebuffers();
      virtual void createSyncObjects();


      //virtual void defer_layout();
      //// Helper functions
      //VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      //   const ::array<VkSurfaceFormatKHR>& availableFormats);
      //VkPresentModeKHR chooseSwapPresentMode(
      //   const ::array<VkPresentModeKHR>& availablePresentModes);
      //VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

   };


}  // namespace gpu_directx



