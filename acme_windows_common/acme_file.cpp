// From acme/filesystem/file/_.cpp by camilo on 2021-08-09 
// 14:17 BRT <3ThomasBorregaardS�rensen
#include "framework.h"
#include "acme/operating_system.h"
#include <stdio.h>


CLASS_DECL_ACME void TRACELASTERROR();


namespace windows_common
{


   //::file::path acme_file::_final(const char * path)
   //{

   //   WCHAR wszPath[8192];

   //   ::u32 dw = GetFullPathNameW(L"\\\\?\\" + wstring(path), sizeof(wszPath) / sizeof(WCHAR), wszPath, nullptr);

   //   return wszPath;

   //}


   //extern "C"
   bool acme_file::copy(const char * pszNew, const char * pszSrc, bool bOverwrite)
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

      //return ::CopyFileExW(wstrSrc,wstrNew, nullptr, nullptr, nullptr, COPY_FILE_NO_BUFFERING | (bOverwrite ? 0 : COPY_FILE_FAIL_IF_EXISTS)) ? true : false;
      auto bCopy = ::CopyFileExW(wstrSrc, wstrNew, nullptr, nullptr, nullptr, (bOverwrite ? 0 : COPY_FILE_FAIL_IF_EXISTS)) ? true : false;

      if (!bCopy)
      {

         TRACELASTERROR();

         return false;

      }

      return true;

   }

   bool acme_file::exists(const char * path)
   {

      //wstring wstr(path);

      auto attributes = windows_get_file_attributes(path);

      if (attributes == INVALID_FILE_ATTRIBUTES)
      {

         return false;

      }

      if (attributes & FILE_ATTRIBUTE_DIRECTORY)
      {

         return false;

      }

      return true;

   }

   bool acme_file::put_contents(const char * path, const char * contents, memsize len)
   {

      m_pacmedir->create(file_path_folder(path));

      wstring wstrPath(path);

      FILE * file = _wfsopen(wstrPath, L"w", _SH_DENYWR);

      if (file == nullptr)
      {

         return false;

      }

      count dwWrite;

      if (len < 0)
         dwWrite = ansi_length(contents);
      else
         dwWrite = len;

      auto dwWritten = fwrite(contents, 1, (u32)dwWrite, file);

      int_bool bOk = dwWritten == dwWrite;

      fclose(file);

      return dwWrite == dwWritten && bOk != false;

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

         return 0;

      }

      DWORD dwHi = 0;

      LARGE_INTEGER largeintegerFileSize;

      if (!GetFileSizeEx(hfile, &largeintegerFileSize))
      {

         return 0;

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


   //memsize acme_file::as_memory(const char * path, void * p, memsize s)
   //{

   //   FILE * file = FILE_open(path, "r", _SH_DENYNO);

   //   if (file == nullptr)
   //   {

   //      return false;

   //   }

   //   memsize sRead = 0;

   //   try
   //   {

   //      sRead = ::fread(p, 1, (size_t)s, file);


   //   }
   //   catch (...)
   //   {

   //   }

   //   fclose(file);

   //   return sRead;

   //}




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


   bool acme_file::delete_file(const char * pszFileName)
   {



      if (!::DeleteFileW(strFilePath))
      {


      }

   }





} // namespace windows_common



