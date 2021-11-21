#include "framework.h"


__FACTORY_EXPORT void write_text_direct2d_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item < ::write_text_direct2d::fonts, ::write_text::fonts >();
   pfactory->add_factory_item < ::write_text_direct2d::write_text, ::write_text::write_text >();
   pfactory->add_factory_item < ::write_text_direct2d::font_enumeration, ::write_text::font_enumeration >();

}
