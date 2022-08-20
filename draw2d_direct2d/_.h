#pragma once


#include "aura/_.h"
#include "aura/operating_system.h"
#include "direct2d/_.h"
#include "acme_windows_common/_library.h"
//#include "aura/os/windows/d2d1_1.h"
//#include "aura/os/windows_common/draw2d_direct2d_global.h"


#ifdef _DRAW2D_DIRECT2D_LIBRARY
#define CLASS_DECL_DRAW2D_DIRECT2D  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_DRAW2D_DIRECT2D  CLASS_DECL_IMPORT
#endif



