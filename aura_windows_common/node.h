//
// Created by camilo on 21/01/2021. <33TBS!!
//

//
// Created by camilo on 21/01/2021. <3-<3ThomasBS!!
//
#pragma once


#include "apex_windows_common/node.h"
#include "aura/platform/node.h"


namespace aura_windows_common
{


      class CLASS_DECL_AURA_WINDOWS_COMMON node :
         virtual public ::apex_windows_common::node,
         virtual public ::aura::node
      {
      public:


         //__creatable_from_base(node, ::acme::node);


         node();
         ~node() override;


         virtual bool _os_calc_app_dark_mode() override;

         virtual bool _os_calc_system_dark_mode() override;


         virtual ::color::color get_default_color(::u64 u) override;

         virtual void set_console_colors(::u32 dwScreenColors, ::u32 dwPopupColors, ::u32 dwWindowAlpha) override;

         //virtual void set_system_dark_mode1(bool bSet = true);

         //virtual void set_app_dark_mode1(bool bSet = true);

         virtual double get_time_zone() override;

         //virtual void get_system_time(system_time_t * psystemtime) override;


         //virtual void open_folder(::file::path& pathFolder) override;

         //virtual void register_dll(const ::file::path& pathDll) override;


         virtual void system_main() override;


      };


} // namespace aura_windows_common



