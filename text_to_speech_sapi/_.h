#pragma once


#include "aqua/_.h"
#include "apex_windows/_.h"


#if defined(_TEXT_TO_SPEECH_SAPI_LIBRARY)
#define CLASS_DECL_TEXT_TO_SPEECH_SAPI CLASS_DECL_EXPORT
#else
#define CLASS_DECL_TEXT_TO_SPEECH_SAPI CLASS_DECL_IMPORT
#endif



