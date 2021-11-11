// Created by camilo on 2021-08-09 14:21 <3ThomasBS_
#pragma once


#include "acme/filesystem/filesystem/acme_file.h"


namespace windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON acme_file :
      virtual public ::acme_file
   {
   public:


      //::file::path _final(const char * path) override;


      ::e_status copy(const char * pszNew, const char * pszSrc, bool bOverwrite) override;
      bool exists(const char * path) override;


      ::e_status ensure_exists(const char* path) override;
      ::e_status touch(const char* path) override;
      ::e_status clear_read_only(const char* path) override;

      ::e_status put_contents(const char * path, const char * contents, memsize len) override;


      using ::acme_file::get_size;
      filesize get_size(const char * path) override;
      
      //int_bool file_is_equal_path_dup(const char * psz1, const char * psz2)

      //string as_string(const char * path, strsize iReadAtMostByteCount) override;
      //memsize as_memory(const char * path, void * p, memsize s) override;
      //bool as_memory(memory_base & memory, const char * path, memsize iReadAtMostByteCount) override;
      ::e_status delete_file(const char * pszFileName) override;


   };


} // namespace windows_common



