// Created by camilo on 2021-08-09 23:27 <3ThomasBS_
#pragma once


#include "acme/filesystem/filesystem/directory_system.h"


namespace acme_windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON directory_system :
      virtual public ::directory_system
   {
   public:


      directory_system();
      ~directory_system() override;


      ::file::path get_current() override;

      ::file::path tool() override;

   };


} // namespace acme_windows_common



