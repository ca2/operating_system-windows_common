#include "framework.h"
#include "approach.h"
#include "cpu_buffer.h"
//#include "cube.h"
#include "frame.h"
#include "frame_storage.h"
//#include "full_screen_quad.h"
#include "input_layout.h"
#include "memory_buffer.h"
#include "model_buffer.h"
#include "object.h"
#include "offscreen_render_target_view.h"
#include "pixmap.h"
#include "program.h"
#include "renderer.h"
#include "shader.h"
#include "swap_chain.h"
#include "texture.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/pixmap.h"
#include "bred/gpu/frame_ephemeral.h"
#include "bred/gpu/frame_storage.h"
#include "gltf/model.h"
#include "gpu/full_screen_quad.h"
#include "ibl/brdf_convolution_framebuffer.h"
#include "ibl/diffuse_irradiance_map.h"
#include "ibl/equirectangular_cubemap.h"
#include "ibl/specular_map.h"
#include "ibl/cubemap_framebuffer.h"
#include "ibl/mipmap_cubemap_framebuffer.h"
//#include "ibl/hdri_cube.h"


// #include "bred/gpu/model_buffer.h"

//BEGIN_FACTORY(gpu_directx11)
//FACTORY_ITEM(::directx11::directx11)
//FACTORY_ITEM(::directx11::program)
//FACTORY_ITEM(::directx11::shader)
//FACTORY_ITEM(::directx11::buffer)
//END_FACTORY()


__FACTORY_EXPORT void gpu_directx11_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item < ::gpu_directx11::approach, ::gpu::approach >();
   pfactory->add_factory_item < ::gpu_directx11::context, ::gpu::context >();
   pfactory->add_factory_item < ::gpu_directx11::frame, ::gpu::frame >();
   pfactory->add_factory_item < ::gpu_directx11::shader, ::gpu::shader >();
   pfactory->add_factory_item < ::gpu_directx11::cpu_buffer, ::gpu::cpu_buffer >();
   pfactory->add_factory_item < ::gpu_directx11::renderer, ::gpu::renderer >();

   pfactory->add_factory_item < ::gpu_directx11::object, ::gpu::object >();

   pfactory->add_factory_item < ::gpu_directx11::texture, ::gpu::texture >();

   pfactory->add_factory_item < ::gpu_directx11::device, ::gpu::device >();

   pfactory->add_factory_item < ::gpu_directx11::offscreen_render_target_view, ::gpu::render_target >();
   pfactory->add_factory_item < ::gpu_directx11::swap_chain, ::gpu::swap_chain >();

   pfactory->add_factory_item < ::gpu_directx11::input_layout, ::gpu::input_layout >();
   pfactory->add_factory_item < ::gpu::command_buffer >();
   pfactory->add_factory_item < ::gpu::layer >();
   pfactory->add_factory_item < ::gpu_directx11::model_buffer, ::gpu::model_buffer >();
   pfactory->add_factory_item < ::gpu_directx11::memory_buffer, ::gpu::memory_buffer >();
   //pfactory->add_factory_item < ::gpu_directx11::pixmap, ::gpu::pixmap >();
   pfactory->add_factory_item < ::gpu::pixmap >();

   //pfactory->add_factory_item < ::gpu_directx11::frame_storage, ::gpu::frame_storage >();

   pfactory->add_factory_item < ::gpu::frame_ephemeral >();
   pfactory->add_factory_item < ::gpu::frame_storage >();


   //pfactory->add_factory_item<::gpu_directx11::cube, ::gpu::cube>();
   //pfactory->add_factory_item< ::gpu::cube>();
   pfactory->add_factory_item<::gpu::full_screen_quad>();


   pfactory->add_factory_item<::gpu_directx11::gltf::mesh, ::gpu::gltf::mesh>();
   pfactory->add_factory_item<::gpu_directx11::gltf::model, ::gpu::gltf::model>();
   pfactory->add_factory_item<::gpu_directx11::ibl::diffuse_irradiance_map, ::gpu::ibl::diffuse_irradiance_map>();
   pfactory->add_factory_item<::gpu_directx11::ibl::specular_map, ::gpu::ibl::specular_map>();
   pfactory->add_factory_item<::gpu_directx11::ibl::mipmap_cubemap_framebuffer, ::gpu::ibl::mipmap_cubemap_framebuffer>();
   pfactory->add_factory_item<::gpu_directx11::ibl::cubemap_framebuffer, ::gpu::ibl::cubemap_framebuffer>();
   pfactory->add_factory_item<::gpu_directx11::ibl::equirectangular_cubemap, ::gpu::ibl::equirectangular_cubemap>();
   pfactory->add_factory_item<::gpu_directx11::ibl::brdf_convolution_framebuffer, ::gpu::ibl::brdf_convolution_framebuffer>();
   //pfactory->add_factory_item<::gpu_directx11::ibl::hdri_cube, ::gpu::ibl::hdri_cube>();
}



