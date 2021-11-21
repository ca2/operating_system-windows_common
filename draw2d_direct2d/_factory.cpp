#include "framework.h"
//#include "aura/printer.h"
#include "printer.h"


CLASS_DECL_AURA void init_draw2d_mutex();


extern CLASS_DECL_AURA image_array * g_pimagea;


//namespace draw2d_direct2d
//{

__FACTORY_EXPORT void draw2d_direct2d_factory(::factory::factory * pfactory)
   {

      if (g_pimagea == nullptr)
      {

         g_pimagea = new image_array;

      }


      pfactory->add_factory_item < ::draw2d_direct2d::image, ::image >();
      pfactory->add_factory_item < ::draw2d_direct2d::bitmap, ::draw2d::bitmap >();
      pfactory->add_factory_item < ::draw2d_direct2d::path, ::draw2d::path >();
      pfactory->add_factory_item < ::draw2d_direct2d::pen, ::draw2d::pen >();
      pfactory->add_factory_item < ::draw2d_direct2d::brush, ::draw2d::brush >();
      pfactory->add_factory_item < ::draw2d_direct2d::palette, ::draw2d::palette >();
      pfactory->add_factory_item < ::draw2d_direct2d::region, ::draw2d::region >();
      pfactory->add_factory_item < ::draw2d_direct2d::font, ::write_text::font >();

      pfactory->add_factory_item < ::draw2d_direct2d::graphics, ::draw2d::graphics > ();

      pfactory->add_factory_item < ::draw2d_direct2d::draw2d, ::draw2d::draw2d >();

   }


   //factory_exchange::~factory_exchange()
   //{

   //   destroy();

   //}


//#if defined(_UWP)
//
//   void factory_exchange::draw2d_direct2d_initialize()
//   {
//
//   }
//
//#endif

//
//} // namespace draw2d_direct2d
//

//__FACTORY_EXPORT //void draw2d_factory(::factory::factory * pfactory)
//{
//
//   psystem->m_pDraw2dFactoryExchange = new ::draw2d_direct2d::factory_exchange();
//
//}

//__FACTORY_EXPORT //void factory_exchange()
//{
//
//   draw2d_factory(::factory::factory * pfactory);
//
//}



