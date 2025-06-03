#include "framework.h"
#include "approach.h"
#include "program.h"
#include "shader.h"
#include "cpu_buffer.h"
#include "object.h"
#include "renderer.h"
#include "frame.h"

//BEGIN_FACTORY(gpu_directx)
//FACTORY_ITEM(::directx::directx)
//FACTORY_ITEM(::directx::program)
//FACTORY_ITEM(::directx::shader)
//FACTORY_ITEM(::directx::buffer)
//END_FACTORY()


__FACTORY_EXPORT void gpu_directx_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item < ::gpu_directx::approach, ::gpu::approach >();
   pfactory->add_factory_item < ::gpu_directx::context, ::gpu::context >();
   pfactory->add_factory_item < ::gpu_directx::frame, ::gpu::frame >();
   pfactory->add_factory_item < ::gpu_directx::shader, ::gpu::shader >();
   pfactory->add_factory_item < ::gpu_directx::cpu_buffer, ::gpu::cpu_buffer >();
   pfactory->add_factory_item < ::gpu_directx::renderer, ::gpu::renderer >();

   pfactory->add_factory_item < ::gpu_directx::object, ::gpu::object >();

   pfactory->add_factory_item < ::gpu_directx::device, ::gpu::device >();

}



