#include "framework.h"


//BEGIN_FACTORY(aura_windows_common)
//FACTORY_ITEM(::aura::windows_common::node)
//END_FACTORY()


__FACTORY_EXPORT void apex_windows_common_factory(::factory::factory* pfactory);


__FACTORY_EXPORT void aura_windows_common_factory(::factory::factory * pfactory)
{

   apex_windows_common_factory(pfactory);

   pfactory->add_factory_item < ::aura::windows_common::node, ::acme::node >();

}



