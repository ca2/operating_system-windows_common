#pragma once



#include "app-core/audio/_.h"


#include "acme/_operating_system.h"


#include <xaudio2.h>


#if defined(_multimedia_xaudio_project)
   #define CLASS_DECL_MULTIMEDIA_XAUDIO  CLASS_DECL_EXPORT
#else
   #define  CLASS_DECL_MULTIMEDIA_XAUDIO  CLASS_DECL_IMPORT
#endif

