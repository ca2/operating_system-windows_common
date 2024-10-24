// Created by camilo on 2021-09-03 00:29 BRT <3ThomasBS_
#include "framework.h"
#include "directory_system.h"


#include "acme/_operating_system.h"


namespace acme_windows_common
{


   directory_system::directory_system()
   {


   }


   directory_system::~directory_system()
   {


   }


   ::file::path directory_system::get_current()
   {

      auto size = GetCurrentDirectoryW(0, nullptr);

      wstring wstr;

      auto buffer = wstr.get_buffer(size);

      GetCurrentDirectoryW(size + 1, buffer);

      wstr.release_buffer(size);

      return wstr;

   }



   ::file::path directory_system::tool()
   {

      return "C:/operating_system/tool-windows";

   }



} // namespace acme_windows_common



