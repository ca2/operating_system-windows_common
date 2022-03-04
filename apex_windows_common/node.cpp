#include "framework.h"
#include "acme/filesystem/filesystem/acme_dir.h"
#include "acme/filesystem/filesystem/acme_path.h"


CLASS_DECL_ACME_WINDOWS_COMMON bool defer_initialize_winsock();


namespace apex
{


   namespace windows_common
   {


      node::node()
      {

         m_papexnode = this;

         defer_initialize_winsock();

      }


      node::~node()
      {


      }


      void node::initialize(::object* pobject)
      {

         //auto estatus = ::acme::windows_common::node::initialize(pobject);

         ::acme::windows_common::node::initialize(pobject);

         //if (!estatus)
         //{

         //   return estatus;

         //}

         //if (!__node_apex_pre_init())
         //{

         //   return error_failed;

         //}

         //if (!__node_apex_pos_init())
         //{

         //   return error_failed;

         //}

         //return estatus;

      }


      bool node::_os_calc_app_dark_mode()
      {

         //try
         //{

         //   ::windows::registry::key key;

         //   key.open(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize");

         //   ::u32 dw;

         //   auto estatus = key._get("AppsUseLightTheme", dw);

         //   if (::failed(estatus))
         //   {

         //      estatus = key._get("SystemUseLightTheme", dw);

         //      if (::failed(estatus))
         //      {

         //         return false;

         //      }

         //   }

         //   return dw == 0;

         //}
         //catch (...)
         //{

         //   return false;

         //}

         return false;

      }


      bool node::_os_calc_system_dark_mode()
      {

         //try
         //{

         //   ::windows::registry::key key;

         //   key.open(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize");

         //   ::u32 dw;

         //   auto estatus = key._get("SystemUseLightTheme", dw);

         //   if (::failed(estatus))
         //   {

         //      estatus = key._get("AppsUseLightTheme", dw);

         //      if (::failed(estatus))
         //      {

         //         return false;

         //      }

         //   }

         //   return dw == 0;

         //}
         //catch (...)
         //{

         //   return false;

         //}

         return false;

      }


      ::color::color node::get_default_color(::u64 u)
      {

         switch (u)
         {
         case COLOR_3DFACE:
            return argb(127, 192, 192, 200);
         case COLOR_WINDOW:
            return argb(127, 255, 255, 255);
         case COLOR_3DLIGHT:
            return argb(127, 218, 218, 210);
         case COLOR_3DHIGHLIGHT:
            return argb(127, 238, 238, 230);
         case COLOR_3DSHADOW:
            return argb(127, 138, 138, 130);
         case COLOR_3DDKSHADOW:
            return argb(127, 90, 90, 80);
         default:
            break;
         }

         return argb(127, 0, 0, 0);

      }


      void node::set_console_colors(::u32 dwScreenColors, ::u32 dwPopupColors, ::u32 dwWindowAlpha)
      {

         //::windows::registry::key key(HKEY_CURRENT_USER, "Console", true);

         //key._set("ScreenColors", dwScreenColors);
         //key._set("PopupColors", dwPopupColors);
         //key._set("WindowAlpha", dwWindowAlpha);

      }



      void node::set_system_dark_mode1(bool bSet)
      {

         //::windows::registry::key key(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", true);

         //::u32 dwSystemUseLightTheme;
         //if (bSet)
         //{
         //   dwSystemUseLightTheme = 0;
         //}
         //else
         //{
         //   dwSystemUseLightTheme = 1;
         //}

         //key._set("SystemUsesLightTheme", dwSystemUseLightTheme);
         //return ::success;

      }


      void node::set_app_dark_mode1(bool bSet)
      {

         //::windows::registry::key key(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", true);

         //::u32 dwAppsUseLightTheme;
         //if (bSet)
         //{
         //   dwAppsUseLightTheme = 0;
         //}
         //else
         //{
         //   dwAppsUseLightTheme = 1;
         //}

         //key._set("AppsUseLightTheme", dwAppsUseLightTheme);

         //return ::success;

      }


      double node::get_time_zone()
      {

         double dTimeZone = 0.;

#ifdef WINDOWS
         {
            //time_t t = time(nullptr);

            //struct tm *p = localtime(&t);

            DYNAMIC_TIME_ZONE_INFORMATION i = {};

            ::u32 dw = GetDynamicTimeZoneInformation(&i);

            if (dw == TIME_ZONE_ID_STANDARD)
            {

               dTimeZone = -((double)(i.Bias + i.StandardBias) / 60.0);

            }
            else if (dw == TIME_ZONE_ID_DAYLIGHT)
            {

               dTimeZone = -((double)(i.Bias + i.DaylightBias) / 60.0);

            }
            else
            {

               dTimeZone = -((double)i.Bias / 60.0);

            }

         }
#else
         {

            time_t t = time(nullptr);

            struct tm lt = { 0 };

            localtime_r(&t, &lt);

            //printf("Offset to GMT is %lds.\n", lt.tm_gmtoff);

            //printf("The time zone is '%s'.\n", lt.tm_zone);

            dTimeZone = +((double)lt.tm_gmtoff / (60.0 * 60.0));

         }
#endif

         return dTimeZone;

      }


      void node::open_folder(::file::path& pathFolder)
      {

        
         //return ::error_failed;

      }


      void node::register_dll(const ::file::path& pathDll)
      {


         string strPathDll;

         //#ifdef _DEBUG

         strPathDll = pathDll;

         //#else
         //
         //   strPathDll = m_psystem->m_pacmedir->matter() / "time" / process_platform_dir_name() /"stage/_desk_tb.dll";
         //
         //#endif

         string strParam;

         strParam = "/s \"" + strPathDll + "\"";

         //wstring wstrParam(strParam);

         //STARTUPINFOW si = {};

         //si.cb = sizeof(si);

         //si.wShowWindow = SW_HIDE;

         //PROCESS_INFORMATION pi = {};

         WCHAR wszSystem[2048];

         GetSystemDirectoryW(wszSystem, sizeof(wszSystem) / sizeof(WCHAR));

         wstring wstrSystem(wszSystem);

         ::file::path path(wstrSystem);

         path /= "regsvr32.exe";

         property_set set;

         set["privileged"] = true;

         //if (!call_sync(path, strParam, path.folder(), ::e_display_none, 3_minute, set))
         call_sync(path, strParam, path.folder(), ::e_display_none, 3_minute, set);
         //{

         //   return false;

         //}

         //if (CreateProcessW(wstrPath, wstrParam, nullptr, nullptr, false, 0, nullptr, wstrSystem, &si, &pi))
         //{

         //   output_debug_string("created");

         //}
         //else
         //{

         //   output_debug_string("not created");

         //}

         //CloseHandle(pi.hProcess);

         //CloseHandle(pi.hthread);

         //return true;

      }


      void node::system_main()
      {

         //auto estatus = 
         m_psystem->m_papexsystem->m_papexnode->thread_initialize(m_psystem->m_papexsystem);

         //if (!estatus)
         //{

         //   return estatus;

         //}

         //estatus = 
         m_psystem->on_start_system();

         //if (!estatus)
         //{

         //   return estatus;

         //}

         //estatus = 
         m_psystem->main();

         //if (!estatus)
         //{

         //   return estatus;

         //}

         //estatus = 
         
         m_psystem->inline_term();

         //if (!estatus)
         //{

         //   return estatus;

         //}

         //return estatus;


      }


      void node::get_firefox_installation_info(string& strPathToExe, string& strInstallDirectory)
      {

#ifdef WINDOWS_DESKTOP

         //try
         //{

         //   ::windows::registry::key key(HKEY_LOCAL_MACHINE, "SOFTWARE\\Mozilla\\Mozilla Firefox");

         //   string strCurrentVersion;

         //   key.get("CurrentVersion", strCurrentVersion);

         //   key.open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Mozilla\\Mozilla Firefox\\" + strCurrentVersion + "\\Main");

         //   key.get("PathToExe", strPathToExe);

         //   key.get("Install Directory", strInstallDirectory);

         //}
         //catch (const ::e_status & estatus)
         //{

         //   return estatus;

         //}

         //return ::success;

#else

         return ::error_failed;

#endif

      }


      void node::_001InitializeShellOpen()
      {

         //ASSERT(m_atomApp == nullptr && m_atomSystemTopic == nullptr); // do once

         //m_atomApp            = ::GlobalAddAtomW(::str::international::utf8_to_unicode(m_strAppName));

         //m_atomSystemTopic    = ::GlobalAddAtomW(L"system");

         //return ::success;

      }


      void node::process_init()
      {



         defer_initialize_winsock();
         //return success;

      }


      //string node::veriwell_multimedia_music_midi_get_default_library_name()
      //{

      //   return "music_midi_mmsystem";

      //}


      //string node::multimedia_audio_mixer_get_default_library_name()
      //{

      //   return "audio_mixer_mmsystem";

      //}


      //string node::multimedia_audio_get_default_library_name()
      //{

      //   string str;

      //   if (m_psystem->m_pacmefile->exists(m_psystem->m_pacmedir->system() / "config\\system\\audio.txt"))
      //   {

      //      str = m_psystem->m_pacmefile->as_string(m_psystem->m_pacmedir->system() / "config\\system\\audio.txt");

      //   }
      //   else
      //   {

      //      ::file::path strPath;

      //      strPath = m_psystem->m_pacmedir->appdata() / "audio.txt";

      //      str = m_psystem->m_pacmefile->as_string(strPath);

      //   }

      //   if (str.has_char())
      //      return "audio_" + str;
      //   else
      //      return "audio_mmsystem";

      //}









      //HICON node::extract_icon(HINSTANCE hInst, const ::string & pszExeFileName, ::u32 nIconIndex)

      //{

      //   return ::ExtractIconW(hInst, ::str::international::utf8_to_unicode(pszExeFileName), nIconIndex);


      //}


      //CLASS_DECL_ACME::file::path user_appdata_local();

      //void node::time_to_filetime(::matter* pobject, const ::earth::time& time, LPFILETIME pFileTime)
      //{

      //   SYSTEMTIME sysTime;

      //   sysTime.wYear = (::u16)time.GetYear();
      //   sysTime.wMonth = (::u16)time.GetMonth();
      //   sysTime.wDay = (::u16)time.GetDay();
      //   sysTime.wHour = (::u16)time.GetHour();
      //   sysTime.wMinute = (::u16)time.GetMinute();
      //   sysTime.wSecond = (::u16)time.GetSecond();
      //   sysTime.wMilliseconds = 0;

      //   // convert system time to local file time
      //   FILETIME localTime;

      //   DWORD dwLastError = ::GetLastError();

      //   if (!SystemTimeToFileTime((LPSYSTEMTIME)&sysTime, &localTime))
      //      ::file::throw_os_error(dwLastError);

      //   // convert local file time to UTC file time
      //   if (!LocalFileTimeToFileTime(&localTime, pFileTime))
      //      ::file::throw_os_error(dwLastError);

      //}



      //::file::path node::user_appdata_local()
      //{

      //   return shell_get_special_folder_path(CSIDL_LOCAL_APPDATA);

      //}



      //::file::path get_known_folder(REFKNOWNFOLDERID kfid)
      //{

      //   ::file::path str;

      //   ::cotaskptr < PWSTR > pwszPath;

      //   HANDLE hToken = nullptr;

      //   ::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY | TOKEN_IMPERSONATE | TOKEN_DUPLICATE, &hToken);

      //   HRESULT hr = SHGetKnownFolderPath(kfid, 0, hToken, &pwszPath);

      //   return pwszPath;

      //}


   } // namespace windows_common


} // namespace apex



