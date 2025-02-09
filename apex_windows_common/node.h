//
// Created by camilo on 21/01/2021. <33TBS!!
//

//
// Created by camilo on 21/01/2021. <3-<3ThomasBS!!
//
#pragma once


#include "acme_windows_common/node.h"
#include "apex/platform/node.h"


namespace apex_windows_common
{


   class CLASS_DECL_APEX_WINDOWS_COMMON node :
      virtual public ::acme_windows_common::node,
      virtual public ::apex::node
   {
   public:


      node();
      ~node() override;


      virtual void initialize(::particle * pparticle) override;

      virtual void process_init();

      virtual void _001InitializeShellOpen() override;

      virtual bool _os_calc_app_dark_mode();

      virtual bool _os_calc_system_dark_mode();

      virtual ::color::color get_default_color(unsigned long long u);

      virtual void set_console_colors(unsigned int dwScreenColors, unsigned int dwPopupColors, unsigned int dwWindowAlpha);

      virtual void set_system_dark_mode1(bool bSet = true);

      virtual void set_app_dark_mode1(bool bSet = true);

      virtual double get_time_zone();

      virtual void open_folder(::file::path& pathFolder);

      virtual void register_dll(const ::file::path& pathDll);

      virtual void system_main() override;

      void get_firefox_installation_info(string& strPathToExe, string& strInstallDirectory);

      //bool has_ip4_internet() override;

      //bool has_ip6_internet() override;

   };


} // namespace apex_windows_common



