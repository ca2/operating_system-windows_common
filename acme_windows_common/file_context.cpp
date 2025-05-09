// Created by camilo on 2023-03-16 12:58 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "file_context.h"
#include "acme/filesystem/file/status.h"
#include "acme/operating_system/windows_common/find_file.h"


namespace acme_windows_common
{


   file_context::file_context()
   {

      m_bFolderResourceCalculated = false;

   }


   file_context::~file_context()
   {

   }



   void file_context::initialize(::particle * pparticle)
   {

      //auto estatus = 

      ::file_context::initialize(pparticle);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //::pointer<::apex::system>psystem = system();

      //m_pfilesystem = psystem->m_pfilesystem;

      //m_pdirectorysystem = psystem->m_pdirectorysystem;

      //return ::success;

   }


   void file_context::init_system()
   {

      //auto estatus = m_pfilesystem->update_module_path();

      //if (!estatus)
      //{

      //   return estatus;

      //}

//      return ::success;

   }


   void file_context::term_system()
   {

      //auto estatus = m_pfilesystem->update_module_path();

      //if (!estatus)
      //{

      //   return estatus;

      //}

//      return ::success;

   }
   
   
   void file_context::destroy()
   {

      ::file_context::destroy();

   }


   void file_context::set_status(const ::file::path & path, const ::file::file_status & status)
   {

      wstring pszFileName(path);


      unsigned int wAttr;
      FILETIME creationTime;
      FILETIME lastAccessTime;
      FILETIME lastWriteTime;
      LPFILETIME pCreationTime = nullptr;

      LPFILETIME pLastAccessTime = nullptr;

      LPFILETIME pLastWriteTime = nullptr;

      if ((wAttr = ::windows::get_file_attributes(path)) == (unsigned int)INVALID_FILE_ATTRIBUTES)
      {

         DWORD dwLastError = ::GetLastError();

         auto estatus = ::windows::last_error_status(dwLastError);

         auto errorcode = ::windows::last_error_error_code(dwLastError);

         throw ::file::exception(estatus, errorcode, path, ::file::e_open_none, "!windows_get_file_attributes");

      }

      if ((unsigned int)status.m_attribute != wAttr && (wAttr & FILE_ATTRIBUTE_READONLY))
      {
         
         // set file attribute, only if currently readonly.
         // This way we will be able to modify the time assuming the
         // caller changed the file from readonly.

         ::windows::set_file_attributes(path, status.m_attribute);
         //{

         //   DWORD dwLastError = ::GetLastError();

         //   auto estatus = ::windows::last_error_status(dwLastError);

         //   auto errorcode = ::windows::last_error_error_code(dwLastError);

         //   throw ::file::exception(estatus, errorcode, ::string(pszFileName), ::file::e_open_none, "!SetFileAttributesW");

         //}

      }

      // last modification time
      if (status.m_timeModification != 0_s)
      {

         lastWriteTime = as_FILETIME(file_time(status.m_timeModification));

         pLastWriteTime = &lastWriteTime;

         // last access time
         if (status.m_timeAccess != 0_s)
         {

            lastAccessTime = as_FILETIME(file_time(status.m_timeAccess));

            pLastAccessTime = &lastAccessTime;

         }

         // create time
         if (status.m_timeCreation != 0_s)
         {

            creationTime = as_FILETIME(file_time(status.m_timeCreation));

            pCreationTime = &creationTime;

         }

         ::windows::file_instance fileinstance;

         if (!fileinstance.safe_create_file(path, GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
            nullptr))
         {

            DWORD dwLastError = ::GetLastError();

            throw_last_error_exception(path, ::file::e_open_read | ::file::e_open_write, dwLastError, "apex_windows::file_context::set_status safe_create_file failed");

         }


         //::pointer < ::file::exception > * ppfileexception

         //HANDLE hFile = ::CreateFileW((LPWSTR)(const ::wide_character *)pszFileName, GENERIC_READ | GENERIC_WRITE,

         //   FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
         //   nullptr);

         //if (hFile == INVALID_HANDLE_VALUE)
         //{

         //   auto estatus = ::windows::last_error_status(::GetLastError());

         //   throw ::exception(estatus);

         //   return;

         //;;
      //}

         fileinstance.set_file_time(pCreationTime, pLastAccessTime, pLastWriteTime);
         //{

         //   auto estatus = ::windows::last_error_status(::GetLastError());

         //   throw ::exception(estatus);

         //}

         //if (!::CloseHandle(hFile))
         //{

         //   auto estatus = ::windows::last_error_status(::GetLastError());

         //   throw ::exception(estatus);

         //}

      }

      if ((unsigned int)status.m_attribute != wAttr && !(wAttr & FILE_ATTRIBUTE_READONLY))
      {

         ::windows::set_file_attributes(path, status.m_attribute);
         //{

         //   auto estatus = ::windows::last_error_status(::GetLastError());

         //   throw ::exception(estatus);

         //}

      }

   }



   void file_context::get_status(const ::file::path & path, ::file::file_status & rStatus)
   {

      wstring wstrFullName(path);

      ::windows::find_file findfile;

      findfile.find_first_file(path);

      //if (hFind == INVALID_HANDLE_VALUE)
      //{

      //   throw ::exception(error_failed);

      //}

      //FindClose(hFind);

      // strip attribute of NORMAL bit, our API doesn't have a "normal" bit.
      rStatus.m_attribute = (unsigned char)(findfile.m_finddata.dwFileAttributes & ~FILE_ATTRIBUTE_NORMAL);

      // get just the low unsigned int of the file int_size
      //ASSERT(findFileData.nFileSizeHigh == 0);
      rStatus.m_filesize = (::filesize)make64_from32(findfile.m_finddata.nFileSizeLow, findfile.m_finddata.nFileSizeHigh);

      //auto pnode = psystem->node();

      // convert times as appropriate
      rStatus.m_timeCreation = class ::time(as_file_time(findfile.m_finddata.ftCreationTime));
      rStatus.m_timeAccess = class ::time(as_file_time(findfile.m_finddata.ftLastAccessTime));
      rStatus.m_timeModification = class ::time(as_file_time(findfile.m_finddata.ftLastWriteTime));

      if (rStatus.m_timeCreation <= 0_s)
         rStatus.m_timeCreation = rStatus.m_timeModification;

      if (rStatus.m_timeAccess <= 0_s)
         rStatus.m_timeAccess = rStatus.m_timeModification;

      //return true;

   }


   //bool file_context::win32_file_system_item_exists(const ::file::path & path)
   //{

   //   auto dwFileAttributes = ::windows::_get_file_attributes(path);

   //   return dwFileAttributes != INVALID_FILE_ATTRIBUTES;

   //}


} // namespace acme_windows_common



