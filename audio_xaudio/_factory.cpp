#include "framework.h"

__FACTORY_EXPORT CLASS_DECL_EXPORT void audio_xaudio_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item < ::multimedia::audio_xaudio::in, ::wave::in >();
   pfactory->add_factory_item < ::multimedia::audio_xaudio::out, ::wave::out >();


}









