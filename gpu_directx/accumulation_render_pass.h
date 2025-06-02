// From offscreen_render_pass by
//    camilo on 2025-05-27 15:49 <3ThomasBorregaardSorensen!!
// From vk_swapchain by camilo on 2025-05-09 <3ThomasBorregaardSorensen!!
#pragma once


#include "render_pass.h"



namespace gpu_directx
{


	class CLASS_DECL_GPU_DIRECTX accumulation_render_pass :
		virtual public render_pass
	{
	public:


		//::array<VkImage>				m_imagesAlphaAccumulation;
		//::array<VkDeviceMemory>		m_imagememoriesAlphaAccumulation;
		//::array<VkImageView>			m_imageviewsAlphaAccumulation;
		//VkFormat							m_formatAlphaAccumulation;

		static constexpr int MAX_FRAMES_IN_FLIGHT = 2;


		accumulation_render_pass();
		//accumulation_render_pass(renderer* pgpurenderer, VkExtent2D windowExtent);
		//accumulation_render_pass(renderer* pgpurenderer, VkExtent2D windowExtent, ::pointer <render_pass>previous);
		~accumulation_render_pass();

		
		void on_before_begin_render(frame* pframe) override;


		//VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
		//VkRenderPass getRenderPass() { return m_vkrenderpass; }
		//VkImageView getImageView(int index) { return m_imageview[index]; }
		//size_t imageCount() { return m_images.size(); }
		//VkFormat getImageFormat() { return imageFormat; }
		//VkExtent2D getExtent() { return m_extent; }
		//uint32_t width() { return m_extent.width; }
		//uint32_t height() { return m_extent.height; }

		//float extentAspectRatio() {
		//	return static_cast<float>(m_extent.width) / static_cast<float>(m_extent.height);
		//}
		//VkFormat findDepthFormat();

		HRESULT acquireNextImage() override;
		//HRESULT submitCommandBuffers(const VkCommandBuffer* buffers);

		//bool compareSwapFormats(const offscreen_render_pass& swapChain) const {
		//	return swapChain.depthFormat == depthFormat &&
		//		swapChain.imageFormat == imageFormat;
		//}

	public:
		void init();
		void createRenderPassImpl();
		void createAlphaAccumulation();
		void createImageViews();
		void createDepthResources();
		void createRenderPass();
		void createFramebuffers();
		void createSyncObjects();

		// HRESULT submitSamplingWork(const VkCommandBuffer buffer, uint32_t* imageIndex);

		//// Helper functions
		//VkSurfaceFormatKHR chooseSwapSurfaceFormat(
		//	const ::array<VkSurfaceFormatKHR>& availableFormats);
		//VkPresentModeKHR chooseSwapPresentMode(
		//	const ::array<VkPresentModeKHR>& availablePresentModes);
		//VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		//VkFormat imageFormat;
		//VkFormat depthFormat;
		//VkExtent2D m_extent;

		//::array<VkFramebuffer> swapChainFramebuffers;
		//VkRenderPass m_vkrenderpass;

		////::array<VkImage> depthImages;
		////::array<VkDeviceMemory> depthImageMemorys;
		////::array<VkImageView> depthImageViews;
		//VkSampler m_vksampler;
		//::array<VkDeviceMemory> m_imagememories;
		////::array<VkImage> swapChainImages;
		////::array<VkImageView> swapChainImageViews;

		//context* m_pgpucontext;
		//VkExtent2D windowExtent;

		////VkSwapchainKHR swapChain;
		//::pointer<offscreen_render_pass> oldOffScreen;

		//::array<VkSemaphore> imageAvailableSemaphores;
		//::array<VkSemaphore> renderFinishedSemaphores;
		//::array<VkFence> inFlightFences;
		//::array<VkFence> imagesInFlight;
		//size_t currentFrame = 0;
	};


}  // namespace gpu_directx



