#pragma once


#include "apex/_.h"
#include "acme_windows_common/_.h"
//#include "apex/operating_system.h"


#if defined(_APEX_WINDOWS_COMMON_LIBRARY)
#define CLASS_DECL_APEX_WINDOWS_COMMON  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APEX_WINDOWS_COMMON  CLASS_DECL_IMPORT
#endif



