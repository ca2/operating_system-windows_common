#include "framework.h"
#include "apex/operating_system.h"
#include "file_system.h"


namespace windows
{


   file_system::file_system()
   {

   }


   file_system::~file_system()
   {

   }


   void file_system::init_system()
   {

      auto estatus = ::file_system::init_system();

      if (!estatus)
      {

         return estatus;

      }

      return estatus;

   }

   
   void file_system::update_module_path()
   {

      m_pathModule = ::path::module(nullptr);

      m_pathCa2Module = ::path::module(::GetModuleHandleA("apex.dll"));

      return true;

   }



} // namespace windows



