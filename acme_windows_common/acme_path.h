// Created by camilo on 2021-08-09 14:21 <3ThomasBS_
#pragma once


#include "acme/filesystem/filesystem/acme_path.h"


namespace windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON acme_path :
      virtual public ::acme_path
   {
   public:


      __creatable_from_library(acme_path, ::acme_path, "acme_windows_common");

      ::file::path _final(const char * path) override;
      bool is_file_or_dir(const char * path, ::file::enum_type * petype) override;


   };


} // namespace windows_common



