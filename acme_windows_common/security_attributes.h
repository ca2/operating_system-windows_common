// Created by camilo on 2022-11-02 09:23 <3ThomasBorregaardSorensen!!
#pragma once


#include "acme/prototype/prototype/memory.h"
#include "acme/operating_system/security_attributes.h"


namespace acme_windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON security_attributes :
      virtual public ::security_attributes
   {
   public:


      ::memory          m_memory;


      security_attributes();
      ~security_attributes() override;


      virtual void * get_os_security_attributes();


   };


} // namespace acme_windows_common



