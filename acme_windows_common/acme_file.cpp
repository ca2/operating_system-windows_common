// From acme/filesystem/file/_.cpp by camilo on 2021-08-09 
// 14:17 BRT <3ThomasBorregaardS�rensen
#include "framework.h"
#include "acme/operating_system.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/utime.h>
#include <io.h>
#include <stdlib.h>
#include <time.h>


CLASS_DECL_ACME void TRACELASTERROR();


namespace windows_common
{


   //::file::path acme_file::_final(const char * path)
   //{

   //   WCHAR wszPath[8192];

   //   ::u32 dw = GetFullPathNameW(L"\\\\?\\" + wstring(path), sizeof(wszPath) / sizeof(WCHAR), wszPath, nullptr);

   //   return wszPath;

   //}


   void acme_file::copy(const char * pszNew, const char * pszSrc, bool bOverwrite)
   {

      string strNew(pszNew);

      if (strNew.get_length() >= MAX_PATH)
      {

         strNew = "\\\\.\\" + strNew;

      }

      string strSrc(pszSrc);

      if (strSrc.get_length() >= MAX_PATH)
      {

         strSrc = "\\\\.\\" + strSrc;

      }

      wstring wstrNew(strNew);

      wstring wstrSrc(strSrc);

      auto pathFolder = file_path_folder(strNew);

      auto bDir = m_pacmedir->is(pathFolder);

      if (!bDir)
      {
       
         m_pacmedir->create(pathFolder);

      }

      //return ::CopyFileExW(wstrSrc,wstrNew, nullptr, nullptr, nullptr, COPY_FILE_NO_BUFFERING | (bOverwrite ? 0 : COPY_FILE_FAIL_IF_EXISTS)) ? true : false;
      auto bCopy = ::CopyFileExW(wstrSrc, wstrNew, nullptr, nullptr, nullptr, (bOverwrite ? 0 : COPY_FILE_FAIL_IF_EXISTS)) ;

      if (!bCopy)
      {

         auto lastError = ::GetLastError();

         auto estatus = last_error_to_status(lastError);

         throw io_exception(estatus);

      }

      //return true;

   }

   void acme_file::ensure_exists(const char* path)
   {

      if (exists(path))
      {

         //return ::success;

         return;

      }
      
      wstring wstrPath(path);
      
      int fd = _wopen(wstrPath, O_WRONLY | O_CREAT);

      if (fd < 0) // Couldn't open that path.
      {
         
         throw_status(::error_io);

      }

      ::close(fd);

      //return ::success;

   }


   void acme_file::clear_read_only(const char* path)
   {

      wstring wstrPath(path);

      auto attributes = ::GetFileAttributesW(wstrPath);

      if (attributes == INVALID_FILE_ATTRIBUTES)
      {

         //return ::error_failed;

         throw_status(::error_failed);

      }

      if (!(attributes & FILE_ATTRIBUTE_READONLY))
      {

         //return ::success_none;

         return;

      }

      if (!::SetFileAttributesW(wstrPath, attributes & ~FILE_ATTRIBUTE_READONLY))
      {

         //return ::error_failed;

         throw_status(::error_failed);

      }

      //return ::success;

   }


   void acme_file::set_file_normal(const char* path)
   {

      wstring wstrPath(path);

      auto attributes = ::GetFileAttributesW(wstrPath);

      if (attributes == INVALID_FILE_ATTRIBUTES)
      {

         //return ::error_failed;

         throw_status(::error_failed);

      }

      if (attributes == FILE_ATTRIBUTE_NORMAL)
      {

         //return ::success_none;

         throw_status(::error_failed);

      }

      if (!::SetFileAttributesW(wstrPath, FILE_ATTRIBUTE_NORMAL))
      {

         //return ::error_failed;

         throw_status(::error_failed);

      }

      //return ::success;

   }


   void acme_file::touch(const char* path)
   {

      m_pacmedir->create(file_path_folder(path));

      wstring wstrPath(path);

      if (exists(path))
      {

         auto time = ::time(nullptr);

         _utimbuf ut;

         ut.actime = time;

         ut.modtime = time;

         int rc = _wutime(wstrPath, &ut);

         if (rc)
         {

            int iErrNo = errno;

            auto estatus = failed_errno_to_status(iErrNo);

            throw_status(estatus);

         }

      }
      else
      {

         int fd = _wopen(wstrPath, O_WRONLY | O_CREAT);

         if (fd < 0)
         {

            int iErrNo = errno;

            auto estatus = failed_errno_to_status(iErrNo);

            throw_status(estatus);

         }

         ::close(fd);

      }

   }


   void acme_file::put_contents(const char * path, const char * contents, memsize len)
   {

      /*auto estatus =*/ m_pacmedir->create(file_path_folder(path));

      //if (!estatus)
      //{

      //   return estatus;

      //}

      wstring wstrPath(path);

      auto pfile = stdio_open(path, "w", _SH_DENYWR);

      if (!pfile)
      {

         throw_status(::error_io);

      }

      if (len < 0)
      {

         len = ansi_length(contents);

      }

      pfile->write(contents, len);

   }


   filesize acme_file::get_size(const char * path)
   {

#ifdef WINDOWS_DESKTOP

      HANDLE hfile = CreateFileW(wstring(path), GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

#else

      hfile hfile = hfile_create(path, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

#endif

      if (hfile == INVALID_HANDLE_VALUE)
      {

         DWORD dwLastError = ::GetLastError();

         auto estatus = last_error_to_status(dwLastError);

         return estatus;

      }

      DWORD dwHi = 0;

      LARGE_INTEGER largeintegerFileSize;

      if (!GetFileSizeEx(hfile, &largeintegerFileSize))
      {

         DWORD dwLastError = ::GetLastError();

         auto estatus = last_error_to_status(dwLastError);

         ::CloseHandle(hfile);

         return estatus;

      }

      u64 u = largeintegerFileSize.LowPart;

      u |= ((u64)largeintegerFileSize.HighPart) << 32ULL;

      CloseHandle(hfile);

      return u;

   }


   int_bool file_is_equal_path_dup(const char * psz1, const char * psz2)
   {

      const i32 iBufSize = MAX_PATH * 8;

      wstring pwsz1 = ::str::international::utf8_to_unicode(psz1);

      wstring pwsz2 = ::str::international::utf8_to_unicode(psz2);

      unichar * pwszFile1;

      unichar * pwszFile2;

      unichar * pwszPath1 = new unichar[iBufSize];

      unichar * pwszPath2 = new unichar[iBufSize];

      i32 iCmp = -1;

      if (GetFullPathNameW(pwsz1, iBufSize, pwszPath1, &pwszFile1))
      {

         if (GetFullPathNameW(pwsz2, iBufSize, pwszPath2, &pwszFile2))
         {

            string path1 = ::str::international::unicode_to_utf8(pwszPath1);

            string path2 = ::str::international::unicode_to_utf8(pwszPath2);

            iCmp = ansi_compare_ci(path1, path2);

         }

      }

      delete pwszPath1;

      delete pwszPath2;

      return iCmp == 0;

   }


   //string acme_file::as_string(const char * path, strsize iReadAtMostByteCount)
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

   //   dwSize = (DWORD)get_size(pfile);

   //   iReadAtMostByteCount = iReadAtMostByteCount < 0 ? dwSize : minimum(iReadAtMostByteCount, (::strsize)dwSize);

   //   char * psz = str.get_string_buffer(iReadAtMostByteCount);

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

   //   str.release_string_buffer(iReadAtMostByteCount);

   //   fclose(file);

   //   return str;

   //}


   memory acme_file::as_memory(const char* path, strsize iReadAtMostByteCount)
   {

      FILE* pfile = _wfsopen(wstring(path), L"r", _SH_DENYNO);
      
      if (pfile == nullptr)
      {

         int iErrNo = errno;

         auto estatus = errno_to_status(iErrNo);

         throw_status(estatus);

      }

      ::memory memory;

      try
      {

         ::memory memoryBuffer;

         memoryBuffer.set_size(1_mb);

         while (true)
         {

            auto iRead = fread(memoryBuffer.get_data(), 1, memoryBuffer.get_size(), pfile);

            if (iRead <= 0)
            {

               break;

            }

            memory.append(memoryBuffer.get_data(), iRead);

         };

      }
      catch (...)
      {

      }

      fclose(pfile);

      return ::move(memory);

   }




   //bool acme_file::as_memory(memory_base & memory, const char * path, memsize iReadAtMostByteCount)
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

   //   iReadAtMostByteCount = minimum_non_negative(iReadAtMostByteCount, (::strsize)iSize);

   //   memory.set_size(iReadAtMostByteCount);

   //   auto dwRead = ::fread(memory.get_data(), 1, (u32)iReadAtMostByteCount, file);

   //   memory.set_size(dwRead);

   //   ::fclose(file);

   //   return true;

   //}


   /*void acme_file::delete_file(const char * pszFileName)
   {

      wstring wstrFilePath(pszFileName);

      if (!::DeleteFileW(wstrFilePath))
      {

         auto dwLastError = ::GetLastError();

         auto estatus = last_error_to_status(dwLastError);

         return estatus;

      }

      return ::success;

   }*/



   void acme_file::put_block(const char* path, const block& block)
   {

      wstring wstr(path);

      m_pacmedir->create(file_path_folder(path));

      HANDLE h = ::CreateFileW(wstr, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

      if (h == INVALID_HANDLE_VALUE)
      {

         DWORD dwLastError = ::GetLastError();

         auto estatus = last_error_to_status(dwLastError);

         throw_status(::error_io);

      }

      ::e_status estatus = success;

      DWORD dwWritten = 0;

      if (!WriteFile(h, block.get_data(), (DWORD) block.get_size(), &dwWritten,nullptr))
      {

         DWORD dwLastError = ::GetLastError();

         estatus = last_error_to_status(dwLastError);

      }

      if (dwWritten != block.get_size())
      {

         estatus = error_failed;

      }

      ::CloseHandle(h);

      //return estatus;

   }


} // namespace windows_common







//#include <sys/types.h>
//#include <sys/stat.h>
//#include <sys/time.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <utime.h>
//
//#include <iostream>
//#include <string>
//
//#include <cstdlib>

//void touch(const std::string& pathname)
//{
//   int fd = open(pathname.c_str(),
//      O_WRONLY | O_CREAT | O_NOCTTY | O_NONBLOCK,
//      0666);
//   if (fd < 0) // Couldn't open that path.
//   {
//      std::cerr
//         << __PRETTY_FUNCTION__
//         << ": Couldn't open() path \""
//         << pathname
//         << "\"\n";
//      return;
//   }
//   int rc = utimensat(AT_FDCWD,
//      pathname.c_str(),
//      nullptr,
//      0);
//   if (rc)
//   {
//      std::cerr
//         << __PRETTY_FUNCTION__
//         << ": Couldn't utimensat() path \""
//         << pathname
//         << "\"\n";
//      return;
//   }
//   std::clog
//      << __PRETTY_FUNCTION__
//      << ": Completed touch() on path \""
//      << pathname
//      << "\"\n";
//}
