#include "framework.h"


extern "C"
void write_text_direct2d_factory_exchange(::factory_map * pfactorymap)
{

   pfactorymap->create_factory < ::write_text_direct2d::font_department, ::write_text::font_department >();
   pfactorymap->create_factory < ::write_text_direct2d::write_text, ::write_text::write_text >();
   pfactorymap->create_factory < ::write_text_direct2d::font_enumeration, ::write_text::font_enumeration >();

}