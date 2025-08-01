// From operating_system-window by camilo on 2022-02-25 18>39 <3ThomasBorregaardSorensen!!
#pragma once


#include "acme/filesystem/file/file.h"
#include "acme/_operating_system.h"


//#ifndef WINDOWS_DESKTOP
//#error "This file is designed to be used only as Windows Desktop Source"
//#endif


//class FileException;
//struct FileStatus;
//
//
//void CLASS_DECL_ACME_WINDOWS vfxGetRoot(const unichar * pszPath, string & wstrRoot);
//
//void CLASS_DECL_ACME_WINDOWS vfxGetRoot(wstring & wstrRoot, const wstring & wstrPath);


namespace acme_windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON file :
      virtual public ::file::file
   {
   public:


      //__creatable_from_base(file, ::file::file);


     /* enum Attribute
      {
         normal =    0x00,
         readOnly =  0x01,
         hidden =    0x02,
         system =    0x04,
         volume =    0x08,
         directory = 0x10,
         archive =   0x20
      };


      enum BufferCommand
      {

         bufferRead,
         bufferWrite,
         bufferCommit,
         bufferCheck

      };*/

      ::windows::file            m_file;
      //unsigned int                      m_dwAccessMode;
      ::windows_path             m_windowspath;
      //int               m_iCharacterPutBack;


      file();
      file(HANDLE hFile);
      file(const ::scoped_string & scopedstrFileName, ::file::e_open eopen);
      ~file() override;


      //void assert_ok() const override;
      //void dump(dump_context & dumpcontext) const override;



      operator HANDLE() const;

      filesize get_position() const override;
      ::file::file_status get_status() const override;
      ::file::path get_file_path() const override;
      void set_file_path(const ::file::path & path) override;


      void open(const ::file::path & pathFileName, ::file::e_open eopen, ::pointer < ::file::exception > * pfileexception = nullptr) override;


      void translate(filesize filesize, ::enum_seek nFrom) override;
      void set_size(filesize dwNewLen) override;
      filesize size() const override;

      
      using ::file::file::read;
      memsize read(void * p, ::memsize s) override;


      using ::file::file::write;
      void write(const void * p, ::memsize s) override;


      //virtual int peek_character();
      //virtual int get_character();
      //virtual int put_character_back(int iCharacter);


      void lock(filesize dwPos, filesize dwCount) override;
      void unlock(filesize dwPos, filesize dwCount) override;

      void flush() override;
      void close() override;

      bool is_opened() const override;



      class ::time modification_time() override;
      void set_modification_time(const class ::time& time) override;



   };


} // namespace windows_common



