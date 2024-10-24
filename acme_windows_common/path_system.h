// Created by camilo on 2021-08-09 14:21 <3ThomasBS_
#pragma once


#include "acme/filesystem/filesystem/path_system.h"


namespace acme_windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON path_system :
      virtual public ::path_system
   {
   public:


      path_system();
      ~path_system() override;


      ::file::path _real_path(const ::file::path & path) override;

      ::file::enum_type get_type(const ::file::path & path) override;


   };


} // namespace acme_windows_common



