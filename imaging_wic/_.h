#pragma once


#include "aura/_.h"
#include "aura_windows/_.h"


#ifdef _IMAGING_WIC_STATIC
#define CLASS_DECL_IMAGING_WIC
#elif defined(_IMAGING_WIC_LIBRARY)
#define CLASS_DECL_IMAGING_WIC  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_IMAGING_WIC  CLASS_DECL_IMPORT
#endif





