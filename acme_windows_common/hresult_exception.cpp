#include "framework.h"
#include "hresult_exception.h"


hresult_exception::hresult_exception(HRESULT hresult, const char * pszMessage)
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

   m_errorcodea.add(__hresult(hresult));

   m_strMessage = pszMessage;

}



hresult_exception::~hresult_exception()
{

}


#define ERRMSGBUFFERSIZE 256


CLASS_DECL_ACME_WINDOWS_COMMON int trace_hr(const char * psz,HRESULT hr)
{

   string strError = last_error_message((DWORD) hr);

   strError.trim();

   //TRACE("%s hr=%d:\"%s\"",psz,hr,strError.c_str());

   return 0;

}


CLASS_DECL_ACME_WINDOWS_COMMON int trace_hr(const char * psz, HRESULT hr);


CLASS_DECL_ACME_WINDOWS_COMMON void throw_if_failed(HRESULT hr)
{


   if (FAILED(hr))
   {

      throw hresult_exception(hr);

   }


}



