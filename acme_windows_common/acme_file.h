// Created by camilo on 2021-08-09 14:21 <3ThomasBS_
#pragma once


#include "acme/filesystem/filesystem/acme_file.h"


namespace acme_windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON acme_file :
      virtual public ::acme_file
   {
   public:


      //__creatable_from_base(acme_file, ::acme_file);


      //::file::path _final(const ::file::path & path) override;


      void _copy(const ::file::path & pathNew, const ::file::path & pathSrc, bool bOverwrite) override;
      //bool exists(const ::file::path & path) override;


      void ensure_exists(const ::file::path & path) override;
      void touch(const ::file::path & path) override;
      void clear_read_only(const ::file::path & path) override;


      void set_file_normal(const ::file::path & path) override;


      using ::acme_file::put_contents;

      void put_contents(const ::file::path & path, const ::scoped_string & scopedstr) override;


      using ::acme_file::get_size;
      
      filesize get_size(const ::file::path & path) override;
      
      //int_bool file_is_equal_path_dup(const ::file::path & path1, const ::file::path & path2)

      //string as_string(const ::file::path & path, strsize iReadAtMostByteCount) override;
      //memsize as_memory(const ::file::path & path, void * p, memsize s) override;
      //bool as_memory(memory_base & memory, const ::file::path & path, memsize iReadAtMostByteCount) override;
      memory as_memory(const ::file::path & path, strsize iReadAtMostByteCount, bool bNoExceptionIfNotFound = true) override;
      //void delete_file(const ::file::path & pathFileName) override;

      void put_block(const ::file::path & path, const block& block) override;


   };


} // namespace acme_windows_common



