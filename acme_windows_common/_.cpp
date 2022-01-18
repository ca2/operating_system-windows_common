#include "framework.h"


#include "acme/library.h"


//void CLASS_DECL_ACME_WINDOWS_COMMON __cdecl _ca2_purecall()
//{
//
//   __throw(::exception());
//
//}


namespace windows_common
{


   HINSTANCE load_library(const ::string & psz)
   {

#ifdef WINDOWS_DESKTOP

      return ::LoadLibraryW(::str::international::utf8_to_unicode(psz));

#else

      return ::LoadPackagedLibrary(::str::international::utf8_to_unicode(psz), 0);

#endif

   }


   ::u32 get_current_directory(string& str)
   {

      return ::GetCurrentDirectoryW(MAX_PATH * 8, wtostring(str, MAX_PATH * 8));

   }


   ::u32 get_temp_path(string& str)
   {

      return ::GetTempPathW(MAX_PATH * 8, wtostring(str, MAX_PATH * 8));

   }


   bool delete_file(const ::string & pFileName)
   {

      return ::DeleteFileW(::str::international::utf8_to_unicode(pFileName)) != false;

   }


} // namespace windows


//LRESULT CALLBACK __window_procedure(HWND oswindow, const ::id & id, wparam wparam, lparam lparam);
//
//WNDPROC get_window_procedure()
//{
//
//   return &::__window_procedure;
//
//}




::e_status hresult_to_status(HRESULT hr)
{

   if (SUCCEEDED(hr))
      return ::success;
   else
      return error_failed;

}


