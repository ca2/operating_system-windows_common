#pragma once


#include "app-veriwell/multimedia/multimedia/_.h"
#include "multimedia_xaudio/_.h"


#if defined(_MUSIC_MIDI_XAUDIO_LIBRARY)
   #define CLASS_DECL_MUSIC_MIDI_XAUDIO  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_MUSIC_MIDI_XAUDIO  CLASS_DECL_IMPORT
#endif


