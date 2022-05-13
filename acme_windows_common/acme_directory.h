// Created by camilo on 2021-08-09 23:27 <3ThomasBS_
#pragma once


#include "acme/filesystem/filesystem/acme_directory.h"


namespace acme_windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON acme_directory :
      virtual public ::acme_directory
   {
   public:


      acme_directory();
      ~acme_directory() override;


   };


} // namespace acme_windows_common



