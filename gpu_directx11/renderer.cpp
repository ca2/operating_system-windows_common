#include "framework.h"
#include "approach.h"
#include "descriptors.h"
#include "frame.h"
#include "input_layout.h"
#include "renderer.h"
#include "texture.h"
#include "offscreen_render_target_view.h"
#include "physical_device.h"
#include "swap_chain_render_target_view.h"
#include "initializers.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context_lock.h"
#include "bred/gpu/cpu_buffer.h"
#include "bred/gpu/render_state.h"
#include "bred/gpu/swap_chain.h"
#include "gpu_directx11/shader.h"
#include "acme/parallelization/synchronous_lock.h"
#include "acme/platform/application.h"
#include "aura/graphics/image/target.h"
#include "aura/user/user/interaction.h"
#include "aura/windowing/window.h"
//#include "tools.h"
//#include "bred/user/user/graphics3d.h"

//#include <array>
#include <cassert>
//#include <stdexcept>
//

using namespace directx11;


namespace gpu_directx11
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
    static const char * g_uaImageBlendFragmentShader = R"(
#include "shader/image_blend.frag"
)";
 //   //
 //   // Vertex shader (GLSL -> SPIR-V):
 //   // layout(location = 0) in vec2 inPos;
 //   // layout(location = 1) in vec2 inUV;
 //   // layout(location = 0) out vec2 fragUV;
 //   // void main() {
 //   //     fragUV = inUV;
 //   //     gl_Position = vec4(inPos, 0.0, 1.0);
 //   // }
   static const char * g_uaImageBlendVertexShader = R"(
#include "shader/image_blend.vertr"
)";



 //   static unsigned int g_uaResolveFragmentShader[] = {
 // #include "shader/resolve.frag.spv.inl"
 //   };


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


   void renderer::initialize_gpu_renderer(::gpu::context* pgpucontext)
   {

      ::gpu::renderer::initialize_gpu_renderer(pgpucontext);

      m_pgpucontext = pgpucontext;

      if (m_pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      {

         //m_pimpact = pgpucontext->m_pimpact;

         pgpucontext->create_cpu_buffer(pgpucontext->rectangle().size());

         øconstruct_new(m_pcpubuffersampler);

         m_pcpubuffersampler->initialize_cpu_buffer_sampler(pgpucontext);

         m_pcpubuffersampler->m_prenderer = this;

      }

      //m_poffscreensampler->set_storing_flag

      //defer_layout();

      create_command_buffers();

   }


   renderer::~renderer()
   {

      free_command_buffers();

   }


   //int renderer::get_frame_index() const
   //{

   //   assert(m_iFrameSerial2 >= 0
   //      && m_iCurrentFrame2 >= 0
   //      && m_estate != e_state_initial
   //      && "Cannot get frame index when frame not in progress");

   //   return (int)m_iCurrentFrame2;

   //}


   //void renderer::on_new_frame()
   //{

   //   ::gpu::renderer::on_new_frame();

   //}


   //void renderer::restart_frame_counter()
   //{

   //   m_iCurrentFrame2 = -1;
   //   m_iFrameSerial2 = -1;

   //   on_happening(e_happening_reset_frame_counter);

   //}



   //int renderer::get_frame_count() const
   //{

   //   return ::gpu_directx11::render_target_view::MAX_FRAMES_IN_FLIGHT;

   //}


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
      //	vkcSwapChain = øcreate_pointer<swap_chain_render_target_view>(m_pgpucontext, extent);
      //}
      //else {
      //	::pointer<swap_chain_render_target_view> oldSwapChain = std::move(vkcSwapChain);
      //	vkcSwapChain = øcreate_pointer<swap_chain_render_target_view>(m_pgpucontext, extent, oldSwapChain);
      //	if (!oldSwapChain->compareSwapFormats(*vkcSwapChain.get())) {
      //		throw ::exception(error_failed, "Swap chain image(or depth) format has changed!");
      //	}

      //}
   }


   //void renderer::on_defer_update_renderer_allocate_render_target(::gpu::enum_output eoutput, const ::int_size& size, ::gpu::render_target* previous)
   //{

   //   //auto eoutput = m_pgpucontext->m_eoutput;

   //   auto prendertargetOld = m_pgpurendertarget;

   //   if (eoutput == ::gpu::e_output_cpu_buffer
   //      || eoutput == ::gpu::e_output_gpu_buffer)
   //   {

   //      //auto poffscreenrendertargetview = øallocate offscreen_render_target_view(this, size, m_pgpurendertarget);
   //      auto poffscreenrendertargetview = øallocate offscreen_render_target_view();

   //      //#ifdef WINDOWS_DESKTOP
   //      //         poffscreenrendertargetview->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
   //      //#else
   //      //         poffscreenrendertargetview->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
   //      //#endif
   //      m_pgpurendertarget = poffscreenrendertargetview;
   //      //         //m_prendererResolve.release();
   //      //
   //   }
   //   else if (eoutput == ::gpu::e_output_swap_chain)
   //   {

   //      auto pswapchain = m_pgpucontext->m_pgpudevice->get_swap_chain();

   //      m_pgpurendertarget = pswapchain;

   //      //m_prendertargetview = øallocate swap_chain_render_target_view(this, size, m_prendertargetview);
   //      //m_prendererResolve.release();

   //   }
   //   //      else if (eoutput == ::gpu::e_output_gpu_buffer)
   //   //      {
   //   //
   //   //         auto poffscreenrendertargetview = øallocate offscreen_render_target_view(this, m_extentRenderer, m_prendertargetview);
   //   //#ifdef WINDOWS_DESKTOP
   //   //         poffscreenrendertargetview->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
   //   //#else
   //   //         poffscreenrendertargetview->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
   //   //#endif
   //   //         m_prendertargetview = poffscreenrendertargetview;
   //   //         //m_prendererResolve;
   //   //
   //   //      }
   //   //      else if (eoutput == ::gpu::e_output_color_and_alpha_accumulation_buffers)
   //   //      {
   //   //
   //   //         auto paccumulationrendertargetview = øallocate accumulation_render_target_view(this, m_extentRenderer, m_prendertargetview);
   //   //         paccumulationrendertargetview->m_formatImage = VK_FORMAT_R32G32B32A32_SFLOAT;
   //   //         paccumulationrendertargetview->m_formatAlphaAccumulation = VK_FORMAT_R32_SFLOAT;
   //   //         m_prendertargetview = paccumulationrendertargetview;
   //   //
   //   //         //øconstruct_new(m_prendererResolve);
   //   //
   //   //         //m_prendererResolve->initialize_renderer(m_pgpucontext, ::gpu::e_output_resolve_color_and_alpha_accumulation_buffers);
   //   //
   //   //         //m_prendererResolve->set_placement(m_pgpucontext->rectangle);
   //   //         //
   //   //         //            auto poffscreenrendertargetview = øallocate offscreen_render_target_view(m_pgpucontext, m_extentRenderer, m_prendertargetviewResolve);
   //   //         //#ifdef WINDOWS_DESKTOP
   //   //         //            poffscreenrendertargetview->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
   //   //         //#else
   //   //         //            poffscreenrendertargetview->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
   //   //         //#endif
   //   //         //            m_prendertargetviewResolve = poffscreenrendertargetview;
   //   //      }
   //   //      else if (eoutput == ::gpu::e_output_resolve_color_and_alpha_accumulation_buffers)
   //   //      {
   //   //
   //   //         auto poffscreenrendertargetview = øallocate offscreen_render_target_view(this, m_extentRenderer, m_prendertargetview);
   //   //#ifdef WINDOWS_DESKTOP
   //   //         poffscreenrendertargetview->m_formatImage = VK_FORMAT_B8G8R8A8_UNORM;
   //   //#else
   //   //         poffscreenrendertargetview->m_formatImage = VK_FORMAT_R8G8B8A8_UNORM;
   //   //#endif
   //   //         m_prendertargetview = poffscreenrendertargetview;
   //   //
   //   //      }
   //   //      else
   //   //      {
   //   //
   //   //         throw ::exception(error_wrong_state, "Unexpected gpu e_output");
   //   //
   //   //      }
   //   //
   ////   if (!m_pgpurendertarget->has_ok_flag() && m_sizeRenderer.area() > 0)
   ////   {
   ////
   ////      m_pgpurendertarget->initialize_render_target(this, size, prendertargetOld);
   ////
   ////      m_pgpurendertarget->init();
   ////
   ////   }
   ////
   ////}
   ////

   //}


   ::pointer < ::gpu::render_target > renderer::allocate_offscreen_render_target()
   {

      auto poffscreenrendertargetview = øallocate offscreen_render_target_view();

      return poffscreenrendertargetview;

   }


   void renderer::create_command_buffers()
   {

      m_commandbuffera.set_size(m_iDefaultFrameCount);

      for(auto & pcommandbuffer : m_commandbuffera)
      {
         
         ødefer_construct(pcommandbuffer);
         
         pcommandbuffer->initialize_command_buffer(
            m_pgpurendertarget,
            m_pgpucontext->m_pgpudevice->graphics_queue(),
            ::gpu::e_command_buffer_graphics);

      }

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


   void renderer::free_command_buffers()
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

         ::cast < render_target_view > pgpurendertargetview = m_pgpurendertarget;

         auto result = pgpurendertargetview->acquireNextImage();

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
         //auto pframe = øcreate_new < frame >();
         //pframe->commandBuffer = commandBuffer;
         //m_pframe = pframe;
         //return m_pframe;

      }
      //else
      //{


      //	auto result = m_pvkcswapchain->acquireNextImage(&m_uCurrentSwapChainImage);

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

      throw todo;
      //_on_begin_render();

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


   void renderer::cpu_buffer_sampler::sample(ID3D11Texture2D * ptextureSource)
   {

      D3D11_TEXTURE2D_DESC texture2ddescSource;

      ptextureSource->GetDesc(&texture2ddescSource);

      ::cast < ::gpu_directx11::context > pcontext = m_pgpucontext;

      ::gpu::context_lock context_lock(m_pgpucontext);

      if (!m_ptextureStaging
         || texture2ddescSource.Width != m_sizeStaging.width()
         || texture2ddescSource.Height != m_sizeStaging.height())
      {

         m_ptextureStaging.Release();

         if (texture2ddescSource.Format != DXGI_FORMAT_B8G8R8A8_UNORM)
         {

            warning() << "Unsupported format for readback.";

            throw ::exception(error_wrong_state);

         }

         D3D11_TEXTURE2D_DESC texture2ddescStaging = texture2ddescSource;
         texture2ddescStaging.BindFlags = 0;
         texture2ddescStaging.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
         texture2ddescStaging.Usage = D3D11_USAGE_STAGING;
         texture2ddescStaging.MiscFlags = 0;

         ::cast < ::gpu_directx11::device > pdevice = pcontext->m_pgpudevice;

         if (FAILED(pdevice->m_pdevice->CreateTexture2D(
            &texture2ddescStaging, NULL, &m_ptextureStaging)))
         {
            
            warning() << "Failed to create staging texture.";

            throw ::exception(error_wrong_state);

         }

         m_sizeStaging.cx = texture2ddescSource.Width;

         m_sizeStaging.cy = texture2ddescSource.Height;

      }

      pcontext->m_pcontext->Flush();

      pcontext->m_pcontext->CopyResource(
         (ID3D11Resource*)m_ptextureStaging, 
         (ID3D11Resource*)ptextureSource);

   }


   void renderer::cpu_buffer_sampler::send_sample()
   {

      D3D11_MAPPED_SUBRESOURCE mapped;

      ::cast < ::gpu_directx11::context > pcontext = m_pgpucontext;

      ::gpu::context_lock context_lock(m_pgpucontext);

      if (FAILED(pcontext->m_pcontext->Map((ID3D11Resource*)m_ptextureStaging, 0, D3D11_MAP_READ, 0, &mapped)))
      {
         
         warning() << "Failed to map_base staging texture.";
         
         throw ::exception(error_wrong_state);

      }

      D3D11_TEXTURE2D_DESC desc;

      m_ptextureStaging->GetDesc(&desc);

      int width = desc.Width;
      int height = desc.Height;
      UINT rowPitch = mapped.RowPitch;
      auto data = mapped.pData;

      auto pgpucontext = m_pgpucontext;

      auto pcpubuffer = pgpucontext->m_pcpubuffer;

      /*memory m;

      m.set_size(height * rowPitch);

      m.set(255);*/

      pcpubuffer->set_pixels(
         data,
         width,
         height,
         (int)rowPitch,
         false);

      pcontext->m_pcontext->Unmap((ID3D11Resource*)m_ptextureStaging, 0);

   }


   void renderer::do_sampling_to_cpu()
   {

      ::cast< context > pgpucontext = m_pgpucontext;

      ::gpu::context_lock context_lock(m_pgpucontext);

      ::cast< renderer > prenderer = this;

      ::cast < render_target_view > prendertargetview = prenderer->m_pgpurendertarget;

      ::cast < offscreen_render_target_view > poffscreenrendertargetview = prendertargetview;

      ::cast< device > pgpudevice = pgpucontext->m_pgpudevice;

      ID3D11Device* device = pgpudevice->m_pdevice;

      ID3D11DeviceContext* context = pgpucontext->m_pcontext;

      ::cast < ::gpu_directx11::texture > ptexture = poffscreenrendertargetview->current_texture(::gpu::current_frame());

      ID3D11Texture2D* offscreenTexture = ptexture->m_ptextureOffscreen;

      if (!device || !context || !offscreenTexture)
      {
         
         throw ::exception(error_wrong_state);

      }

      m_pcpubuffersampler->sample(offscreenTexture);

      m_pcpubuffersampler->send_sample();


   }


   void renderer::gpu_blend(::draw2d::graphics * pgraphics)
   {

      m_pcpubuffersampler->send_sample();

   }


   void renderer::swap_chain()
   {


   }


   float renderer::getAspectRatio() const
   {

      //if (m_bOffScreen)
      {

         ::cast < render_target_view > prendertargetview = m_pgpurendertarget;

         return prendertargetview->extentAspectRatio();

      }
      //else
      //{

      //	return m_pvkcswapchain->extentAspectRatio();

      //}

   }


   void renderer::on_end_draw()
   {

      throw todo;

      //_on_end_render();

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
      //   auto result = m_prendertargetview->submitCommandBuffers(&commandBuffer, &m_uCurrentSwapChainImage);
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
         
         do_sampling_to_cpu();

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
      //	auto result = m_pvkcswapchain->submitCommandBuffers(&commandBuffer, &m_uCurrentSwapChainImage);
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

         auto pinputlayoutEmpty = øcreate <::gpu::input_layout >();

         //pshadervertexinput->m_bindings.add(
         //   {
         //      .binding = 0,
         //      .stride = sizeof(float) * 4,
         //      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
         //   });

         //pshadervertexinput->m_attribs.add({ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 0 });
         //pshadervertexinput->m_attribs.add({ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = sizeof(float) * 2 });

         auto pshaderImageBlend = øcreate_new<::gpu_directx11::shader>();

         m_pshaderImageBlend = pshaderImageBlend;

         pshaderImageBlend->m_bEnableBlend = true;
         pshaderImageBlend->m_bDisableDepthTest = true;

         ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

         pshaderImageBlend->initialize_shader_with_block(
            this, as_block(g_uaImageBlendVertexShader), as_block(g_uaImageBlendFragmentShader),
            {::gpu::shader::e_descriptor_set_slot_local}, m_psetdescriptorlayoutImageBlend, 
            //{},
            pinputlayoutEmpty);

      }

      return m_pshaderImageBlend;

   }


   ::gpu::shader* renderer::get_image_set_shader()
   {

      if (!m_pshaderImageBlend)
      {

         auto pinputlayoutEmpty = øcreate< ::gpu::input_layout >();

         //pshadervertexinput->m_bindings.add(
         //   {
         //      .binding = 0,
         //      .stride = sizeof(float) * 4,
         //      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
         //   });

         //pshadervertexinput->m_attribs.add({ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 0 });
         //pshadervertexinput->m_attribs.add({ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = sizeof(float) * 2 });

         auto pshaderImageBlend = øcreate_new<::gpu_directx11::shader>();

         m_pshaderImageBlend = pshaderImageBlend;

         ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;

         pshaderImageBlend->initialize_shader_with_block(
            this,
            as_memory_block(g_uaImageBlendVertexShader),
            as_memory_block(g_uaImageBlendFragmentShader),
            { ::gpu::shader::e_descriptor_set_slot_local },
            m_psetdescriptorlayoutImageBlend,
            pinputlayoutEmpty);

      }

      return m_pshaderImageBlend;

   }


   void renderer::_blend_renderer(::gpu_directx11::renderer* prendererSrc, bool bYSwap)
   {


   }


   void renderer::_on_begin_render(::gpu::frame * pgpuframe)
   {

      ::cast < frame > pframe = pgpuframe;

      ::cast < ::gpu_directx11::context > pgpucontext = m_pgpucontext;

      ::cast < device > pdevice = pgpucontext->m_pgpudevice;

      ::gpu::context_lock context_lock(m_pgpucontext);

      auto pcontext = pgpucontext->m_pcontext;

      ::cast < render_target_view > pgpurendertargetview = m_pgpurendertarget;

      auto size = pgpucontext->m_rectangle.size();

      if (pgpurendertargetview)
      {

         ::cast < texture > ptexture = pgpurendertargetview->current_texture(pframe);

         ::comptr < ID3D11RenderTargetView > prendertargetview;

         ::comptr < ID3D11DepthStencilView > pdepthstencilview;

         if (!m_pdepthstencilstateForCleaning)
         {
            ::cast < ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;
            D3D11_DEPTH_STENCIL_DESC dsDesc = {};

dsDesc.DepthEnable = TRUE;
dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

HRESULT hrCreateDepthStencilState = pgpudevice->m_pdevice->CreateDepthStencilState(&dsDesc,
   &m_pdepthstencilstateForCleaning);
::defer_throw_hresult(hrCreateDepthStencilState);

         }

         //::comptr < ID3D11DepthStencilState > pdepthstencilstate;

         if (ptexture)
         {

            if (m_pgpucontext->m_escene == ::gpu::e_scene_2d)
            {

               if (!ptexture->m_prendertargetview)
               {

                  ptexture->create_render_target();

               }

            }
            else if(m_pgpucontext->m_escene == ::gpu::e_scene_3d)
            {

               if (!ptexture->m_prendertargetview)
               {

                  ptexture->create_render_target();


               }

               if (!ptexture->m_pshaderresourceview)
               {

                  ptexture->create_shader_resource_view();

               }

               if (!ptexture->m_pdepthstencilview)
               {

                  ptexture->create_depth_resources();

               }

               //pdepthstencilstate = ptexture->m_pdepthstencilstate;

               pdepthstencilview = ptexture->m_pdepthstencilview;

            }

            prendertargetview = ptexture->m_prendertargetview;

         }

         if (prendertargetview)
         {

            if (m_pdepthstencilstateForCleaning && pdepthstencilview)
            { 

               pcontext->OMSetDepthStencilState(m_pdepthstencilstateForCleaning, 0);

               pcontext->OMSetRenderTargets(1, ptexture->m_prendertargetview.pp(), pdepthstencilview);

               pcontext->ClearDepthStencilView(pdepthstencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);

            }
            else
            {

               pcontext->OMSetRenderTargets(1, ptexture->m_prendertargetview.pp(), nullptr);

            }

         }


         D3D11_VIEWPORT vp = {};
         vp.TopLeftX = 0;
         vp.TopLeftY = 0;
         vp.Width = static_cast<float>(size.width());
         vp.Height = static_cast<float>(size.height());
         vp.MinDepth = 0.0f;
         vp.MaxDepth = 1.0f;

         pcontext->RSSetViewports(1, &vp);

         D3D11_RECT scissorRect;

         scissorRect.left = 0;
         scissorRect.top = 0;
         scissorRect.right = size.width();
         scissorRect.bottom = size.height();

         pcontext->RSSetScissorRects(1, &scissorRect);



         ////::cast < offscreen_render_target_view > poffscreenrendertargetview = pgpurendertargetview;

         ////if (poffscreenrendertargetview)
         //{

         //   auto psamplerstate = ptexture->m_psamplerstate;

         //   if (psamplerstate)
         //   {

         //      pcontext->PSSetSamplers(0, 1, psamplerstate.pp());

         //   }

         //}

      }

      //if (!pgpucontext->m_prasterizerstate)
      //{

      //   D3D11_RASTERIZER_DESC rasterizerDesc = {};
      //   rasterizerDesc.FillMode = D3D11_FILL_SOLID;
      //   rasterizerDesc.CullMode = D3D11_CULL_BACK;        // Cull back faces
      //   //rasterizerDesc.CullMode = D3D11_CULL_FRONT; 
      //   rasterizerDesc.FrontCounterClockwise = false; // Treat CCW as front-facing
      //   //rasterizerDesc.FrontCounterClockwise = true;
      //   rasterizerDesc.DepthClipEnable = TRUE;

      //   HRESULT hr = pgpucontext->m_pgpudevice->m_pdevice->CreateRasterizerState(
      //      &rasterizerDesc,
      //      &pgpucontext->m_prasterizerstate);

      //   ::defer_throw_hresult(hr);

      //}

      //pgpucontext->m_pcontext->RSSetState(pgpucontext->m_prasterizerstate);

      //::cast < ::gpu_directx11::context > pcontext = m_pgpucontext;

      m_hlsClear.m_dH = fmod(m_hlsClear.m_dH + 0.0001, 1.0);

      //::color::color colorClear = m_hlsClear;

      ::color::color colorClear(::color::transparent);

      float clear[4] = {
         colorClear.f32_red() * colorClear.f32_opacity(),
         colorClear.f32_green() * colorClear.f32_opacity(),
         colorClear.f32_blue() * colorClear.f32_opacity(),
         colorClear.f32_opacity()};

      //::cast < render_target_view > pgpurendertargetview = m_pgpurendertarget;

      ::cast < texture > ptexture = pgpurendertargetview->current_texture(::gpu::current_frame());

      if (!ptexture->m_prendertargetview)
      {

         ptexture->create_render_target();

      }

      pgpucontext->m_pcontext->ClearRenderTargetView(
         ptexture->m_prendertargetview, clear);

   }


   //void renderer::on_begin_render(::gpu::frame* pframeParam)
   //{

   //   ::cast < ::gpu_directx11::context > pcontext = m_pgpucontext;

   //   m_hlsClear.m_dH = fmod(m_hlsClear.m_dH + 0.0001, 1.0);

   //   ::color::color colorClear = m_hlsClear;
   //   
   //   float clear[4] = { 
   //      colorClear.f32_red() * .5f, 
   //      colorClear.f32_green() * .5f, 
   //      colorClear.f32_blue() * .5f, .5f };

   //   ::cast < render_target_view > pgpurendertargetview = m_pgpurendertarget;

   //   ::cast < texture > ptexture = pgpurendertargetview->current_texture();

   //   pcontext->m_pcontext->ClearRenderTargetView(ptexture->m_prendertargetview, clear);

   //   on_happening(e_happening_begin_render);

   //}


   //void renderer::on_end_render(::gpu::frame* pframeParam)
   //{

   //   on_happening(e_happening_end_render);

   //}


   void renderer::_on_end_render(::gpu::frame * pframe)
   {

      
   }


   void renderer::on_end_layer(::gpu::layer* player)
   {

      ::gpu::renderer::on_end_layer(player);

   }


   ::pointer < ::gpu::frame > renderer::beginFrame()
   {

      return ::gpu::renderer::beginFrame();

      //assert(!isFrameStarted && "Can't call beginFrame while already in progress");

      //ødefer_construct(m_pgpurendertarget->m_pgpuframe);
      //
      //m_prenderstate->on_happening(::gpu::e_happening_begin_frame);

      //isFrameStarted = true;

      //return m_pgpurendertarget->m_pgpuframe;

   }


   void renderer::endFrame()
   {

      m_prenderstate->on_happening(::gpu::e_happening_end_frame);

      defer_end_frame_layer_copy();

      auto eoutput = m_pgpucontext->m_eoutput;

      if (eoutput == ::gpu::e_output_swap_chain)
      {

         m_pgpucontext->get_swap_chain()->swap_buffers();

      } 
      else if (eoutput == ::gpu::e_output_cpu_buffer)
      {

         this->do_sampling_to_cpu();

      }

      isFrameStarted = false;

   }


   void renderer::copy(::gpu::texture* pgputextureTarget, ::gpu::texture* pgputextureSource)
   {

      ::cast < context > pgpucontext = m_pgpucontext;

      pgpucontext->copy(pgputextureTarget, pgputextureSource);

   }


   void renderer::_on_graphics_end_draw(::gpu_directx11::renderer* prendererSrc)
   {

      ::gpu_directx11::renderer* prenderer;

      prenderer = prendererSrc;

      m_pgpucontext->set_placement(prenderer->m_pgpucontext->rectangle());

      defer_update_renderer();

   }


   void renderer::endDraw(::gpu::graphics * pgraphics, ::user::interaction* puserinteraction)
   {

      ::cast < renderer > prenderer = this;

      //if (m_pgpucontext->m_eoutput == ::gpu::e_output_swap_chain)
      //{

      //   m_pgpucontext->swap_buffers();

      //}
      //else 
       /*  if (m_pgpucontext->m_pgpudevice->m_edevicetarget == ::gpu::e_device_target_swap_chain)
      {
         
         auto pswapchain = m_pgpucontext->m_pgpudevice->get_swap_chain();

         pswapchain->endDraw(pgraphics, puserinteraction, this);

      }*/

   }


} // namespace gpu_directx11



