#include "framework.h"
#include "apex/operating_system.h"
#include "acme/filesystem/filesystem/directory_system.h"
#include "file_context.h"


namespace windows
{


   file_context::file_context()
   {

   }


   file_context::~file_context()
   {

   }


   void file_context::initialize(::particle * pparticle)
   {

      auto estatus = ::object::initialize(pobject);

      if (!estatus)
      {

         return estatus;

      }

      ::pointer<::apex::system>psystem = system();

      __refer(m_pfilesystem, psystem->m_pfilesystem);

      __refer(m_pdirectorysystem, psystem->m_pdirectorysystem);

      return ::success;

   }


   void file_context::init_system()
   {

      auto estatus = m_pfilesystem->update_module_path();

      if (!estatus)
      {

         return estatus;

      }

      return ::success;

   }


   bool file_context::FullPath(string & str, const ::string & pszFileIn)

   {

      //if(::apex::file_context::FullPath(str, pszFileIn))

      // return true;

      if (pszFileIn.case_insensitive_begins("http://"))

      {

         str = pszFileIn;


         return true;

      }
      else if (pszFileIn.case_insensitive_begins("https://"))

      {

         str = pszFileIn;


         return true;

      }
      wstring wstrFileIn;
      wstrFileIn = utf8_to_unicode(pszFileIn);

      wstring wstrFileOut;
      bool b = windows_full_path(wstrFileOut.get_buffer(MAX_PATH * 8), wstrFileIn) != false;
      if (b)
      {
         unicode_to_utf8(str, wstrFileOut);
      }
      return b;
   }

   bool file_context::FullPath(wstring & wstrFullPath, const wstring & wstrPath)
   {

      /*      if(::apex::file_context::FullPath(wstrFullPath, wstrPath))
      return true;*/

      if (wstrPath.case_insensitive_begins(L"http://"))
      {
         wstrFullPath = wstrPath;
         return true;
      }
      else if (wstrPath.case_insensitive_begins(L"https://"))
      {
         wstrFullPath = wstrPath;
         return true;
      }

      return windows_full_path(wstrFullPath, wstrPath) != false;

   }


   unsigned int file_context::GetFileName(const ::string & pszPathName, string & str)

   {
      int nMax = MAX_PATH * 8;
      wstring wstrPathName;
      wstrPathName = utf8_to_unicode(pszPathName);

      wstring wstrTitle;
      unsigned int user = vfxGetFileName(wstrPathName, wstrTitle.get_buffer(nMax), nMax);
      str = unicode_to_utf8(wstrTitle);
      return user;
   }

   void file_context::GetModuleShortFileName(HINSTANCE hInst, string & strShortName)
   {
      vfxGetModuleShortFileName(hInst, strShortName);
   }

   ::payload file_context::length(const ::file::path & path)
   {

      ::payload varRet;

      //varRet = ::file_context::length(path);

      //if (!varRet.is_null())
      //   return varRet;


#ifdef WINDOWS

      WIN32_FILE_ATTRIBUTE_DATA data;

      if (!GetFileAttributesExW(utf8_to_unicode(path), GetFileExInfoStandard, &data))
      {
         varRet.set_type(::e_type_null);
      }
      else
      {
         varRet = (unsigned int)data.nFileSizeLow;
      }

#else

      struct stat stat;

      if (::stat(strPath, &stat) == -1)
      {
         varRet.set_type(::e_type_null);
      }
      else
      {
         varRet = stat.st_size;
      }

#endif

      return varRet;

   }


   ::payload file_context::length(const ::file::path & path, ::payload * pvarQuery)
   {

      // TODO: I don't remember what pvarQuery is used for, in the time of reimplementation
      // of this function. Maybe you should consider it in some case(s).

      return length(path);

   }


   ::extended::status file_context::move(const ::file::path & pszNew, const ::file::path & psz)
   {


#ifdef WINDOWS_DESKTOP

      if (!::MoveFileW(
         utf8_to_unicode(psz),
         utf8_to_unicode(pszNew)))
      {

         DWORD dwError = ::GetLastError();

         if (dwError == ERROR_ALREADY_EXISTS)
         {

            if (::CopyFileW(
               utf8_to_unicode(psz),
               utf8_to_unicode(pszNew),
               false))
            {

               if (!::DeleteFileW(utf8_to_unicode(psz)))
               {

                  dwError = ::GetLastError();

                  string strError;

                  strError.Format("Failed to delete the file to move \"%s\" error=%d", psz, dwError);

                  informationf("%s", strError);

               }

               return ::success;

            }

            dwError = ::GetLastError();

         }

         string strError;

         strError.Format("Failed to move file \"%s\" to \"%s\" error=%d", psz, pszNew, dwError);

         throw ::exception(::error_io, strError);

      }

#elif defined(UNIVERSAL_WINDOWS)

      ::winrt::Windows::Storage::StorageFile ^ file = get_os_file(psz, 0, 0, nullptr, OPEN_EXISTING, 0, nullptr);

      if (file == nullptr)
      {

         //output_debug_string("test");

         throw ::exception(::exception("file::file_context::move Could not move file, could not open source file"));

      }

      string strDirOld = psz.folder();
      string strDirNew = pszNew.folder();
      string strNameOld = psz.name();
      string strNameNew = pszNew.name();

      if (strDirOld == strDirNew)
      {
         if (strNameOld == strNameNew)
         {
            return ::success;
         }
         else
         {
            ::wait(file->RenameAsync(strNameNew));
         }
      }
      else
      {
         ::winrt::Windows::Storage::StorageFolder ^ folder = get_os_folder(strDirNew);
         if (strNameOld == strNameNew)
         {
            ::wait(file->MoveAsync(folder));
         }
         else
         {
            ::wait(file->MoveAsync(folder, strNameNew));
         }
      }


#else
      if (::rename(psz, pszNew) != 0)
      {
         int err = errno;
         string strError;
         strError.Format("Failed to delete file error=%d", err);
         throw ::exception(::exception(strError));
      }
#endif

      return ::success;

   }


   ::extended::status file_context::del(const ::file::path & psz)
   {


#ifdef WINDOWS_DESKTOP

      HANDLE h = ::CreateFileW(utf8_to_unicode(string("\\\\?\\") + psz),
         GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING,
         FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_DELETE_ON_CLOSE, nullptr);

      if (h == INVALID_HANDLE_VALUE)
      {

         DWORD dwError = ::GetLastError();

         if (dwError == 2) // the file does not exist, so delete "failed"
         {

            return ::success;

         }

         string strError;

         strError.Format("Failed to delete file \"%s\" error=%d", psz, dwError);

         return ::error_failed;

      }
      else
      {
         ::FlushFileBuffers(h);
         ::CloseHandle(h);
      }

      /*      if(!::DeleteFileW(utf8_to_unicode(string("\\\\?\\") + psz)))
      {
      unsigned int dwError = ::get_last_error();
      if(dwError == 2) // the file does not exist, so delete "failed"
      return;
      string strError;
      strError.Format("Failed to delete file \"%s\" error=%d", psz, dwError);
      throw ::exception(io_exception(strError));
      }*/


#else

      if (unlink(psz) != 0)
      {
         int err = errno;
         if (err != ENOENT) // already does not exist - consider removal successful - does not issue an exception
         {
            string strError;
            strError.Format("Failed to delete file error=%d", err);
            throw ::exception(::exception(strError));
         }
      }
#endif

      return ::success;

   }


   bool file_context::is_read_only(const ::file::path & psz)
   {

#ifdef WINDOWS_DESKTOP

      unsigned int dwAttrib = windows_get_file_attributes(psz);

      if (dwAttrib & FILE_ATTRIBUTE_READONLY)
      {

         return true;

      }

      return false;

#elif defined(UNIVERSAL_WINDOWS)

      throw ::exception(todo);

#else

      struct stat st;

      if (stat(psz, &st) != 0)
         return true;

      return !(st.st_mode & S_IWUSR);

#endif

   }


   ::extended::transport < ::file::file > file_context::resource_get_file(const ::file::path & path)
   {

#ifdef WINDOWS_DESKTOP

      auto pfile = create_memory_file();

      int iId = atoi(path);

      string strExtension = path.final_extension();

      strExtension.make_upper();
      const char * psz = strExtension;
      if (strExtension == "HTML")
      {

         psz = (const ::string &)RT_HTML;

      }

      ::pointer<::apex::system>psystem = system();

      if (read_resource_as_memory(*pfile->get_primitive_memory(), (HINSTANCE) psystem->m_hinstance, iId, psz))
      {

         return pfile;

      }

#else

      throw_todo();

#endif

      return nullptr;

   }


   bool file_context::get_last_write_time(FILETIME * pfiletime, const ::string & strFilename)
   {

      WIN32_FILE_ATTRIBUTE_DATA data;

      zero(data);

      if (!GetFileAttributesExW(wstring(strFilename), GetFileExInfoStandard, &data))
      {

         return false;

      }

      *pfiletime = data.ftLastWriteTime;

      return true;

   }




   bool file_context::get_status(const ::file::path & path, ::file::file_status & rStatus)
   {

      // attempt to fully qualify path first
      wstring wstrFullName;
      wstring wstrFileName;
      wstrFileName = utf8_to_unicode(path);
      if (!windows_full_path(wstrFullName, wstrFileName))
      {
         rStatus.m_strFullName.empty();
         return false;
      }
      unicode_to_utf8(rStatus.m_strFullName, wstrFullName);

      WIN32_FIND_DATAW findFileData;
      HANDLE hFind = FindFirstFileW((LPWSTR)(const ::wide_character *)wstrFullName, &findFileData);
      if (hFind == INVALID_HANDLE_VALUE)
         return false;
      VERIFY(FindClose(hFind));

      // strip attribute of NORMAL bit, our API doesn't have a "normal" bit.
      rStatus.m_attribute = (unsigned char)(findFileData.dwFileAttributes & ~FILE_ATTRIBUTE_NORMAL);

      // get just the low unsigned int of the file int_size
      ASSERT(findFileData.nFileSizeHigh == 0);
      rStatus.m_size = (int)findFileData.nFileSizeLow;



      //auto pnode = psystem->node();

      // convert times as appropriate
      file_time_to_time(&rStatus.m_ctime.m_time, (file_time_t *)&findFileData.ftCreationTime);
      file_time_to_time(&rStatus.m_atime.m_time, (file_time_t *)&findFileData.ftLastAccessTime);
      file_time_to_time(&rStatus.m_mtime.m_time, (file_time_t *)&findFileData.ftLastWriteTime);

      if (rStatus.m_ctime.get_time() == 0)
         rStatus.m_ctime = rStatus.m_mtime;

      if (rStatus.m_atime.get_time() == 0)
         rStatus.m_atime = rStatus.m_mtime;

      return true;

   }

   //void file_context::set_status(const ::file::path & path,const ::file::file_status& status)
   //{

   //   wstring pszFileName(path);

   //   unsigned int wAttr;
   //   FILETIME creationTime;
   //   FILETIME lastAccessTime;
   //   FILETIME lastWriteTime;
   //   LPFILETIME pCreationTime = nullptr;

   //   LPFILETIME pLastAccessTime = nullptr;

   //   LPFILETIME pLastWriteTime = nullptr;


   //   if((wAttr = windows_get_file_attributes((LPWSTR)(const ::wide_character *)pszFileName)) == (unsigned int)-1L)

   //      file_exception::throw_os_error((int)GetLastError());

   //   if((unsigned int)status.m_attribute != wAttr && (wAttr & readOnly))
   //   {
   //      // set file attribute, only if currently readonly.
   //      // This way we will be able to modify the time assuming the
   //      // caller changed the file from readonly.

   //      if(!SetFileAttributesW((LPWSTR)(const ::wide_character *)pszFileName,(unsigned int)status.m_attribute))

   //         file_exception::throw_os_error((int)GetLastError());
   //   }

   //   // last modification time
   //   if(status.m_mtime.get_time() != 0)
   //   {
   //      ::apex::TimeToFileTime(status.m_mtime,&lastWriteTime);
   //      pLastWriteTime = &lastWriteTime;


   //      // last access time
   //      if(status.m_atime.get_time() != 0)
   //      {
   //         ::apex::TimeToFileTime(status.m_atime,&lastAccessTime);
   //         pLastAccessTime = &lastAccessTime;

   //      }

   //      // create time
   //      if(status.m_ctime.get_time() != 0)
   //      {
   //         ::apex::TimeToFileTime(status.m_ctime,&creationTime);
   //         pCreationTime = &creationTime;

   //      }

   //      HANDLE hFile = ::CreateFileW((LPWSTR)(const ::wide_character *)pszFileName,GENERIC_READ | GENERIC_WRITE,

   //         FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,
   //         nullptr);

   //      if(hFile == INVALID_HANDLE_VALUE)
   //         file_exception::throw_os_error((int)::GetLastError());

   //      if(!SetFileTime((HANDLE)hFile,pCreationTime,lpLastAccessTime,lpLastWriteTime))

   //         file_exception::throw_os_error((int)::GetLastError());

   //      if(!::CloseHandle(hFile))
   //         file_exception::throw_os_error((int)::GetLastError());
   //   }

   //   if((unsigned int)status.m_attribute != wAttr && !(wAttr & readOnly))
   //   {
   //      if(!SetFileAttributes((char *)pszFileName,(unsigned int)status.m_attribute))

   //         file_exception::throw_os_error((int)GetLastError());
   //   }
   //}


   //void file::SetStatus(const ::string & pszFileName,const ::file::file_status& status)

   //{
   //   unsigned int wAttr;
   //   FILETIME creationTime;
   //   FILETIME lastAccessTime;
   //   FILETIME lastWriteTime;
   //   LPFILETIME pCreationTime = nullptr;

   //   LPFILETIME pLastAccessTime = nullptr;

   //   LPFILETIME pLastWriteTime = nullptr;


   //   if((wAttr = GetFileAttributes((char *)pszFileName)) == (unsigned int)-1L)

   //      file_exception::throw_os_error((int)GetLastError());

   //   if((unsigned int)status.m_attribute != wAttr && (wAttr & readOnly))
   //   {
   //      // set file attribute, only if currently readonly.
   //      // This way we will be able to modify the time assuming the
   //      // caller changed the file from readonly.

   //      if(!SetFileAttributes((char *)pszFileName,(unsigned int)status.m_attribute))

   //         file_exception::throw_os_error((int)GetLastError());
   //   }

   //   // last modification time
   //   if(status.m_mtime.get_time() != 0)
   //   {
   //      ::apex::TimeToFileTime(status.m_mtime,&lastWriteTime);
   //      pLastWriteTime = &lastWriteTime;


   //      // last access time
   //      if(status.m_atime.get_time() != 0)
   //      {
   //         ::apex::TimeToFileTime(status.m_atime,&lastAccessTime);
   //         pLastAccessTime = &lastAccessTime;

   //      }

   //      // create time
   //      if(status.m_ctime.get_time() != 0)
   //      {
   //         ::apex::TimeToFileTime(status.m_ctime,&creationTime);
   //         pCreationTime = &creationTime;

   //      }

   //      HANDLE hFile = ::CreateFile(pszFileName,GENERIC_READ | GENERIC_WRITE,

   //         FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,
   //         nullptr);

   //      if(hFile == INVALID_HANDLE_VALUE)
   //         file_exception::throw_os_error((int)::GetLastError());

   //      if(!SetFileTime((HANDLE)hFile,pCreationTime,lpLastAccessTime,lpLastWriteTime))

   //         file_exception::throw_os_error((int)::GetLastError());

   //      if(!::CloseHandle(hFile))
   //         file_exception::throw_os_error((int)::GetLastError());
   //   }

   //   if((unsigned int)status.m_attribute != wAttr && !(wAttr & readOnly))
   //   {
   //      if(!SetFileAttributes((char *)pszFileName,(unsigned int)status.m_attribute))

   //         file_exception::throw_os_error((int)GetLastError());
   //   }
   //}

   ::extended::status file_context::set_status(const ::file::path & path, const ::file::file_status & status)
   {

      wstring pszFileName(path);


      unsigned int wAttr;
      FILETIME creationTime;
      FILETIME lastAccessTime;
      FILETIME lastWriteTime;
      LPFILETIME pCreationTime = nullptr;

      LPFILETIME pLastAccessTime = nullptr;

      LPFILETIME pLastWriteTime = nullptr;


      if ((wAttr = windows_get_file_attributes(path)) == (unsigned int)INVALID_FILE_ATTRIBUTES)
      {

         ::file::throw_os_error(::GetLastError());

      }

      if ((unsigned int)status.m_attribute != wAttr && (wAttr & FILE_ATTRIBUTE_READONLY))
      {
         // set file attribute, only if currently readonly.
         // This way we will be able to modify the time assuming the
         // caller changed the file from readonly.

         if (!SetFileAttributesW((LPWSTR)(const ::wide_character *)pszFileName, (unsigned int)status.m_attribute))
         {

            ::file::throw_os_error(::GetLastError());

         }

      }

      // last modification time
      if (status.m_mtime.get_time() != 0)
      {
         
         lastWriteTime = __FILETIME(status.m_mtime);

         pLastWriteTime = &lastWriteTime;

         // last access time
         if (status.m_atime.get_time() != 0)
         {

            lastAccessTime = __FILETIME(status.m_atime);

            pLastAccessTime = &lastAccessTime;

         }

         // create time
         if (status.m_ctime.get_time() != 0)
         {

            creationTime = __FILETIME(status.m_ctime);

            pCreationTime = &creationTime;

         }

         HANDLE hFile = ::CreateFileW((LPWSTR)(const ::wide_character *)pszFileName, GENERIC_READ | GENERIC_WRITE,

            FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
            nullptr);

         if (hFile == INVALID_HANDLE_VALUE)
         {

            return ::os_error_to_status(::GetLastError());

         }

         if (!SetFileTime((HANDLE)hFile, pCreationTime, pLastAccessTime, pLastWriteTime))
         {

            return ::os_error_to_status(::GetLastError());

         }

         if (!::CloseHandle(hFile))
         {

            return ::os_error_to_status(::GetLastError());

         }

      }

      if ((unsigned int)status.m_attribute != wAttr && !(wAttr & FILE_ATTRIBUTE_READONLY))
      {

         if (!SetFileAttributesW((LPWSTR)(const ::wide_character *)pszFileName, (unsigned int)status.m_attribute))
         {

            return ::os_error_to_status(::GetLastError());

         }

      }

      return ::success;

   }


   void file_context::update_module_path()
   {

      auto estatus = ::file_context::update_module_path();

      if(!estatus)
   {
      
      return estatus;
      
   }

      return estatus;

   }


   file_pointer file_context::get_file(const ::payload & payloadFile, ::file::e_open eopen)
   {

      return ::file_context::get_file(payloadFile, eopenFlags);

   }


   ::file::path file_context::dropbox_info_network_payload()
   {

      ::file::path pathJson;

      pathJson = directory_system()->user_appdata_local() / "Dropbox/info.network_payload";

      return pathJson;

   }


} // namespace windows


