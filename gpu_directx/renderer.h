#pragma once

//#include "AppCore/vk_window.h"
//#include "cube/container.h"
#include "context.h"
//#include "offscreen.h"
//#include "swapchain.h"
#include "render_target_view.h"
#include "aura/graphics/gpu/renderer.h"


constexpr unsigned int FRAME_OVERLAP = 2;


namespace gpu_directx
{

   class CLASS_DECL_GPU_DIRECTX shader_vertex_input :
      virtual public ::particle
   {
   public:

      //::array < VkVertexInputBindingDescription >     m_bindings;
      //::array < VkVertexInputAttributeDescription >   m_attribs;

   };


   class CLASS_DECL_GPU_DIRECTX renderer :
      virtual public ::gpu::renderer
   {
   public:

      ::color::hls m_hlsClear;

      struct CLASS_DECL_GPU_DIRECTX cpu_buffer_sampler :
         virtual public ::particle
      {


         //VkExtent2D			      m_vkextent2d;
         //VkDeviceMemory		      m_vkdevicememory;
         //VkImage				      m_vkimage;


         ::pointer < context >   m_pgpucontext;
         ::pointer < renderer >  m_prenderer;
         comptr<ID3D11Texture2D>       m_ptextureStaging;
         ::int_size              m_size;

         cpu_buffer_sampler();
         ~cpu_buffer_sampler();


         void initialize_cpu_buffer_sampler(::gpu::context* pgpucontext);

         void clear();
         //void update(VkExtent2D vkextent2d);
         void destroy();

         void sample(ID3D11Texture2D* ptexture);

         void send_sample();

      };


      class descriptor :
         virtual public ::particle
      {
      public:
         //::array<VkDescriptorSet>   m_descriptorsets;
         //VkPipelineLayout		      m_vkpipelinelayout = nullptr;
      };

      class model :
         virtual public ::particle
      {
      public:
         //VkBuffer m_vertexBuffer = nullptr;
         //VkDeviceMemory m_vertexMemory = nullptr;
         //VkBuffer m_indexBuffer = nullptr;
         //VkDeviceMemory m_indexMemory = nullptr;
      };

      ::pointer<::gpu::shader>                        m_pshaderImageBlend;
      ::pointer<::gpu::shader>                        m_pshaderImageSet;
      //map < VkImage, ::pointer < descriptor > >       m_imagedescriptor;
      //map < VkImage, ::pointer < model > >       m_imagemodel;
      //::pointer < ::user::graphics3d >	m_pimpact;
      ::pointer < context >				               m_pgpucontext;
      ::pointer < cpu_buffer_sampler >	               m_pcpubuffersampler;
      //::pointer<swap_chain_render_target_view>			m_pvkcswapchain;
      //::pointer<offscreen_render_target_view>			m_pvkcoffscreen;
      ::pointer<render_target_view>			            m_prendertargetview;
      //::pointer<renderer>			                     m_prendererResolve;
      //::pointer<::gpu::shader>                        m_pshaderResolve;
      //::pointer<model>                                m_pmodelResolve;

      //::array<VkCommandBuffer>	commandBuffers;
      //VkExtent2D m_extentRenderer;
      //int currentFrameIndex = 0;
      bool isFrameStarted = false;
      bool m_bNeedToRecreateSwapChain = false;
      //bool m_bOffScreen = true;
      //renderer(VkWindow &window, context * pvkcdevice);

      ::pointer<::gpu_directx::set_descriptor_layout>           m_psetdescriptorlayoutImageBlend;
      ::pointer <::gpu_directx::descriptor_pool>                m_pdescriptorpoolImageBlend;


      ::pointer<::gpu_directx::set_descriptor_layout>           m_psetdescriptorlayoutResolve;
      ::pointer <::gpu_directx::descriptor_pool>                m_pdescriptorpoolResolve;
      ::pointer < descriptor >                                 m_pdescriptorResolve;

      ::procedure_array m_procedureaAfterEndRender;

      ::collection::index m_iCurrentFrame2 = -1;
      ::collection::index m_iFrameSerial2 = -1;

      ::int_size m_sizeRenderer;


      renderer();
      ~renderer();


      virtual void restart_frame_counter();
      void on_new_frame() override;
      void initialize_renderer(::gpu::context* pgpucontext, ::gpu::enum_output eoutput, ::gpu::enum_scene escene) override;

      //int width()  override;
      //int height() override;

      bool is_starting_frame()const
      {

         return m_iFrameSerial2 == m_iCurrentFrame2;

      }

      //VkRenderPass getRenderPass() const
      //{

      //   return m_prendertargetview->getRenderPass();
      //   //return m_bOffScreen ?
      //      //m_pvkcoffscreen->getRenderPass():
      //      //m_pvkcswapchain->getRenderPass(); 

      //}

      void sample();
      void gpu_blend(::draw2d::graphics * pgraphics);
      //void resolve_color_and_alpha_accumulation_buffers();
      //void _resolve_color_and_alpha_accumulation_buffers();
      void swap_chain();

      float getAspectRatio() const
      {

         //if (m_bOffScreen)
         {

            return m_prendertargetview->extentAspectRatio();

         }
         //else
         //{

         //	return m_pvkcswapchain->extentAspectRatio();

         //}

      }


      bool isFrameInProgress() const { return isFrameStarted; }


      //VkCommandBuffer getCurrentCommandBuffer() const {
      //   assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
      //   return commandBuffers[get_frame_index()];
      //}

      int get_frame_index() const override;
      int get_frame_count() const override;

      void defer_update_renderer() override;

      //::pointer < ::graphics3d::frame> beginFrame() override;
      //void endFrame() override;
      //void on_begin_render(::graphics3d::frame * pframe) override;
      //void on_end_render(::graphics3d::frame * pframe) override;

      void on_context_resize() override;

      void on_begin_draw() override;
      virtual void _on_begin_render();
      virtual void _on_end_render();
      void on_end_draw() override;

   //public:

      void createCommandBuffers();

      void freeCommandBuffers();
      //void recreateSwapchain();
      //void set_size(const ::int_size & size) override;

      //void prepareOffScreen();

      ::pointer < ::gpu::frame > beginFrame() override;
      void on_begin_render(::gpu::frame* pframeParam) override;
      void on_end_render(::gpu::frame* pframeParam) override;
      void endFrame() override;
      void endDraw(::user::interaction * puserinteraction, ::gpu::renderer* pgpurendererSrc) override;


      //void _set_image(VkImage image, const ::int_rectangle& rectangle, bool bYSwap);

      //void _blend_image(VkImage image, const ::int_rectangle& rectangle, bool bYSwap);
      //void _on_graphics_end_draw(VkImage image, const ::int_rectangle& rectangle);

      void _blend_renderer(::gpu_directx::renderer* prendererSrc, bool bYSwap);
      void _on_graphics_end_draw(::gpu_directx::renderer * prendererSrc);

      //void _on_frame_draw(::gpu_directx::renderer* prendererUpper);

      //void _copy_image(VkImage image, const ::int_rectangle& rectangle, bool bYSwap);

      ::gpu::shader * get_image_blend_shader();

      ::gpu::shader* get_image_set_shader();



   };


} // namespace gpu_directx



