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


   void acme_dir::_create_directory(const char * path)
   {

      wstring wstr;

      if (file_path_is_absolute(path))
      {

         wstr = L"\\\\?\\" + wstring(path);

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



} // namespace windows_common



