#include "framework.h"
#include "direct2d.h"


__FACTORY_EXPORT void direct2d_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item<::direct2d::direct2d>();

}



