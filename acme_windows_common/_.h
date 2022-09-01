#pragma once


#include "acme/_.h"
#include "acme/operating_system.h"


#if defined(_ACME_WINDOWS_COMMON_LIBRARY)
#define CLASS_DECL_ACME_WINDOWS_COMMON  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_ACME_WINDOWS_COMMON  CLASS_DECL_IMPORT
#endif


using hfile = HANDLE;



CLASS_DECL_ACME_WINDOWS_COMMON HRESULT defer_co_initialize_ex(bool bMultiThread, bool bDisableOleDDE = false);
CLASS_DECL_ACME::u32 windows_get_file_attributes(const char * path);


#include "bstring.h"
#include "comptr.h"
#include "cotaskptr.h"


#include "variant.h"
#include "prop_variant.h"



