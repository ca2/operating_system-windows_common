#include "framework.h"
#include "node.h"


__FACTORY_EXPORT void acme_windows_common_factory(::factory::factory* pfactory);


__FACTORY_EXPORT void apex_windows_common_factory(::factory::factory * pfactory)
{

   acme_windows_common_factory(pfactory);

   pfactory->add_factory_item < ::apex_windows_common::node, ::acme::node >();


}



