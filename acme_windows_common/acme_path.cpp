// From acme/filesystem/file/_.cpp by camilo on 2021-08-09 
// 14:17 BRT <3ThomasBorregaardS�rensen
#include "framework.h"
//#include "acme/operating_system.h"
#include <stdio.h>


namespace windows_common
{


   bool acme_path::is_file_or_dir(const char * path, ::file::enum_type * petype)
   {

      auto dwFileAttributes = windows_get_file_attributes(path);

      if (dwFileAttributes == INVALID_FILE_ATTRIBUTES)
      {

//#ifdef _UWP
//
         DWORD dwLastError = ::GetLastError();
//
//         string strPrefix;
//
//         string strRelative = path;
//
//         auto folderBase = winrt_folder(strRelative, strPrefix);
//
//         if (folderBase != nullptr)
//         {
//
//            strRelative.replace("/", "\\");
//
//            strPrefix.replace("/", "\\");
//
//            ::str::begins_eat_ci(strRelative, strPrefix);
//
//            strRelative.trim("/\\");
//
//            //strPrefix.trim_right("/\\");
//
//            try
//            {
//
//               auto item = ::wait(folderBase->GetItemAsync(strRelative));
//
//               if (item != nullptr)
//               {
//
//                  if (item->IsOfType(::winrt::Windows::Storage::StorageItemTypes::Folder))
//                  {
//
//                     if (is_set(petype))
//                     {
//
//                        *petype = ::file::e_type_folder;
//
//                     }
//
//                     return true;
//
//                  }
//                  else if (item->IsOfType(::winrt::Windows::Storage::StorageItemTypes::File))
//                  {
//
//                     if (is_set(petype))
//                     {
//
//                        *petype = ::file::e_type_file;
//
//                     }
//
//                     return true;
//
//                  }
//
//                  return false;
//
//               }
//
//            }
//            catch (...)
//            {
//
//            }
//
//         }
//
//#endif

         if (is_set(petype))
         {

            *petype = ::file::e_type_none;

         }

         return false;

      }

      if (is_set(petype))
      {

         if (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
         {

            *petype = ::file::e_type_folder;

         }
         else
         {

            *petype = ::file::e_type_file;

         }

      }

      return true;

   }







   ::file::path acme_path::_final(const char * path)
   {

      WCHAR wszPath[8192];

      ::u32 dw = GetFullPathNameW(L"\\\\?\\" + wstring(path), sizeof(wszPath) / sizeof(WCHAR), wszPath, nullptr);

      return wszPath;

   }


} // namespace windows_common



