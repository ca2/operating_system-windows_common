// Created by camilo on 2021-08-09 14:21 <3ThomasBS_
#pragma once


#include "acme/filesystem/filesystem/file_system.h"


namespace acme_windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON file_system :
      virtual public ::file_system
   {
   public:


      //__creatable_from_base(file_system, ::file_system);


      //::file::path _final(const ::file::path & path) override;

      ::file::path module() override;


      void _copy(const ::file::path & pathNew, const ::file::path & pathSrc, bool bOverwrite) override;
      //bool exists(const ::file::path & path) override;


      void ensure_exists(const ::file::path & path) override;
      void touch(const ::file::path & path) override;
      void clear_read_only(const ::file::path & path) override;


      void set_file_normal(const ::file::path & path) override;


      using ::file_system::put_contents;

      void put_contents(const ::file::path & path, const ::block & block) override;


      using ::file_system::get_size;
      
      filesize get_size(const ::file::path & path) override;
      
      //int_bool file_is_equal_path_dup(const ::file::path & path1, const ::file::path & path2)

      //string as_string(const ::file::path & path, character_count iReadAtMostByteCount) override;
      //memsize as_memory(const ::file::path & path, void * p, memsize s) override;
      //bool as_memory(memory_base & memory, const ::file::path & path, memsize iReadAtMostByteCount) override;
      memory as_memory(const ::file::path & path, character_count iReadAtMostByteCount, bool bNoExceptionIfNotFound = true) override;
      //void delete_file(const ::file::path & pathFileName) override;

      void put_block(const ::file::path & path, const block& block) override;


   };


} // namespace acme_windows_common



