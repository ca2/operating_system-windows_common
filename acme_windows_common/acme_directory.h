// Created by camilo on 2021-08-09 23:27 <3ThomasBS_
#pragma once


#include "acme/filesystem/filesystem/acme_directory.h"


namespace windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON acme_directory :
      virtual public ::acme_directory
   {
   public:


      //__creatable_from_base(acme_directory, ::acme_directory);


      acme_directory();
      ~acme_directory() override;



      //void __create(const char * path) override;

      //bool _is(const char * path) override;


   };


} // namespace windows_common



