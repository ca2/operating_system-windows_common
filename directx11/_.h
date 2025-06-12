// Created by camilo on 2021-06-06 00:04 BRT <3ThomasBS_!!
#pragma once


#include "aura_windows_common/_.h"


#if defined(_directx11_project)
#define CLASS_DECL_DIRECTX11  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_DIRECTX11  CLASS_DECL_IMPORT
#endif


#include "_directx11.h"



