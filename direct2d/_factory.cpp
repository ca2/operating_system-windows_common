#include "platform.h"
#include "direct2d.h"

void tiny_dxgi_device_source_factory(::factory::factory * pfactory);

__FACTORY_EXPORT void direct2d_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item<::direct2d::direct2d>();
   tiny_dxgi_device_source_factory(pfactory);

}



