#include "framework.h"
#include "approach.h"
#include "descriptors.h"
#include "frame.h"
#include "renderer.h"
//#include "accumulation_render_target_view.h"
#include "offscreen_render_target_view.h"
#include "physical_device.h"
#include "swap_chain_render_target_view.h"
#include "initializers.h"
#include "aura/graphics/gpu/cpu_buffer.h"
#include "gpu_directx/shader.h"
#include "acme/parallelization/synchronous_lock.h"
#include "acme/platform/application.h"
#include "aura/graphics/image/target.h"
#include "aura/user/user/interaction.h"
#include "aura/windowing/window.h"
//#include "tools.h"
//#include "aura/user/user/graphics3d.h"

//#include <array>
//#include <cassert>
//#include <stdexcept>
//

using namespace directx;


namespace gpu_directx
{

   //// Create vertex and index buffers
   //void create_quad_buffers(VkDevice device, VkPhysicalDevice physicalDevice,
   //   VkBuffer* vertexBuffer, VkDeviceMemory* vertexMemory,
   //   VkBuffer* indexBuffer, VkDeviceMemory* indexMemory, bool bYSwap);


   // Fragment shader (GLSL -> SPIR-V):
   // layout(set = 0, binding = 0) uniform sampler2D srcImage;
   // layout(location = 0) in vec2 fragUV;
   // layout(location = 0) out vec4 outColor;
   // void main() {
   //     outColor = texture(srcImage, fragUV);
   // }
   static unsigned int g_uaImageBlendFragmentShader[] = {
 #include "shader/image_blend.frag.spv.inl"
   };
   // 
   // Vertex shader (GLSL -> SPIR-V):
   // layout(location = 0) in vec2 inPos;
   // layout(location = 1) in vec2 inUV;
   // layout(location = 0) out vec2 fragUV;
   // void main() {
   //     fragUV = inUV;
   //     gl_Position = vec4(inPos, 0.0, 1.0);
   // }
   static unsigned int g_uaImageBlendVertexShader[] = {
 #include "shader/image_blend.vert.spv.inl"
   };


   static unsigned int g_uaResolveFragmentShader[] = {
 #include "shader/resolve.frag.spv.inl"
   };


   // renderer::renderer(VkWindow& window, context* pvkcdevice) : vkcWindow{ window }, m_pgpucontext{ pvkcdevice } 
   renderer::renderer()
   {
      m_hlsClear.m_dL = 0.75;
      m_hlsClear.m_dS = 0.5;
   }


   //int renderer::width()
   //{

   //   return m_prendertargetview->width();

   //}

   //int renderer::height()
   //{

   //   return m_prendertargetview->height();

   //}


   void renderer::initialize_renderer(::gpu::context* pgpucontext, ::gpu::enum_output eoutput, ::gpu::enum_scene escene)
   {

      ::gpu::renderer::initialize_renderer(pgpucontext, eoutput, escene);

      m_pgpucontext = pgpucontext;

      if (eoutput == ::gpu::e_output_cpu_buffer)
      {

         //m_pimpact = pgpucontext->m_pimpact;

         pgpucontext->create_cpu_buffer(pgpucontext->rectangle().size());

         __construct_new(m_pcpubuffersampler);

         m_pcpubuffersampler->initialize_cpu_buffer_sampler(pgpucontext);

         m_pcpubuffersampler->m_prenderer = this;

      }

      //m_poffscreensampler->set_storing_flag

      //defer_layout();

      createCommandBuffers();

   }


   renderer::~renderer()
   {

      freeCommandBuffers();

   }


   int renderer::get_frame_index() const
   {

      assert(m_iFrameSerial2 >= 0
         && m_iCurrentFrame2 >= 0
         && m_estate != e_state_initial
         && "Cannot get frame index when frame not in progress");

      return (int)m_iCurrentFrame2;

   }


   void renderer::on_new_frame()
   {

      m_iFrameSerial2++;

      m_iCurrentFrame2 = (m_iCurrentFrame2 + 1) % render_target_view::MAX_FRAMES_IN_FLIGHT;

      on_happening(e_happening_new_frame);

   }


   void renderer::restart_frame_counter()
   {

      m_iCurrentFrame2 = -1;
      m_iFrameSerial2 = -1;

      on_happening(e_happening_reset_frame_counter);

   }



   int renderer::get_frame_count() const
   {

      return ::gpu_directx::render_target_view::MAX_FRAMES_IN_FLIGHT;

   }


   void renderer::on_context_resize()
   {

      //m_pgpucontext->m_rectangle = rectanglePlacement.size();

      //::gpu::renderer::set_placement(rectanglePlacement);

      //auto size = m_pimpact->size();

      defer_update_renderer();

      //auto extent = vkcWindow.getExtent();
      //while (extent.width == 0 || extent.height == 0) {
      //	glfwWaitEvents();
      //}
      //vkDeviceWaitIdle(m_pgpucontext->logicalDevice());

      //if (vkcSwapChain == nullptr) {
      //	vkcSwapChain = std::make_unique<swap_chain_render_target_view>(m_pgpucontext, extent);
      //}
      //else {
      //	::pointer<swap_chain_render_target_view> oldSwapChain = std::move(vkcSwapChain);
      //	vkcSwapChain = std::make_unique<swap_chain_render_target_view>(m_pgpucontext, extent, oldSwapChain);
      //	if (!oldSwapChain->compareSwapFormats(*vkcSwapChain.get())) {
      //		throw ::exception(error_failed, "Swap chain image(or depth) format has changed!");
      //	}

      //}
   }


   void renderer::defer_update_renderer()
   {

      if (m_sizeRenderer.width() == m_pgpucontext->rectangle().width()
         && m_sizeRenderer.height() == m_pgpucontext->rectangle().height())
      {

         return;

      }

      m_bNeedToRecreateSwapChain = true;

      auto size = m_pgpucontext->rectangle().size();

      m_sizeRenderer = size;

      auto eoutput = m_eoutput;

      if (eoutput == ::gpu::e_output_cpu_buffer)
      {

         auto poffscreenrendertargetview = __allocate offscreen_render_target_view(this, size, m_prendertargetview);
//#ifdef WINDOWS_DESKTOP
//         poffscreenrendertargetview->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
//#else
//         poffscreenrendertargetview->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
//#endif
         m_prendertargetview = poffscreenrendertargetview;
//         //m_prendererResolve.release();
//
      }
      else if (eoutput == ::gpu::e_output_swap_chain)
      {

         m_prendertargetview = __allocate swap_chain_render_target_view(this, size, m_prendertargetview);
         //m_prendererResolve.release();

      }
//      else if (eoutput == ::gpu::e_output_gpu_buffer)
//      {
//
//         auto poffscreenrendertargetview = __allocate offscreen_render_target_view(this, m_extentRenderer, m_prendertargetview);
//#ifdef WINDOWS_DESKTOP
//         poffscreenrendertargetview->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
//#else
//         poffscreenrendertargetview->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
//#endif
//         m_prendertargetview = poffscreenrendertargetview;
//         //m_prendererResolve;
//
//      }
//      else if (eoutput == ::gpu::e_output_color_and_alpha_accumulation_buffers)
//      {
//
//         auto paccumulationrendertargetview = __allocate accumulation_render_target_view(this, m_extentRenderer, m_prendertargetview);
//         paccumulationrendertargetview->m_formatImage = VK_FORMAT_R32G32B32A32_SFLOAT;
//         paccumulationrendertargetview->m_formatAlphaAccumulation = VK_FORMAT_R32_SFLOAT;
//         m_prendertargetview = paccumulationrendertargetview;
//
//         //__construct_new(m_prendererResolve);
//
//         //m_prendererResolve->initialize_renderer(m_pgpucontext, ::gpu::e_output_resolve_color_and_alpha_accumulation_buffers);
//
//         //m_prendererResolve->set_placement(m_pgpucontext->rectangle);
//         //
//         //            auto poffscreenrendertargetview = __allocate offscreen_render_target_view(m_pgpucontext, m_extentRenderer, m_prendertargetviewResolve);
//         //#ifdef WINDOWS_DESKTOP
//         //            poffscreenrendertargetview->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
//         //#else
//         //            poffscreenrendertargetview->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
//         //#endif
//         //            m_prendertargetviewResolve = poffscreenrendertargetview;
//      }
//      else if (eoutput == ::gpu::e_output_resolve_color_and_alpha_accumulation_buffers)
//      {
//
//         auto poffscreenrendertargetview = __allocate offscreen_render_target_view(this, m_extentRenderer, m_prendertargetview);
//#ifdef WINDOWS_DESKTOP
//         poffscreenrendertargetview->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
//#else
//         poffscreenrendertargetview->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
//#endif
//         m_prendertargetview = poffscreenrendertargetview;
//
//      }
//      else
//      {
//
//         throw ::exception(error_wrong_state, "Unexpected gpu e_output");
//
//      }
//
      if (!m_prendertargetview->has_ok_flag() && m_sizeRenderer.area() > 0)
      {

         m_prendertargetview->init();

      }

      //if (m_prendererResolve)
      //{

      //	if (m_prendererResolve->m_prendertargetview->m_images.is_empty())
      //	{

      //		m_prendererResolve->defer_update_render_target_view();

      //	}

      //}

      m_bNeedToRecreateSwapChain = false;

   }


   void renderer::createCommandBuffers()
   {

      //commandBuffers.resize(render_target_view::MAX_FRAMES_IN_FLIGHT);

      //VkCommandBufferAllocateInfo allocInfo{};
      //allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      //allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      //allocInfo.commandPool = m_pgpucontext->m_pgpudevice->getCommandPool();
      //allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

      //if (vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &allocInfo, commandBuffers.data()) !=
      //   VK_SUCCESS) {
      //   throw ::exception(error_failed, "failed to allocate command buffers!");
      //}

   }


   void renderer::freeCommandBuffers()
   {

      //vkFreeCommandBuffers(
      //   m_pgpucontext->logicalDevice(),
      //   m_pgpucontext->m_pgpudevice->getCommandPool(),
      //   (uint32_t)commandBuffers.size(),
      //   commandBuffers.data());
      //commandBuffers.clear();

   }


   void renderer::on_begin_draw()
   {

      if (m_pgpucontext->m_rectangle.is_empty())
      {

         throw ::exception(error_wrong_state, "please call set size before at least once with no empty preferrably good initial size");

         //set_size(size);

      }

      assert(!isFrameStarted && "Can't call beginFrame while already in progress");

      //if (m_bOffScreen)
      {

         auto result = m_prendertargetview->acquireNextImage();

         //if (result == VK_ERROR_OUT_OF_DATE_KHR
         //   || m_prendertargetview->m_bNeedRebuild)
         //{
         //   vkDeviceWaitIdle(m_pgpucontext->logicalDevice());
         //   m_prendertargetview->init();
         //   //set_placement(size);
         //   //throw ::exception(todo, "resize?!?!");
         //   //return nullptr;
         //   return;
         //}
         //if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
         //   throw ::exception(error_failed, "Failed to aquire swap chain image");
         //}

         isFrameStarted = true;

         //auto commandBuffer = getCurrentCommandBuffer();

         //VkCommandBufferBeginInfo beginInfo{};
         //beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

         //if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
         //   throw ::exception(error_failed, "failed to begin recording command buffer!");
         //}

         //m_
         //auto pframe = __create_new < frame >();
         //pframe->commandBuffer = commandBuffer;
         //m_pframe = pframe;
         //return m_pframe;

      }
      //else
      //{


      //	auto result = m_pvkcswapchain->acquireNextImage(&currentImageIndex);

      //	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      //		recreateRenderPass();
      //		return nullptr;
      //	}
      //	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      //		throw ::exception(error_failed, "Failed to aquire swap chain image");
      //	}

      //	isFrameStarted = true;

      //	auto commandBuffer = getCurrentCommandBuffer();

      //	VkCommandBufferBeginInfo beginInfo{};
      //	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      //	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
      //		throw ::exception(error_failed, "failed to begin recording command buffer!");
      //	}
      //	return commandBuffer;

      //}

      _on_begin_render();

   }


   renderer::cpu_buffer_sampler::cpu_buffer_sampler()
   {

      clear();

   }


   renderer::cpu_buffer_sampler::~cpu_buffer_sampler()
   {

      destroy();

   }


   void renderer::cpu_buffer_sampler::initialize_cpu_buffer_sampler(::gpu::context* pgpucontext)
   {

      m_pgpucontext = pgpucontext;

   }


   void renderer::cpu_buffer_sampler::clear()
   {

      //m_vkextent2d.width = 0;
      //m_vkextent2d.height = 0;
      //m_vkdevicememory = nullptr;
      //m_vkimage = nullptr;

   }


   //void renderer::cpu_buffer_sampler::update(VkExtent2D vkextent2d)
   //{

   //   //if (vkextent2d.width == m_vkextent2d.width
   //   //   && vkextent2d.height == m_vkextent2d.height)
   //   //{

   //   //   return;

   //   //}

   //   //destroy();

   //   //if (vkextent2d.width <= 0 ||
   //   //   vkextent2d.height <= 0)
   //   //{

   //   //   return;

   //   //}

   //   //m_vkextent2d.width = vkextent2d.width;
   //   //m_vkextent2d.height = vkextent2d.height;

   //   //VkImageCreateInfo imgCreateInfo(initializers::imageCreateInfo());
   //   //imgCreateInfo.imageType = VK_IMAGE_TYPE_2D;
   //   //imgCreateInfo.format = VK_FORMAT_B8G8R8A8_UNORM;
   //   //imgCreateInfo.extent.width = m_vkextent2d.width;
   //   //imgCreateInfo.extent.height = m_vkextent2d.height;
   //   //imgCreateInfo.extent.depth = 1;
   //   //imgCreateInfo.arrayLayers = 1;
   //   //imgCreateInfo.mipLevels = 1;
   //   //imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
   //   //imgCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
   //   //imgCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
   //   //imgCreateInfo.usage =
   //   //   VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
   //   //   VK_IMAGE_USAGE_SAMPLED_BIT;
   //   //// Create the image
   //   ////VkImage dstImage;
   //   //VK_CHECK_RESULT(vkCreateImage(m_pgpucontext->logicalDevice(), &imgCreateInfo, nullptr, &m_vkimage));
   //   //// Create memory to back up the image
   //   //VkMemoryRequirements memRequirements;
   //   //VkMemoryAllocateInfo memAllocInfo(initializers::memoryAllocateInfo());
   //   //vkGetImageMemoryRequirements(m_pgpucontext->logicalDevice(), m_vkimage, &memRequirements);
   //   //memAllocInfo.allocationSize = memRequirements.size;
   //   //// Memory must be host visible to copy from
   //   //memAllocInfo.memoryTypeIndex = m_pgpucontext->m_pgpudevice->m_pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
   //   //VK_CHECK_RESULT(vkAllocateMemory(m_pgpucontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
   //   //VK_CHECK_RESULT(vkBindImageMemory(m_pgpucontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));

   //   //if (1)
   //   //{
   //   //   auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();

   //   //   VkImageMemoryBarrier barrier = {
   //   //      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //   //      .srcAccessMask = 0,
   //   //      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //   //      .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
   //   //      .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //   //      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      .image = m_vkimage,
   //   //      .subresourceRange = {
   //   //         .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //   //         .baseMipLevel = 0,
   //   //         .levelCount = 1,
   //   //         .baseArrayLayer = 0,
   //   //         .layerCount = 1
   //   //      },
   //   //   };

   //   //   vkCmdPipelineBarrier(
   //   //      cmdBuffer,
   //   //      VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
   //   //      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
   //   //      0,
   //   //      0, NULL,
   //   //      0, NULL,
   //   //      1, &barrier
   //   //   );
   //   //   m_pgpucontext->endSingleTimeCommands(cmdBuffer);
   //   //}


   //}


   void renderer::cpu_buffer_sampler::destroy()
   {

      //if (m_vkdevicememory)
      //{

      //   vkFreeMemory(m_pgpucontext->logicalDevice(), m_vkdevicememory, nullptr);
      //   vkDestroyImage(m_pgpucontext->logicalDevice(), m_vkimage, nullptr);

      //   clear();

      //}

   }


   void renderer::cpu_buffer_sampler::sample(ID3D11Texture2D * ptexture)
   {

      D3D11_TEXTURE2D_DESC desc;

      ptexture->GetDesc(&desc);

      ::cast < ::gpu_directx::context > pcontext = m_pgpucontext;

      if (desc.Width != m_size.width() || desc.Height != m_size.height())
      {

         m_ptextureStaging.Release();

         //m_pimagetarget->m_imagebuffer.set_storing_fla
         //if (outWidth) *outWidth = desc.Width;
         //if (outHeight) *outHeight = desc.Height;

         if (desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM) {
            printf("Unsupported format for readback.\n");
            throw ::exception(error_wrong_state);
         }

         D3D11_TEXTURE2D_DESC stagingDesc = desc;
         stagingDesc.BindFlags = 0;
         stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
         stagingDesc.Usage = D3D11_USAGE_STAGING;
         stagingDesc.MiscFlags = 0;

         ::cast < ::gpu_directx::device > pdevice = pcontext->m_pgpudevice;

         if (FAILED(pdevice->m_pdevice->CreateTexture2D(&stagingDesc, NULL, &m_ptextureStaging)))
         {
            printf("Failed to create staging texture.\n");
            throw ::exception(error_wrong_state);
         }

         m_size.cx() = desc.Width;
         m_size.cy() = desc.Height;


      }

      pcontext->m_pcontext->Flush();

      pcontext->m_pcontext->CopyResource((ID3D11Resource*)m_ptextureStaging, (ID3D11Resource*)ptexture);


      //stagingTexture->lpVtbl->Release(stagingTexture);

      //return rgbaData;

  //    if (!m_vkimage || !m_vkdevicememory)
  //    {

  //       return;

  //    }


  //    // Create the linear tiled destination image to copy to and to read the memory from

  //// Do the actual blit from the offscreen image to our host visible destination image
  //    VkCommandBufferAllocateInfo cmdBufAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->m_pgpudevice->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
  //    VkCommandBuffer copyCmd;
  //    VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &cmdBufAllocateInfo, &copyCmd));
  //    VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
  //    VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));

  //    // Transition source image to transfer destination layout
  //    insertImageMemoryBarrier(
  //       copyCmd,
  //       vkimage,
  //       0,
  //       VK_ACCESS_TRANSFER_WRITE_BIT,
  //       VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  //       VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
  //       VK_PIPELINE_STAGE_TRANSFER_BIT,
  //       VK_PIPELINE_STAGE_TRANSFER_BIT,
  //       VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

  //    // Transition destination image to transfer destination layout
  //    insertImageMemoryBarrier(
  //       copyCmd,
  //       m_vkimage,
  //       0,
  //       VK_ACCESS_TRANSFER_WRITE_BIT,
  //       VK_IMAGE_LAYOUT_UNDEFINED,
  //       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
  //       VK_PIPELINE_STAGE_TRANSFER_BIT,
  //       VK_PIPELINE_STAGE_TRANSFER_BIT,
  //       VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

  //    // colorAttachment.image is already in VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, and does not need to be transitioned

  //    VkImageCopy imageCopyRegion{};
  //    imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  //    imageCopyRegion.srcSubresource.layerCount = 1;
  //    imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  //    imageCopyRegion.dstSubresource.layerCount = 1;
  //    imageCopyRegion.extent.width = m_vkextent2d.width;
  //    imageCopyRegion.extent.height = m_vkextent2d.height;
  //    imageCopyRegion.extent.depth = 1;

  //    vkCmdCopyImage(
  //       copyCmd,
  //       //colorAttachment.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
  //       vkimage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
  //       m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
  //       1,
  //       &imageCopyRegion);

  //    // Transition destination image to general layout, which is the required layout for mapping the image memory later on
  //    insertImageMemoryBarrier(
  //       copyCmd,
  //       m_vkimage,
  //       VK_ACCESS_TRANSFER_WRITE_BIT,
  //       VK_ACCESS_MEMORY_READ_BIT,
  //       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
  //       VK_IMAGE_LAYOUT_GENERAL,
  //       VK_PIPELINE_STAGE_TRANSFER_BIT,
  //       VK_PIPELINE_STAGE_TRANSFER_BIT,
  //       VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

  //    VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));

  //    ::cast < offscreen_render_target_view > ppass = m_prenderer->m_prendertargetview;

  //    ppass->submitSamplingWork(copyCmd);

  //    vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

  //    vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_pgpudevice->getCommandPool(), 1, &copyCmd);


   }


   void renderer::cpu_buffer_sampler::send_sample()
   {

      D3D11_MAPPED_SUBRESOURCE mapped;

      ::cast < ::gpu_directx::context > pcontext = m_pgpucontext;

      D3D11_TEXTURE2D_DESC desc;

      m_ptextureStaging->GetDesc(&desc);

      if (FAILED(pcontext->m_pcontext->Map((ID3D11Resource*)m_ptextureStaging, 0, D3D11_MAP_READ, 0, &mapped)))
      {
         
         warning() << "Failed to map staging texture.";
         
         throw ::exception(error_wrong_state);

      }

      int width = desc.Width;
      int height = desc.Height;
      UINT rowPitch = mapped.RowPitch;
      auto data = mapped.pData;

      //m_pimagetarget->m_pimage->create({ width, height });

      // Allocate RGBA buffer (contiguous memory)
      //auto lock = m_pimagetarget->no_padded_lock(::image::e_copy_disposition_none);

      //if (!lock.data()) {
      //   context->Unmap((ID3D11Resource*)stagingTexture, 0);
      //   throw ::exception(error_wrong_state);
      //}

      auto pgpucontext = m_pgpucontext;

      auto pcpubuffer = pgpucontext->m_pcpubuffer;

      pcpubuffer->set_pixels(
         data,
         width,
         height,
         (int)rowPitch,
         //false);
         false);

      //// Copy row by row
      //for (UINT y = 0; y < height; ++y) {
      //   memcpy(lock.data() + y * width, (unsigned char*)mapped.pData + y * rowPitch, width * 4);
      //}

      pcontext->m_pcontext->Unmap((ID3D11Resource*)m_ptextureStaging, 0);
   //   if (!m_vkimage || !m_vkdevicememory)
   //   {

   //      return;

   //   }

   //   //auto pimpact = m_pgpucontext->m_pimpact;

   //   //auto callback = pimpact->m_callbackImage32CpuBuffer;

   //   //auto callback = m_prenderer->m_callbackImage32CpuBuffer;

   //   // Get layout of the image (including row pitch)
   //   VkImageSubresource subResource{};
   //   subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   //   VkSubresourceLayout subResourceLayout;

   //   vkGetImageSubresourceLayout(m_pgpucontext->logicalDevice(), m_vkimage, &subResource, &subResourceLayout);

   //   const char* imagedata = nullptr;
   //   // Map image memory so we can start copying from it
   //   vkMapMemory(m_pgpucontext->logicalDevice(), m_vkdevicememory, 0, VK_WHOLE_SIZE, 0, (void**)&imagedata);
   //   imagedata += subResourceLayout.offset;

   //   /*
   //      Save host visible framebuffer image to disk (ppm format)
   //   */

   //   //::memory mem;

   //   //mem.set_size(m_width * m_height * 4);

   //   //::array<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
   //   //const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());
   //   //{

      //auto pgpucontext = m_pgpucontext;

      //auto pcpubuffer = pgpucontext->m_pcpubuffer;

      //pcpubuffer->set_pixels(
      //   imagedata,
      //   m_vkextent2d.width,
      //   m_vkextent2d.height,
      //   (int)subResourceLayout.rowPitch);

   //   //_synchronous_lock synchronouslock(m_pgpucontext->m_pmutexOffscreen);
   //   //   m_pgpucontext->m_sizeOffscreen.cx() = m_vkextent2d.width;
   //   //m_pgpucontext->m_sizeOffscreen.cy() = m_vkextent2d.height;
   //   //m_pgpucontext->m_iScanOffscreen = subResourceLayout.rowPitch;
   //   //auto area = m_pgpucontext->m_iScanOffscreen * m_pgpucontext->m_sizeOffscreen.cy();
   //   //m_pgpucontext->m_memoryOffscreen.set_size(area);
   //   //m_pgpucontext->m_memoryOffscreen.assign(imagedata, area);
   //   //callback((void *)imagedata,
   //     // m_vkextent2d.width,
   //      //m_vkextent2d.height,
   //      //subResourceLayout.rowPitch);

   ////}
   ////else
   ////{


   ////}


   //   vkUnmapMemory(m_pgpucontext->logicalDevice(), m_vkdevicememory);

   }


   void renderer::sample()
   {

      //::cast < ::gpu_directx::offscreen_render_target_view > ptargetview = m_prendertargetview;

      //__defer_construct(m_pgpucontext->m_pcpubuffer);

      //m_pgpucontext->m_pcpubuffer->gpu_read();

      ///auto& memory = m_pimagetarget->m_imagebuffer.m_memory;
      ::cast< context > pgpucontext = m_pgpucontext;
      ::cast< renderer > prenderer = pgpucontext->m_pgpurenderer;
      auto prendertargetview = prenderer->m_prendertargetview;
      ::cast < offscreen_render_target_view > poffscreenrendertargetview = prendertargetview;
      ::cast< device > pgpudevice = pgpucontext->m_pgpudevice;
      ID3D11Device* device = pgpudevice->m_pdevice;
      ID3D11DeviceContext* context = pgpucontext->m_pcontext;
      ID3D11Texture2D* offscreenTexture = poffscreenrendertargetview->m_ptextureOffscreen;
      if (!device || !context || !offscreenTexture)
      {
         throw ::exception(error_wrong_state);
      }


      m_pcpubuffersampler->sample(offscreenTexture);

      //auto callback = m_callbackImage32CpuBuffer;

  //      if (callback)
      //{

      //   m_pcpubuffersampler->update(m_prendertargetview->getExtent());
      //   /*
      //      Copy framebuffer image to host visible image
      //   */
      //   /*const char* imagedata;*/
      //   {

      //      m_pcpubuffersampler->sample(m_prendertargetview->m_images[get_frame_index()]);

      //      //// Create the linear tiled destination image to copy to and to read the memory from

      //      //// Do the actual blit from the offscreen image to our host visible destination image
      //      //VkCommandBufferAllocateInfo cmdBufAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
      //      //VkCommandBuffer copyCmd;
      //      //VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &cmdBufAllocateInfo, &copyCmd));
      //      //VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
      //      //VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));

      //      //// Transition destination image to transfer destination layout
      //      //::graphics3d_directx::tools::insertImageMemoryBarrier(
      //      //	copyCmd,
      //      //	m_poffscreensampler->m_vkimage,
      //      //	0,
      //      //	VK_ACCESS_TRANSFER_WRITE_BIT,
      //      //	VK_IMAGE_LAYOUT_UNDEFINED,
      //      //	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //      //	VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      //	VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      //	VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      //      //// colorAttachment.image is already in VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, and does not need to be transitioned

      //      //VkImageCopy imageCopyRegion{};
      //      //imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      //      //imageCopyRegion.srcSubresource.layerCount = 1;
      //      //imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      //      //imageCopyRegion.dstSubresource.layerCount = 1;
      //      //imageCopyRegion.extent.width = m_prendertargetview->width();
      //      //imageCopyRegion.extent.height = m_prendertargetview->height();
      //      //imageCopyRegion.extent.depth = 1;

      //      //vkCmdCopyImage(
      //      //	copyCmd,
      //      //	//colorAttachment.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      //      //	m_prendertargetview->m_images[iIndex], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      //      //	dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //      //	1,
      //      //	&imageCopyRegion);

      //      //// Transition destination image to general layout, which is the required layout for mapping the image memory later on
      //      //::graphics3d_directx::tools::insertImageMemoryBarrier(
      //      //	copyCmd,
      //      //	dstImage,
      //      //	VK_ACCESS_TRANSFER_WRITE_BIT,
      //      //	VK_ACCESS_MEMORY_READ_BIT,
      //      //	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //      //	VK_IMAGE_LAYOUT_GENERAL,
      //      //	VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      //	VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      //	VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      //      //VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));

      //      //submitWork(copyCmd, queue);

      //      //submitInfo.commandBufferCount = 1;
      //      //submitInfo.pCommandBuffers = &copyCmd;
      //      //VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));


      //      //// Get layout of the image (including row pitch)
      //      //VkImageSubresource subResource{};
      //      //subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      //      //VkSubresourceLayout subResourceLayout;

      //      //vkGetImageSubresourceLayout(m_pgpucontext->logicalDevice(), dstImage, &subResource, &subResourceLayout);

      //      //// Map image memory so we can start copying from it
      //      //vkMapMemory(m_pgpucontext->logicalDevice(), dstImageMemory, 0, VK_WHOLE_SIZE, 0, (void**)&imagedata);
      //      //imagedata += subResourceLayout.offset;

            m_pcpubuffersampler->send_sample();

      //      ///*
      //      //	Save host visible framebuffer image to disk (ppm format)
      //      //*/

      //      ////::memory mem;

      //      ////mem.set_size(m_width * m_height * 4);

      //      ////::array<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
      //      ////const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());
      //      //if (callback)
      //      //{
      //      //	callback((void*)imagedata, 
      //      //		m_prendertargetview->width(),
      //      //		m_prendertargetview->height(),
      //      //		subResourceLayout.rowPitch);

      //      //}

      //      //// ppm binary pixel data
      //      //for (int32_t y = 0; y < m_height; y++) {
      //      //   unsigned int * row = (unsigned int *)imagedata;
      //      //   for (int32_t x = 0; x < m_width; x++) {
      //      //      //               if (colorSwizzle) {
      //      //         file.write((char *)row + 2, 1);
      //      //         file.write((char *)row + 1, 1);
      //      //         file.write((char *)row, 1);
      //      //      }
      //      //      //else {
      //      //      //   file.write((char *)row, 3);
      //      //      //}
      //      //      row++;
      //      //   }
      //      //   imagedata += subResourceLayout.rowPitch;
      //      //}


      //  //         callback

      //  //#if defined (VK_USE_PLATFORM_ANDROID_KHR)
      //  //         const char * filename = strcat(getenv("EXTERNAL_STORAGE"_ansi), "/headless.ppm"_ansi);
      //  //#else
      //  //         const char * filename = "headless.ppm"_ansi;
      //  //#endif
      //  //         std::ofstream file(filename, std::ios::out | std::ios::binary);
      //  //
      //  //         // ppm header
      //  //         file << "P6\n"_ansi << m_width << "\n"_ansi << m_height << "\n"_ansi << 255 << "\n"_ansi;

      //         //// If source is BGR (destination is always RGB) and we can't use blit (which does automatic conversion), we'hi have to manually swizzle color components
      //         //// Check if source is BGR and needs swizzle
      //         //::array<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
      //         //const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());

      //         //// ppm binary pixel data
      //         //for (int32_t y = 0; y < m_height; y++) {
      //         //   unsigned int * row = (unsigned int *)imagedata;
      //         //   for (int32_t x = 0; x < m_width; x++) {
      //         //      if (colorSwizzle) {
      //         //         file.write((char *)row + 2, 1);
      //         //         file.write((char *)row + 1, 1);
      //         //         file.write((char *)row, 1);
      //         //      }
      //         //      else {
      //         //         file.write((char *)row, 3);
      //         //      }
      //         //      row++;
      //         //   }
      //         //   imagedata += subResourceLayout.rowPitch;
      //         //}
      //         //file.close();

      //         //LOG("Framebuffer image saved to %s\n"_ansi, filename);

      //         // Clean up resources
      //   }


      //}
      //vkQueueWaitIdle(m_pgpucontext->graphicsQueue());


   }


   //	void renderer::resolve_color_and_alpha_accumulation_buffers()
   //	{
   //
   //		auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();
   //
   //		::cast < accumulation_render_target_view > ppass = m_prendertargetview;
   //
   //		auto iPassCurrentFrame = get_frame_index();
   //
   //		auto image = ppass->m_images[iPassCurrentFrame];
   //
   //		insertImageMemoryBarrier(cmdBuffer,
   //			image,
   //			0,
   //			VK_ACCESS_TRANSFER_WRITE_BIT,
   //			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //			VK_PIPELINE_STAGE_TRANSFER_BIT,
   //			VK_PIPELINE_STAGE_TRANSFER_BIT,
   //			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
   //
   //		auto imageAlphaAccumulation = ppass->m_imagesAlphaAccumulation[iPassCurrentFrame];
   //
   //		insertImageMemoryBarrier(cmdBuffer,
   //			imageAlphaAccumulation,
   //			0,
   //			VK_ACCESS_TRANSFER_WRITE_BIT,
   //			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //			VK_PIPELINE_STAGE_TRANSFER_BIT,
   //			VK_PIPELINE_STAGE_TRANSFER_BIT,
   //			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
   //
   //
   //		VkSubmitInfo submitInfo{};
   //		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
   //		submitInfo.commandBufferCount = 1;
   //		submitInfo.pCommandBuffers = &cmdBuffer;
   //		::array<VkSemaphore> waitSemaphores;
   //		::array<VkPipelineStageFlags> waitStages;
   //		waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
   //		waitSemaphores.add(m_prendertargetview->renderFinishedSemaphores[iPassCurrentFrame]);
   //		submitInfo.waitSemaphoreCount = waitSemaphores.size();
   //		submitInfo.pWaitSemaphores = waitSemaphores.data();
   //		submitInfo.pWaitDstStageMask = waitStages.data();
   //		m_pgpucontext->endSingleTimeCommands(cmdBuffer, 1, &submitInfo);
   //
   //		//m_prendererResolve->m_prendertargetview->m_semaphoreaWaitToSubmit.add(
   //		//   m_prendertargetview->renderFinishedSemaphores[iPassCurrentFrame]
   //		//);
   ////
   //		//m_prendererResolve->_resolve_color_and_alpha_accumulation_buffers();
   //
   //	}


      //void renderer::_resolve_color_and_alpha_accumulation_buffers()
      //{

      //	on_new_frame();

      //	auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();

      //	auto iFrameIndex1 = get_frame_index();

      //	VkImage image1 = m_prendertargetview->m_images[iFrameIndex1];

      //	if (is_starting_frame())
      //	{

      //		insertImageMemoryBarrier(cmdBuffer,
      //			image1,
      //			0,
      //			VK_ACCESS_TRANSFER_WRITE_BIT,
      //			VK_IMAGE_LAYOUT_UNDEFINED,
      //			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //			VK_PIPELINE_STAGE_TRANSFER_BIT,
      //			VK_PIPELINE_STAGE_TRANSFER_BIT,
      //			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
      //	}
      //	else
      //	{

      //		insertImageMemoryBarrier(cmdBuffer,
      //			image1,
      //			0,
      //			VK_ACCESS_TRANSFER_WRITE_BIT,
      //			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //			VK_PIPELINE_STAGE_TRANSFER_BIT,
      //			VK_PIPELINE_STAGE_TRANSFER_BIT,
      //			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
      //	}

      //	m_pgpucontext->endSingleTimeCommands(cmdBuffer);

      //	auto pframe = beginFrame();

      //	on_begin_render(pframe);

      //	// Resolve (Color and Alpha Accumulation Buffers) descriptors
      //	if (!m_psetdescriptorlayoutResolve)
      //	{

      //		m_psetdescriptorlayoutResolve = ::gpu_directx::set_descriptor_layout::Builder(m_pgpucontext)
      //			.addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //			.addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //			.build();

      //		int iFrameCount = get_frame_count();

      //		auto pdescriptorpoolbuilder = __allocate::gpu_directx::descriptor_pool::Builder();

      //		pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
      //		pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
      //		pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

      //		m_pdescriptorpoolResolve = pdescriptorpoolbuilder->build();

      //	}

      //	if (!m_pshaderResolve)
      //	{

      //		auto pshadervertexinput = __allocate  shader_vertex_input();

      //		pshadervertexinput->m_bindings.add(
      //			{
      //			   .binding = 0,
      //			   .stride = sizeof(float) * 4,
      //			   .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
      //			});

      //		pshadervertexinput->m_attribs.add({ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 0 });
      //		pshadervertexinput->m_attribs.add({ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = sizeof(float) * 2 });

      //		auto pshaderResolve = __create_new<::gpu_directx::shader>();

      //		m_pshaderResolve = pshaderResolve;

      //		pshaderResolve->m_bDisableDepthTest = true;

      //		//VkDescriptorSetLayoutBinding samplerLayoutBinding = {
      //		//   .binding = 0,
      //		//   .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
      //		//   .descriptorCount = 1,
      //		//   .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
      //		//   .pImmutableSamplers = NULL,
      //		//};

      //		//VkDescriptorSetLayoutCreateInfo layoutInfo =
      //		//{
      //		//   .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
      //		//   .bindingCount = 1,
      //		//   .pBindings = &samplerLayoutBinding,
      //		//};

      //		//VkDescriptorSetLayout descriptorSetLayout;
      //		//if (vkCreateDescriptorSetLayout(device, &layoutInfo, NULL, &descriptorSetLayout) != VK_SUCCESS) 
      //		//{
      //		//   // Handle error
      //		//}

      //		::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

      //		pshaderResolve->initialize_shader_with_block(
      //			this,
      //			as_memory_block(g_uaImageBlendVertexShader),
      //			as_memory_block(g_uaResolveFragmentShader),
      //			{ ::gpu::shader::e_descriptor_set_slot_local },
      //			m_psetdescriptorlayoutResolve,
      //			pshadervertexinput);

      //	}

      //	auto pshader = m_pshaderResolve;

      //	pshader->bind();

      //	auto& pdescriptor = m_pdescriptorResolve;

      //	if (__defer_construct_new(pdescriptor))
      //	{

      //		pdescriptor->m_descriptorsets.set_size(get_frame_count());

      //		::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

      //		::cast < renderer > pgpurendererParent = m_pgpucontext->m_pgpurenderer;

      //		::cast < accumulation_render_target_view > paccumulationrendertargetview = pgpurendererParent->m_prendertargetview;

      //		for (int i = 0; i < get_frame_count(); i++)
      //		{
      //			VkDescriptorImageInfo imageinfo;

      //			imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      //			imageinfo.imageView = paccumulationrendertargetview->m_imageviews[i];
      //			imageinfo.sampler = m_pgpucontext->_001VkSampler();

      //			VkDescriptorImageInfo imageinfo2;

      //			imageinfo2.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      //			imageinfo2.imageView = paccumulationrendertargetview->m_imageviewsAlphaAccumulation[i];
      //			imageinfo2.sampler = m_pgpucontext->_001VkSampler();

      //			auto& playout = m_psetdescriptorlayoutResolve;

      //			auto& ppool = m_pdescriptorpoolResolve;

      //			descriptor_writer(*playout, *ppool)
      //				.writeImage(0, &imageinfo)
      //				.writeImage(1, &imageinfo2)
      //				.build(pdescriptor->m_descriptorsets[i]);

      //		}

      //		auto descriptorSetLayout = m_psetdescriptorlayoutResolve->getDescriptorSetLayout();

      //		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
      //   .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      //   .setLayoutCount = 1,
      //   .pSetLayouts = &descriptorSetLayout,
      //		};

      //		//VkPipelineLayout pipelineLayout;
      //		if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, NULL, &pdescriptor->m_vkpipelinelayout) != VK_SUCCESS) {
      //			// Handle error
      //		}

      //	}

      //	auto commandBuffer = getCurrentCommandBuffer();

      //	//auto commandBuffer = this->getCurrentCommandBuffer();

      //	// Bind pipeline and descriptor sets
      // //      vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
      //   //    vkCmdBindDescriptorSets(commandBuffer, ...);
      //	vkCmdBindDescriptorSets(
      //		commandBuffer,
      //		VK_PIPELINE_BIND_POINT_GRAPHICS,   // Bind point
      //		pdescriptor->m_vkpipelinelayout,                     // Layout used when pipeline was created
      //		0,                                  // First set (set = 0)
      //		1,                                  // Descriptor set count
      //		&pdescriptor->m_descriptorsets[get_frame_index()],                     // Pointer to descriptor set
      //		0,                                  // Dynamic offset count
      //		NULL                                // Dynamic offsets
      //	);


      //	auto& pmodel = m_pmodelResolve;

      //	if (__defer_construct_new(pmodel))
      //	{

      //		create_quad_buffers(m_pgpucontext->logicalDevice(),
      //			m_pgpucontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice,
      //			&pmodel->m_vertexBuffer,
      //			&pmodel->m_vertexMemory,
      //			&pmodel->m_indexBuffer,
      //			&pmodel->m_indexMemory);

      //	}



      //	VkDeviceSize offsets[] = { 0 };
      //	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pmodel->m_vertexBuffer, offsets);
      //	vkCmdBindIndexBuffer(commandBuffer, pmodel->m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);
      //	auto rectangle = m_rectangle;
      //	VkViewport vp = {
      //	   (float)rectangle.left(),
      //	   (float)rectangle.top(),
      //	   (float)rectangle.width(),
      //	   (float)rectangle.height(),
      //	   0.0f, 1.0f };
      //	VkRect2D sc = {
      //	   {
      //	   (float)rectangle.left(),
      //	   (float)rectangle.top(),
      //	   },
      //	   {
      //				   (float)rectangle.width(),
      //	   (float)rectangle.height(),


      //	}
      //	};
      //	vkCmdSetViewport(commandBuffer, 0, 1, &vp);
      //	vkCmdSetScissor(commandBuffer, 0, 1, &sc);

      //	vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
      //	// Draw full-screen quad
      //	//vkCmdDraw(commandBuffer, 6, 1, 0, 0); // assuming full-screen triangle/quad

      //	pshader->unbind();

      //	on_end_render(pframe);

      //	endFrame();

      //}


   void renderer::swap_chain()
   {



      //assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
      //auto commandBuffer = getCurrentCommandBuffer();
      //if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      //   throw ::exception(error_failed, "failed to record command buffer!");
      //}
      //auto result = m_prendertargetview->submitCommandBuffers(&commandBuffer);
      //if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      //   m_bNeedToRecreateSwapChain)
      //{
      //   m_bNeedToRecreateSwapChain = false;
      //   defer_update_render_target_view();
      //}
      //else if (result != VK_SUCCESS)
      //{
      //   throw ::exception(error_failed, "failed to present swap chain image!");
      //}


      //vkQueueWaitIdle(m_pgpucontext->graphicsQueue());


   }



   void renderer::on_end_draw()
   {
      _on_end_render();

      for (auto& procedure : m_procedureaAfterEndRender)
      {

         procedure();

      }

      m_procedureaAfterEndRender.clear();
      //if (m_bOffScreen)
      //{

      //   assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
      //   auto commandBuffer = getCurrentCommandBuffer();
      //   if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      //      throw ::exception(error_failed, "failed to record command buffer!");
      //   }
      //   auto result = m_prendertargetview->submitCommandBuffers(&commandBuffer, &currentImageIndex);
      //   //if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      //   //	vkcWindow.wasWindowResized()) 
      //   //{
      //   //	vkcWindow.resetWindowResizedFlag();
      //   //	recreateSwapchain();
      //   //}
      //   //else 
      //   //	if (result != VK_SUCCESS) {
      //   //	throw ::exception(error_failed, "failed to present swap chain image!");
      //   //}

      if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      {
         sample();
      }
      else if (m_pgpucontext->m_eoutput == ::gpu::e_output_swap_chain)
      {
         swap_chain();


      }
      isFrameStarted = false;
      //currentFrameIndex = (currentFrameIndex + 1) % render_target_view::MAX_FRAMES_IN_FLIGHT;

      //}
      //else
      //{


      //	assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
      //	auto commandBuffer = getCurrentCommandBuffer();
      //	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      //		throw ::exception(error_failed, "failed to record command buffer!");
      //	}
      //	auto result = m_pvkcswapchain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
      //	//if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      //	//	vkcWindow.wasWindowResized()) 
      //	//{
      //	//	vkcWindow.resetWindowResizedFlag();
      //	//	recreateSwapchain();
      //	//}
      //	//else 
      //	//	if (result != VK_SUCCESS) {
      //	//	throw ::exception(error_failed, "failed to present swap chain image!");
      //	//}
      //	isFrameStarted = false;
      //	currentFrameIndex = (currentFrameIndex + 1) % swap_chain_render_target_view::MAX_FRAMES_IN_FLIGHT;

      //}

   }

   ::gpu::shader* renderer::get_image_blend_shader()
   {

      if (!m_pshaderImageBlend)
      {

         auto pshadervertexinput = __allocate  shader_vertex_input();

         //pshadervertexinput->m_bindings.add(
         //   {
         //      .binding = 0,
         //      .stride = sizeof(float) * 4,
         //      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
         //   });

         //pshadervertexinput->m_attribs.add({ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 0 });
         //pshadervertexinput->m_attribs.add({ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = sizeof(float) * 2 });

         auto pshaderImageBlend = __create_new<::gpu_directx::shader>();

         m_pshaderImageBlend = pshaderImageBlend;

         pshaderImageBlend->m_bEnableBlend = true;
         pshaderImageBlend->m_bDisableDepthTest = true;

         ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

         pshaderImageBlend->initialize_shader_with_block(
            this,
            as_memory_block(g_uaImageBlendVertexShader),
            as_memory_block(g_uaImageBlendFragmentShader),
            { ::gpu::shader::e_descriptor_set_slot_local },
            m_psetdescriptorlayoutImageBlend,
            pshadervertexinput);

      }

      return m_pshaderImageBlend;

   }


   ::gpu::shader* renderer::get_image_set_shader()
   {

      if (!m_pshaderImageBlend)
      {

         auto pshadervertexinput = __allocate  shader_vertex_input();

         //pshadervertexinput->m_bindings.add(
         //   {
         //      .binding = 0,
         //      .stride = sizeof(float) * 4,
         //      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
         //   });

         //pshadervertexinput->m_attribs.add({ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 0 });
         //pshadervertexinput->m_attribs.add({ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = sizeof(float) * 2 });

         auto pshaderImageBlend = __create_new<::gpu_directx::shader>();

         m_pshaderImageBlend = pshaderImageBlend;

         ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

         pshaderImageBlend->initialize_shader_with_block(
            this,
            as_memory_block(g_uaImageBlendVertexShader),
            as_memory_block(g_uaImageBlendFragmentShader),
            { ::gpu::shader::e_descriptor_set_slot_local },
            m_psetdescriptorlayoutImageBlend,
            pshadervertexinput);

      }

      return m_pshaderImageBlend;

   }



   //// Fullscreen quad vertex data
   //float quadVertices[] = {
   //   // pos     // uv
   //   -1.0f, -1.0f, 0.0f, 0.0f,
   //    1.0f, -1.0f, 1.0f, 0.0f,
   //    1.0f,  1.0f, 1.0f, 1.0f,
   //   -1.0f,  1.0f, 0.0f, 1.0f,
   //};
   //uint16_t quadIndices[] = {
   //   0, 1, 2,
   //   2, 3, 0
   //};

   //// Create vertex and index buffers
   //void create_quad_buffers(VkDevice device, VkPhysicalDevice physicalDevice,
   //   VkBuffer* vertexBuffer, VkDeviceMemory* vertexMemory,
   //   VkBuffer* indexBuffer, VkDeviceMemory* indexMemory, bool bYSwap)
   //{

   //   float y0;
   //   float y1;

   //   if (bYSwap)
   //   {

   //      y0 = 1.0f;
   //      y1 = -1.0f;


   //   }
   //   else
   //   {

   //      y0 = -1.0f;
   //      y1 = 1.0f;

   //   }


   //   // Fullscreen quad vertex data
   //   float quadVertices[] = {
   //      // pos     // uv
   //      -1.0f, y0, 0.0f, 0.0f,
   //       1.0f, y0, 1.0f, 0.0f,
   //       1.0f, y1, 1.0f, 1.0f,
   //      -1.0f, y1, 0.0f, 1.0f,
   //   };
   //   uint16_t quadIndices[] = {
   //      0, 1, 2,
   //      2, 3, 0
   //   };


   //   VkDeviceSize vertexSize = sizeof(quadVertices);
   //   VkDeviceSize indexSize = sizeof(quadIndices);

   //   VkBufferCreateInfo bufferInfo = {
   //      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
   //      .size = vertexSize,
   //      .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
   //      .sharingMode = VK_SHARING_MODE_EXCLUSIVE
   //   };
   //   vkCreateBuffer(device, &bufferInfo, NULL, vertexBuffer);

   //   VkMemoryRequirements memReq;
   //   vkGetBufferMemoryRequirements(device, *vertexBuffer, &memReq);

   //   VkMemoryAllocateInfo allocInfo = {
   //      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
   //      .allocationSize = memReq.size,
   //      .memoryTypeIndex = 0
   //   };

   //   VkPhysicalDeviceMemoryProperties memProps;
   //   vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);
   //   for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
   //      if ((memReq.memoryTypeBits & (1 << i)) &&
   //         (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
   //         (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
   //         allocInfo.memoryTypeIndex = i;
   //         break;
   //      }
   //   }

   //   vkAllocateMemory(device, &allocInfo, NULL, vertexMemory);
   //   vkBindBufferMemory(device, *vertexBuffer, *vertexMemory, 0);

   //   void* data;
   //   vkMapMemory(device, *vertexMemory, 0, vertexSize, 0, &data);
   //   memcpy(data, quadVertices, (size_t)vertexSize);
   //   vkUnmapMemory(device, *vertexMemory);

   //   // Index buffer
   //   bufferInfo.size = indexSize;
   //   bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
   //   vkCreateBuffer(device, &bufferInfo, NULL, indexBuffer);
   //   vkGetBufferMemoryRequirements(device, *indexBuffer, &memReq);
   //   allocInfo.allocationSize = memReq.size;

   //   for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
   //      if ((memReq.memoryTypeBits & (1 << i)) &&
   //         (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
   //         (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
   //         allocInfo.memoryTypeIndex = i;
   //         break;
   //      }
   //   }

   //   vkAllocateMemory(device, &allocInfo, NULL, indexMemory);
   //   vkBindBufferMemory(device, *indexBuffer, *indexMemory, 0);
   //   vkMapMemory(device, *indexMemory, 0, indexSize, 0, &data);
   //   memcpy(data, quadIndices, (size_t)indexSize);
   //   vkUnmapMemory(device, *indexMemory);
   //}


   //void renderer::_blend_image(VkImage image, const ::int_rectangle& rectangle, bool bYSwap)
   //{

   //   // Image Blend descriptors
   //   if (!m_psetdescriptorlayoutImageBlend)
   //   {

   //      m_psetdescriptorlayoutImageBlend = ::gpu_directx::set_descriptor_layout::Builder(m_pgpucontext)
   //         .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
   //         .build();

   //      int iFrameCount = get_frame_count();

   //      auto pdescriptorpoolbuilder = __allocate::gpu_directx::descriptor_pool::Builder();

   //      pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
   //      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
   //      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

   //      m_pdescriptorpoolImageBlend = pdescriptorpoolbuilder->build();

   //   }

   //   //vkCmdBeginRenderPass(...);

   //   auto commandBuffer = this->getCurrentCommandBuffer();

   //   //VkCommandBufferAllocateInfo commandBufferAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

   //   //VkCommandBuffer commandBuffer;
   //   //VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &commandBufferAllocateInfo, &commandBuffer));
   //   //VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
   //   //VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &cmdBufInfo));


   //   //m_procedureaAfterEndRender.add(
   //   //   [this, image, commandBuffer]()
   //   //   {

   //   //      //            {
   //   //      //            VkImageMemoryBarrier barrier = {
   //   //      //.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //   //      //.pNext = nullptr,
   //   //      //   .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //   //      //.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //   //      //
   //   //      //.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //   //      //.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //   //      //.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //.image = image,
   //   //      //.subresourceRange = {
   //   //      //    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //   //      //    .baseMipLevel = 0,
   //   //      //    .levelCount = 1,
   //   //      //    .baseArrayLayer = 0,
   //   //      //    .layerCount = 1,
   //   //      //},
   //   //      //            };
   //   //      //
   //   //      //            vkCmdPipelineBarrier(
   //   //      //               commandBuffer,
   //   //      //               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
   //   //      //               VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
   //   //      //               0,
   //   //      //               0, NULL,
   //   //      //               0, NULL,
   //   //      //               1, &barrier
   //   //      //            );
   //   //      //
   //   //      //         }
   //   //      //            {
   //   //      //               VkImageMemoryBarrier barrier = {
   //   //      //         .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //   //      //         .pNext = nullptr,
   //   //      //         .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //   //      //         .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //   //      //
   //   //      //         .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //   //      //         .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //   //      //         .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //         .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //         .image = image,  // <-- your VkImage here
   //   //      //         .subresourceRange = {
   //   //      //             .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //   //      //             .baseMipLevel = 0,
   //   //      //             .levelCount = 1,
   //   //      //             .baseArrayLayer = 0,
   //   //      //             .layerCount = 1,
   //   //      //         },
   //   //      //               };
   //   //      //
   //   //      //               vkCmdPipelineBarrier(
   //   //      //                  commandBuffer,
   //   //      //                  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // srcStageMask
   //   //      //                  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,         // dstStageMask
   //   //      //                  0,
   //   //      //                  0, NULL,
   //   //      //                  0, NULL,
   //   //      //                  1, &barrier
   //   //      //               );
   //   //      //
   //   //      //            }
   //   //      //
   //   //      //         }

   //   //if(1)
   //   //      {


   //   //   VkImageMemoryBarrier barrier = {
   //   //       .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //   //       .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //   //       .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //   //       .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //   //       .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //   //       .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //       .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //       .image = image,
   //   //       .subresourceRange = {
   //   //           .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //   //           .baseMipLevel = 0,
   //   //           .levelCount = 1,
   //   //           .baseArrayLayer = 0,
   //   //           .layerCount = 1
   //   //       },
   //   //   };

   //   //   vkCmdPipelineBarrier(
   //   //      commandBuffer,
   //   //      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
   //   //      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
   //   //      0,
   //   //      0, NULL,
   //   //      0, NULL,
   //   //      1, &barrier
   //   //   );

   //   //      }

   //   /*   }
   //      );*/

   //   auto& pmodel = m_imagemodel[image];

   //   if (__defer_construct_new(pmodel))
   //   {

   //      create_quad_buffers(m_pgpucontext->logicalDevice(),
   //         m_pgpucontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice,
   //         &pmodel->m_vertexBuffer,
   //         &pmodel->m_vertexMemory,
   //         &pmodel->m_indexBuffer,
   //         &pmodel->m_indexMemory, true);

   //   }

   //   auto pshader = get_image_blend_shader();

   //   pshader->bind();

   //   auto& pdescriptor = m_imagedescriptor[image];

   //   if (__defer_construct_new(pdescriptor))
   //   {

   //      pdescriptor->m_descriptorsets.set_size(get_frame_count());

   //      VkImageViewCreateInfo viewInfo = {
   //   .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
   //   .image = image,  // <-- Your existing VkImage
   //   .viewType = VK_IMAGE_VIEW_TYPE_2D,
   //   .format = VK_FORMAT_B8G8R8A8_UNORM,  // <-- Match your image's format
   //   .components = {
   //      .r = VK_COMPONENT_SWIZZLE_IDENTITY,
   //      .g = VK_COMPONENT_SWIZZLE_IDENTITY,
   //      .b = VK_COMPONENT_SWIZZLE_IDENTITY,
   //      .a = VK_COMPONENT_SWIZZLE_IDENTITY,
   //   },
   //   .subresourceRange = {
   //      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //      .baseMipLevel = 0,
   //      .levelCount = 1,
   //      .baseArrayLayer = 0,
   //      .layerCount = 1,
   //   },
   //      };

   //      VkImageView imageView;
   //      if (vkCreateImageView(m_pgpucontext->logicalDevice(), &viewInfo, NULL, &imageView) != VK_SUCCESS) {
   //         // Handle error
   //      }

   //      ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

   //      for (int i = 0; i < get_frame_count(); i++)
   //      {
   //         VkDescriptorImageInfo imageinfo;

   //         imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
   //         imageinfo.imageView = imageView;
   //         imageinfo.sampler = m_pgpucontext->_001VkSampler();

   //         auto& playout = m_psetdescriptorlayoutImageBlend;

   //         auto& ppool = m_pdescriptorpoolImageBlend;

   //         descriptor_writer(*playout, *ppool)
   //            .writeImage(0, &imageinfo)
   //            .build(pdescriptor->m_descriptorsets[i]);

   //      }

   //      auto descriptorSetLayout = m_psetdescriptorlayoutImageBlend->getDescriptorSetLayout();

   //      VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
   //   .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
   //   .setLayoutCount = 1,
   //   .pSetLayouts = &descriptorSetLayout,
   //      };

   //      //VkPipelineLayout pipelineLayout;
   //      if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, NULL, &pdescriptor->m_vkpipelinelayout) != VK_SUCCESS) {
   //         // Handle error
   //      }

   //   }

   //   //auto commandBuffer = this->getCurrentCommandBuffer();

   //   // Bind pipeline and descriptor sets
   // //      vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
   //   //    vkCmdBindDescriptorSets(commandBuffer, ...);
   //   vkCmdBindDescriptorSets(
   //      commandBuffer,
   //      VK_PIPELINE_BIND_POINT_GRAPHICS,   // Bind point
   //      pdescriptor->m_vkpipelinelayout,                     // Layout used when pipeline was created
   //      0,                                  // First set (set = 0)
   //      1,                                  // Descriptor set count
   //      &pdescriptor->m_descriptorsets[get_frame_index()],                     // Pointer to descriptor set
   //      0,                                  // Dynamic offset count
   //      NULL                                // Dynamic offsets
   //   );



   //   VkDeviceSize offsets[] = { 0 };
   //   vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pmodel->m_vertexBuffer, offsets);
   //   vkCmdBindIndexBuffer(commandBuffer, pmodel->m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);

   //   VkViewport vp = {
   //      (float)rectangle.left(),
   //      (float)rectangle.top(),
   //      (float)rectangle.width(),
   //      (float)rectangle.height(),
   //      0.0f, 1.0f };
   //   VkRect2D sc = {
   //      {
   //      (float)rectangle.left(),
   //      (float)rectangle.top(),
   //      },
   //      {
   //               (float)rectangle.width(),
   //      (float)rectangle.height(),


   //   }
   //   };
   //   vkCmdSetViewport(commandBuffer, 0, 1, &vp);
   //   vkCmdSetScissor(commandBuffer, 0, 1, &sc);

   //   vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
   //   // Draw full-screen quad
   //   //vkCmdDraw(commandBuffer, 6, 1, 0, 0); // assuming full-screen triangle/quad

   //   pshader->unbind();

   //   //vkCmdEndRenderPass(...);

   //   vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

   //   //vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_vkcommandpool, 1, &commandBuffer);


   //}


   //void renderer::_copy_image(VkImage image, const ::int_rectangle& rectangle, bool bYSwap)
   //{

   //   // Image Blend descriptors
   //   if (!m_psetdescriptorlayoutImageBlend)
   //   {

   //      m_psetdescriptorlayoutImageBlend = ::gpu_directx::set_descriptor_layout::Builder(m_pgpucontext)
   //         .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
   //         .build();

   //      int iFrameCount = get_frame_count();

   //      auto pdescriptorpoolbuilder = __allocate::gpu_directx::descriptor_pool::Builder();

   //      pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
   //      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
   //      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

   //      m_pdescriptorpoolImageBlend = pdescriptorpoolbuilder->build();

   //   }

   //   VkCommandBuffer commandBuffer = this->getCurrentCommandBuffer();

   //   auto& pmodel = m_imagemodel[image];

   //   if (__defer_construct_new(pmodel))
   //   {

   //      create_quad_buffers(m_pgpucontext->logicalDevice(),
   //         m_pgpucontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice,
   //         &pmodel->m_vertexBuffer,
   //         &pmodel->m_vertexMemory,
   //         &pmodel->m_indexBuffer,
   //         &pmodel->m_indexMemory, bYSwap);

   //   }

   //   auto pshader = get_image_blend_shader();

   //   pshader->bind();

   //   auto& pdescriptor = m_imagedescriptor[image];

   //   if (__defer_construct_new(pdescriptor))
   //   {

   //      pdescriptor->m_descriptorsets.set_size(get_frame_count());

   //      VkImageViewCreateInfo viewInfo =
   //      {
   //         .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
   //         .image = image,  // <-- Your existing VkImage
   //         .viewType = VK_IMAGE_VIEW_TYPE_2D,
   //         .format = VK_FORMAT_B8G8R8A8_UNORM,  // <-- Match your image's format
   //         .components = 
   //         {
   //            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
   //            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
   //            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
   //            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
   //         },
   //         .subresourceRange = 
   //         {
   //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //            .baseMipLevel = 0,
   //            .levelCount = 1,
   //            .baseArrayLayer = 0,
   //            .layerCount = 1,
   //         },
   //      };

   //      VkImageView imageView;

   //      if (vkCreateImageView(m_pgpucontext->logicalDevice(), &viewInfo, NULL, &imageView) != VK_SUCCESS)
   //      {
   //         // Handle error
   //      }

   //      ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

   //      for (int i = 0; i < get_frame_count(); i++)
   //      {

   //         VkDescriptorImageInfo imageinfo;

   //         imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
   //         imageinfo.imageView = imageView;
   //         imageinfo.sampler = m_pgpucontext->_001VkSampler();

   //         auto& playout = m_psetdescriptorlayoutImageBlend;

   //         auto& ppool = m_pdescriptorpoolImageBlend;

   //         descriptor_writer(*playout, *ppool)
   //            .writeImage(0, &imageinfo)
   //            .build(pdescriptor->m_descriptorsets[i]);

   //      }

   //      auto descriptorSetLayout = m_psetdescriptorlayoutImageBlend->getDescriptorSetLayout();

   //      VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
   //   .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
   //   .setLayoutCount = 1,
   //   .pSetLayouts = &descriptorSetLayout,
   //      };

   //      //VkPipelineLayout pipelineLayout;
   //      if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, NULL, &pdescriptor->m_vkpipelinelayout) != VK_SUCCESS) {
   //         // Handle error
   //      }

   //   }

   //   //auto commandBuffer = this->getCurrentCommandBuffer();

   //   // Bind pipeline and descriptor sets
   // //      vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
   //   //    vkCmdBindDescriptorSets(commandBuffer, ...);
   //   vkCmdBindDescriptorSets(
   //      commandBuffer,
   //      VK_PIPELINE_BIND_POINT_GRAPHICS,   // Bind point
   //      pdescriptor->m_vkpipelinelayout,                     // Layout used when pipeline was created
   //      0,                                  // First set (set = 0)
   //      1,                                  // Descriptor set count
   //      &pdescriptor->m_descriptorsets[get_frame_index()],                     // Pointer to descriptor set
   //      0,                                  // Dynamic offset count
   //      NULL                                // Dynamic offsets
   //   );



   //   VkDeviceSize offsets[] = { 0 };
   //   vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pmodel->m_vertexBuffer, offsets);
   //   vkCmdBindIndexBuffer(commandBuffer, pmodel->m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);

   //   VkViewport vp = {
   //      (float)rectangle.left(),
   //      (float)rectangle.top(),
   //      (float)rectangle.width(),
   //      (float)rectangle.height(),
   //      0.0f, 1.0f };
   //   VkRect2D sc = {
   //      {
   //      (float)rectangle.left(),
   //      (float)rectangle.top(),
   //      },
   //      {
   //               (float)rectangle.width(),
   //      (float)rectangle.height(),


   //   }
   //   };
   //   vkCmdSetViewport(commandBuffer, 0, 1, &vp);
   //   vkCmdSetScissor(commandBuffer, 0, 1, &sc);

   //   vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
   //   // Draw full-screen quad
   //   //vkCmdDraw(commandBuffer, 6, 1, 0, 0); // assuming full-screen triangle/quad

   //   pshader->unbind();

   //   //vkCmdEndRenderPass(...);

   //   vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

   //   //vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_vkcommandpool, 1, &commandBuffer);


   //}


   //void renderer::_set_image(VkImage image, const ::int_rectangle& rectangle, bool bYSwap)
   //{

   //   // Image Blend descriptors
   //   if (!m_psetdescriptorlayoutImageBlend)
   //   {

   //      m_psetdescriptorlayoutImageBlend = ::gpu_directx::set_descriptor_layout::Builder(m_pgpucontext)
   //         .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
   //         .build();

   //      int iFrameCount = get_frame_count();

   //      auto pdescriptorpoolbuilder = __allocate::gpu_directx::descriptor_pool::Builder();

   //      pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
   //      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
   //      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

   //      m_pdescriptorpoolImageBlend = pdescriptorpoolbuilder->build();

   //   }

   //   //vkCmdBeginRenderPass(...);

   //   auto commandBuffer = this->getCurrentCommandBuffer();

   //   //VkCommandBufferAllocateInfo commandBufferAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

   //   //VkCommandBuffer commandBuffer;
   //   //VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &commandBufferAllocateInfo, &commandBuffer));
   //   //VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
   //   //VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &cmdBufInfo));


   //   //m_procedureaAfterEndRender.add(
   //   //   [this, image, commandBuffer]()
   //   //   {

   //   //      //            {
   //   //      //            VkImageMemoryBarrier barrier = {
   //   //      //.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //   //      //.pNext = nullptr,
   //   //      //   .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //   //      //.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //   //      //
   //   //      //.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //   //      //.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //   //      //.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //.image = image,
   //   //      //.subresourceRange = {
   //   //      //    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //   //      //    .baseMipLevel = 0,
   //   //      //    .levelCount = 1,
   //   //      //    .baseArrayLayer = 0,
   //   //      //    .layerCount = 1,
   //   //      //},
   //   //      //            };
   //   //      //
   //   //      //            vkCmdPipelineBarrier(
   //   //      //               commandBuffer,
   //   //      //               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
   //   //      //               VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
   //   //      //               0,
   //   //      //               0, NULL,
   //   //      //               0, NULL,
   //   //      //               1, &barrier
   //   //      //            );
   //   //      //
   //   //      //         }
   //   //      //            {
   //   //      //               VkImageMemoryBarrier barrier = {
   //   //      //         .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //   //      //         .pNext = nullptr,
   //   //      //         .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //   //      //         .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //   //      //
   //   //      //         .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //   //      //         .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //   //      //         .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //         .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //      //         .image = image,  // <-- your VkImage here
   //   //      //         .subresourceRange = {
   //   //      //             .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //   //      //             .baseMipLevel = 0,
   //   //      //             .levelCount = 1,
   //   //      //             .baseArrayLayer = 0,
   //   //      //             .layerCount = 1,
   //   //      //         },
   //   //      //               };
   //   //      //
   //   //      //               vkCmdPipelineBarrier(
   //   //      //                  commandBuffer,
   //   //      //                  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // srcStageMask
   //   //      //                  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,         // dstStageMask
   //   //      //                  0,
   //   //      //                  0, NULL,
   //   //      //                  0, NULL,
   //   //      //                  1, &barrier
   //   //      //               );
   //   //      //
   //   //      //            }
   //   //      //
   //   //      //         }

   //   //if(1)
   //   //      {


   //   //   VkImageMemoryBarrier barrier = {
   //   //       .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //   //       .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //   //       .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //   //       .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //   //       .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //   //       .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //       .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //   //       .image = image,
   //   //       .subresourceRange = {
   //   //           .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //   //           .baseMipLevel = 0,
   //   //           .levelCount = 1,
   //   //           .baseArrayLayer = 0,
   //   //           .layerCount = 1
   //   //       },
   //   //   };

   //   //   vkCmdPipelineBarrier(
   //   //      commandBuffer,
   //   //      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
   //   //      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
   //   //      0,
   //   //      0, NULL,
   //   //      0, NULL,
   //   //      1, &barrier
   //   //   );

   //   //      }

   //   /*   }
   //      );*/

   //   auto& pmodel = m_imagemodel[image];

   //   if (__defer_construct_new(pmodel))
   //   {

   //      create_quad_buffers(m_pgpucontext->logicalDevice(),
   //         m_pgpucontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice,
   //         &pmodel->m_vertexBuffer,
   //         &pmodel->m_vertexMemory,
   //         &pmodel->m_indexBuffer,
   //         &pmodel->m_indexMemory, 
   //         bYSwap);

   //   }

   //   auto pshader = get_image_blend_shader();

   //   pshader->bind();

   //   auto& pdescriptor = m_imagedescriptor[image];

   //   if (__defer_construct_new(pdescriptor))
   //   {

   //      pdescriptor->m_descriptorsets.set_size(get_frame_count());

   //      VkImageViewCreateInfo viewInfo = {
   //   .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
   //   .image = image,  // <-- Your existing VkImage
   //   .viewType = VK_IMAGE_VIEW_TYPE_2D,
   //   .format = VK_FORMAT_B8G8R8A8_UNORM,  // <-- Match your image's format
   //   .components = {
   //      .r = VK_COMPONENT_SWIZZLE_IDENTITY,
   //      .g = VK_COMPONENT_SWIZZLE_IDENTITY,
   //      .b = VK_COMPONENT_SWIZZLE_IDENTITY,
   //      .a = VK_COMPONENT_SWIZZLE_IDENTITY,
   //   },
   //   .subresourceRange = {
   //      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //      .baseMipLevel = 0,
   //      .levelCount = 1,
   //      .baseArrayLayer = 0,
   //      .layerCount = 1,
   //   },
   //      };

   //      VkImageView imageView;
   //      if (vkCreateImageView(m_pgpucontext->logicalDevice(), &viewInfo, NULL, &imageView) != VK_SUCCESS) {
   //         // Handle error
   //      }

   //      ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

   //      for (int i = 0; i < get_frame_count(); i++)
   //      {
   //         VkDescriptorImageInfo imageinfo;

   //         imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
   //         imageinfo.imageView = imageView;
   //         imageinfo.sampler = m_pgpucontext->_001VkSampler();

   //         auto& playout = m_psetdescriptorlayoutImageBlend;

   //         auto& ppool = m_pdescriptorpoolImageBlend;

   //         descriptor_writer(*playout, *ppool)
   //            .writeImage(0, &imageinfo)
   //            .build(pdescriptor->m_descriptorsets[i]);

   //      }

   //      auto descriptorSetLayout = m_psetdescriptorlayoutImageBlend->getDescriptorSetLayout();

   //      VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
   //   .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
   //   .setLayoutCount = 1,
   //   .pSetLayouts = &descriptorSetLayout,
   //      };

   //      //VkPipelineLayout pipelineLayout;
   //      if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, NULL, &pdescriptor->m_vkpipelinelayout) != VK_SUCCESS) {
   //         // Handle error
   //      }

   //   }

   //   //auto commandBuffer = this->getCurrentCommandBuffer();

   //   // Bind pipeline and descriptor sets
   // //      vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
   //   //    vkCmdBindDescriptorSets(commandBuffer, ...);
   //   vkCmdBindDescriptorSets(
   //      commandBuffer,
   //      VK_PIPELINE_BIND_POINT_GRAPHICS,   // Bind point
   //      pdescriptor->m_vkpipelinelayout,                     // Layout used when pipeline was created
   //      0,                                  // First set (set = 0)
   //      1,                                  // Descriptor set count
   //      &pdescriptor->m_descriptorsets[get_frame_index()],                     // Pointer to descriptor set
   //      0,                                  // Dynamic offset count
   //      NULL                                // Dynamic offsets
   //   );



   //   VkDeviceSize offsets[] = { 0 };
   //   vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pmodel->m_vertexBuffer, offsets);
   //   vkCmdBindIndexBuffer(commandBuffer, pmodel->m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);

   //   VkViewport vp = {
   //      (float)rectangle.left(),
   //      (float)rectangle.top(),
   //      (float)rectangle.width(),
   //      (float)rectangle.height(),
   //      0.0f, 1.0f };
   //   VkRect2D sc = {
   //      {
   //      (float)rectangle.left(),
   //      (float)rectangle.top(),
   //      },
   //      {
   //               (float)rectangle.width(),
   //      (float)rectangle.height(),


   //   }
   //   };
   //   vkCmdSetViewport(commandBuffer, 0, 1, &vp);
   //   vkCmdSetScissor(commandBuffer, 0, 1, &sc);

   //   vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
   //   // Draw full-screen quad
   //   //vkCmdDraw(commandBuffer, 6, 1, 0, 0); // assuming full-screen triangle/quad

   //   pshader->unbind();

   //   //vkCmdEndRenderPass(...);

   //   vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

   //   //vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_vkcommandpool, 1, &commandBuffer);


   //}




   void renderer::_blend_renderer(::gpu_directx::renderer* prendererSrc, bool bYSwap)
   {

    //  VkImage image = prendererSrc->m_prendertargetview->m_images[prendererSrc->get_frame_index()];

    //  auto rectanglePlacement = prendererSrc->m_pgpucontext->m_rectangle;

    //  // Image Blend descriptors
    //  if (!m_psetdescriptorlayoutImageBlend)
    //  {

    //     m_psetdescriptorlayoutImageBlend = ::gpu_directx::set_descriptor_layout::Builder(m_pgpucontext)
    //        .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
    //        .build();

    //     int iFrameCount = get_frame_count();

    //     auto pdescriptorpoolbuilder = __allocate::gpu_directx::descriptor_pool::Builder();

    //     pdescriptorpoolbuilder->initialize_builder(m_pgpucontext);
    //     pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
    //     pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

    //     m_pdescriptorpoolImageBlend = pdescriptorpoolbuilder->build();

    //  }

    //  //vkCmdBeginRenderPass(...);

    //  auto commandBuffer = this->getCurrentCommandBuffer();

    //  //VkCommandBufferAllocateInfo commandBufferAllocateInfo = initializers::commandBufferAllocateInfo(m_pgpucontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

    //  //VkCommandBuffer commandBuffer;
    //  //VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &commandBufferAllocateInfo, &commandBuffer));
    //  //VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
    //  //VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &cmdBufInfo));


    //  //m_procedureaAfterEndRender.add(
    //  //   [this, image, commandBuffer]()
    //  //   {

    //  //      //            {
    //  //      //            VkImageMemoryBarrier barrier = {
    //  //      //.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    //  //      //.pNext = nullptr,
    //  //      //   .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
    //  //      //.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    //  //      //
    //  //      //.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    //  //      //.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    //  //      //.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    //  //      //.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    //  //      //.image = image,
    //  //      //.subresourceRange = {
    //  //      //    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
    //  //      //    .baseMipLevel = 0,
    //  //      //    .levelCount = 1,
    //  //      //    .baseArrayLayer = 0,
    //  //      //    .layerCount = 1,
    //  //      //},
    //  //      //            };
    //  //      //
    //  //      //            vkCmdPipelineBarrier(
    //  //      //               commandBuffer,
    //  //      //               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
    //  //      //               VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    //  //      //               0,
    //  //      //               0, NULL,
    //  //      //               0, NULL,
    //  //      //               1, &barrier
    //  //      //            );
    //  //      //
    //  //      //         }
    //  //      //            {
    //  //      //               VkImageMemoryBarrier barrier = {
    //  //      //         .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    //  //      //         .pNext = nullptr,
    //  //      //         .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    //  //      //         .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
    //  //      //
    //  //      //         .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    //  //      //         .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    //  //      //         .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    //  //      //         .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    //  //      //         .image = image,  // <-- your VkImage here
    //  //      //         .subresourceRange = {
    //  //      //             .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
    //  //      //             .baseMipLevel = 0,
    //  //      //             .levelCount = 1,
    //  //      //             .baseArrayLayer = 0,
    //  //      //             .layerCount = 1,
    //  //      //         },
    //  //      //               };
    //  //      //
    //  //      //               vkCmdPipelineBarrier(
    //  //      //                  commandBuffer,
    //  //      //                  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // srcStageMask
    //  //      //                  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,         // dstStageMask
    //  //      //                  0,
    //  //      //                  0, NULL,
    //  //      //                  0, NULL,
    //  //      //                  1, &barrier
    //  //      //               );
    //  //      //
    //  //      //            }
    //  //      //
    //  //      //         }

    //  //if(1)
    //  //      {


    //  //   VkImageMemoryBarrier barrier = {
    //  //       .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    //  //       .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    //  //       .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
    //  //       .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    //  //       .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    //  //       .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    //  //       .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    //  //       .image = image,
    //  //       .subresourceRange = {
    //  //           .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
    //  //           .baseMipLevel = 0,
    //  //           .levelCount = 1,
    //  //           .baseArrayLayer = 0,
    //  //           .layerCount = 1
    //  //       },
    //  //   };

    //  //   vkCmdPipelineBarrier(
    //  //      commandBuffer,
    //  //      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    //  //      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
    //  //      0,
    //  //      0, NULL,
    //  //      0, NULL,
    //  //      1, &barrier
    //  //   );

    //  //      }

    //  /*   }
    //     );*/

    //  auto& pmodel = m_imagemodel[image];

    //  if (__defer_construct_new(pmodel))
    //  {

    //     create_quad_buffers(m_pgpucontext->logicalDevice(),
    //        m_pgpucontext->m_pgpudevice->m_pphysicaldevice->m_physicaldevice,
    //        &pmodel->m_vertexBuffer,
    //        &pmodel->m_vertexMemory,
    //        &pmodel->m_indexBuffer,
    //        &pmodel->m_indexMemory, 
    //        bYSwap);

    //  }

    //  auto pshader = get_image_blend_shader();

    //  pshader->bind();

    //  auto& pdescriptor = m_imagedescriptor[image];

    //  if (__defer_construct_new(pdescriptor))
    //  {

    //     pdescriptor->m_descriptorsets.set_size(get_frame_count());

    //     //     VkImageViewCreateInfo viewInfo = {
    //     //.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    //     //.image = image,  // <-- Your existing VkImage
    //     //.viewType = VK_IMAGE_VIEW_TYPE_2D,
    //     //.format = VK_FORMAT_B8G8R8A8_UNORM,  // <-- Match your image's format
    //     //.components = {
    //     //    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
    //     //    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
    //     //    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
    //     //    .a = VK_COMPONENT_SWIZZLE_IDENTITY,
    //     //},
    //     //.subresourceRange = {
    //     //    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
    //     //    .baseMipLevel = 0,
    //     //    .levelCount = 1,
    //     //    .baseArrayLayer = 0,
    //     //    .layerCount = 1,
    //     //},
    //     //     };

    //     //     VkImageView imageView;
    //     //     if (vkCreateImageView(m_pgpucontext->logicalDevice(), &viewInfo, NULL, &imageView) != VK_SUCCESS) {
    //     //        // Handle error
    //     //     }

    //     ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;
    //     ::cast < accumulation_render_target_view > ppass = prendererSrc->m_prendertargetview;

    //     for (int i = 0; i < get_frame_count(); i++)
    //     {

    //        VkDescriptorImageInfo imageinfo;

    //        auto imageview = ppass->m_imageviews[i];

    //        imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //        imageinfo.imageView = imageview;
    //        imageinfo.sampler = m_pgpucontext->_001VkSampler();

    //        VkDescriptorImageInfo imageinfoAlpha;

    //        auto imageviewAlpha = ppass->m_imageviewsAlphaAccumulation[i];

    //        imageinfoAlpha.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //        imageinfoAlpha.imageView = imageviewAlpha;
    //        imageinfoAlpha.sampler = m_pgpucontext->_001VkSampler();


    //        auto& playout = m_psetdescriptorlayoutImageBlend;

    //        auto& ppool = m_pdescriptorpoolImageBlend;

    //        descriptor_writer(*playout, *ppool)
    //           .writeImage(0, &imageinfo)
    //           .writeImage(0, &imageinfoAlpha)
    //           .build(pdescriptor->m_descriptorsets[i]);

    //     }

    //     auto descriptorSetLayout = m_psetdescriptorlayoutImageBlend->getDescriptorSetLayout();

    //     VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
    //  .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    //  .setLayoutCount = 1,
    //  .pSetLayouts = &descriptorSetLayout,
    //     };

    //     //VkPipelineLayout pipelineLayout;
    //     if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, NULL, &pdescriptor->m_vkpipelinelayout) != VK_SUCCESS) {
    //        // Handle error
    //     }

    //  }

    //  //auto commandBuffer = this->getCurrentCommandBuffer();

    //  // Bind pipeline and descriptor sets
    ////      vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    //  //    vkCmdBindDescriptorSets(commandBuffer, ...);
    //  vkCmdBindDescriptorSets(
    //     commandBuffer,
    //     VK_PIPELINE_BIND_POINT_GRAPHICS,   // Bind point
    //     pdescriptor->m_vkpipelinelayout,                     // Layout used when pipeline was created
    //     0,                                  // First set (set = 0)
    //     1,                                  // Descriptor set count
    //     &pdescriptor->m_descriptorsets[get_frame_index()],                     // Pointer to descriptor set
    //     0,                                  // Dynamic offset count
    //     NULL                                // Dynamic offsets
    //  );



    //  VkDeviceSize offsets[] = { 0 };
    //  vkCmdBindVertexBuffers(commandBuffer, 0, 1, &pmodel->m_vertexBuffer, offsets);
    //  vkCmdBindIndexBuffer(commandBuffer, pmodel->m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);


    //  VkViewport vp = {
    //     (float)rectanglePlacement.left(),
    //     (float)rectanglePlacement.top(),
    //     (float)rectanglePlacement.width(),
    //     (float)rectanglePlacement.height(),
    //     0.0f, 1.0f };
    //  VkRect2D sc = {
    //     {
    //     (float)rectanglePlacement.left(),
    //     (float)rectanglePlacement.top(),
    //     },
    //     {
    //     (float)rectanglePlacement.width(),
    //     (float)rectanglePlacement.height(),


    //  }
    //  };
    //  vkCmdSetViewport(commandBuffer, 0, 1, &vp);
    //  vkCmdSetScissor(commandBuffer, 0, 1, &sc);

    //  vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
    //  // Draw full-screen quad
    //  //vkCmdDraw(commandBuffer, 6, 1, 0, 0); // assuming full-screen triangle/quad

    //  pshader->unbind();

    //  //vkCmdEndRenderPass(...);

    //  vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

    //  //vkFreeCommandBuffers(m_pgpucontext->logicalDevice(), m_pgpucontext->m_vkcommandpool, 1, &commandBuffer);


   }


   void renderer::_on_begin_render()
   {

      ////::cast < frame > pframe = pframeParam;

      ////auto commandBuffer = pframe->commandBuffer;

      //auto commandBuffer = this->getCurrentCommandBuffer();

      ////if (m_bOffScreen)
      //{

      //   assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
      //   assert(
      //      commandBuffer == getCurrentCommandBuffer() &&
      //      "Can't begin render pass on command buffer from a different frame");

      //   VkRenderPassBeginInfo renderPassInfo{};
      //   renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      //   renderPassInfo.renderPass = m_prendertargetview->getRenderPass();
      //   renderPassInfo.framebuffer = m_prendertargetview->getCurrentFrameBuffer();

      //   renderPassInfo.renderArea.offset = { 0, 0 };
      //   renderPassInfo.renderArea.extent = m_prendertargetview->getExtent();

      //   VkClearValue clearValues[2];
      //   //clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
      //   clearValues[0].color = { 0.f, 0.0f, 0.0f, 0.0f };
      //   clearValues[1].depthStencil = { 1.0f, 0 };
      //   renderPassInfo.clearValueCount = 2;
      //   renderPassInfo.pClearValues = clearValues;

      //   vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      //   VkViewport viewport{};
      //   viewport.x = 0.0f;
      //   viewport.y = 0.0f;
      //   viewport.width = static_cast<float>(m_prendertargetview->getExtent().width);
      //   viewport.height = static_cast<float>(m_prendertargetview->getExtent().height);
      //   viewport.minDepth = 0.0f;
      //   viewport.maxDepth = 1.0f;
      //   VkRect2D scissor{ {0, 0}, m_prendertargetview->getExtent() };
      //   vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
      //   vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

      //}
      ////else
      ////{

      ////	assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
      ////	assert(
      ////		commandBuffer == getCurrentCommandBuffer() &&
      ////		"Can't begin render pass on command buffer from a different frame");

      ////	VkRenderPassBeginInfo renderPassInfo{};
      ////	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      ////	renderPassInfo.renderPass = m_pvkcswapchain->getRenderPass();
      ////	renderPassInfo.framebuffer = m_pvkcswapchain->getFrameBuffer(currentImageIndex);

      ////	renderPassInfo.renderArea.offset = { 0, 0 };
      ////	renderPassInfo.renderArea.extent = m_pvkcswapchain->getExtent();

      ////	std::array<VkClearValue, 2> clearValues{};
      ////	clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
      ////	clearValues[1].depthStencil = { 1.0f, 0 };
      ////	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      ////	renderPassInfo.pClearValues = clearValues.data();

      ////	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      ////	VkViewport viewport{};
      ////	viewport.x = 0.0f;
      ////	viewport.y = 0.0f;
      ////	viewport.width = static_cast<float>(vkcSwapChain->getSwapChainExtent().width);
      ////	viewport.height = static_cast<float>(vkcSwapChain->getSwapChainExtent().height);
      ////	viewport.minDepth = 0.0f;
      ////	viewport.maxDepth = 1.0f;
      ////	VkRect2D scissor{ {0, 0}, vkcSwapChain->getSwapChainExtent() };
      ////	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
      ////	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


      ////}
   }


   void renderer::on_begin_render(::gpu::frame* pframeParam)
   {

      //::cast < frame > pframe = pframeParam;

      //auto commandBuffer = pframe->commandBuffer;

      ////m_prendertargetview->m_iFrameSerial++;

      ////m_prendertargetview->m_iCurrentFrame = (m_prendertargetview->m_iCurrentFrame + 1) % 
      ////   get_frame_count();

      //m_prendertargetview->on_before_begin_render(pframe);

      ////if (m_bOffScreen)
      //{

      //   assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
      //   assert(
      //      commandBuffer == getCurrentCommandBuffer() &&
      //      "Can't begin render pass on command buffer from a different frame");

      //   VkRenderPassBeginInfo renderPassInfo{};
      //   renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      //   renderPassInfo.renderPass = m_prendertargetview->getRenderPass();
      //   renderPassInfo.framebuffer = m_prendertargetview->getCurrentFrameBuffer();

      //   renderPassInfo.renderArea.offset = { 0, 0 };
      //   renderPassInfo.renderArea.extent = m_prendertargetview->getExtent();

      //   VkClearValue clearValues[2];
      //   //clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
      //   clearValues[0].color = { 0.0f, 0.0f, 0.0f, 0.0f };
      //   clearValues[1].depthStencil = { 1.0f, 0 };
      //   renderPassInfo.clearValueCount = 2;
      //   renderPassInfo.pClearValues = clearValues;

      //   vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      //   VkViewport viewport{};
      //   viewport.x = 0.0f;
      //   viewport.y = 0.0f;
      //   viewport.width = static_cast<float>(m_prendertargetview->getExtent().width);
      //   viewport.height = static_cast<float>(m_prendertargetview->getExtent().height);
      //   viewport.minDepth = 0.0f;
      //   viewport.maxDepth = 1.0f;
      //   VkRect2D scissor{ {0, 0}, m_prendertargetview->getExtent() };
      //   vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
      //   vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

      //}
      ////else
      ////{

      ////	assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
      ////	assert(
      ////		commandBuffer == getCurrentCommandBuffer() &&
      ////		"Can't begin render pass on command buffer from a different frame");

      ////	VkRenderPassBeginInfo renderPassInfo{};
      ////	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      ////	renderPassInfo.renderPass = m_pvkcswapchain->getRenderPass();
      ////	renderPassInfo.framebuffer = m_pvkcswapchain->getFrameBuffer(currentImageIndex);

      ////	renderPassInfo.renderArea.offset = { 0, 0 };
      ////	renderPassInfo.renderArea.extent = m_pvkcswapchain->getExtent();

      ////	std::array<VkClearValue, 2> clearValues{};
      ////	clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
      ////	clearValues[1].depthStencil = { 1.0f, 0 };
      ////	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      ////	renderPassInfo.pClearValues = clearValues.data();

      ////	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      ////	VkViewport viewport{};
      ////	viewport.x = 0.0f;
      ////	viewport.y = 0.0f;
      ////	viewport.width = static_cast<float>(vkcSwapChain->getSwapChainExtent().width);
      ////	viewport.height = static_cast<float>(vkcSwapChain->getSwapChainExtent().height);
      ////	viewport.minDepth = 0.0f;
      ////	viewport.maxDepth = 1.0f;
      ////	VkRect2D scissor{ {0, 0}, vkcSwapChain->getSwapChainExtent() };
      ////	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
      ////	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

      ::cast < ::gpu_directx::context > pcontext = m_pgpucontext;

      m_hlsClear.m_dH = fmod(m_hlsClear.m_dH + 0.0001, 1.0);
      ::color::color colorClear = m_hlsClear;
      // Clear render target
      float clear[4] = { 
         colorClear.f32_red() * .5f, 
         colorClear.f32_green() * .5f, 
         colorClear.f32_blue() * .5f, .5f };


///pcontext->g_pImmediateContext->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);

      pcontext->m_pcontext->ClearRenderTargetView(m_prendertargetview->m_prendertargetview, clear);

      ////}
      on_happening(e_happening_begin_render);
   }


   void renderer::on_end_render(::gpu::frame* pframeParam)
   {

      on_happening(e_happening_end_render);

      //::cast < frame > pframe = pframeParam;

      //auto commandBuffer = pframe->commandBuffer;

      //assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
      //assert(
      //   commandBuffer == getCurrentCommandBuffer() &&
      //   "Can't end render pass on command buffer from a different frame");
      //vkCmdEndRenderPass(commandBuffer);
   }


   //void renderer::on_end_render(::graphics3d::frame * pframeParam)
   void renderer::_on_end_render()
   {

      ////::cast < frame > pframe = pframeParam;

      ////auto commandBuffer = pframe->commandBuffer;

      //auto commandBuffer = getCurrentCommandBuffer();

      //assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
      //assert(
      //   commandBuffer == getCurrentCommandBuffer() &&
      //   "Can't end render pass on command buffer from a different frame");
      //vkCmdEndRenderPass(commandBuffer);
   }


   ::pointer < ::gpu::frame > renderer::beginFrame()
   {

      //defer_layout();

      assert(!isFrameStarted && "Can't call beginFrame while already in progress");

      ////if (m_bOffScreen)
      //{

      //   auto result = m_prendertargetview->acquireNextImage();

      //   if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      //      //defer_layout();
      //      m_prendertargetview->init();
      //      //throw ::exception(todo, "resize happened?!?!");
      //      return nullptr;
      //   }
      //   if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      //      throw ::exception(error_failed, "Failed to aquire swap chain image");
      //   }

      //   isFrameStarted = true;

      //   auto commandBuffer = getCurrentCommandBuffer();

      //   VkCommandBufferBeginInfo beginInfo{};
      //   beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      //   if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
      //      throw ::exception(error_failed, "failed to begin recording command buffer!");
      //   }
        //auto pframe = __create_new < ::gpu_directx::frame >();
      //   pframe->commandBuffer = commandBuffer;
         __defer_construct(m_pframe);
      //   on_happening(e_happening_begin_frame);
      //   return m_pframe;

      //}
      ////else
      ////{


      ////	auto result = m_pvkcswapchain->acquireNextImage(&currentImageIndex);

      ////	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      ////		recreateRenderPass();
      ////		return nullptr;
      ////	}
      ////	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      ////		throw ::exception(error_failed, "Failed to aquire swap chain image");
      ////	}

      ////	isFrameStarted = true;

      ////	auto commandBuffer = getCurrentCommandBuffer();

      ////	VkCommandBufferBeginInfo beginInfo{};
      ////	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      ////	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
      ////		throw ::exception(error_failed, "failed to begin recording command buffer!");
      ////	}
      ////	return commandBuffer;

      ////}
      on_happening(e_happening_begin_frame);

      return m_pframe;

   }

   void renderer::endFrame()
   {

      on_happening(e_happening_end_frame);

      ////if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      ////{

      //assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
      //auto commandBuffer = getCurrentCommandBuffer();
      //if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
      //{
      //   throw ::exception(error_failed, "failed to record command buffer!");
      //}

      //auto result = m_prendertargetview->submitCommandBuffers(&commandBuffer);

      //if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      //   m_bNeedToRecreateSwapChain)
      //{
      //   m_bNeedToRecreateSwapChain = false;
      //   defer_update_render_target_view();
      //}
      //else if (result != VK_SUCCESS)
      //{
      //   throw ::exception(error_failed, "failed to present swap chain image!");
      //}

      //isFrameStarted = false;

      //if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      //{
      //   sample();

      //}
      ////else if (m_eoutput == ::gpu::e_output_color_and_alpha_accumulation_buffers)
      ////{

      ////	resolve_color_and_alpha_accumulation_buffers();

      ////}

      auto eoutput = m_pgpucontext->m_eoutput;

      if (eoutput == ::gpu::e_output_swap_chain)
      {

         m_pgpucontext->swap_buffers();

      }
      else if (eoutput == ::gpu::e_output_cpu_buffer)
      {

         this->sample();

      }

      ////rrentImageIndex = m_prendertargetview->currentFrame;
      ////currentFrameIndex = (currentFrameIndex + 1) % ::gpu_directx::render_target_view::MAX_FRAMES_IN_FLIGHT;

      ////}
      ////else
      ////{


      ////	assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
      ////	auto commandBuffer = getCurrentCommandBuffer();
      ////	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      ////		throw ::exception(error_failed, "failed to record command buffer!");
      ////	}
      ////	auto result = m_pvkcswapchain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
      ////	//if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      ////	//	vkcWindow.wasWindowResized()) 
      ////	//{
      ////	//	vkcWindow.resetWindowResizedFlag();
      ////	//	recreateSwapchain();
      ////	//}
      ////	//else 
      ////	//	if (result != VK_SUCCESS) {
      ////	//	throw ::exception(error_failed, "failed to present swap chain image!");
      ////	//}
      ////	isFrameStarted = false;
      ////	currentFrameIndex = (currentFrameIndex + 1) % swap_chain_render_target_view::MAX_FRAMES_IN_FLIGHT;

      ////}

   }


   //void renderer::_on_graphics_end_draw(VkImage image, const ::int_rectangle& rectangle)
   //{

   //   m_pgpucontext->set_placement(rectangle);

   //   on_new_frame();

   //   if (1)
   //   {
   //      auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();

   //      VkImageMemoryBarrier barrier = {
   //         .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //         .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //         .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //         .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //         .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //         .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //         .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //         .image = image,
   //         .subresourceRange = {
   //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //            .baseMipLevel = 0,
   //            .levelCount = 1,
   //            .baseArrayLayer = 0,
   //            .layerCount = 1
   //         },
   //      };

   //      vkCmdPipelineBarrier(
   //         cmdBuffer,
   //         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
   //         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
   //         0,
   //         0, NULL,
   //         0, NULL,
   //         1, &barrier
   //      );
   //      m_pgpucontext->endSingleTimeCommands(cmdBuffer);
   //   }


   //   if (auto pframe = beginFrame())
   //   {

   //      //on_begin_frame();
   //      // render
   //      on_begin_render(pframe);



   //      //if (m_pimpact->global_ubo_block().size() > 0)
   //      //{

   //        // update_global_ubo(m_pgpucontext);

   //      //}

   //      //m_pscene->on_render(m_pgpucontext);

   //      _blend_image(image, rectangle, false);

   //      on_end_render(pframe);

   //      endFrame();

   //   }

   //   if (1)
   //   {
   //      auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();

   //      VkImageMemoryBarrier barrier = {
   //         .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
   //         .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
   //         .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
   //         .oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
   //         .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
   //         .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //         .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
   //         .image = image,
   //         .subresourceRange = {
   //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //            .baseMipLevel = 0,
   //            .levelCount = 1,
   //            .baseArrayLayer = 0,
   //            .layerCount = 1
   //         },
   //      };

   //      vkCmdPipelineBarrier(
   //         cmdBuffer,
   //         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
   //         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
   //         0,
   //         0, NULL,
   //         0, NULL,
   //         1, &barrier
   //      );
   //      m_pgpucontext->endSingleTimeCommands(cmdBuffer);
   //   }


   //}


   void renderer::_on_graphics_end_draw(::gpu_directx::renderer* prendererSrc)
   {

      ::gpu_directx::renderer* prenderer;

      //if (prendererSrc->m_prendererResolve)
      //{

      //	prenderer = prendererSrc->m_prendererResolve;

      //}
      //else
      {

         prenderer = prendererSrc;

      }

      m_pgpucontext->set_placement(prenderer->m_pgpucontext->rectangle());

      //VkImage image = prenderer->m_prendertargetview->m_images[prenderer->get_frame_index()];

      defer_update_renderer();

      //on_new_frame();

      //if (1)
      //{
      //   //auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();

      //   //VkImageMemoryBarrier barrier = {
      //   //    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      //   //    .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //   //    .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
      //   //    .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //   //    .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //   //    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //   //    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      //   //    .image = image,
      //   //    .subresourceRange = {
      //   //        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //   //        .baseMipLevel = 0,
      //   //        .levelCount = 1,
      //   //        .baseArrayLayer = 0,
      //   //        .layerCount = 1
      //   //    },
      //   //};

      //   //vkCmdPipelineBarrier(
      //   //   cmdBuffer,
      //   //   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      //   //   VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      //   //   0,
      //   //   0, NULL,
      //   //   0, NULL,
      //   //   1, &barrier
      //   //);

      //   //VkSubmitInfo submitInfo = {};
      //   //submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

      //   //VkSemaphore waitSemaphores[] = { prendererSrc->m_prendertargetview->renderFinishedSemaphores[prendererSrc->m_prendertargetview->currentFrame] };
      //   //VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
      //   //submitInfo.waitSemaphoreCount = 1;
      //   //submitInfo.pWaitSemaphores = waitSemaphores;
      //   //submitInfo.pWaitDstStageMask = waitStages;

      //   //submitInfo.commandBufferCount = 1;
      //   //submitInfo.pCommandBuffers = &cmdBuffer;

      //   //m_pgpucontext->endSingleTimeCommands(cmdBuffer, 1, &submitInfo);
      //   auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();


      //   insertImageMemoryBarrier(cmdBuffer,
      //      image,
      //      0,
      //      VK_ACCESS_TRANSFER_WRITE_BIT,
      //      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //      VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      VK_PIPELINE_STAGE_TRANSFER_BIT,
      //      VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });



      //   VkSubmitInfo submitInfo{};
      //   submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      //   submitInfo.commandBufferCount = 1;
      //   submitInfo.pCommandBuffers = &cmdBuffer;
      //   ::array<VkSemaphore> waitSemaphores;
      //   ::array<VkPipelineStageFlags> waitStages;
      //   waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
      //   waitSemaphores.add(prenderer->m_prendertargetview->renderFinishedSemaphores[prenderer->get_frame_index()]);
      //   submitInfo.waitSemaphoreCount = waitSemaphores.size();
      //   submitInfo.pWaitSemaphores = waitSemaphores.data();
      //   submitInfo.pWaitDstStageMask = waitStages.data();
      //   m_pgpucontext->endSingleTimeCommands(cmdBuffer, 1, &submitInfo);

      //   //m_prendererResolve->m_prendertargetview->m_semaphoreaWaitToSubmit.add(
      //   //   m_prendertargetview->renderFinishedSemaphores[iPassCurrentFrame]
      //   //);


      //}

      //if (auto pframe = beginFrame())
      //{

      //   m_prendertargetview->m_semaphoreaSignalOnSubmit.add(prendererSrc->m_prendertargetview->imageAvailableSemaphores[prendererSrc->get_frame_index()]);


      //   //on_begin_frame();
      //   // render
      //   on_begin_render(pframe);



      //   //if (m_pimpact->global_ubo_block().size() > 0)
      //   //{

      //     // update_global_ubo(m_pgpucontext);

      //   //}

      //   //m_pscene->on_render(m_pgpucontext);

      //   _blend_image(image, m_pgpucontext->rectangle(), false);

      //   on_end_render(pframe);

      //   endFrame();

      //}

      ////if (1)
      ////{
      ////   auto cmdBuffer = m_pgpucontext->beginSingleTimeCommands();

      ////   VkImageMemoryBarrier barrier = {
      ////       .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      ////       .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
      ////      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      ////      .oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      ////       .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      ////       .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      ////       .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      ////       .image = image,
      ////       .subresourceRange = {
      ////           .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      ////           .baseMipLevel = 0,
      ////           .levelCount = 1,
      ////           .baseArrayLayer = 0,
      ////           .layerCount = 1
      ////       },
      ////   };

      ////   vkCmdPipelineBarrier(
      ////      cmdBuffer,
      ////      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      ////      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      ////      0,
      ////      0, NULL,
      ////      0, NULL,
      ////      1, &barrier
      ////   );
      ////   m_pgpucontext->endSingleTimeCommands(cmdBuffer);
      ////}


   }


   //void renderer::_on_frame_draw(::gpu_directx::renderer* prendererUpper)
   //{




   //}


   void renderer::endDraw(::user::interaction* puserinteraction, ::gpu::renderer* pgpurendererSrc)
   {

      ::cast < renderer > prenderer = pgpurendererSrc;

      //VkImage vkimage = prenderer->m_prendertargetview->m_images[prenderer->get_frame_index()];

      //::int_rectangle rectangle = prenderer->m_pgpucontext->rectangle();

      //auto copyCmd = m_pgpucontext->beginSingleTimeCommands();

      //insertImageMemoryBarrier(
      //   copyCmd,
      //   vkimage,
      //   0,
      //   VK_ACCESS_TRANSFER_WRITE_BIT,
      //   VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      //   VK_PIPELINE_STAGE_TRANSFER_BIT,
      //   VK_PIPELINE_STAGE_TRANSFER_BIT,
      //   VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      //VkSubmitInfo submitInfo{};
      //submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      //submitInfo.commandBufferCount = 1;
      //submitInfo.pCommandBuffers = &copyCmd;
      //::array<VkSemaphore> waitSemaphores;
      //::array<VkPipelineStageFlags> waitStages;
      //waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
      //waitSemaphores.add(prenderer->m_prendertargetview->renderFinishedSemaphores[prenderer->get_frame_index()]);
      //submitInfo.waitSemaphoreCount = waitSemaphores.size();
      //submitInfo.pWaitSemaphores = waitSemaphores.data();
      //submitInfo.pWaitDstStageMask = waitStages.data();

      //m_pgpucontext->endSingleTimeCommands(copyCmd);

      //defer_update_render_target_view();

      //on_new_frame();

      //auto pframe = beginFrame();

      //on_begin_render(pframe);

      //_copy_image(vkimage, rectangle, false);

      //on_end_render(pframe);

      //endFrame();
  
      ////vkQueueWaitIdle(m_pgpucontext->graphicsQueue());

      ////vkQueueWaitIdle(m_pgpucontext->presentQueue());

   }


} // namespace gpu_directx



