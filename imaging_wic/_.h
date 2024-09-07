#pragma once


#include "aura/_.h"


#ifdef UNIVERSAL_WINDOWS
#include "aura_universal_windows/_.h"
#elif defined(WINDOWS_DESKTOP)
#include "aura_windows/_.h"
#endif


//#include "aura/graphics/image/save_options.h"
//
//
//
//#include "factory_exchange.h"



//
//#include "image_context.h"



#if defined(_imaging_wic_project)
#define CLASS_DECL_IMAGING_WIC  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_IMAGING_WIC  CLASS_DECL_IMPORT
#endif


