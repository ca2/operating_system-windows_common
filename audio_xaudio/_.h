#pragma once


#include "multimedia_xaudio/_.h"
#include "acme_windows_common/_.h"


#if defined(_audio_xaudio_project)
   #define CLASS_DECL_AUDIO_XAUDIO  CLASS_DECL_EXPORT
#else
   #define  CLASS_DECL_AUDIO_XAUDIO  CLASS_DECL_IMPORT
#endif




