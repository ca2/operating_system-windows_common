#include "framework.h"


//BEGIN_FACTORY(apex_windows_common)
//FACTORY_DEPENDENCY(acme_windows_common)
//FACTORY_ITEM(::apex::windows_common::node)
//END_FACTORY()


__FACTORY_EXPORT void acme_windows_common_factory(::factory::factory* pfactory);


__FACTORY_EXPORT void apex_windows_common_factory(::factory::factory * pfactory)
{


   acme_windows_common_factory(pfactory);

   pfactory->add_factory_item < ::apex::windows_common::node, ::acme::node >();


}



