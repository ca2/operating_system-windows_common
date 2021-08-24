// Create by camilo on 2021-04-29 2:18 PM <3Tbs!!
#pragma once


#include "acme/operating_system.h"


//#include "exception_engine.h"

CLASS_DECL_ACME_WINDOWS_COMMON HRESULT defer_co_initialize_ex(bool bMultiThread, bool bDisableOleDDE = false);
//CLASS_DECL_ACME_WINDOWS_COMMON string get_last_error_message(DWORD dwError);
//CLASS_DECL_ACME_WINDOWS_COMMON string get_hresult_message(HRESULT hr);
CLASS_DECL_ACME_WINDOWS_COMMON ::u32 windows_get_file_attributes(const char * path);
CLASS_DECL_ACME_WINDOWS_COMMON bool windows_create_directory(const char * pszPath ARG_SEC_ATTRS);


//CLASS_DECL_ACME_WINDOWS_COMMON BSTR AllocSysString(const ::string & str) noexcept;
////CLASS_DECL_ACME_WINDOWS_COMMON bool ReAllocSysString(BSTR * pbstr, const ansichar * pchData, strsize nDataLength) noexcept;
//CLASS_DECL_ACME_WINDOWS_COMMON BSTR SetSysString(BSTR * pbstr, const ::string & str);
//CLASS_DECL_ACME_WINDOWS_COMMON wstring bstr_to_wstr(BSTR bstr);
//CLASS_DECL_ACME_WINDOWS_COMMON string bstr_to_str(BSTR bstr);


#include "comptr.h"
#include "cotaskptr.h"


#include "prop_variant.h"


#include "exception/hresult.h"

#include "exception/translator.h"

#include "exception/engine.h"

#include "standard_exception.h"

#include "file.h"

#include "bstring.h"


#include "node.h"


#include "acme_dir.h"


#include "acme_file.h"


#include "acme_path.h"



