// Created by camilo on 2021-09-03 00:29 BRT <3ThomasBS_
#include "framework.h"


namespace windows_common
{


   acme_dir::acme_dir()
   {


   }


   acme_dir::~acme_dir()
   {


   }


   ::e_status acme_dir::_create_directory(const char * path)
   {

      wstring wstr;

      if (file_path_is_absolute(path))
      {

         wstr = "\\\\?\\" + wstring(path);

      }
      else
      {

         wstr = path;

      }

      if (!::CreateDirectoryW(wstr, nullptr))
      {

         DWORD dwLastError = ::GetLastError();

         auto estatus = last_error_to_status(dwLastError);

         return false;

      }

      //#else
      //
      //         if (::mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) != 0)
      //         {
      //
      //            TranslateLastError();
      //
      //            return false;
      //
      //         }
      //
      //         set_last_status(success);
      //
      //#endif

      return true;

   }


   bool acme_dir::_is(const char * path)
   {

//#ifdef _UWP
//
//      //string str;
//
//      ////str = "\\\\?\\";
//      ////str += path1;
//
//      //str = path1;
//
//      //str.ends_eat_ci("\\");
//      //str.ends_eat_ci("/");
//      //str.ends_eat_ci("\\");
//      //str.ends_eat_ci("/");
//
//      u32 dwFileAttributes = ::windows_get_file_attributes(path1);
//
//      if (dwFileAttributes != INVALID_FILE_ATTRIBUTES)
//      {
//
//         return dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
//
//      }
//      else
//      {
//
//         ::u32 dwLastError = ::GetLastError();
//
//         string strPrefix;
//
//         {
//
//            string strRelative = path1;
//
//            auto folderBase = winrt_folder(strRelative, strPrefix);
//
//            if (folderBase != nullptr)
//            {
//
//               strRelative.replace("/", "\\");
//
//               strPrefix.replace("/", "\\");
//
//               ::str::begins_eat_ci(strRelative, strPrefix);
//
//               strRelative.trim("/\\");
//
//               //strPrefix.trim_right("/\\");
//
//               try
//               {
//
//                  auto folder = folderBase->GetFolderAsync(strRelative);
//
//                  if (folder != nullptr)
//                  {
//
//                     return true;
//
//                  }
//
//               }
//               catch (...)
//               {
//
//               }
//
//            }
//
//         }
//
//         return false;
//
//         //auto folder = wait(::winrt::Windows::Storage::StorageFolder::GetFolderFromPathAsync(path1));
//
//         //bool bOk = folder != nullptr;
//
//         //if (!bOk)
//         //{
//
//         //   set_last_error(dwLastError);
//
//         //}
//
//         //if (bOk)
//         //{
//
//         //   return true;
//
//         //}
//
//         //return bOk;
//
//      }
//
//
//#elif defined(WINDOWS_DESKTOP)

      //auto dwFileAttributes = ::windows_get_file_attributes(path1);

      wstring wstrPath(path);

      auto dwFileAttributes = ::GetFileAttributesW(wstrPath);

      if (dwFileAttributes == INVALID_FILE_ATTRIBUTES || !(dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {

         return false;

      }

      return true;

//#else
//
//      // dedicaverse stat -> Sir And Arthur - Cesar Serenato
//
//      return is_dir(path1);
//
//#endif

   }


} // namespace windows_common



