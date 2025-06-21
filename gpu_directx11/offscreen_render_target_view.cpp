// From vk_swapchain by camilo on 2025-05-09 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "offscreen_render_target_view.h"
#include "initializers.h"
#include "physical_device.h"
#include "renderer.h"


using namespace directx11;


namespace gpu_directx11
{


   offscreen_render_target_view::offscreen_render_target_view()
   {


   }


   offscreen_render_target_view::~offscreen_render_target_view() 
   {


   }


   void offscreen_render_target_view::initialize_render_target(::gpu::renderer* pgpurenderer, const ::int_size& size, ::pointer <::gpu::render_target>previous)
   {

      ::gpu::render_target::initialize_render_target(pgpurenderer, size, previous);

   }


   void offscreen_render_target_view::on_init()
   {

      ::gpu_directx11::render_target_view::on_init();

      createRenderPassImpl();
      createImageViews();
      createRenderPass();
      createDepthResources();
      createFramebuffers();
      createSyncObjects();

   }


   HRESULT offscreen_render_target_view::acquireNextImage() 
   {

      return S_OK;

   }


   void offscreen_render_target_view::defer_resize(const ::int_size& size)
   {


   }


   void offscreen_render_target_view::createRenderPassImpl()
   {


   }



   void offscreen_render_target_view::createImageViews()
   {

      render_target_view::createImageViews();

   }


   void offscreen_render_target_view::createRenderPass()
   {


   }


   void offscreen_render_target_view::createFramebuffers()
   {

      render_target_view::createFramebuffers();

   }


   void offscreen_render_target_view::createDepthResources()
   {

      render_target_view::createDepthResources();

   }


   void offscreen_render_target_view::createSyncObjects()
   {

      render_target_view::createSyncObjects();

   }


} // namespace gpu_directx11



