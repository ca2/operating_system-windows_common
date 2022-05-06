// From acme/filesystem/file/_.cpp by camilo on 2021-08-09 
// 14:17 BRT <3ThomasBorregaardSørensen
#include "framework.h"
//#include "acme/operating_system.h"
#include <stdio.h>


namespace acme_windows_common
{


   acme_path::acme_path()
   {


   }


   acme_path::~acme_path()
   {


   }


   ::file::enum_type acme_path::get_type(const char * path)
   {

      auto dwFileAttributes = windows_get_file_attributes(path);

      if (dwFileAttributes == INVALID_FILE_ATTRIBUTES)
      {

         DWORD dwLastError = ::GetLastError();

         return ::file::e_type_doesnt_exist;

      }

      if (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {

         return ::file::e_type_folder;

      }

      return ::file::e_type_file;

   }


   ::file::path acme_path::_final(const char * path)
   {

      WCHAR wszPath[8192];

      ::u32 dw = GetFullPathNameW(L"\\\\?\\" + wstring(path), sizeof(wszPath) / sizeof(WCHAR), wszPath, nullptr);

      return wszPath;

   }


} // namespace acme_windows_common



