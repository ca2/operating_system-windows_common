#include "framework.h"
#include "context.h"

//BEGIN_FACTORY(imaging_wic)
//FACTORY_ITEM(::imaging_wic::image_context)
//END_FACTORY()


//
//namespace imaging_wic
//{
//
//   class factory_exchange
//   {
//   public:
//
//      
//      factory_exchange();
//      virtual ~factory_exchange();
//
//   };
//
//
////#define new ACME_NEW
////
////
//   factory_exchange::factory_exchange()
//   {
//
//      factory()->add_factory_item < ::imaging_wic::image_context, ::image_context >();
//
//   }
//
////
//   factory_exchange::~factory_exchange()
//   {
//
//   }
////
////
//} // namespace imaging_wic
////

__FACTORY_EXPORT void imaging_wic_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item < ::imaging_wic::image_context, ::image::image_context >();

}




//imaging_wic::factory_exchange g_imagingwicfactoryexchange;
