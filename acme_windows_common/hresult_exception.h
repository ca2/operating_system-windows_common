#pragma once


#include "acme/exception/exception.h"


class CLASS_DECL_ACME_WINDOWS_COMMON hresult_exception :
   public ::exception
{
public:


   hresult_exception(HRESULT hresult, const char * pszMessage = nullptr);
   ~hresult_exception() override;


};


CLASS_DECL_ACME_WINDOWS_COMMON int trace_hr(const char * psz, HRESULT hr);


CLASS_DECL_ACME_WINDOWS_COMMON void throw_if_failed(HRESULT hr);


inline error_code __hresult(HRESULT hresult)
{

   return { e_error_code_type_hresult, (::i64)hresult };

}

