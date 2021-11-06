#include "framework.h"


extern "C"
void acme_windows_common_factory_exchange(::factory_map* pfactorymap);


extern "C"
void apex_windows_common_factory_exchange(::factory_map * pfactorymap)
{

   acme_windows_common_factory_exchange(pfactorymap);


   pfactorymap->create_factory < ::apex::windows_common::node, ::acme::node >();


}



