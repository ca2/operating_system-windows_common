#pragma once

//#include "AppCore/vk_window.h"
//#include "cube/container.h"
#include "context.h"
//#include "offscreen.h"
//#include "swapchain.h"
#include "render_target_view.h"
#include "bred/gpu/renderer.h"


//constexpr unsigned int FRAME_OVERLAP = 2;


namespace gpu_directx11
{

   class CLASS_DECL_GPU_DIRECTX11 shader_vertex_input :
      virtual public ::particle
   {
   public:

      //::array < VkVertexInputBindingDescription >     m_bindings;
      //::array < VkVertexInputAttributeDescription >   m_attribs;

   };


   class CLASS_DECL_GPU_DIRECTX11 renderer :
      virtual public ::gpu::renderer
   {
   public:

      ::color::hls m_hlsClear;

      struct CLASS_DECL_GPU_DIRECTX11 cpu_buffer_sampler :
         virtual public ::particle
      {


         ::pointer < context >      m_pgpucontext;
         ::pointer < renderer >     m_prenderer;
         comptr<ID3D11Texture2D>    m_ptextureStaging;
         ::int_size                 m_sizeStaging;


         cpu_buffer_sampler();
         ~cpu_buffer_sampler();


         void initialize_cpu_buffer_sampler(::gpu::context* pgpucontext);

         void clear();
         //void update(VkExtent2D vkextent2d);
         void destroy();

         void sample(ID3D11Texture2D* ptextureSource);

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
      //::pointer < context >				               m_pgpucontext;
      ::pointer < cpu_buffer_sampler >	               m_pcpubuffersampler;
      //::pointer<swap_chain_render_target_view>			m_pvkcswapchain;
      //::pointer<offscreen_render_target_view>			m_pvkcoffscreen;
      //::pointer<render_target_view>			            m_prendertargetview;
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

      ::pointer<::gpu_directx11::set_descriptor_layout>           m_psetdescriptorlayoutImageBlend;
      ::pointer <::gpu_directx11::descriptor_pool>                m_pdescriptorpoolImageBlend;


      ::pointer<::gpu_directx11::set_descriptor_layout>           m_psetdescriptorlayoutResolve;
      ::pointer <::gpu_directx11::descriptor_pool>                m_pdescriptorpoolResolve;
      ::pointer < descriptor >                                 m_pdescriptorResolve;

      ::procedure_array m_procedureaAfterEndRender;

      //::collection::index m_iCurrentFrame2 = -1;
      //::collection::index m_iFrameSerial2 = -1;

      //::int_size m_sizeRenderer;


      renderer();
      ~renderer();


      //virtual void restart_frame_counter();
      void on_new_frame() override;
      void initialize_gpu_renderer(::gpu::context* pgpucontext) override;

      //int width()  override;
      //int height() override;



      //VkRenderPass getRenderPass() const
      //{

      //   return m_prendertargetview->getRenderPass();
      //   //return m_bOffScreen ?
      //      //m_pvkcoffscreen->getRenderPass():
      //      //m_pvkcswapchain->getRenderPass(); 

      //}

      void do_sampling_to_cpu();
      void gpu_blend(::draw2d::graphics * pgraphics);
      //void resolve_color_and_alpha_accumulation_buffers();
      //void _resolve_color_and_alpha_accumulation_buffers();
      void swap_chain();

      float getAspectRatio() const;
    

      bool isFrameInProgress() const { return isFrameStarted; }


      //VkCommandBuffer getCurrentCommandBuffer() const {
      //   assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
      //   return commandBuffers[get_frame_index()];
      //}

      //int get_frame_index() const override;
      //int get_frame_count() const override;

      //void defer_update_renderer() override;
      //void on_defer_update_renderer_allocate_render_target(::gpu::enum_output eoutput, const ::int_size& size, ::gpu::render_target* previous) override;

      //::pointer < ::graphics3d::frame> beginFrame() override;
      //void endFrame() override;
      //void on_begin_render(::graphics3d::frame * pframe) override;
      //void on_end_render(::graphics3d::frame * pframe) override;

      void on_context_resize() override;

      void on_begin_draw() override;
      void _on_begin_render(::gpu::frame * pframe) override;
      void _on_end_render(::gpu::frame * pframe) override;
      void on_end_draw() override;

   //public:

      void createCommandBuffers();

      void freeCommandBuffers();
      //void recreateSwapchain();
      //void set_size(const ::int_size & size) override;

      //void prepareOffScreen();

      void on_end_layer(::gpu::layer * player) override;

      ::pointer < ::gpu::frame > beginFrame() override;
      //void _on_begin_render(::gpu::frame* pframeParam) override;
      //void _on_end_render(::gpu::frame* pframeParam) override;
      void endFrame() override;
      void endDraw(::draw2d_gpu::graphics * pgraphics, ::user::interaction * puserinteraction) override;


      void copy(::gpu::texture* pgputextureTarget, ::gpu::texture* pgputextureSource) override;

      //void _set_image(VkImage image, const ::int_rectangle& rectangle, bool bYSwap);

      //void _blend_image(VkImage image, const ::int_rectangle& rectangle, bool bYSwap);
      //void _on_graphics_end_draw(VkImage image, const ::int_rectangle& rectangle);

      void _blend_renderer(::gpu_directx11::renderer* prendererSrc, bool bYSwap);
      void _on_graphics_end_draw(::gpu_directx11::renderer * prendererSrc);

      //void _on_frame_draw(::gpu_directx11::renderer* prendererUpper);

      //void _copy_image(VkImage image, const ::int_rectangle& rectangle, bool bYSwap);

      ::gpu::shader * get_image_blend_shader();

      ::gpu::shader* get_image_set_shader();

      //virtual void endDrawEndDraw();

      virtual ::pointer < ::gpu::render_target > allocate_offscreen_render_target();


   };


} // namespace gpu_directx11



