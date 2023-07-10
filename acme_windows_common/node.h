//
// Created by camilo on 21/01/2021. <33TBS!!
//
//
// Created by camilo on 21/01/2021. <3-<3ThomasBS!!
//
#pragma once


#include "acme/platform/node.h"
#include "acme/primitive/collection/numeric_array.h"


namespace acme_windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON node :
      virtual public ::acme::node
   {
   public:

         
      //__creatable_from_base(node, ::acme::node);


      ::file::path            m_strTimeFolder;
      ::file::path            m_strNetSeedFolder;

      ::file::path            m_strCommonAppData;
      ::file::path            m_strAppData;
      ::file::path            m_strPrograms;
      ::file::path            m_strCommonPrograms;


      node();
      ~node() override;


      void initialize(::particle * pparticle) override;

      //virtual ::string dir_root() override;

      //virtual ::file::path get_memory_map_base_folder_path() const override;

      //virtual ::file::path home() override;

      //virtual ::file::path program_data() override;

      //virtual ::file::path roaming() override;

      void install_crash_dump_reporting(const string& strModuleNameWithTheExeExtension) override;


      virtual ::pointer < ::mutex > create_named_mutex(::particle * pparticleContext, bool bInitiallyOwn, const char * pszName, security_attributes * psecurityattributes = nullptr);


      ::pointer < ::mutex > create_local_named_mutex(::particle * pparticleContext, bool bInitiallyOwned, const ::string & strName, security_attributes * psecurityattributes = nullptr) override;
      ::pointer < ::mutex > create_global_named_mutex(::particle * pparticleContext, bool bInitiallyOwned, const ::string & strName, security_attributes * psecurityattributes = nullptr) override;

      ::pointer < ::mutex > open_local_named_mutex(::particle * pparticleContext, const ::string & strName) override;
      ::pointer < ::mutex > open_global_named_mutex(::particle * pparticleContext, const ::string & strName) override;


      ::pointer < ::mutex > get_install_mutex(::particle * pparticleContext, const ::string & strPlatform, const ::string & strSuffix) override;


//      ::pointer < ::acme::exclusive > _get_exclusive(::particle * pparticleContext, const ::string & strName, const ::security_attributes & securityattributes = nullptr) override;

      //virtual bool memcnts();

      //virtual ::file::path memcnts_base_path();

      //void datetime_to_filetime(file_time_t* pFileTime, const ::earth::time& time) override;


      //virtual ::e_status ::windows::last_error_status(DWORD dwLastError);


      string audio_get_default_implementation_name() override;


      string font_name(enum_font efont) override;

      //bool succeeded(const ::error_code& errorcode) override;

      error_code defer_co_initialize_ex(bool bMultiThread, bool bDisableOleDDE = false) override;


      ::file::path library_file_name(const ::scoped_string & scopedstr) override;


   };


} // namespace acme_windows_common



