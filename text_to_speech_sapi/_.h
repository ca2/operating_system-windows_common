#pragma once


#include "aqua/_.h"
#include "apex_windows/_.h"


#if defined(_TTS_SAPI_LIBRARY)
#define CLASS_DECL_TTS_SAPI  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_TTS_SAPI  CLASS_DECL_IMPORT
#endif



