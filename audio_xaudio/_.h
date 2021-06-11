#pragma once


#include "multimedia_xaudio/_.h"


#if defined(_AUDIO_XAUDIO_LIBRARY)
   #define CLASS_DECL_AUDIO_XAUDIO  CLASS_DECL_EXPORT
#else
   #define  CLASS_DECL_AUDIO_XAUDIO  CLASS_DECL_IMPORT
#endif




