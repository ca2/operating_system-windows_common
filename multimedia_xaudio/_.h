#pragma once



#include "app-core/audio/audio/_.h"



#if defined(_MULTIMEDIA_XAUDIO_LIBRARY)
   #define CLASS_DECL_MULTIMEDIA_XAUDIO  CLASS_DECL_EXPORT
#else
   #define  CLASS_DECL_MULTIMEDIA_XAUDIO  CLASS_DECL_IMPORT
#endif

