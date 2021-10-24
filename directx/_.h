// Created by camilo on 2021-06-05 23:48 BRT <3ThomasBS_!!
#pragma once


#include "aura/_.h"
#include "aura_windows_common/_.h"


#ifdef _DIRECTX_LIBRARY
#define CLASS_DECL_DIRECTX  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_DIRECTX  CLASS_DECL_IMPORT
#endif



