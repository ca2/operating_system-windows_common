// Create by camilo on 2021-04-29 2:18 PM <3Tbs!!
#pragma once


#include "_.h"
#include "acme/operating_system.h"


CLASS_DECL_ACME_WINDOWS_COMMON HRESULT defer_co_initialize_ex(bool bMultiThread, bool bDisableOleDDE = false);
CLASS_DECL_ACME ::u32 windows_get_file_attributes(const char * path);


using hfile = HANDLE;



#include "comptr.h"
#include "cotaskptr.h"


#include "prop_variant.h"


#include "exception/hresult.h"
#include "exception/translator.h"
#include "exception/standard.h"

#include "file.h"

#include "bstring.h"


#include "node.h"


#include "acme_directory.h"


#include "acme_file.h"


#include "acme_path.h"



