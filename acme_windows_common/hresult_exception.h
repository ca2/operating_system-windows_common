#pragma once


#include "acme/exception/exception.h"


class CLASS_DECL_ACME_WINDOWS_COMMON hresult_exception :
   public ::exception
{
public:


   hresult_exception(HRESULT hresult, const char * pszMessage = nullptr);
   ~hresult_exception() override;


};


CLASS_DECL_ACME_WINDOWS_COMMON int trace_hresult(const char * psz, HRESULT hresult);


CLASS_DECL_ACME_WINDOWS_COMMON void throw_hresult_if_failed(HRESULT hresult);


CLASS_DECL_ACME_WINDOWS_COMMON error_code hresult_error_code(HRESULT hresult);



