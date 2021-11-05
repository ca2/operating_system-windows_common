#include "framework.h"


extern "C"
void text_to_speech_sapi_factory_exchange(::factory_map * pfactorymap)
{

   pfactorymap->create_factory < ::text_to_speech_sapi::speaker, ::text_to_speech::speaker >();

}



