#include "framework.h"


#include "acme/library.h"


void CLASS_DECL_ACME_WINDOWS_COMMON __cdecl _ca2_purecall()
{

   __throw(::exception::exception());

}







namespace windows
{




   HINSTANCE load_library(const ::string & psz)

   {

      return ::LoadLibraryW(::str::international::utf8_to_unicode(psz));


   }





   ::u32 get_current_directory(string& str)
   {

      return ::GetCurrentDirectoryW(MAX_PATH * 8, wtostring(str, MAX_PATH * 8));

   }


   ::u32 get_temp_path(string& str)
   {

      return ::GetTempPathW(MAX_PATH * 8, wtostring(str, MAX_PATH * 8));

   }


   ::i32 reg_query_value(HKEY hkey, const ::string & pszSubKey, string& str)
   {

      DWORD dwType = 0;
      DWORD dwSize = 0;
      ::i32 lResult = RegQueryValueExW(hkey, wstring(pszSubKey), nullptr, &dwType, nullptr, &dwSize);

      if (lResult != ERROR_SUCCESS)
         return lResult;
      ASSERT(dwType == REG_SZ || dwType == REG_MULTI_SZ || dwType == REG_EXPAND_SZ);
      if (dwType == REG_SZ || dwType == REG_MULTI_SZ || dwType == REG_EXPAND_SZ)
      {

         natural_wstring pwsz(byte_count, dwSize);

         lResult = RegQueryValueExW(hkey, wstring(pszSubKey), nullptr, &dwType, (byte*)(unichar*)pwsz, &dwSize);

         str = pwsz;

         //str.release_string_buffer(dwSize);

         return lResult;

      }
      else
      {

         return ERROR_NOT_SUPPORTED;

      }

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





