#include "framework.h"
#include "file.h"
#include "directory_system.h"
#include "file_system.h"
#include "acme/exception/interface_only.h"
#include "acme/exception/io.h"
//#include "acme/handler/sequence.h"
#include "acme/filesystem/file/exception.h"
#include "acme/filesystem/file/status.h"
#include "acme/filesystem/file/exception.h"
#include "acme/nano/nano.h"
#include "acme/user/micro/user.h"
#include "acme/operating_system/windows_common/_string.h"
#include "acme/platform/system.h"
#include "acme/prototype/datetime/system_time.h"
#include "acme/windowing/windowing.h"


#include <intsafe.h>


CLASS_DECL_ACME_WINDOWS_COMMON bool _os_may_have_alias(const char * psz)
{

   string str(psz);

   return str.case_insensitive_ends(".lnk") || str.case_insensitive_contains(".lnk/") || str.case_insensitive_contains(".lnk\\");

}


CLASS_DECL_ACME_WINDOWS_COMMON memsize hfile_read(HANDLE hfile, void* p, memsize s)
{

   DWORD dwRead = 0;

   if (!::ReadFile(hfile, p, (DWORD) minimum(INT_MAX, s), &dwRead, nullptr))
   {

      return 0;

   }

   return dwRead;

}


namespace acme_windows_common
{


   file::file() :
      m_file(m_path, m_windowspath, m_eopen)
   {

      //m_dwAccessMode = 0;

   }


   file::file(HANDLE handleFile) :
      m_file(m_path, m_windowspath, m_eopen, handleFile)
   {

      //m_dwAccessMode = 0;

   }


   file::file(const ::scoped_string & scopedstrFileName, ::file::e_open eopen) :
      m_file(m_path, m_windowspath, m_eopen)
   {

      //m_iCharacterPutBack = I32_MINIMUM;

      ASSERT(is_string_ok(scopedstrFileName));


      open(scopedstrFileName, eopen);

      //{

      //   throw ::file::exception(error_file, -1, pszFileName);


      //}

   }


   file::~file()
   {

      if (m_file.is_ok())
      {

         close();

      }

   }


   void file::open(const ::file::path & path, ::file::e_open eopen, ::pointer < ::file::exception > * ppfileexception)
   {

      if (path.is_empty())
      {

         m_estatus = error_invalid_empty_argument;

         RETURN_OR_THROW(
            eopen & ::file::e_open_no_exception_on_open,
            ppfileexception,
            ::file::exception,
            m_estatus, ::error_code(e_error_code_type_unknown, -1), "", eopen, "file with empty name!!");

      }

      if (m_file.is_ok())
      {

         close();

      }

      ASSERT_VALID(this);
      ASSERT(is_string_ok(path));

      eopen -= ::file::e_open_binary;

      if ((eopen & ::file::e_open_defer_create_directory) && (eopen & ::file::e_open_write))
      {

         directory_system()->create(path.folder());

      }

      m_path.empty();

      m_path = path;

      HANDLE handleFile = INVALID_HANDLE_VALUE;

      auto timeStart = ::time::now();

      //unsigned int dwFileSharingViolationRetryTimeout = ::get_task() != nullptr ? ::get_task()->get_file_sharing_violation_timeout_total_milliseconds() : 0;

   retry:

      //wstring wstrFileName(m_path.get_os_path());

      // attempt file creation
      //HANDLE handleFile = shell::CreateFile(utf8_to_unicode(m_path), dwAccess, dwShareMode, &sa, dwCreateFlag, FILE_ATTRIBUTE_NORMAL, nullptr);
      //HANDLE h = INVALID_HANDLE_VALUE;
      //handleFile = ::CreateFile2(wstrFileName, dwAccess, dwShareMode, dwCreateFlag, pextendedparameters);
      m_file._create_file(path, eopen);

      if (m_file.nok())
      {

         DWORD lasterror = ::GetLastError();

         if (!(eopen & ::file::e_open_no_share_violation_wait))
         {

            if (lasterror == ERROR_SHARING_VIOLATION && ::task_get_run() && timeStart.elapsed() < m_timeErrorBlockTimeout)
            {

               preempt(maximum(m_timeErrorBlockTimeout / 10, 50_ms));

               goto retry;

            }

         }

         if (lasterror == ERROR_ACCESS_DENIED)
         {

            if (eopen & ::file::e_open_write && !(eopen & ::file::e_open_read))
            {

               eopen += ::file::e_open_read;

               goto retry;

            }

         }

         ::e_status estatus = ::windows::last_error_status(lasterror);

         auto errorcode = ::windows::last_error_error_code(lasterror);

         m_estatus = estatus;

         set_nok();

         if (eopen & ::file::e_open_no_exception_on_open && m_estatus == error_file_access_denied)
         {

            if (eopen & ::file::e_open_write)
            {

               auto pmessagebox = __initialize_new ::message_box("Couldn't write to file \"" + m_path + "\".\nAccess Denied!!\n(Is any anti-virus program blocking this program: \"" + file_system()->module() + "\"?", file_system()->module().title() + " - Access Denied!", e_message_box_ok);

               pmessagebox->async();

            }

         }

         RETURN_OR_THROW(
            eopen & ::file::e_open_no_exception_on_open,
            ppfileexception, 
            ::file::exception,
            m_estatus, errorcode, m_path, m_eopen, "Create File has failed.");

      }

      m_eopen = eopen;

      m_estatus = ::success;

      set_ok_flag();

   }


   memsize file::read(void * p, ::memsize s)
   {

      ASSERT_VALID(this);
      ASSERT(m_file.is_ok());

      if (s <= 0)
      {

         return 0;   // avoid Win32 "null-read"

      }

      //if (m_iCharacterPutBack != I32_MINIMUM)
      //{

      //   ((unsigned char *)pdata)[0] = (unsigned char)m_iCharacterPutBack;

      //   m_iCharacterPutBack = I32_MINIMUM;

      //   pdata = ((char *)pdata) + 1;

      //   nCount--;

      //   if (nCount == 0)
      //   {

      //      return 0;   // avoid Win32 "null-read"

      //   }


      //}

      auto data = (unsigned char *) p;

      ASSERT(::is_set(data));

      ASSERT(is_memory_segment_ok(data, s));

      auto amountRead = m_file.read(data, s);

      return amountRead;

   }


   void file::write(const void * p, ::memsize s)
   {

      ASSERT_VALID(this);
      ASSERT(m_file.is_ok());

      if (::is_null(p) || s <= 0)
      {

         return;     // avoid Win32 "null-write" option

      }

      ASSERT(::is_set(p));

      ASSERT(is_memory_segment_ok(p, s));

      //DWORD nWritten;

      m_file.write(p, s);
      //{

      //   auto dwLastError = ::GetLastError();

      //   auto estatus = ::windows::last_error_status(dwLastError);

      //   auto errorcode = ::windows::last_error_error_code(dwLastError);

      //   throw ::file::exception(estatus, errorcode, m_path, "!WriteFile", m_eopen);

      //}

      //// Win32s will not return an error all the time (usually DISK_FULL)
      //if (nWritten != block.size())
      //{


      //}

   }


   void file::translate(filesize offset, ::enum_seek nFrom)
   {

      if (m_file.nok())
      {

         auto dwLastError = ::GetLastError();

         auto estatus = ::windows::last_error_status(dwLastError);

         auto errorcode = ::windows::last_error_error_code(dwLastError);

         throw ::file::exception(estatus, errorcode, m_path, m_eopen, "m_file.nok()");

      }

      ASSERT_VALID(this);
      ASSERT(m_file.is_ok());
      ASSERT(nFrom == ::e_seek_set || nFrom == ::e_seek_from_end || nFrom == ::e_seek_current);
      ASSERT(::e_seek_set == FILE_BEGIN && ::e_seek_from_end == FILE_END && ::e_seek_current == FILE_CURRENT);

      m_file.set_file_pointer(offset, nFrom);

      //if (posNew == (filesize)-1)
      //{

      //   auto dwLastError = ::GetLastError();

      //   auto estatus = ::windows::last_error_status(dwLastError);

      //   auto errorcode = ::windows::last_error_error_code(dwLastError);

      //   throw ::file::exception(estatus, errorcode, m_path, "SetFilePointer == -1", m_eopen);

      //}

      //return posNew;
   }


   //int file::peek_character()
   //{

   //   if (m_iCharacterPutBack != I32_MINIMUM)
   //   {

   //      return m_iCharacterPutBack;

   //   }

   //   return ::file::file::peek_character();

   //}


   //int file::get_character()
   //{

   //   auto iCharacterPutBack = m_iCharacterPutBack;

   //   if (iCharacterPutBack != I32_MINIMUM)
   //   {

   //      m_iCharacterPutBack = I32_MINIMUM;

   //      return iCharacterPutBack;

   //   }

   //   return ::file::file::get_character();

   //}


   //int file::put_character_back(int iCharacter)
   //{

   //   m_iCharacterPutBack = iCharacter;

   //   //seek(-1, ::enum_seek::seek_current);

   //   return iCharacter;

   //}


   filesize file::get_position() const
   {

      ASSERT_VALID(this);
      ASSERT(m_file.is_ok());

      auto position = m_file.get_file_pointer();

      return position;

   }


   void file::flush()
   {

      ASSERT_VALID(this);

      if (m_file.nok() || !(m_eopen & ::file::e_open_write))
      {

         return;

      }

      m_file.flush_file_buffers();

   }


   void file::close()
   {

      if (m_eopen & ::file::e_open_write)
      {

         flush();

      }

      ASSERT_VALID(this);
      ASSERT(m_file.is_ok());

      //bool bError = false;
      //unsigned int dwLastError = 0;
      //m_dwAccessMode = 0;

      m_eopen = ::file::e_open_none;

      m_windowspath.clear();

      m_path.clear();

      if (m_file.is_ok())
      {

         m_file.close_handle();

         //bError = !::CloseHandle((HANDLE)m_handleFile);

         /*f (bError)
         {

            dwLastError = ::GetLastError();

         }*/

      }

      //m_handleFile = INVALID_HANDLE_VALUE;

      //if (bError)
      //{

      //   auto dwLastError = ::GetLastError();

      //   auto estatus = ::windows::last_error_status(dwLastError);

      //   auto errorcode = ::windows::last_error_error_code(dwLastError);

      //   throw ::file::exception(estatus, errorcode, m_path, "file::close", m_eopen);

      //}

   }


   void file::lock(filesize dwPos, filesize dwCount)
   {
      
      ASSERT_VALID(this);
      ASSERT(m_file.is_ok());

      m_file.lock_file(dwPos, dwCount);

   }


   void file::unlock(filesize dwPos, filesize dwCount)
   {

      ASSERT_VALID(this);
      ASSERT(m_file.is_ok());

      m_file.unlock_file(dwPos, dwCount);

   }



   void file::set_size(filesize dwNewLen)
   {

      ASSERT_VALID(this);
      ASSERT(m_file.is_ok());

      m_file.set_file_size(dwNewLen);


   }


   filesize file::size() const
   {

      ASSERT_VALID(this);
      ASSERT(m_file.is_ok());

      return m_file.get_file_size();

   }


   //void file::assert_ok() const
   //{

   //   ::matter::assert_ok();

   //}


   //void file::dump(dump_context & dumpcontext) const
   //{

   //   ::matter::dump(dumpcontext);

   //   //dumpcontext << "with handle " << m_handleFile;
   //   //dumpcontext << " and name \"" << m_path << "\"";
   //   //dumpcontext << "\n";

   //}


#define EPERM           1
#define ENOENT          2
#define ESRCH           3
#define EINTR           4
#define EIO             5
#define ENXIO           6
#define E2BIG           7
#define ENOEXEC         8
#define EBADF           9
#define ECHILD          10
#define EAGAIN          11
#define ENOMEM          12
#define EACCES          13
#define EFAULT          14
#define EBUSY           16
#define EEXIST          17
#define EXDEV           18
#define ENODEV          19
#define ENOTDIR         20
#define EISDIR          21
#define EINVAL          22
#define ENFILE          23
#define EMFILE          24
#define ENOTTY          25
#define EFBIG           27
#define ENOSPC          28
#define ESPIPE          29
#define EROFS           30
#define EMLINK          31
#define EPIPE           32
#define EDOM            33
#define ERANGE          34
#define EDEADLK         36
#define ENAMETOOLONG    38
#define ENOLCK          39
#define ENOSYS          40
#define ENOTEMPTY       41
#define EILSEQ          42

   /*
   * Support EDEADLOCK for compatibiity with older MS-C versions.
   */
#define EDEADLOCK       EDEADLK


   /////////////////////////////////////////////////////////////////////////////
   // file name handlers

   //string file::GetFileName() const
   //{
   //   ASSERT_VALID(this);

   //   ::file::file_status status;
   //   GetStatus(status);
   //   return file_path_name(status.m_strFullName);
   //}

   //string file::GetFileTitle() const
   //{
   //   ASSERT_VALID(this);

   //   ::file::file_status status;
   //   GetStatus(status);
   //   return file_path_title(status.m_strFullName);
   //}

   ::file::path file::get_file_path() const
   {

      return m_path;

   }



   ::file::file_status file::get_status() const
   {

      ASSERT_VALID(this);
      ASSERT(m_file.is_ok());

      ::file::file_status filestatus;

      filestatus.m_pathFullName = m_path;

      BY_HANDLE_FILE_INFORMATION information;

      m_file.get_file_information(information);

      ULARGE_INTEGER integer;

      integer.HighPart = information.nFileSizeHigh;

      integer.LowPart = information.nFileSizeLow;

      filestatus.m_filesize = integer.QuadPart;

      // don't return an error for this because previous versions of acme API didn't
      if (information.dwFileAttributes == 0xFFFFFFFF)
      {

         filestatus.m_attribute = 0;

      }
      else
      {

         filestatus.m_attribute = (unsigned char)information.dwFileAttributes & 0xff;

      }

      filestatus.m_timeCreation = class ::time(as_file_time(information.ftCreationTime));

      filestatus.m_timeAccess = class ::time(as_file_time(information.ftLastAccessTime));

      filestatus.m_timeModification = class ::time(as_file_time(information.ftLastWriteTime));

      if (filestatus.m_timeCreation <= 0_s)
      {

         filestatus.m_timeCreation = filestatus.m_timeModification;

      }

      if (filestatus.m_timeAccess <= 0_s)
      {

         filestatus.m_timeAccess = filestatus.m_timeModification;

      }

      return filestatus;

   }


   bool file::is_opened() const
   {

      return m_file.is_ok();

   }


   void file::set_file_path(const ::file::path & path)
   {

      ASSERT_VALID(this);

      m_path = path;

   }


   file::operator HANDLE() const
   {

      return (HANDLE) m_file.m_u;

   }


   class ::time file::modification_time()
   {

      ASSERT_VALID(this);
      ASSERT(m_file.is_ok());

      FILETIME filetimeLastWrite;

      m_file.get_file_time(nullptr, nullptr, &filetimeLastWrite);

      return class ::time(as_file_time(filetimeLastWrite));

   }


   void file::set_modification_time(const class ::time& time)
   {

      ASSERT_VALID(this);
      ASSERT(m_file.is_ok());

      auto FILETIMELastWrite = as_FILETIME(file_time(time));

      m_file.set_file_time(nullptr, nullptr, & FILETIMELastWrite);

   }


} // namespace acme_windows_common


#define _wcsdec(_cpc1, _cpc2) ((_cpc1)>=(_cpc2) ? nullptr : (_cpc2)-1)

#define _wcsinc(_pc)    ((_pc)+1)




// turn a file, relative path or other into an absolute path
bool CLASS_DECL_ACME_WINDOWS_COMMON window_get_full_path(wstring & wstrFullPath, const wstring & wstrPath)
// pszPathOut = buffer of _MAX_PATH

// pszFileIn = file, relative path or absolute path

// (both in ANSI character set)
{

   character_count dwAllocLen = wstrPath.length() + _MAX_PATH;

   auto pwszFullPath = wstrFullPath.get_buffer(dwAllocLen);

   // first, fully qualify the path name
   unichar * pszFilePart;

   character_count dwLen = GetFullPathNameW(wstrPath, (unsigned int)dwAllocLen, pwszFullPath, &pszFilePart);

   wstrFullPath.release_buffer();

   if (dwLen == 0)
   {

      wstrFullPath.release_buffer();

      wstrFullPath = wstrPath; // take it literally

      return false;

   }

   else if (dwLen > dwAllocLen)
   {

      dwAllocLen = dwLen + _MAX_PATH;

      dwLen = GetFullPathNameW(wstrPath, (unsigned int)dwAllocLen, pwszFullPath, &pszFilePart);

      wstrFullPath.release_buffer();


      if (dwLen == 0 || dwLen > dwAllocLen)
      {

         wstrFullPath = wstrPath; // take it literally

         return false;

      }

   }

   wstring wstrRoot;

   // determine the root name of the volume
   wstrRoot = ::windows::get_file_path_root(wstrFullPath);

   // get file system information for the volume
   DWORD dwFlags, dwDummy;

   if (!GetVolumeInformationW(wstrRoot, nullptr, 0, nullptr, &dwDummy, &dwFlags, nullptr, 0))
   {
      
      //      TRACE1("Warning: could not get volume information '%s'.\n", strRoot);

      return false;   // preserving case may not be correct

   }

   // not all characters have complete uppercase/lowercase
   if (!(dwFlags & FS_CASE_IS_PRESERVED))
   {

      wstrFullPath.make_upper();

   }

   // assume non-UNICODE file systems, use OEM character set
   if (!(dwFlags & FS_UNICODE_STORED_ON_DISK))
   {
      
      WIN32_FIND_DATAW data;
      
      HANDLE h = FindFirstFileW(wstrPath, &data);

      if (h != INVALID_HANDLE_VALUE)
      {
         
         FindClose(h);
         
         auto iLenFileName = wcslen(data.cFileName);

         if (iLenFileName >= MAX_PATH)
         {

            wstring wstrBackup = wstrFullPath;

            character_count iFilePart = pszFilePart - wstrFullPath;

            auto pwsz = wstrFullPath.get_buffer(iFilePart + iLenFileName + 32); // arrange more space with more 32 extra wchars

            wcsncpy(pwsz, wstrBackup, (int)iFilePart);

            wstrFullPath.release_buffer();

            pszFilePart = pwsz + iFilePart;

         }

         wcscpy(pszFilePart, data.cFileName);

         wstrFullPath.release_buffer();

      }

   }

   return true;

}


string CLASS_DECL_ACME_WINDOWS_COMMON windows_string_from_clsid(REFCLSID rclsid)
{

   WCHAR szCLSID[256];

   wprintf(szCLSID, L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
      rclsid.Data1, rclsid.Data2, rclsid.Data3,
      rclsid.Data4[0], rclsid.Data4[1], rclsid.Data4[2], rclsid.Data4[3],
      rclsid.Data4[4], rclsid.Data4[5], rclsid.Data4[6], rclsid.Data4[7]);

   return szCLSID;

}



