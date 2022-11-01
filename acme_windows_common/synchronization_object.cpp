// Created by camilo on 2022-10-28 14:35 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "synchronization_object.h"
#include "acme/exception/exception.h"
#include "acme/operating_system.h"


namespace acme_windows_common
{


   synchronization_object::~synchronization_object()
   {

      if (m_hsynchronization != nullptr)
      {

         if (is_own_synchronization())
         {

            ::CloseHandle(m_hsynchronization);

         }

         m_hsynchronization = nullptr;

      }


   }


   bool synchronization_object::_wait(const class ::wait & wait)
   {
   
      if (!m_hsynchronization)
      {

         throw ::exception(error_wrong_state);

      }

      DWORD dwResult = ::WaitForSingleObjectEx(m_hsynchronization, wait, false);

      auto estatus = windows_wait_result_to_status(dwResult);

      if (estatus == error_wait_timeout)
      {

         return false;

      }
      else if (estatus == signaled_base)
      {

         return true;

      }
      else
      {

         throw ::exception(estatus);

         return false;

      }

   }
   

} // namespace acme_windows_common



