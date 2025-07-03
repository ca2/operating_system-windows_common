#include "framework.h"
#include "image.h"
#include "bitmap.h"
#include "path.h"
#include "pen.h"
#include "brush.h"
#include "palette.h"
#include "region.h"
#include "font.h"
#include "internal_font.h"
#include "graphics.h"
#include "draw2d.h"
#include "directx11/directx11.h"
#include "bred/gpu/command_buffer.h"


CLASS_DECL_AURA void init_draw2d_mutex();


__FACTORY_EXPORT void draw2d_directx11_factory(::factory::factory* pfactory)
{

   pfactory->add_factory_item < ::draw2d_directx11::image, ::image::image >();
   pfactory->add_factory_item < ::draw2d_directx11::bitmap, ::draw2d::bitmap >();
   pfactory->add_factory_item < ::draw2d_directx11::path, ::draw2d::path >();
   pfactory->add_factory_item < ::draw2d_directx11::pen, ::draw2d::pen >();
   pfactory->add_factory_item < ::draw2d_directx11::brush, ::draw2d::brush >();
   pfactory->add_factory_item < ::draw2d_directx11::palette, ::draw2d::palette >();
   pfactory->add_factory_item < ::draw2d_directx11::region, ::draw2d::region >();
   pfactory->add_factory_item < ::draw2d_directx11::font, ::write_text::font >();
   pfactory->add_factory_item < ::draw2d_directx11::internal_font, ::write_text::internal_font >();

   pfactory->add_factory_item < ::draw2d_directx11::graphics, ::draw2d::graphics >();

   pfactory->add_factory_item < ::draw2d_directx11::draw2d, ::draw2d::draw2d >();
   pfactory->add_factory_item < ::gpu::command_buffer >();
   //pfactory->add_factory_item < ::draw2d_directx11::directx11_swap_chain, ::gpu::swap_chain >();


}


//factory_exchange::~factory_exchange()
//{

//   destroy();

//}


//#if defined(UNIVERSAL_WINDOWS)
//
//   void factory_exchange::draw2d_directx11_initialize()
//   {
//
//   }
//
//#endif

//
//} // namespace draw2d_directx11
//

//__FACTORY_EXPORT //void draw2d_factory(::factory::factory * pfactory)
//{
//
//   psystem->m_pDraw2dFactoryExchange = ___new ::draw2d_directx11::factory_exchange();
//
//}

//__FACTORY_EXPORT //void factory_exchange()
//{
//
//   draw2d_factory(::factory::factory * pfactory);
//
//}



