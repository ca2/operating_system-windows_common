#include "framework.h"
#include "file.h"
#include "node.h"
#include "acme_directory.h"
#include "acme_file.h"
#include "acme_path.h"
#include "exception_translator.h"
#include "mutex.h"
//#include "exclusive.h"


__FACTORY_EXPORT void acme_windows_common_factory(::factory::factory * pfactory)
{

   //_set_purecall_handler(_ca2_purecall);

   //pfactory->add_factory_item < ::windows::dir_system, ::dir_system >();
   //pfactory->add_factory_item < ::windows::file_system, ::file_system >();

   //pfactory->add_factory_item < ::windows::dir_context, ::dir_context >();
   //pfactory->add_factory_item < ::windows::file_context, ::file_context >();

   //add_factory_item < ::windows::stdio_file, ::file::text_file >();
   pfactory->add_factory_item < ::acme_windows_common::file, ::file::file >();
   //pfactory->add_factory_item < ::windows::os_context, ::os_context >();
   //pfactory->add_factory_item < ::windows::pipe, ::operating_system::pipe >();
   //pfactory->add_factory_item < ::windows::process, ::operating_system::process >();

   ////add_factory_item < ::windows::console, ::console::console >();
   //pfactory->add_factory_item < ::windows::crypto, ::crypto::crypto >();
   //pfactory->add_factory_item < ::windows::ip_enum, ::net::ip_enum >();

   pfactory->add_factory_item < ::acme_windows_common::node, ::acme::node >();
   pfactory->add_factory_item < ::acme_windows_common::acme_directory, ::acme_directory >();
   pfactory->add_factory_item < ::acme_windows_common::acme_file, ::acme_file >();
   pfactory->add_factory_item < ::acme_windows_common::acme_path, ::acme_path >();

   //pfactory->add_factory_item < ::windows::interprocess_communication_base, ::inteprocess_channel::base >();
   //pfactory->add_factory_item < ::windows::interprocess_handler, ::interprocess::handler >();
   //pfactory->add_factory_item < ::windows::interprocess_caller, ::interprocess::caller >();
   //add_factory_item < ::windows::inteprocess_channel, ::inteprocess_channel::inteprocess_channel >();


   //add_factory_item < ::windows::buffer, ::graphics::graphics >();
   //add_factory_item < ::windows::interaction_impl, ::windowing::window >();

   //pfactory->add_factory_item < ::file::os_watcher, ::file::watcher >();
   //pfactory->add_factory_item < ::file::os_watch, ::file::watch >();

   //pfactory->add_factory_item < ::windows::file_context, ::file_context >();
   //add_factory_item < ::windows::copydesk, ::user::cop
   // 
   // 
   // ydesk >();
   ////add_factory_item < ::windows::shell, ::user::shell >();

   //pfactory->add_factory_item < ::windows::serial, ::serial::serial >();
   //pfactory->add_factory_item < ::windows::file_memory_map, ::file::memory_map >();

   //pfactory->add_factory_item < ::windows::pipe, ::operating_system::pipe >();
   //pfactory->add_factory_item < ::windows::file, ::file::file >();
   //pfactory->add_factory_item < ::windows::console, ::console::console >();

   //pfactory->add_factory_item < ::windows::dir_system, ::dir_system >();
   //pfactory->add_factory_item < ::windows::file_system, ::file_system >();

   //pfactory->add_factory_item < ::windows::dir_context, ::dir_context >();
   //pfactory->add_factory_item < ::windows::file_context, ::file_context >();

   //pfactory->add_factory_item < ::windows::stdio_file, ::file::text_file >();
   //pfactory->add_factory_item < ::windows::os_context, ::os_context >();
   //pfactory->add_factory_item < ::windows::future, ::operating_system::process >();

   //pfactory->add_factory_item < ::windows::crypto, ::crypto::crypto >();
   //pfactory->add_factory_item < ::windows::ip_enum, ::net::ip_enum >();

   //pfactory->add_factory_item < ::windows::file_memory_map, ::file::memory_map >();
   //pfactory->add_factory_item < ::windows::buffer, ::graphics::graphics >();
   //pfactory->add_factory_item < ::windows::interaction_impl, ::windowing::window >();

   //pfactory->add_factory_item < ::file::os_watcher, ::file::watcher >();
   //pfactory->add_factory_item < ::file::os_watch, ::file::watch >();

   //pfactory->add_factory_item < ::windows::copydesk, ::user::copydesk >();
   //pfactory->add_factory_item < ::windows::shell, ::user::shell >();

   pfactory->add_factory_item < ::acme_windows_common::mutex, ::mutex >();
   //pfactory->add_factory_item < ::acme_windows_common::exclusive, ::acme::exclusive >();

   pfactory->add_factory_item < ::acme_windows_common::exception_translator, ::exception_translator >();
   //pfactory->add_factory_item < ::windows_common::exception_engine, ::exception_engine >();


}



