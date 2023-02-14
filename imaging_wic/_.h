#pragma once


#include "aura/_.h"


#ifdef _UWP
#include "aura_universal_windows/_.h"
#elif defined(WINDOWS_DESKTOP)
#include "aura_windows/_.h"
#endif


//#include "aura/graphics/image/save_image.h"
//
//
//
//#include "factory_exchange.h"



//
//#include "context_image.h"



#if defined(_imaging_wic_project)
#define CLASS_DECL_IMAGING_WIC  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_IMAGING_WIC  CLASS_DECL_IMPORT
#endif


