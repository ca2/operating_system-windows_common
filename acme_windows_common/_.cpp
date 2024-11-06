#include "framework.h"
#include "acme/prototype/string/string.h"
#include "acme/prototype/string/adaptor.h"
#include "acme/_library.h"


//CLASS_DECL_ACME_WINDOWS_COMMON HRESULT defer_co_initialize_ex(bool bMultiThread, bool bDisableOleDDE = false);


CLASS_DECL_ACME_WINDOWS_COMMON int trace_hr(const char * psz, HRESULT hr);
CLASS_DECL_ACME_WINDOWS_COMMON void throw_if_failed(HRESULT hr);


//void CLASS_DECL_ACME_WINDOWS_COMMON __cdecl _ca2_purecall()
//{
//
//   throw ::exception(::exception());
//
//}


namespace windows_common
{


   HINSTANCE load_library(const ::string & str)
   {

#ifdef WINDOWS_DESKTOP

      return ::LoadLibraryW(wstring(str));

#else

      return ::LoadPackagedLibrary(wstring(str), 0);

#endif

   }


   unsigned int get_current_directory(string& str)
   {

      return ::GetCurrentDirectoryW(MAX_PATH * 8, wstring_adaptor(str, MAX_PATH * 8));

   }


   unsigned int get_temp_path(string& str)
   {

      return ::GetTempPathW(MAX_PATH * 8, wstring_adaptor(str, MAX_PATH * 8));

   }


   //bool delete_file(const ::file::path & path)
   //{

   //   return ::DeleteFileW(path.get_os_path()) != false;

   //}


} // namespace windows


//LRESULT CALLBACK __window_procedure(HWND oswindow, const ::atom & atom, wparam wparam, lparam lparam);
//
//WNDPROC get_window_procedure()
//{
//
//   return &::__window_procedure;
//
//}






