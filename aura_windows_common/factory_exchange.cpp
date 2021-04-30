#include "framework.h"


extern "C"
void apex_windows_common_factory_exchange(::factory_map* pfactorymap);


extern "C"
void aura_windows_common_factory_exchange(::factory_map * pfactorymap)
{

   apex_windows_common_factory_exchange(pfactorymap);

   pfactorymap->create_factory < ::aura::windows_common::node, ::acme::node >();

}



