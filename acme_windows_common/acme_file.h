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


      void copy(const char * pszNew, const char * pszSrc, bool bOverwrite) override;
      //bool exists(const char * path) override;


      void ensure_exists(const char* path) override;
      void touch(const char* path) override;
      void clear_read_only(const char* path) override;


      void set_file_normal(const char* path) override;

      void put_contents(const char * path, const char * contents, memsize len) override;


      using ::acme_file::get_size;
      
      holding_status < filesize > get_size(const char * path) override;
      
      //int_bool file_is_equal_path_dup(const char * psz1, const char * psz2)

      //string as_string(const char * path, strsize iReadAtMostByteCount) override;
      //memsize as_memory(const char * path, void * p, memsize s) override;
      //bool as_memory(memory_base & memory, const char * path, memsize iReadAtMostByteCount) override;
      status < memory > as_memory(const char* path, strsize iReadAtMostByteCount) override;
      //void delete_file(const char * pszFileName) override;

      void put_block(const char* path, const block& block) override;


   };


} // namespace windows_common



