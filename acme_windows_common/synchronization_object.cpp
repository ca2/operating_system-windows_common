// Created by camilo on 2022-10-28 14:35 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "synchronization_object.h"
#include "acme/exception/exception.h"
#include "acme/parallelization/task.h"
#include "acme/_operating_system.h"


namespace acme_windows_common
{


   synchronization_object::synchronization_object() :
      m_handle(nullptr)
   {


   }


   synchronization_object::~synchronization_object()
   {

      if (m_handle != nullptr)
      {

         if (has_own_synchronization_flag())
         {

            ::CloseHandle(m_handle);

         }

         m_handle = nullptr;

      }

   }


   hsynchronization synchronization_object::get_synchronization_handle()
   {

      return m_handle;

   }


   bool synchronization_object::_wait(const class time & timeWait)
   {
   
      if (!m_handle)
      {

         throw ::exception(error_wrong_state);

      }

#ifdef MUTEX_DEBUG
      
      m_strThread = ::get_task_name(::get_task());

      m_itask = ::get_current_itask();

#endif

      DWORD dwResult = ::WaitForSingleObjectEx(m_handle, ::windows::wait(timeWait), false);

      auto estatus = ::windows::wait_result_status(dwResult);

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

         auto dwLastError = ::GetLastError();

         auto estatus = ::windows::last_error_status(dwLastError);

         auto errorcode = ::windows::last_error_error_code(dwLastError);

         throw ::exception(estatus, { errorcode }, "WaitForSingleObjectEx WAIT_FAILED");

         return false;

      }

   }
   

} // namespace acme_windows_common



