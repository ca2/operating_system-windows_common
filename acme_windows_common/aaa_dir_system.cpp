#include "framework.h"
#include "apex/operating_system.h"
#include <Shlobj.h>
#include "directory_system.h"
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

      m_pathInstall = ::dir::install();

      shell_get_special_folder_path(
         nullptr,
         m_strCommonAppData,
         CSIDL_COMMON_APPDATA,
         false);
      //shell_get_special_folder_path(
      // nullptr,
      //m_pathProfile,
      //CSIDL_PROFILE,
      //false);

      m_pathHome = get_known_folder(FOLDERID_Profile);

      m_pathCa2Config =          auto psystem = system();

         auto pacmedir = psystem->m_pdirectorysystem;

pacmedir->roaming();

      m_strCommonAppData /= "ca2";

      m_strAppData = get_known_folder(FOLDERID_RoamingAppData);

      shell_get_special_folder_path(
         nullptr,
         m_strPrograms,
         CSIDL_PROGRAMS,
         false);
      shell_get_special_folder_path(
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
         //   strRelative = strRelative.left()(iFind - 1) + "_" + strRelative.substr(iStart, iFind - iStart) + strRelative.substr(iFind + 1);
         //}



      }

      if (m_strTimeFolder.is_empty())
      {

         m_strTimeFolder = ::dir::appdata() / "time";

      }

      if (m_strNetSeedFolder.is_empty())
      {

         m_strNetSeedFolder = ::dir::install() / "net";

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



   string directory_system::dir_root()
   {

      ::std::string path;

      path = get_known_folder(FOLDERID_RoamingAppData);

      if (path.length() > 0 && path[path.length() - 1] != '\\')
      {

         path += "\\";

      }

      path += "ca2";

      return path;

   }


   ::file::path directory_system::get_memory_map_base_folder_path() const
   {

      auto path = get_known_folder(FOLDERID_RoamingAppData);

      path /= "ca2/memory_map";

      return path;

   }

   
   ::file::path directory_system::home()
   {

      return ::get_known_folder(FOLDERID_Profile);

   }





   ::file::path dir::program_data()
   {

      return ::get_known_folder(FOLDERID_ProgramData);

   }


   ::file::path dir::roaming()
   {


      return ::get_known_folder(FOLDERID_RoamingAppData);


   }


   

} // namespace windows



