#include "framework.h"
#include "hresult_exception.h"
#include "acme/operating_system/windows_common/_string.h"


hresult_exception::hresult_exception(HRESULT hresult, const char * pszMessage) :
   ::exception(::error_hresult, pszMessage)
{

   if (hresult == S_OK)
   {

      m_estatus = success;

   }
   else if (FAILED(hresult))
   {

      m_estatus = error_hresult;

   }
   else if (SUCCEEDED(hresult))
   {

      m_estatus = success_hresult;

   }

   m_errorcodea.add(hresult_error_code(hresult));

   //m_strMessage = pszMessage;

}


hresult_exception::~hresult_exception()
{


}


#define ERRMSGBUFFERSIZE 256


CLASS_DECL_ACME_WINDOWS_COMMON int trace_hresult(const char * psz, HRESULT hresult)
{

   string strError = ::windows::last_error_message((DWORD)hresult);

   strError.trim();

   ////TRACE("%s hr=%d:\"%s\"",psz,hr,strError.c_str());

   return 0;

}


CLASS_DECL_ACME_WINDOWS_COMMON int trace_hresult(const char * psz, HRESULT hresult);


CLASS_DECL_ACME_WINDOWS_COMMON void throw_hresult_if_failed(HRESULT hresult)
{

   if (FAILED(hresult))
   {

      throw hresult_exception(hresult);

   }

}


CLASS_DECL_ACME_WINDOWS_COMMON error_code hresult_error_code(HRESULT hresult)
{

   return { e_error_code_type_hresult, (::i64)hresult };

}



