// Created by camilo on 2022-10-28 14:35 <3ThomasBorregaardSørensen!!
#pragma once


#include "acme/primitive/primitive/particle.h"


#define MUTEX_DEBUG


namespace acme_windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON synchronization_object :
      virtual public ::particle
   {
   public:


#ifdef MUTEX_DEBUG
      string                  m_strThread;
      itask_t                 m_itask;
#endif



      hsynchronization        m_handle;


      synchronization_object();
      ~synchronization_object();


      hsynchronization get_synchronization_handle() override;


      bool _wait(const class time & timeWait) override;


   };


} // namespace acme_windows_common



