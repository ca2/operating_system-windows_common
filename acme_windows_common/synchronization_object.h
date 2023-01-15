// Created by camilo on 2022-10-28 14:35 <3ThomasBorregaardSørensen!!
#pragma once


#include "acme/primitive/primitive/particle.h"


namespace acme_windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON synchronization_object :
      virtual public ::particle
   {
   public:


      ~synchronization_object();


      bool _wait(const class time & timeWait) override;


   };


} // namespace acme_windows_common



