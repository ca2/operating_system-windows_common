#include "framework.h"
//#include "acme/operating_system.h"


//#include "_file_c.h"
//#include "file.h"


//#ifdef _UWP
//CLASS_DECL_ACME::winrt::Windows::Storage::StorageFolder^ winrt_folder(string& strPath, string& strPrefix);
//#endif




//int_bool windows_create_directory(const char* path, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
//{
//
//   wstring wstr(path);
//
//
//   if (::CreateDirectoryW(wstr, lpSecurityAttributes))
//   {
//
//      return true;
//
//   }
//
//   if (windows_get_alternate_path(wstr))
//   {
//
//      if (::CreateDirectoryW(wstr, lpSecurityAttributes))
//      {
//
//         return true;
//
//      }
//
//   }
//
//   return false;
//
//}
//
//















//bool context::_os_resolve_alias(::file::path & path, const char * psz, ::user::primitive * pinteraction, bool bNoUI, bool bNoMount)
//{
//
//   if (os_is_alias(psz))
//   {
//
//      return os().resolve_link(path, psz, nullptr, nullptr, pinteraction);
//
//   }
//
//   return false;
//
//}


CLASS_DECL_ACME_WINDOWS_COMMON bool _os_may_have_alias(const char * psz)
{

   string str(psz);

   return str.ends_ci(".lnk") || str.contains_ci(".lnk/") || str.contains_ci(".lnk\\");

}

//
//bool context::os_is_alias(const char * psz)
//{
//
//   return ::str::ends_ci(psz, ".lnk");
//
//}





CLASS_DECL_ACME_WINDOWS_COMMON memsize hfile_read(hfile hfile, void* p, memsize s)
{

   DWORD dwRead = 0;

   if (!::ReadFile(hfile, p, (DWORD) minimum(INT_MAX, s), &dwRead, nullptr))
   {

      return 0;

   }

   return dwRead;

}


CLASS_DECL_ACME_WINDOWS_COMMON void hfile_write(hfile hfile, const void* p, memsize s)
{

   memsize dwWrittenTotal = 0;

   while (s > 0)
   {

      int iWrite = (int) minimum(INT_MAX, s);

      DWORD dwWritten = 0;

      if (!WriteFile(hfile, p, iWrite, &dwWritten, nullptr))
      {

         //return ::error_failed;
         throw ::exception(::error_failed);

      }

      if (dwWritten != iWrite)
      {

         throw ::exception(::error_failed);

      }

      s -= iWrite;

   }

   //return ::success;

}

// namespace path
// {



// } // namespace path 





