// From acme/filesystem/file/_.cpp by camilo on 2021-08-09 
// 14:17 BRT <3ThomasBorregaardSorensen
#include "framework.h"
#include "path_system.h"
#include "file_system.h"
#include "acme/_operating_system.h"


namespace acme_windows_common
{


   path_system::path_system()
   {


   }


   path_system::~path_system()
   {


   }


   ::file::enum_type path_system::get_type(const ::file::path & path)
   {

      auto dwFileAttributes = ::windows::_get_file_attributes(path);

      if (dwFileAttributes == INVALID_FILE_ATTRIBUTES)
      {

         DWORD lasterror = ::GetLastError();

         if (lasterror == ERROR_FILE_NOT_FOUND || lasterror == ERROR_PATH_NOT_FOUND)
         {

            return ::file::e_type_doesnt_exist;

         }

         throw_last_error_exception(nullptr, lasterror);

         return ::file::e_type_unknown;

      }

      if (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {

         return ::file::e_type_existent_folder;

      }

      return ::file::e_type_existent_file;

   }


   ::file::path path_system::_real_path(const ::file::path & path)
   {

      WCHAR wszPath[8192];

      unsigned int dw = GetFullPathNameW(L"\\\\?\\" + wstring(path), sizeof(wszPath) / sizeof(WCHAR), wszPath, nullptr);

      return wszPath;

   }


} // namespace acme_windows_common



