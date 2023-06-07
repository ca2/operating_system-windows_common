// Created by camilo on 2022-11-07 11:02 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "security_attributes.h"


namespace acme_windows_common
{


   security_attributes::security_attributes()
   {

   }


   security_attributes::~security_attributes()
   {


   }


   void * security_attributes::get_os_security_attributes()
   {

      return m_memory.begin();

   }


} // namespace acme_windows_common



