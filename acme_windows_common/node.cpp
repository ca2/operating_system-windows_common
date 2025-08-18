#include "framework.h"
#include "node.h"
#include "mutex.h"
#include "acme/constant/id.h"
#include "acme/constant/user_key.h"
#include "acme/filesystem/filesystem/directory_system.h"
#include "acme/platform/scoped_restore.h"
#include "acme/platform/system.h"
#include "acme/user/user/key_state.h"

extern "C" void nano_idn_windows_common_factory(::factory::factory * pfactory);


#include "acme/_operating_system.h"


namespace acme_windows_common
{


   node::node()
   {

   }


   node::~node()
   {


   }


   void node::initialize(::particle * pparticle)
   {

      /*auto estatus =*/
      ::platform::node::initialize(pparticle);

   //   //if (!estatus)
   //   //{

   //   //   return estatus;

   //   //}

   //   //return estatus;

   }

   //   string node::get_user_name()
   //   {
   //
   //      WCHAR wsz[1024];
   //
   //      DWORD dwSize = sizeof(wsz) / sizeof(WCHAR);
   //
   //      ::GetUserNameW(wsz, &dwSize);
   //
   //      return string(wsz);
   //
   //   }
   //#include "aura/os/windows/_c.h"
   //
   //
   //   bool node::_os_calc_app_dark_mode()
   //   {
   //
   //      try
   //      {
   //
   //         ::acme_windows::registry::key key;
   //
   //         key.open(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize");
   //
   //         unsigned int dw;
   //
   //         auto estatus = key._get("AppsUseLightTheme", dw);
   //
   //         if (::failed(estatus))
   //         {
   //
   //            estatus = key._get("SystemUseLightTheme", dw);
   //
   //            if (::failed(estatus))
   //            {
   //
   //               return false;
   //
   //            }
   //
   //         }
   //
   //         return dw == 0;
   //
   //      }
   //      catch (...)
   //      {
   //
   //         return false;
   //
   //      }
   //
   //   }
   //
   //
   //   bool node::_os_calc_system_dark_mode()
   //   {
   //
   //      try
   //      {
   //
   //         ::acme_windows::registry::key key;
   //
   //         key.open(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize");
   //
   //         unsigned int dw;
   //
   //         auto estatus = key._get("SystemUseLightTheme", dw);
   //
   //         if (::failed(estatus))
   //         {
   //
   //            estatus = key._get("AppsUseLightTheme", dw);
   //
   //            if (::failed(estatus))
   //            {
   //
   //               return false;
   //
   //            }
   //
   //         }
   //
   //         return dw == 0;
   //
   //      }
   //      catch (...)
   //      {
   //
   //         return false;
   //
   //      }
   //
   //   }
   //
   //
   //   ::color::color node::get_default_color(unsigned long long u)
   //   {
   //
   //      switch (u)
   //      {
   //      case COLOR_3DFACE:
   //         return argb(127, 192, 192, 200);
   //      case COLOR_WINDOW:
   //         return argb(127, 255, 255, 255);
   //      case COLOR_3DLIGHT:
   //         return argb(127, 218, 218, 210);
   //      case COLOR_3DHIGHLIGHT:
   //         return argb(127, 238, 238, 230);
   //      case COLOR_3DSHADOW:
   //         return argb(127, 138, 138, 130);
   //      case COLOR_3DDKSHADOW:
   //         return argb(127, 90, 90, 80);
   //      default:
   //         break;
   //      }
   //
   //      return argb(127, 0, 0, 0);
   //
   //   }
   //
   //   
   //   void node::set_console_colors(unsigned int dwScreenColors, unsigned int dwPopupColors, unsigned int dwWindowAlpha)
   //   {
   //
   //      ::acme_windows::registry::key key(HKEY_CURRENT_USER, "Console", true);
   //
   //      key._set("ScreenColors", dwScreenColors);
   //      key._set("PopupColors", dwPopupColors);
   //      key._set("WindowAlpha", dwWindowAlpha);
   //
   //   }
   //
   //
   //
   //   void node::set_system_dark_mode1(bool bSet)
   //   {
   //
   //      ::acme_windows::registry::key key(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", true);
   //
   //      unsigned int dwSystemUseLightTheme;
   //      if (bSet)
   //      {
   //         dwSystemUseLightTheme = 0;
   //      }
   //      else
   //      {
   //         dwSystemUseLightTheme = 1;
   //      }
   //
   //      key._set("SystemUsesLightTheme", dwSystemUseLightTheme);
   //      return ::success;
   //
   //   }
   //
   //
   //   void node::set_app_dark_mode1(bool bSet)
   //   {
   //
   //      ::acme_windows::registry::key key(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", true);
   //
   //      unsigned int dwAppsUseLightTheme;
   //      if (bSet)
   //      {
   //         dwAppsUseLightTheme = 0;
   //      }
   //      else
   //      {
   //         dwAppsUseLightTheme = 1;
   //      }
   //
   //      key._set("AppsUseLightTheme", dwAppsUseLightTheme);
   //
   //      return ::success;
   //
   //   }
   //
   //   
   //   double node::get_time_zone()
   //   {
   //
   //      double dTimeZone = 0.;
   //
   //#ifdef WINDOWS
   //      {
   //         //posix_time t = time(nullptr);
   //
   //         //struct tm *p = localtime(&t);
   //
   //         DYNAMIC_TIME_ZONE_INFORMATION i = {};
   //
   //         unsigned int dw = GetDynamicTimeZoneInformation(&i);
   //
   //         if (dw == TIME_ZONE_ID_STANDARD)
   //         {
   //
   //            dTimeZone = -((double)(i.Bias + i.StandardBias) / 60.0);
   //
   //         }
   //         else if (dw == TIME_ZONE_ID_DAYLIGHT)
   //         {
   //
   //            dTimeZone = -((double)(i.Bias + i.DaylightBias) / 60.0);
   //
   //         }
   //         else
   //         {
   //
   //            dTimeZone = -((double)i.Bias / 60.0);
   //
   //         }
   //
   //      }
   //#else
   //      {
   //
   //         posix_time t = time(nullptr);
   //
   //         struct tm lt = { 0 };
   //
   //         localtime_r(&t, &lt);
   //
   //         //printf("Offset to GMT is %lds.\n", lt.tm_gmtoff);
   //
   //         //printf("The time zone is '%s'.\n", lt.tm_zone);
   //
   //         dTimeZone = +((double)lt.tm_gmtoff / (60.0 * 60.0));
   //
   //      }
   //#endif
   //
   //      return dTimeZone;
   //
   //   }
   //
   //
   //   void node::open_folder(::file::path & pathFolder)
   //   {
   //
   //      wstring wstrFolder(pathFolder);
   //
   //      int i = (int) (iptr) ::ShellExecuteW(nullptr, L"open", wstrFolder, nullptr, nullptr, SW_NORMAL);
   //
   //      if (i < 32)
   //      {
   //
   //         switch (i)
   //         {
   //         case 0:
   //            //The operating system is out of memory or resources.
   //            return error_no_memory;
   //         case ERROR_FILE_NOT_FOUND:
   //            return error_file_not_found;
   //            //The specified file was not found.
   //         case ERROR_PATH_NOT_FOUND:
   //            return error_path_not_found;
   //            //            The specified path was not found.
   //         case          ERROR_BAD_FORMAT:
   //            return error_bad_format;
   //            //The.exe file is invalid(non - Win32.exe or error in.exe image).
   //            //case SE_ERR_ACCESSDENIED:
   //            //         return error_access_denied;
   //            ////The operating system denied access to the specified file.
   //            //SE_ERR_ASSOCINCOMPLETE
   //            //The file name association is incomplete or invalid.
   //            //SE_ERR_DDEBUSY
   //            //The DDE transaction could not be completed because other DDE transactions were being processed.
   //            //SE_ERR_DDEFAIL
   //            //The DDE transaction failed.
   //            //SE_ERR_DDETIMEOUT
   //            //The DDE transaction could not be completed because the request timed out.
   //            //SE_ERR_DLLNOTFOUND
   //            //The specified DLL was not found.
   //            //SE_ERR_FNF
   //            //The specified file was not found.
   //            //SE_ERR_NOASSOC
   //            //There is no application associated with the given file name extension.This error will also be returned if you attempt to print a file that is not printable.
   //            //SE_ERR_OOM
   //            //There was not enough memory to complete the operation.
   //            //SE_ERR_PNF
   //            //The specified path was not found.
   //            //SE_ERR_SHARE
   //            //A sharing violation occurred.*/
   //         default:
   //            return error_failed;
   //         }
   //
   //      }
   //
   //      return ::success;
   //
   //   }
   //
   //
   //   void node::register_dll(const ::file::path & pathDll)
   //   {
   //
   //
   //      string strPathDll;
   //         
   //      //#ifdef _DEBUG
   //         
   //      strPathDll = pathDll;
   //         
   //      //#else
   //      //
   //      //   strPathDll = directory_system()->matter() / "time" / process_platform_name() /"stage/_desk_tb.dll";
   //      //
   //      //#endif
   //         
   //      string strParam;
   //         
   //      strParam = "/s \"" + strPathDll + "\"";
   //         
   //      //wstring wstrParam(strParam);
   //         
   //      //STARTUPINFOW si = {};
   //         
   //      //si.cb = sizeof(si);
   //         
   //      //si.wShowWindow = SW_HIDE;
   //         
   //      //PROCESS_INFORMATION pi = {};
   //         
   //      WCHAR wszSystem[2048];
   //         
   //      GetSystemDirectoryW(wszSystem, sizeof(wszSystem) / sizeof(WCHAR));
   //         
   //      wstring wstrSystem(wszSystem);
   //         
   //      ::file::path path(wstrSystem);
   //         
   //      path /= "regsvr32.exe";
   //         
   //      ::property_set set;
   //         
   //      set["privileged"] = true;
   //         
   //      if (!call_sync(path, strParam, path.folder(), ::e_display_none, 3_min, set))
   //      {
   //         
   //         return false;
   //         
   //      }
   //         
   //      //if (CreateProcessW(wstrPath, wstrParam, nullptr, nullptr, false, 0, nullptr, wstrSystem, &si, &pi))
   //      //{
   //         
   //      //   output_debug_string("created");
   //         
   //      //}
   //      //else
   //      //{
   //         
   //      //   output_debug_string("not created");
   //         
   //      //}
   //         
   //      //CloseHandle(pi.hProcess);
   //         
   //      //CloseHandle(pi.hthread);
   //         
   //      return true;
   //         
   //   }
   //
   //
   //   void node::start()
   //   {
   //
   //      auto estatus = system()->m_papex->thread_initialize(system());
   //
   //      if (!estatus)
   //      {
   //
   //         return estatus;
   //
   //      }
   //
   //      estatus = system()->on_start();
   //
   //      if (!estatus)
   //      {
   //
   //         return estatus;
   //
   //      }
   //
   //      estatus = system()->main();
   //
   //      if (!estatus)
   //      {
   //
   //         return estatus;
   //
   //      }
   //
   //      estatus = system()->inline_term();
   //
   //      if (!estatus)
   //      {
   //
   //         return estatus;
   //
   //      }
   //
   //      return estatus;
   //
   //
   //   }
   //
   //
   //   void node::get_firefox_installation_info(string& strPathToExe, string& strInstallDirectory)
   //   {
   //
   //#ifdef WINDOWS_DESKTOP
   //
   //      try
   //      {
   //
   //         ::acme_windows::registry::key key(HKEY_LOCAL_MACHINE, "SOFTWARE\\Mozilla\\Mozilla Firefox");
   //
   //         string strCurrentVersion;
   //
   //         key.get("CurrentVersion", strCurrentVersion);
   //
   //         key.open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Mozilla\\Mozilla Firefox\\" + strCurrentVersion + "\\Main");
   //
   //         key.get("PathToExe", strPathToExe);
   //
   //         key.get("Install Directory", strInstallDirectory);
   //
   //      }
   //      catch (const ::e_status & estatus)
   //      {
   //
   //         return estatus;
   //
   //      }
   //
   //      return ::success;
   //
   //#else
   //
   //      return ::error_failed;
   //
   //#endif
   //
   //   }
   //
   //
   //   void node::_001InitializeShellOpen()
   //   {
   //
   //      //ASSERT(m_atomApp == nullptr && m_atomSystemTopic == nullptr); // do once
   //
   //      //m_atomApp            = ::GlobalAddAtomW(utf8_to_unicode(m_strAppName));
   //
   //      //m_atomSystemTopic    = ::GlobalAddAtomW(L"system");
   //
   //      return ::success;
   //
   //   }
   //
   //
   //   void node::process_init()
   //   {
   //
   //      
   //
   //      defer_initialize_winsock();
   //      return success;
   //
   //   }
   //
   //
   //   string node::veriwell_multimedia_music_midi_get_default_implementation_name()
   //   {
   //
   //      return system()->implementation_name("music_midi", "mmsystem");
   //
   //   }
   //
   //
   //   string node::multimedia_audio_mixer_get_default_implementation_name()
   //   {
   //
   //      return "audio_mixer_mmsystem";
   //
   //   }
   //
   //
   //   string node::multimedia_audio_get_default_implementation_name()
   //   {
   //
   //      string str;
   //
   //      if (file_system()->exists(directory_system()->system() / "config\\system\\audio.txt"))
   //      {
   //
   //         str = file_system()->as_string(directory_system()->system() / "config\\system\\audio.txt");
   //
   //      }
   //      else
   //      {
   //
   //         ::file::path strPath;
   //
   //         strPath = directory_system()->appdata() / "audio.txt";
   //
   //         str = file_system()->as_string(strPath);
   //
   //      }
   //
   //      if (str.has_character())
   //         return "audio_" + str;
   //      else
   //         return "audio_mmsystem";
   //
   //   }
   //
   //
      // Twitter Automator and Denis Lakic and UpWork contribution
   // enzymes: Liveedu.tv, Twitch.tv and Mixer.com streamers and viewers
   // Mummi and bilbo!!
   // create call to :
   void node::install_crash_dump_reporting(const ::scoped_string & scopedstrModuleNameWithTheExeExtension)
   {

      //::acme_windows::registry::key k;

      //string strKey = "SOFTWARE\\Microsoft\\Windows\\Windows Error Reporting\\LocalDumps\\" + strModuleNameWithTheExeExtension;

      //if (k._open(HKEY_LOCAL_MACHINE, strKey, true))
      //{
      //   ::file::path str = directory_system()->system() / "CrashDumps" / strModuleNameWithTheExeExtension;
      //   wstring wstr = str;
      //   RegSetValueExW(k.m_hkey, L"DumpFolder", 0, REG_EXPAND_SZ, (unsigned char*)wstr.c_str(), unsigned int((wcslen(wstr) + 1) * sizeof(wchar_t)));
      //   unsigned int dw = 10;
      //   RegSetValueExW(k.m_hkey, L"DumpCount", 0, REG_DWORD, (unsigned char*)&dw, sizeof(dw));
      //   dw = 2;
      //   RegSetValueExW(k.m_hkey, L"DumpType", 0, REG_DWORD, (unsigned char*)&dw, sizeof(dw));

      //}

      //output_debug_string("test01");

   }
   //
   //
   //   int g_iMemoryCountersStartable = 0;
   //
   //   bool node::memcnts()
   //   {
   //
   //      if (g_iMemoryCountersStartable && g_iMemoryCounters < 0)
   //      {
   //
   //         g_iMemoryCounters = file_system()->exists(directory_system()->config() / "system/memory_counters.txt") ? 1 : 0;
   //
   //         if (g_iMemoryCounters)
   //         {
   //
   //            g_pmutexMemoryCounters = ___new ::mutex(e_create_new, false, "Global\\ca2_memory_counters");
   //
   //         }
   //
   //      }
   //
   //      return g_iMemoryCountersStartable && g_iMemoryCounters;
   //
   //   }
   //
   //
   //   ::file::path* g_pMemoryCounters = nullptr;
   //
   //
   //   CLASS_DCL_ACME::file::path node::memcnts_base_path()
   //   {
   //
   //      if (g_iMemoryCountersStartable && g_pMemoryCounters == nullptr)
   //      {
   //
   //         g_pMemoryCounters = ___new ::file::path();
   //
   //#if defined(UNIVERSAL_WINDOWS)
   //
   //         string strBasePath = directory_system()->system() / "memory_counters";
   //
   //#else
   //
   //         ::file::path strModule = module_path_from_pid(getpid());
   //
   //         string strBasePath = directory_system()->system() / "memory_counters" / strModule.title() / ::as_string(getpid());
   //
   //#endif
   //
   //         * g_pMemoryCounters = strBasePath;
   //
   //      }
   //
   //      return *g_pMemoryCounters;
   //
   //   }
   //
   //


   //::e_status node::last_error_status(DWORD dwLastError)
   //{

   //   if (dwLastError == 0)
   //   {

   //      return ::success;

   //      //return;

   //   }
   //   else
   //   {

   //      return ::error_failed;

   //   }

   //}


   string node::audio_get_default_implementation_name()
   {

      return system()->implementation_name("audio", "mmsystem");

   }


   //void node::on_start_system()
   //{

   //   if (m_bHasNodeSystemStarted)
   //   {

   //      return ::success_none;

   //   }

   //   auto psystem = system();

   //   auto estatus = psystem->post_initial_request();

   //   if (!estatus)
   //   {

   //      return estatus;

   //   }

   //   return estatus;

   //}


   string node::font_name(enum_font efont)
   {

      if (efont == e_font_sans)
      {

         return "Arial";

      }
      else if (efont == e_font_sans_ex)
      {

         return "Geneva";

      }
      else if (efont == e_font_sans_fx)
      {

         return "Tahoma";

      }
      else if (efont == e_font_sans_ui)
      {

         return "Segoe UI";

      }
      else if (efont == e_font_serif)
      {

         return "Times New Roman";

      }
      else if (efont == e_font_serif_ex)
      {

         return "Palatino Linotype";

      }
      else if (efont == e_font_serif_fx)
      {

         return "Georgia";

      }
      else if (efont == e_font_monospace)
      {

         return "Courier New";

      }
      else if (efont == e_font_monospace_fx)
      {

         return "Lucida Console";

      }

      return "Helvetica";

   }


   ::enum_id node::key_command(::user::enum_key ekey, ::user::key_state* pkeystate)
   {

      if (ekey == ::user::e_key_a && pkeystate->is_key_pressed(::user::e_key_control))
      {

         return ::id_edit_select_all;

      }
      else if (ekey == ::user::e_key_c && pkeystate->is_key_pressed(::user::e_key_control))
      {

         return ::id_edit_copy;

      }
      else if (ekey == ::user::e_key_v && pkeystate->is_key_pressed(::user::e_key_control))
      {

         return ::id_edit_paste;

      }
      else if (ekey == ::user::e_key_x && pkeystate->is_key_pressed(::user::e_key_control))
      {

         return ::id_edit_cut;

      }

      return ::id_none;

   }


   ::pointer < ::mutex > node::create_named_mutex(::particle * pparticleContext, bool bInitiallyOwn, const_char_pointer pszName, security_attributes * psecurityattributes)
   {

      //return øallocate mutex(pparticleContext, bInitiallyOwn, pszName ADD_PARAM_SEC_ATTRS);
      return øallocate ::acme_windows_common::mutex(pparticleContext, bInitiallyOwn, pszName, psecurityattributes);

   }


   ::pointer < ::mutex > node::create_local_named_mutex(::particle * pparticleContext, bool bInitiallyOwned, const ::scoped_string & scopedstrName, security_attributes * psecurityattributes)
   {

      ::string strLocalName;

      strLocalName = "Local\\" + scopedstrName;

      return create_named_mutex(pparticleContext, bInitiallyOwned, strLocalName, psecurityattributes);

   }


   ::pointer < ::mutex > node::create_global_named_mutex(::particle * pparticleContext, bool bInitiallyOwned, const ::scoped_string & scopedstrName, security_attributes * psecurityattributes)
   {

      ::string strGlobalName;

      strGlobalName = "Global\\" + scopedstrName;

      return create_named_mutex(pparticleContext, bInitiallyOwned, strGlobalName, psecurityattributes);

   }


   ::pointer < ::mutex > node::open_local_named_mutex(::particle * pparticleContext, const ::scoped_string & scopedstrName)
   {

      return nullptr;

   }


   ::pointer < ::mutex > node::open_global_named_mutex(::particle * pparticleContext, const ::scoped_string & scopedstrName)
   {

      return nullptr;

   }

   
   ::pointer < ::mutex > node::get_install_mutex(::particle * pparticleContext, const ::scoped_string & scopedstrPlatform, const ::scoped_string & scopedstrSuffix)
   {

      return nullptr;

   }


   //bool node::succeeded(const ::error_code& errorcode)
   //{

   //   if (errorcode.m_etype == e_error_code_type_hresult)
   //   {

   //      return SUCCEEDED((HRESULT) errorcode.m_iOsError);

   //   }

   //   return ::platform::node::succeeded(errorcode);

   //}


   thread_local HRESULT t_hresultCoInitialize;

   thread_local bool t_bCoInitialize = false;

   error_code node::defer_co_initialize_ex(bool bMultiThread, bool bDisableOleDDE)
   {

      if (!t_bCoInitialize)
      {

         t_bCoInitialize = true;

         if (bMultiThread)
         {

            t_hresultCoInitialize = ::CoInitializeEx(nullptr, COINIT_MULTITHREADED);

         }
         else
         {

            t_hresultCoInitialize = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | (bDisableOleDDE ? COINIT_DISABLE_OLE1DDE : 0));

         }

      }

      if (FAILED(t_hresultCoInitialize))
      {

         ::output_debug_string("Failed to ::CoInitializeEx(nullptr, COINIT_MULTITHREADED) at __node_pre_init");

         return { e_error_code_type_hresult, (long long) t_hresultCoInitialize };

      }

      return { e_error_code_type_hresult, (long long)t_hresultCoInitialize };

   }


   //void node::operating_system_file_dialog(
   //   void* poswindow,
   //   const ::array < ::pair < ::string, ::string > >& filetypesParam,
   //   const ::function < void(const ::file::path_array_base&) >& function,
   //   bool save, bool multiple)
   //{


   //}


   ::file::path node::library_file_name(const ::scoped_string & scopedstr)
   {
   
      return scopedstr + ".dll";

   }


   //void node::unzip_to_folder(const ::file::path& pathFolder, const ::file::path& pathZip)
   //{

   //   auto strFolderWindowsPath = pathFolder.windows_path();

   //   auto strZipWindowsPath = pathZip.windows_path();

   //   _unzip_to_folder(strZipWindowsPath, strFolderWindowsPath);

   //}


   ////https ://social.msdn.microsoft.com/Forums/vstudio/en-US/45668d18-2840-4887-87e1-4085201f4103/visual-c-to-unzip-a-zip-file-to-a-specific-directory
   //// removed return type and changed error returns to exceptions
   //// replace __try __finally with at_end_of_scope
   //// changed arguments to ansi_character * and used bstring class for string conversion
   //// use of comptr to guard COM objets and variant to guard VARIANTs
   //void node::_com_unzip_to_folder(const_char_pointer pszZip, const_char_pointer pszFolder)
   //{

   //   comptr < IShellDispatch> pISD;

   //   comptr < Folder> pZippedFile;
   //   comptr < Folder > pDestination;

   //   long FilesCount = 0;
   //   comptr< IDispatch > pItem;
   //   comptr < FolderItems > pFilesInside;

   //   variant Options, OutFolder, InZipFile, Item;
   //
   //   CoInitialize(NULL);

   //   directory_system()->create(pszFolder);


   //   {
   //      // speccylad@twitch contribution recalled
   //      at_end_of_scope
   //      {

   //         CoUninitialize();

   //      };

   //      HRESULT hr = pISD.CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER);

   //      defer_throw_hresult(hr);

   //      bstring bstrZip(pszZip);

   //      InZipFile.vt = VT_BSTR;
   //      InZipFile.bstrVal = bstrZip;
   //      pISD->NameSpace(InZipFile, &pZippedFile);
   //      if (!pZippedFile)
   //      {
   //         //pISD->Release();
   //         throw ::exception(error_failed, "pISD->NameSpace(InZipFile, &pZippedFile)");
   //      }

   //      bstring bstrFolder(pszFolder);

   //      OutFolder.vt = VT_BSTR;
   //      OutFolder.bstrVal = bstrFolder;
   //      pISD->NameSpace(OutFolder, &pDestination);
   //      if (!pDestination)
   //      {
   //         //pZippedFile->Release();
   //         //pISD->Release();
   //         throw ::exception(error_failed, "::acme_windows::common::node::_unzip_to_folder (1)");
   //      }

   //      pZippedFile->Items(&pFilesInside);
   //      if (!pFilesInside)
   //      {
   //         //pDestination->Release();
   //         //pZippedFile->Release();
   //         //pISD->Release();
   //         throw ::exception(error_failed, "::acme_windows::common::node::_unzip_to_folder (2)");
   //      }

   //      pFilesInside->get_Count(&FilesCount);
   //      if (FilesCount < 1)
   //      {
   //         //pFilesInside->Release();
   //         //pDestination->Release();
   //         //pZippedFile->Release();
   //         //pISD->Release();
   //         throw ::exception(error_failed, "::acme_windows::common::node::_unzip_to_folder (3)");
   //      }

   //      pFilesInside->QueryInterface(IID_IDispatch, (void**)&pItem);

   //      Item.vt = VT_DISPATCH;
   //      Item.pdispVal = pItem;

   //      Options.vt = VT_I4;
   //      Options.lVal = 1024 | 512 | 16 | 4;//http://msdn.microsoft.com/en-us/library/bb787866(VS.85).aspx

   //      bool retval = pDestination->CopyHere(Item, Options) == S_OK;

   //      //pItem->Release(); pItem = 0L;
   //      //pFilesInside->Release(); pFilesInside = 0L;
   //      //pDestination->Release(); pDestination = 0L;
   //      //pZippedFile->Release(); pZippedFile = 0L;
   //      //pISD->Release(); pISD = 0L;


   //   }

   //}

   ::string node::eol()
   {

      return "\n";

   }


   bool node::defer_component_factory(const ::scoped_string & scopedstrComponent)
   {


            if (scopedstrComponent == "nano_idn")
            {

               auto pfactory = this->factory();

               nano_idn_windows_common_factory(pfactory);

               return true;

      }

      return false;
   }


   ::string node::display_file_path(const ::file::path & path)
   {

      return path.windows_path().path();

   }


   ::string node::get_computer_name()
   {

#define INFO_BUFFER_SIZE 32767
      CHAR  infoBuf[INFO_BUFFER_SIZE];
      DWORD  bufCharCount = INFO_BUFFER_SIZE;

      // Get and display the name of the computer.
      if (!::GetComputerNameA(infoBuf, &bufCharCount))
      {
         return{};

      }

      return {infoBuf, infoBuf + bufCharCount};

   }


   ::string node::dynamic_library_suffix()
   {

      return ".dll";

   }



   
} // namespace acme_windows_common



