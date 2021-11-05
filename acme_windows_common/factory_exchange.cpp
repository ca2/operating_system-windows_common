#include "framework.h"


void CLASS_DECL_ACME __cdecl _ca2_purecall();


extern "C"
void acme_windows_common_factory_exchange(::factory_map * pfactorymap)
{

   _set_purecall_handler(_ca2_purecall);

   //pfactorymap->create_factory < ::windows::dir_system, ::dir_system >();
   //pfactorymap->create_factory < ::windows::file_system, ::file_system >();

   //pfactorymap->create_factory < ::windows::dir_context, ::dir_context >();
   //pfactorymap->create_factory < ::windows::file_context, ::file_context >();

   //create_factory < ::windows::stdio_file, ::file::text_file >();
   //create_factory < ::windows::file, ::file::file >();
   //pfactorymap->create_factory < ::windows::os_context, ::os_context >();
   //pfactorymap->create_factory < ::windows::pipe, ::operating_system::pipe >();
   //pfactorymap->create_factory < ::windows::process, ::operating_system::process >();

   ////create_factory < ::windows::console, ::console::console >();
   //pfactorymap->create_factory < ::windows::crypto, ::crypto::crypto >();
   //pfactorymap->create_factory < ::windows::ip_enum, ::net::ip_enum >();

   pfactorymap->create_factory < ::acme::windows_common::node, ::acme::node >();
   pfactorymap->create_factory < ::windows_common::acme_dir, ::acme_dir >();
   pfactorymap->create_factory < ::windows_common::acme_file, ::acme_path >();
   pfactorymap->create_factory < ::windows_common::acme_path, ::acme_path >();

   //pfactorymap->create_factory < ::windows::interprocess_communication_base, ::interprocess_communication::base >();
   //pfactorymap->create_factory < ::windows::interprocess_communication_rx, ::interprocess_communication::rx >();
   //pfactorymap->create_factory < ::windows::interprocess_communication_tx, ::interprocess_communication::tx >();
   //create_factory < ::windows::interprocess_communication, ::interprocess_communication::interprocess_communication >();


   //create_factory < ::windows::buffer, ::graphics::graphics >();
   //create_factory < ::windows::interaction_impl, ::user::interaction_impl >();

   //pfactorymap->create_factory < ::file::os_watcher, ::file::watcher >();
   //pfactorymap->create_factory < ::file::os_watch, ::file::watch >();

   //pfactorymap->create_factory < ::windows::file_context, ::file_context >();
   //create_factory < ::windows::copydesk, ::user::cop
   // 
   // 
   // ydesk >();
   ////create_factory < ::windows::shell, ::user::shell >();

   //pfactorymap->create_factory < ::windows::serial, ::serial::serial >();
   //pfactorymap->create_factory < ::windows::file_memory_map, ::file::memory_map >();

   //pfactorymap->create_factory < ::windows::pipe, ::operating_system::pipe >();
   //pfactorymap->create_factory < ::windows::file, ::file::file >();
   //pfactorymap->create_factory < ::windows::console, ::console::console >();

   //pfactorymap->create_factory < ::windows::dir_system, ::dir_system >();
   //pfactorymap->create_factory < ::windows::file_system, ::file_system >();

   //pfactorymap->create_factory < ::windows::dir_context, ::dir_context >();
   //pfactorymap->create_factory < ::windows::file_context, ::file_context >();

   //pfactorymap->create_factory < ::windows::stdio_file, ::file::text_file >();
   //pfactorymap->create_factory < ::windows::os_context, ::os_context >();
   //pfactorymap->create_factory < ::windows::future, ::operating_system::process >();

   //pfactorymap->create_factory < ::windows::crypto, ::crypto::crypto >();
   //pfactorymap->create_factory < ::windows::ip_enum, ::net::ip_enum >();

   //pfactorymap->create_factory < ::windows::file_memory_map, ::file::memory_map >();
   //pfactorymap->create_factory < ::windows::buffer, ::graphics::graphics >();
   //pfactorymap->create_factory < ::windows::interaction_impl, ::user::interaction_impl >();

   //pfactorymap->create_factory < ::file::os_watcher, ::file::watcher >();
   //pfactorymap->create_factory < ::file::os_watch, ::file::watch >();

   //pfactorymap->create_factory < ::windows::copydesk, ::user::copydesk >();
   //pfactorymap->create_factory < ::windows::shell, ::user::shell >();

   pfactorymap->create_factory < ::windows_common::exception_translator, ::exception_translator >();
   //pfactorymap->create_factory < ::windows_common::exception_engine, ::exception_engine >();


}



