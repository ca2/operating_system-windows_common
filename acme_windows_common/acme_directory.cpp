// Created by camilo on 2021-09-03 00:29 BRT <3ThomasBS_
#include "framework.h"
#include "acme_directory.h"


#include "acme/_operating_system.h"


namespace acme_windows_common
{


   acme_directory::acme_directory()
   {


   }


   acme_directory::~acme_directory()
   {


   }


   ::file::path acme_directory::get_current()
   {

      auto size = GetCurrentDirectoryW(0, nullptr);

      wstring wstr;

      auto buffer = wstr.get_buffer(size);

      GetCurrentDirectoryW(size + 1, buffer);

      wstr.release_buffer(size);

      return wstr;

   }



   ::file::path acme_directory::tool()
   {

      return "C:/operating_system/tool-windows";

   }



} // namespace acme_windows_common



