#include "framework.h"
#include "file.h"
#include "acme_directory.h"
#include "acme_file.h"
#include "acme/exception/io.h"
#include "acme/platform/sequencer.h"
#include "acme/filesystem/file/exception.h"
#include "acme/filesystem/file/status.h"
#include "acme/operating_system/time.h"
#include "acme/filesystem/file/exception.h"
#include "acme/platform/system.h"


CLASS_DECL_ACME_WINDOWS_COMMON bool _os_may_have_alias(const char * psz)
{

   string str(psz);

   return str.ends_ci(".lnk") || str.contains_ci(".lnk/") || str.contains_ci(".lnk\\");

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


   file::file()
   {

      //m_iCharacterPutBack = 0x80000000;

      m_handleFile = INVALID_HANDLE_VALUE;
      m_dwAccessMode = 0;

   }


   file::file(HANDLE handleFile)
   {

      //m_iCharacterPutBack = 0x80000000;
      m_handleFile = handleFile;
      m_dwAccessMode = 0;

   }


   file::file(const ::string & pszFileName, const ::file::e_open & eopen)
   {

      //m_iCharacterPutBack = 0x80000000;

      ASSERT(__is_valid_string(pszFileName));


      open(pszFileName, eopen);

      //{

      //   throw ::file::exception(error_file, -1, pszFileName);


      //}

   }

   file::~file()
   {

      if (m_handleFile != INVALID_HANDLE_VALUE)
      {
         close();
      }

   }


   void file::open(const ::file::path & path, const ::file::e_open & eopenParam)
   {

      auto eopen = eopenParam;

      if (path.is_empty())
      {

         m_estatus = error_invalid_empty_argument;

         if (eopenParam & ::file::e_open_no_exception_on_open)
         {

            return;

         }
         else
         {

            error_code errorcode = { e_error_code_type_unknown, -1 };

            throw ::file::exception(m_estatus, errorcode, "", "file with empty name!!", m_eopen);

         }

      }

      if (m_handleFile != INVALID_HANDLE_VALUE)
      {

         close();

      }

      ASSERT_VALID(this);
      ASSERT(__is_valid_string(path));

      eopen -= ::file::e_open_binary;

      if ((eopen & ::file::e_open_defer_create_directory) && (eopen & ::file::e_open_write))
      {

         acmedirectory()->create(path.folder());

      }

      m_handleFile = INVALID_HANDLE_VALUE;

      m_path.Empty();

      m_path = path;

      ASSERT(sizeof(HANDLE) == sizeof(uptr));
      ASSERT(::file::e_open_share_compat == 0);

      // map read/write mode
      ASSERT((::file::e_open_read | ::file::e_open_write | ::file::e_open_read_write) == 3);
      ::u32 dwAccess = 0;
      switch (eopen & 3)
      {
      case ::file::e_open_read:
         dwAccess = GENERIC_READ;
         break;
      case ::file::e_open_write:
         dwAccess = GENERIC_WRITE;
         break;
      case ::file::e_open_read_write:
         dwAccess = GENERIC_READ | GENERIC_WRITE;
         break;
      default:
         dwAccess = GENERIC_READ;
         break;
      }

      auto eopenShare = eopen & ::file::e_open_share_mask;

      // map share mode
      ::u32 dwShareMode = 0;
      switch (eopenShare)    // map compatibility mode to exclusive
      {
      default:
         ASSERT(false);  // invalid share mode?
      case ::file::e_open_share_compat:
      case ::file::e_open_share_exclusive:
         dwShareMode = 0;
         break;
      case ::file::e_open_share_deny_write:
         dwShareMode = FILE_SHARE_READ;
         break;
      case ::file::e_open_share_deny_read:
         dwShareMode = FILE_SHARE_WRITE;
         break;
      case ::file::e_open_share_deny_none:
         dwShareMode = FILE_SHARE_WRITE | FILE_SHARE_READ;
         break;
      }


      LPCREATEFILE2_EXTENDED_PARAMETERS pextendedparameters = nullptr;

      CREATEFILE2_EXTENDED_PARAMETERS extendedparameters;

      // Note: type_text and type_binary are used in derived classes only.

      // map modeNoInherit flag
      SECURITY_ATTRIBUTES securityattributes;

      if (eopen & ::file::e_open_no_inherit)
      {

         if (!pextendedparameters)
         {

            __zero(extendedparameters);

            pextendedparameters = &extendedparameters;

         }

         if (!extendedparameters.lpSecurityAttributes)
         {

            __zero(securityattributes);

            extendedparameters.lpSecurityAttributes = &securityattributes;

         }

         securityattributes.nLength = sizeof(securityattributes);
         
         securityattributes.lpSecurityDescriptor = nullptr;

         securityattributes.bInheritHandle = false;

      }

      // map creation flags
      ::u32 dwCreateFlag;

      if (eopen & ::file::e_open_create)
      {

         if (eopen & ::file::e_open_no_truncate)
         {

            dwCreateFlag = OPEN_ALWAYS;

         }
         else
         {

            dwCreateFlag = CREATE_ALWAYS;

         }

      }
      else
      {

         dwCreateFlag = OPEN_EXISTING;

      }

      HANDLE handleFile = INVALID_HANDLE_VALUE;

      ::u32 dwWaitSharingViolation = 84;

      auto durationStart = ::duration::now();

      //::u32 dwFileSharingViolationRetryTimeout = ::get_task() != nullptr ? ::get_task()->get_file_sharing_violation_timeout_total_milliseconds() : 0;

   retry:

      wstring wstrFileName(m_path);

      // attempt file creation
      //HANDLE handleFile = shell::CreateFile(utf8_to_unicode(m_path), dwAccess, dwShareMode, &sa, dwCreateFlag, FILE_ATTRIBUTE_NORMAL, nullptr);
      //HANDLE h = INVALID_HANDLE_VALUE;
      //handleFile = ::CreateFile2(wstrFileName, dwAccess, dwShareMode, dwCreateFlag, pextendedparameters);
#ifdef _UWP

      handleFile = ::CreateFile2(wstrFileName, dwAccess, dwShareMode, dwCreateFlag, pextendedparameters);

#else

      if (eopen & ::file::e_open_no_inherit)
      {
         
         handleFile = ::CreateFileW(wstrFileName, dwAccess, dwShareMode, &securityattributes, dwCreateFlag, FILE_ATTRIBUTE_NORMAL, nullptr);

      }
      else
      {

         handleFile = ::CreateFileW(wstrFileName, dwAccess, dwShareMode, NULL, dwCreateFlag, FILE_ATTRIBUTE_NORMAL, nullptr);

      }

#endif

      if (handleFile == INVALID_HANDLE_VALUE)
      {

         DWORD dwLastError = ::GetLastError();

         if (!(eopen & ::file::e_open_no_share_violation_wait))
         {

            if (dwLastError == ERROR_SHARING_VIOLATION && ::task_get_run() && durationStart.elapsed() < m_durationErrorBlockTimeout)
            {

               preempt(maximum(m_durationErrorBlockTimeout.integral_millisecond() / 10, 50_ms));

               goto retry;

            }

         }

         if (dwLastError == ERROR_ACCESS_DENIED)
         {

            if (dwAccess == GENERIC_WRITE)
            {

               dwAccess = GENERIC_WRITE | GENERIC_READ;

               goto retry;

            }

         }

         ::e_status estatus = last_error_to_status(dwLastError);

         auto errorcode = __last_error(dwLastError);

         //if (::file::should_ignore_file_exception_callstack(estatus))
         //{

         //   throw ::exception(error_failed);

         //}

         m_estatus = estatus;

         set_nok();

         if (eopenParam & ::file::e_open_no_exception_on_open)
         {

            return;

         }
         else
         {

            if (m_estatus == error_file_access_denied)
            {

               if (dwAccess & GENERIC_WRITE)
               {

                  auto psequencer = acmesystem()->create_message_box_sequencer("Couldn't write to file \"" + m_path + "\".\nAccess Denied!!\n(Is any anti-virus program blocking this program: \"" + acmefile()->module() + "\"?", acmefile()->module().title() + " - Access Denied!", e_message_box_ok);

                  psequencer->do_asynchronously();

               }

            }

            throw ::file::exception(m_estatus, errorcode, m_path, "Create File has failed.", m_eopen);

         }

      }

      m_handleFile = handleFile;

      m_dwAccessMode = dwAccess;

      m_eopen = eopen;

      m_estatus = ::success;

      set_ok();

      //return ::success;

   }


   memsize file::read(void * pdata, memsize nCount)
   {

      ASSERT_VALID(this);

      if (m_handleFile == INVALID_HANDLE_VALUE)
      {

         throw ::exception(error_wrong_state);

      }

      if (nCount == 0)
      {

         return 0;   // avoid Win32 "null-read"

      }

      //if (m_iCharacterPutBack != 0x80000000)
      //{

      //   ((byte *)pdata)[0] = (byte)m_iCharacterPutBack;

      //   m_iCharacterPutBack = 0x80000000;

      //   pdata = ((char *)pdata) + 1;

      //   nCount--;

      //   if (nCount == 0)
      //   {

      //      return 0;   // avoid Win32 "null-read"

      //   }


      //}

      ASSERT(pdata != nullptr);

      ASSERT(__is_valid_address(pdata, nCount));

      DWORD dwRead;

      if (!::ReadFile((HANDLE)m_handleFile, pdata, (::u32)nCount, &dwRead, nullptr))
      {

         auto dwLastError = ::GetLastError();

         auto estatus = last_error_to_status(dwLastError);

         auto errorcode = __last_error(dwLastError);

         throw ::file::exception(estatus, errorcode, m_path, "!ReadFile", m_eopen);

      }

      return (::u32)dwRead;

   }


   void file::write(const void * pdata, memsize nCount)
   {

      ASSERT_VALID(this);
      ASSERT(m_handleFile != INVALID_HANDLE_VALUE);

      if (nCount == 0)
      {

         return;     // avoid Win32 "null-write" option

      }

      ASSERT(pdata != nullptr);

      ASSERT(__is_valid_address(pdata, nCount, false));

      DWORD nWritten;

      if (!::WriteFile((HANDLE)m_handleFile, pdata, (::u32)nCount, &nWritten, nullptr))
      {

         auto dwLastError = ::GetLastError();

         auto estatus = last_error_to_status(dwLastError);

         auto errorcode = __last_error(dwLastError);

         throw ::file::exception(estatus, errorcode, m_path, "!WriteFile", m_eopen);

      }

      // Win32s will not return an error all the time (usually DISK_FULL)
      if (nWritten != nCount)
      {

         if (nWritten < nCount)
         {

            FORMATTED_ERROR("file::status nWritten < nCount is disk full?");

         }

         auto dwLastError = ::GetLastError();

         auto estatus = last_error_to_status(dwLastError);

         auto errorcode = __last_error(dwLastError);

         throw ::file::exception(estatus, errorcode, m_path, "nWritten != nCount", m_eopen);

      }

   }


   filesize file::translate(filesize offset, ::enum_seek nFrom)
   {

      if (m_handleFile == INVALID_HANDLE_VALUE)
      {

         auto dwLastError = ::GetLastError();

         auto estatus = last_error_to_status(dwLastError);

         auto errorcode = __last_error(dwLastError);

         throw ::file::exception(estatus, errorcode, m_path, "m_handleFile == INVALID_HANDLE_VALUE", m_eopen);

      }

      ASSERT_VALID(this);
      ASSERT(m_handleFile != INVALID_HANDLE_VALUE);
      ASSERT(nFrom == ::e_seek_set || nFrom == ::e_seek_from_end || nFrom == ::e_seek_current);
      ASSERT(::e_seek_set == FILE_BEGIN && ::e_seek_from_end == FILE_END && ::e_seek_current == FILE_CURRENT);

      LONG lLoOffset = offset & 0xffffffff;
      LONG lHiOffset = (offset >> 32) & 0xffffffff;

      filesize posNew = ::SetFilePointer((HANDLE)m_handleFile, lLoOffset, &lHiOffset, (::u32)nFrom);
      posNew |= ((filesize)lHiOffset) << 32;
      if (posNew == (filesize)-1)
      {

         auto dwLastError = ::GetLastError();

         auto estatus = last_error_to_status(dwLastError);

         auto errorcode = __last_error(dwLastError);

         throw ::file::exception(estatus, errorcode, m_path, "SetFilePointer == -1", m_eopen);

      }

      return posNew;
   }


   //int file::peek_character()
   //{

   //   if (m_iCharacterPutBack != 0x80000000)
   //   {

   //      return m_iCharacterPutBack;

   //   }

   //   return ::file::file::peek_character();

   //}


   //int file::get_character()
   //{

   //   auto iCharacterPutBack = m_iCharacterPutBack;

   //   if (iCharacterPutBack != 0x80000000)
   //   {

   //      m_iCharacterPutBack = 0x80000000;

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
      ASSERT(m_handleFile != INVALID_HANDLE_VALUE);

      LONG lLoOffset = 0;
      LONG lHiOffset = 0;

      filesize pos = ::SetFilePointer((HANDLE)m_handleFile, lLoOffset, &lHiOffset, FILE_CURRENT);
      pos |= ((filesize)lHiOffset) << 32;
      if (pos == (filesize)-1)
      {

         auto dwLastError = ::GetLastError();

         auto estatus = last_error_to_status(dwLastError);

         auto errorcode = __last_error(dwLastError);

         throw ::file::exception(estatus, errorcode, m_path, "SetFilePointer == -1", m_eopen);

      }

      return pos;
   }


   void file::flush()
   {

      ASSERT_VALID(this);

      if (m_handleFile == INVALID_HANDLE_VALUE || !(m_dwAccessMode & GENERIC_WRITE))
      {

         return;

      }

      if (!::FlushFileBuffers((HANDLE)m_handleFile))
      {

         DWORD dwLastError = ::GetLastError();

         if (dwLastError == ERROR_INVALID_HANDLE
            || dwLastError == ERROR_ACCESS_DENIED)
         {

         }
         else if (dwLastError == ERROR_NO_SYSTEM_RESOURCES)
         {

            output_debug_string("Insufficient system resources exist to complete the requested service.");

         }
         else
         {

            auto dwLastError = ::GetLastError();

            auto estatus = last_error_to_status(dwLastError);

            auto errorcode = __last_error(dwLastError);

            throw ::file::exception(estatus, errorcode, m_path, "!FlushFileBuffers");

         }

      }

   }

   void file::close()
   {

      if (m_eopen & ::file::e_open_write)
      {

         flush();

      }

      ASSERT_VALID(this);
      ASSERT(m_handleFile != INVALID_HANDLE_VALUE);

      bool bError = false;
      ::u32 dwLastError = 0;
      if (m_handleFile != INVALID_HANDLE_VALUE)
      {

         bError = !::CloseHandle((HANDLE)m_handleFile);

         if (bError)
         {

            dwLastError = ::GetLastError();

         }

      }

      m_handleFile = INVALID_HANDLE_VALUE;

      m_dwAccessMode = 0;

      if (bError)
      {

         auto dwLastError = ::GetLastError();

         auto estatus = last_error_to_status(dwLastError);

         auto errorcode = __last_error(dwLastError);

         throw ::file::exception(estatus, errorcode, m_path, "file::close", m_eopen);

      }

   }


   void file::lock(filesize dwPos, filesize dwCount)
   {
      ASSERT_VALID(this);
      ASSERT(m_handleFile != INVALID_HANDLE_VALUE);

      if (!::LockFile((HANDLE)m_handleFile, LODWORD(dwPos), HIDWORD(dwPos), LODWORD(dwCount), HIDWORD(dwCount)))
      {

         auto dwLastError = ::GetLastError();

         auto estatus = last_error_to_status(dwLastError);

         auto errorcode = __last_error(dwLastError);

         throw ::file::exception(estatus, errorcode,m_path, "!LockFile");

      }
   }


   void file::unlock(filesize dwPos, filesize dwCount)
   {
      ASSERT_VALID(this);
      ASSERT(m_handleFile != INVALID_HANDLE_VALUE);

      if (!::UnlockFile((HANDLE)m_handleFile, LODWORD(dwPos), HIDWORD(dwPos), LODWORD(dwCount), HIDWORD(dwCount)))
      {

         auto dwLastError = ::GetLastError();

         auto estatus = last_error_to_status(dwLastError);

         auto errorcode = __last_error(dwLastError);

         throw ::file::exception(estatus, errorcode, m_path, "!UnlockFile");

      }

   }


   void file::set_size(filesize dwNewLen)
   {

      ASSERT_VALID(this);
      ASSERT(m_handleFile != INVALID_HANDLE_VALUE);

      set_position(dwNewLen);

      if (!::SetEndOfFile((HANDLE)m_handleFile))
      {

         auto dwLastError = ::GetLastError();

         auto estatus = last_error_to_status(dwLastError);

         auto errorcode = __last_error(dwLastError);

         throw ::file::exception(estatus, errorcode, m_path, "!SetEndOfFile", m_eopen);

      }

   }


   filesize file::get_size() const
   {

      ASSERT_VALID(this);

      LARGE_INTEGER largeinteger{};

      if (!::GetFileSizeEx(m_handleFile, &largeinteger))
      {

         throw ::io_exception(error_io);

      }

      return largeinteger.QuadPart;

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



   bool file::get_status(::file::file_status & rStatus) const
   {

      ASSERT_VALID(this);

      rStatus.m_strFullName = m_path;

      if (m_handleFile != INVALID_HANDLE_VALUE)
      {

         BY_HANDLE_FILE_INFORMATION information;

         if (!::GetFileInformationByHandle((HANDLE)m_handleFile, &information))
         {

            return false;

         }

         ULARGE_INTEGER integer;

         integer.HighPart = information.nFileSizeHigh;
         integer.LowPart = information.nFileSizeLow;

         rStatus.m_filesize = integer.QuadPart;

         // don't return an error for this because previous versions of acme API didn't
         if (information.dwFileAttributes == 0xFFFFFFFF)
         {

            rStatus.m_attribute = 0;

         }
         else
         {
            rStatus.m_attribute = (byte)information.dwFileAttributes & 0xff;

         }

         // convert times as appropriate
         file_time_to_time(&rStatus.m_ctime.m_i, (file_time_t *)&information.ftCreationTime);
         file_time_to_time(&rStatus.m_atime.m_i, (file_time_t *)&information.ftLastAccessTime);
         file_time_to_time(&rStatus.m_mtime.m_i, (file_time_t *)&information.ftLastWriteTime);

         if (rStatus.m_ctime.get_time() == 0)
         {

            rStatus.m_ctime = rStatus.m_mtime;

         }

         if (rStatus.m_atime.get_time() == 0)
         {

            rStatus.m_atime = rStatus.m_mtime;

         }

      }

      return true;

   }


   bool file::is_opened() const
   {

      return m_handleFile != INVALID_HANDLE_VALUE;

   }


   void file::set_file_path(const ::file::path & path)
   {

      ASSERT_VALID(this);

      m_path = path;

   }


   file::operator HANDLE() const
   {

      return m_handleFile;

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

   strsize dwAllocLen = wstrPath.get_length() + _MAX_PATH;

   auto pwszFullPath = wstrFullPath.get_string_buffer(dwAllocLen);

   // first, fully qualify the path name
   unichar * pszFilePart;

   strsize dwLen = GetFullPathNameW(wstrPath, (::u32)dwAllocLen, pwszFullPath, &pszFilePart);

   wstrFullPath.release_string_buffer();

   if (dwLen == 0)
   {

      wstrFullPath.release_string_buffer();

      wstrFullPath = wstrPath; // take it literally

      return false;

   }

   else if (dwLen > dwAllocLen)
   {

      dwAllocLen = dwLen + _MAX_PATH;

      dwLen = GetFullPathNameW(wstrPath, (::u32)dwAllocLen, pwszFullPath, &pszFilePart);

      wstrFullPath.release_string_buffer();


      if (dwLen == 0 || dwLen > dwAllocLen)
      {

         wstrFullPath = wstrPath; // take it literally

         return false;

      }

   }

   wstring wstrRoot;

   // determine the root name of the volume
   wstrRoot = windows_get_root(wstrFullPath);

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

            strsize iFilePart = pszFilePart - wstrFullPath;

            auto pwsz = wstrFullPath.get_string_buffer(iFilePart + iLenFileName + 32); // arrange more space with more 32 extra wchars

            wcsncpy(pwsz, wstrBackup, (i32)iFilePart);

            wstrFullPath.release_string_buffer();

            pszFilePart = pwsz + iFilePart;

         }

         wcscpy(pszFilePart, data.cFileName);

         wstrFullPath.release_string_buffer();

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



