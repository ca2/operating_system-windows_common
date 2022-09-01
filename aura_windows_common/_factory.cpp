#include "framework.h"
#include "node.h"


__FACTORY_EXPORT void apex_windows_common_factory(::factory::factory* pfactory);


__FACTORY_EXPORT void aura_windows_common_factory(::factory::factory * pfactory)
{

   apex_windows_common_factory(pfactory);

   pfactory->add_factory_item < ::aura_windows_common::node, ::acme::node >();

}



