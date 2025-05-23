// From acme/filesystem/file/_.cpp by camilo on 2021-08-09 
// 14:17 BRT <3ThomasBorregaardSorensen
#include "framework.h"
#include "file_system.h"
#include "directory_system.h"
#include "acme/exception/exception.h"
#include "acme/filesystem/file/exception.h"
#include "acme/filesystem/file/file.h"
#include "acme/filesystem/filesystem/path_system.h"
#include "acme/prototype/prototype/memory.h"
#include "acme/_operating_system.h"
#include "acme/operating_system/shared_posix/c_error_number.h"
#include "acme/operating_system/shared_posix/stdio_file.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/utime.h>
#include <io.h>
#include <stdlib.h>
#include <time.h>

//#ifdef UNIVERSAL_WINDOWS
//#include "acme/operating_system/universal_windows/_.h"
//#endif

CLASS_DECL_ACME void TRACELASTERROR();


namespace acme_windows_common
{


   void file_system::_copy(const ::file::path & pathNewParam, const ::file::path & pathSrcParam, bool bOverwrite)
   {

      string strNew(pathNewParam);

      //if (strNew.length() >= MAX_PATH)
      //{

      //   strNew = "\\\\.\\" + strNew;

      //}

      string strSrc(pathSrcParam);

      //if (strSrc.length() >= MAX_PATH)
      //{

      //   strSrc = "\\\\.\\" + strSrc;

      //}

      auto pathNew = path_system()->defer_process_relative_path(strNew);

      auto pathSrc = path_system()->defer_process_relative_path(strSrc);

      wstring wstrNew(pathNew);

      wstring wstrSrc(pathSrc);

      auto pathFolder = pathNew.folder();

      auto bDir = m_pdirectorysystem->is(pathFolder);

      if (!bDir)
      {
       
         m_pdirectorysystem->create(pathFolder);

      }

      auto pfileSrc = get_file(pathSrc, ::file::e_open_read | ::file::e_open_share_deny_none);

      auto pfileNew = get_file(pathNew, ::file::e_open_defer_create_directory |
         (bOverwrite ? ::file::e_open_create | ::file::e_open_truncate : ::file::e_open_create) | ::file::e_open_write);

      if (pfileSrc.nok() || pfileNew.nok())
      {

         return;
         
      }

      memory memory;

      memory.set_size(1_MiB);

      while (auto read = pfileSrc->read(memory))
      {

         if (read <= 0)
         {

            break;

         }

         pfileNew->write(memory(0, read));

      }

      pfileNew->set_modification_time(pfileSrc->modification_time());

      ///return ::CopyFileExW(wstrSrc,wstrNew, nullptr, nullptr, nullptr, COPY_FILE_NO_BUFFERING | (bOverwrite ? 0 : COPY_FILE_FAIL_IF_EXISTS)) ? true : false;
      //auto bCopy = ::CopyFileExW(wstrSrc, wstrNew, nullptr, nullptr, nullptr, (bOverwrite ? 0 : COPY_FILE_FAIL_IF_EXISTS)) ;

      //if (!bCopy)
      //{

      //   auto lasterror = ::GetLastError();

      //   if (lasterror == ERROR_INVALID_PARAMETER)
      //   {

      //      ::file_system::copy(pszNew, pszSrc, bOverwrite);

      //      return;

      //   }

      //   auto estatus = ::windows::last_error_status(lasterror);

      //   throw io_exception(estatus);

      //}

      //return true;

   }

   void file_system::ensure_exists(const ::file::path & pathParam)
   {

      if (exists(pathParam))
      {

         //return ::success;

         return;

      }

      auto path = path_system()->defer_process_relative_path(pathParam);
      
      wstring wstrPath(path);
      
      int fd = _wopen(wstrPath, O_WRONLY | O_CREAT);

      if (fd < 0) // Couldn't open that path.
      {
         
         throw ::exception(::error_io, "ensure_exists failed to open file at path \"" + path + "\"");

      }

      ::close(fd);

      clear_read_only(path);

      //return ::success;

   }


   void file_system::clear_read_only(const ::file::path & pathParam)
   {

      auto path = path_system()->defer_process_relative_path(pathParam);

      wstring wstrPath(path);

      auto attributes = ::GetFileAttributesW(wstrPath);

      if (attributes == INVALID_FILE_ATTRIBUTES)
      {

         //return ::error_failed;

         throw ::exception(::error_failed);

      }

      if (!(attributes & FILE_ATTRIBUTE_READONLY))
      {

         //return ::success_none;

         return;

      }

      if (!::SetFileAttributesW(wstrPath, attributes & ~FILE_ATTRIBUTE_READONLY))
      {

         //return ::error_failed;

         throw ::exception(::error_failed);

      }

      //return ::success;

   }


   void file_system::set_file_normal(const ::file::path & pathParam)
   {

      auto path = path_system()->defer_process_relative_path(pathParam);

      wstring wstrPath(path);

      auto attributes = ::GetFileAttributesW(wstrPath);

      if (attributes == INVALID_FILE_ATTRIBUTES)
      {

         //return ::error_failed;

         throw ::exception(::error_failed);

      }

      if (attributes == FILE_ATTRIBUTE_NORMAL)
      {

         return;

      }

      if (!::SetFileAttributesW(wstrPath, FILE_ATTRIBUTE_NORMAL))
      {

         //return ::error_failed;

         throw ::exception(::error_failed);

      }

      //return ::success;

   }


   void file_system::touch(const ::file::path & pathParam)
   {

      auto path = path_system()->defer_process_relative_path(pathParam);

      m_pdirectorysystem->create(file_path_folder(path));

      wstring wstrPath(path.windows_path().path());

      if (exists(path))
      {

         set_file_normal(path);

         clear_read_only(path);

         auto time = ::time(nullptr);

         _utimbuf ut;

         ut.actime = time;

         ut.modtime = time;

         int rc = _wutime(wstrPath, &ut);

         if (rc)
         {

            auto cerrornumber = c_error_number();

            auto estatus = cerrornumber.failed_estatus();

            throw ::exception(estatus);

         }

      }
      else
      {

         int fd = _wopen(wstrPath, O_WRONLY | O_CREAT);

         if (fd < 0)
         {

            auto cerrornumber = c_error_number();

            auto estatus = cerrornumber.failed_estatus();

            throw ::exception(estatus);

         }

         ::close(fd);

      }

   }


   void file_system::put_contents(const ::file::path & pathParam, const ::block & blockContents)
   {

      auto path = path_system()->defer_process_relative_path(pathParam);

      auto pathFolder = file_path_folder(path);

      m_pdirectorysystem->create(pathFolder);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      auto windowsPath = path.windows_path();

      auto pfile = stdio_open(this, path, "w", _SH_DENYWR);

      if (!pfile)
      {

         throw ::exception(::error_io);

      }

      pfile->write(blockContents);

   }


   filesize file_system::get_size(const ::file::path & pathParam)
   {

      auto path = path_system()->defer_process_relative_path(pathParam);

      ::windows::file_instance file;

//#ifdef WINDOWS_DESKTOP

      if (!file.safe_create_file(path, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr))
      {

         DWORD dwLastError = ::GetLastError();

         throw_last_error_exception(path, ::file::e_open_read, dwLastError, "acme_windows_common::file_system::get_size safe_create_file failed");

      }

//#else

  //    HANDLE hfile = hfile_create(path.get_os_path(), GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

//#endif

      auto uFileSize = file.get_file_size();

      return uFileSize;

   }


   //int_bool file_is_equal_path_dup(const ::file::path & path1, const ::file::path & path2)
   //{

   //   auto path1 = ::g_psystem->m_ppathsystem->defer_process_relative_path(psz1);

   //   auto path2 = ::g_psystem->m_ppathsystem->defer_process_relative_path(psz2);

   //   const int iBufSize = MAX_PATH * 8;

   //   wstring pwsz1 = utf8_to_unicode(path1);

   //   wstring pwsz2 = utf8_to_unicode(path2);

   //   unichar * pwszFile1;

   //   unichar * pwszFile2;

   //   unichar * pwszPath1 = ___new unichar[iBufSize];

   //   unichar * pwszPath2 = ___new unichar[iBufSize];

   //   int iCmp = -1;

   //   if (GetFullPathNameW(pwsz1, iBufSize, pwszPath1, &pwszFile1))
   //   {

   //      if (GetFullPathNameW(pwsz2, iBufSize, pwszPath2, &pwszFile2))
   //      {

   //         string path1 = unicode_to_utf8(pwszPath1);

   //         string path2 = unicode_to_utf8(pwszPath2);

   //         iCmp = ansi_compare_ci(path1, path2);

   //      }

   //   }

   //   delete pwszPath1;

   //   delete pwszPath2;

   //   return iCmp == 0;

   //}


   //string file_system::as_string(const ::file::path & path, character_count iReadAtMostByteCount)
   //{

   //   string str;

   //   wstring wstr(path);

   //   FILE * pfile = _wfsopen(wstr, L"r", _SH_DENYNO);

   //   if (::is_null(pfile))
   //   {

   //      DWORD dwLastError = ::GetLastError();

   //      TRACELASTERROR();

   //      return "";

   //   }

   //   DWORD dwSize;

   //   dwSize = (DWORD)size(pfile);

   //   iReadAtMostByteCount = iReadAtMostByteCount < 0 ? dwSize : minimum(iReadAtMostByteCount, (::character_count)dwSize);

   //   char * psz = str.get_buffer(iReadAtMostByteCount);

   //   ::size_t iPos = 0;

   //   while (iReadAtMostByteCount - iPos > 0)
   //   {

   //      auto dwRead = ::fread(psz + iPos, 1, (size_t)iReadAtMostByteCount - iPos, file);

   //      if (dwRead <= 0)
   //      {

   //         break;

   //      }

   //      iPos += dwRead;

   //   }

   //   psz[iPos] = '\0';

   //   str.release_buffer(iReadAtMostByteCount);

   //   fclose(file);

   //   return str;

   //}


   memory file_system::as_memory(const ::file::path & pathParam, character_count iReadAtMostByteCount, bool bNoExceptionIfNotFound)
   {

      return ::file_system::as_memory(pathParam, iReadAtMostByteCount, bNoExceptionIfNotFound);

      //auto path = path_system()->defer_process_relative_path(pathParam);

      //FILE* pfile = _wfsopen(wstring(path), L"r", _SH_DENYNO);
      //
      //if (pfile == nullptr)
      //{

      //   if (bNoExceptionIfNotFound)
      //   {

      //      return {};

      //   }

      //   int iErrNo = errno;

      //   auto estatus = errno_status(iErrNo);

      //   throw ::exception(estatus);

      //}

      //::memory memory;

      //if(fseek(pfile, (long)0, SEEK_END))

      //auto uSize = pfile->size();

      //if (iReadAtMostByteCount < 0)
      //{

      //   iReadAtMostByteCount = (character_count)uSize;

      //}
      //else
      //{

      //   iReadAtMostByteCount = minimum(iReadAtMostByteCount, (character_count)uSize);

      //}

      //memory.set_size(iReadAtMostByteCount);

      //auto p = memory.data();

      //::size_t iPos = 0;

      //while (iReadAtMostByteCount - iPos > 0)
      //{

      //   auto dwRead = pfile->read(p + iPos, (size_t)iReadAtMostByteCount - iPos);

      //   if (dwRead <= 0)
      //   {

      //      break;

      //   }

      //   iPos += dwRead;

      //}

      //return memory;


      //::memory memory;

      //try
      //{

      //   ::memory memoryBuffer;

      //   memoryBuffer.set_size(1_mb);

      //   while (true)
      //   {

      //      auto iRead = fread(memoryBuffer.data(), 1, memoryBuffer.size(), pfile);

      //      if (iRead <= 0)
      //      {

      //         break;

      //      }

      //      memory.append(memoryBuffer.data(), iRead);

      //   };

      //}
      //catch (...)
      //{

      //}

      //fclose(pfile);

      //return ::move(memory);

   }




   //bool file_system::as_memory(memory_base & memory, const ::file::path & path, memsize iReadAtMostByteCount)
   //{

   //   memory.set_size(0);

   //   FILE * file = FILE_open(path, "r", _SH_DENYNO);

   //   if (file == nullptr)
   //   {

   //      return false;

   //   }

   //   filesize iSize = FILE_get_size(file);

   //   if (iSize < 0)
   //   {

   //      ::fclose(file);

   //      return false;

   //   }

   //   iReadAtMostByteCount = minimum_non_negative(iReadAtMostByteCount, (::character_count)iSize);

   //   memory.set_size(iReadAtMostByteCount);

   //   auto dwRead = ::fread(memory.data(), 1, (unsigned int)iReadAtMostByteCount, file);

   //   memory.set_size(dwRead);

   //   ::fclose(file);

   //   return true;

   //}


   /*void file_system::delete_file(const ::file::path & path)
   {

      wstring wstrFilePath(path.get_os_path());

      if (!::DeleteFileW(wstrFilePath))
      {

         auto dwLastError = ::GetLastError();

         auto estatus = ::windows::last_error_status(dwLastError);

         return estatus;

      }

      return ::success;

   }*/



   void file_system::put_block(const ::file::path & pathParam, const block& block)
   {

      auto path = path_system()->defer_process_relative_path(pathParam);

      m_pdirectorysystem->create(file_path_folder(path));

      ::windows::file_instance file;

//#ifdef UNIVERSAL_WINDOWS
//
//      HANDLE h = ::CreateFile2(path.get_os_path(), GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS, nullptr);
//
//#else

      if (!file.safe_create_file(path, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr))
      {

         DWORD dwLastError = ::GetLastError();

         throw_last_error_exception(path, ::file::e_open_write, dwLastError, "acme_windows_common::file_system::put_block safe_create_file failed");

      }
//#endif

      auto size = block.size();

      ::memsize position = 0;

      while ((size -= file.defer_write(block.begin() + position, block.size() - position)) > 0);

   }


   ::file::path file_system::module()
   {

      ::file::path path;

      {

         wstring wstrPath;

         auto p = wstrPath.get_buffer(MAX_PATH * 16);

         if (!GetModuleFileNameW(nullptr, p, (DWORD)wstrPath.length()))
         {

            return "";

         }

         wstrPath.release_buffer();

         path = ::string(wstrPath);

      }

      return path;

   }


} // namespace acme_windows_common



