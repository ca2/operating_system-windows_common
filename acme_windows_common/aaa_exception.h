#pragma once


namespace acme
{


#ifndef _NO_EXCEPTIONS


   // Throw a atl_exception corresponding to the result of ::get_last_error
   NOINLINE DECLSPEC_NO_RETURN inline void WINAPI gen_ThrowLastWin32(::matter * pobject)
   {
      
      DWORD dwError = ::GetLastError();

      throw win32_exception(dwError);

   }

#else  // no exception handling

   // Throw a atl_exception corresponding to the result of ::get_last_error
   NOINLINE inline void WINAPI gen_ThrowLastWin32(::matter * pobject)
   {
      unsigned int dwError = ::GetLastError();
      throw ::exception(hresult_exception(HRESULT_FROM_WIN32( dwError ) ));
   }

#endif  // no exception handling


}  // namespace acme




