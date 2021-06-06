// Created by camilo on 2021-06-06 00:04 BRT <3ThomasBS_!!
#pragma once


#include "aura/_.h"
#include "directx/_.h"


#ifdef _DIRECT2D_LIBRARY
#define CLASS_DECL_DIRECT2D  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_DIRECT2D  CLASS_DECL_IMPORT
#endif



