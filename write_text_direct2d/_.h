// Created by camilo 2021-02-10 <3TBS_!!
#pragma once


#include "aura/_.h"
#include "direct2d/_.h"


#ifdef _WRITE_TEXT_DIRECT2D_STATIC
#define CLASS_DECL_WRITE_TEXT_WIN32
#elif defined(_WRITE_TEXT_DIRECT2D_LIBRARY)
#define CLASS_DECL_WRITE_TEXT_DIRECT2D  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_WRITE_TEXT_DIRECT2D  CLASS_DECL_IMPORT
#endif



