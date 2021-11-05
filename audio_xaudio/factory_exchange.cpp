#include "framework.h"

extern "C"
CLASS_DECL_EXPORT void audio_xaudio_factory_exchange(::factory_map * pfactorymap)
{

   pfactorymap->create_factory < ::multimedia::audio_xaudio::in, ::wave::in >();
   pfactorymap->create_factory < ::multimedia::audio_xaudio::out, ::wave::out >();


}









