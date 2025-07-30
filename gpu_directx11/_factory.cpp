#include "framework.h"
#include "approach.h"
#include "cpu_buffer.h"
//#include "direct2d_draw2d_swap_chain.h"
#include "frame.h"
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


}



