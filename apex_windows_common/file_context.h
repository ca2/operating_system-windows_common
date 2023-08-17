// Created by camilo on 2023-03-16 12:58 <3ThomasBorregaardSorensen!!
#pragma once



#include "acme/_operating_system.h"


#include "acme/filesystem/filesystem/file_context.h"


namespace apex_windows_common
{


   class CLASS_DECL_APEX_WINDOWS_COMMON file_context :
      virtual public ::file_context
   {
   public:


      file_context();
      ~file_context() override;


      void initialize(::particle * pparticle) override;


      void init_system() override;


      virtual void get_status(const ::file::path & path, ::file::file_status & status);
      virtual void set_status(const ::file::path & path, const ::file::file_status & status);


      //virtual bool win32_file_system_item_exists(const ::file::path & path);



   };


} // namespace apex_windows_common



