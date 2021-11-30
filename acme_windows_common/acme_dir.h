// Created by camilo on 2021-08-09 23:27 <3ThomasBS_
#pragma once


#include "acme/filesystem/filesystem/acme_dir.h"


namespace windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON acme_dir :
      virtual public ::acme_dir
   {
   public:


      acme_dir();
      ~acme_dir() override;



      ::e_status _create_directory(const char * path) override;

      //bool _is(const char * path) override;


   };


} // namespace windows_common



