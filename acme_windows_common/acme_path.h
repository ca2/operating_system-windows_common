// Created by camilo on 2021-08-09 14:21 <3ThomasBS_
#pragma once


#include "acme/filesystem/filesystem/acme_path.h"


namespace acme_windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON acme_path :
      virtual public ::acme_path
   {
   public:


      acme_path();
      ~acme_path() override;


      ::file::path _final(const char * path) override;

      ::file::enum_type get_type(const char * path) override;


   };


} // namespace acme_windows_common



