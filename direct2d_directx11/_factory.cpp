#include "framework.h"
#include "direct2d_directx11.h"


__FACTORY_EXPORT void direct2d_directx11_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item<::direct2d_directx11::direct2d_directx11>();

}



