#include "framework.h"
#include "apex/operating_system.h"
#include <Shlobj.h>
#include "directory_system.h"
#include "acme/filesystem/filesystem/directory_system.h"
#include "acme_windows/directory_system.h"
//#include "_windows.h"
//#include "acme/os/windows_common/cotaskptr.h"

//#include "apex/xml/_.h"




namespace windows
{


   directory_system::directory_system()
   {


   }


   directory_system::~directory_system()
   {

   }



   void directory_system::initialize(::particle * pparticle)
   {

      auto estatus = ::directory_system::initialize(pobject);

      if (!estatus)
      {

         return estatus;

      }

      m_pathInstall = directory_system()->install();

      directory_system()->m_pplatformdir->_shell_get_special_folder_path(
         nullptr,
         m_strCommonAppData,
         CSIDL_COMMON_APPDATA,
         false);
      //shell_get_special_folder_path(
      // nullptr,
      //m_pathProfile,
      //CSIDL_PROFILE,
      //false);

      m_pathHome = directory_system()->m_pplatformdir->_get_known_folder(FOLDERID_Profile);

      m_pathCa2Config = directory_system()->ca2roaming();

      m_strCommonAppData /= "ca2";

      m_strAppData = directory_system()->m_pplatformdir->_get_known_folder(FOLDERID_RoamingAppData);

      directory_system()->m_pplatformdir->_shell_get_special_folder_path(
         nullptr,
         m_strPrograms,
         CSIDL_PROGRAMS,
         false);
      directory_system()->m_pplatformdir->_shell_get_special_folder_path(
         nullptr,
         m_strCommonPrograms,
         CSIDL_COMMON_PROGRAMS,
         false);

      {

         //string strRelative;
         //strRelative = install();
         //index iFind = strRelative.find(':');
         //if (iFind >= 0)
         //{
         //   character_count iFind1 = strRelative.rear_find("\\", iFind);
         //   character_count iFind2 = strRelative.rear_find("/", iFind);
         //   character_count iStart = maximum(iFind1 + 1, iFind2 + 1);
         //   strRelative = strRelative.left(iFind - 1) + "_" + strRelative.substr(iStart, iFind - iStart) + strRelative.substr(iFind + 1);
         //}



      }

      if (m_strTimeFolder.is_empty())
      {

         m_strTimeFolder = directory_system()->appdata() / "time";

      }

      if (m_strNetSeedFolder.is_empty())
      {

         m_strNetSeedFolder = directory_system()->install() / "net";

      }

               auto psystem = system();

         auto pacmedir = psystem->m_pdirectorysystem;

pacmedir->create(m_strTimeFolder);
      //xxdebug_box("win_dir::initialize (m_strTimeFolder)", "win_dir::initialize", 0);

      if (!         auto psystem = system();

         auto pacmedir = psystem->m_pdirectorysystem;

pacmedir->is(m_strTimeFolder))
         return false;

               auto psystem = system();

         auto pacmedir = psystem->m_pdirectorysystem;

pacmedir->create(m_strTimeFolder / "time");

      //xxdebug_box("win_dir::initialize", "win_dir::initialize", 0);

      return ::success;

   }


   void directory_system::init_system()
   {

      auto estatus = ::directory_system::init_system();

      if (!estatus)
      {

         return estatus;

      }


      return ::success;

   }







   ::file::path directory_system::application_installer_folder(const ::file::path& pathExe, string strAppId, const ::string & pszPlatform, const ::string & pszConfiguration, const ::string & pszLocale, const ::string & pszSchema)
   {

      string strFolder = pathExe.folder();

      strFolder.replace(":", "");

      return directory_system()->ca2roaming() / "appdata" / strFolder / strAppId / pszPlatform / pszConfiguration / pszLocale / pszSchema;

   }




   ::file::path directory_system::get_application_path(string strAppId, const ::string & pszPlatform, const ::string & pszConfiguration)
   {

      ::file::path pathFolder;

      pathFolder = directory_system()->stage(strAppId, pszPlatform, pszConfiguration);

      string strName;

      strName = ::process::app_id_to_app_name(strAppId);

      ::file::path path;

      path = pathFolder / (strName + ".exe");

      return path;

   }


} // namespace windows



