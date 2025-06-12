#include "framework.h"
#include "directx11.h"


__FACTORY_EXPORT void directx11_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item<::directx11::directx11>();

}



