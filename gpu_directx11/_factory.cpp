#include "framework.h"
#include "approach.h"
#include "direct2d_draw2d_swap_chain.h"
#include "program.h"
#include "shader.h"
#include "cpu_buffer.h"
#include "object.h"
#include "renderer.h"
#include "frame.h"

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

   pfactory->add_factory_item < ::gpu_directx11::device, ::gpu::device >();

   pfactory->add_factory_item < ::gpu_directx11::direct2d_draw2d_swap_chain, ::gpu::swap_chain >();

}



