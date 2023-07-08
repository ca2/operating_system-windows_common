// From acme/parallelization/mutex.cpp by camilo on 2022-10-28 14:04 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "mutex.h"
#include "acme/exception/exception.h"
#include "acme/filesystem/filesystem/acme_directory.h"
#include "acme/platform/system.h"
#include "acme/exception/exception.h"
#include "acme/_operating_system.h"


namespace acme_windows_common
{


   mutex::mutex(enum_create_new, bool bInitiallyOwn)
   {

#ifdef MUTEX_DEBUG
      m_itask = -1;
#endif

      m_handle = ::CreateMutexExW(nullptr, nullptr, bInitiallyOwn ? CREATE_MUTEX_INITIAL_OWNER : 0, MUTEX_ALL_ACCESS);

      set_own_synchronization_flag();

   }


   mutex::mutex(::particle * pparticle, bool bInitiallyOwn, const char * pszName, security_attributes * psecurityattributes)
   {

#ifdef MUTEX_DEBUG

      m_itask = -1;

#endif

      initialize(pparticle);

      wstring wstrName(pszName);

      const unichar * pwszName = pszName == nullptr ? nullptr : (const unichar *)wstrName;

      m_handle = ::CreateMutexExW(
         (LPSECURITY_ATTRIBUTES)psecurityattributes->get_os_security_attributes(),
         pwszName,
         bInitiallyOwn ? CREATE_MUTEX_INITIAL_OWNER : 0, MUTEX_ALL_ACCESS);

      DWORD dwLastError = ::GetLastError();

      set_already_exists_flag(dwLastError == ERROR_ALREADY_EXISTS);

      if (m_handle == nullptr)
      {

         DWORD dwError1 = ::GetLastError();

         if (pszName != nullptr)
         {

            wstring wstrName(pszName);

            m_handle = ::OpenMutexW(SYNCHRONIZE, false, wstrName);

         }

         if (m_handle == nullptr)
         {

            DWORD dwError2 = ::GetLastError();

            throw ::exception(error_resource);

         }

      }

      set_own_synchronization_flag();

   }


   mutex::mutex(enum_create_new, const char * pstrName, HANDLE handleSyncObject, bool bOwner)
   {

#ifdef MUTEX_DEBUG
      m_itask = -1;
#endif

      set_own_synchronization_flag(bOwner);

      m_handle = handleSyncObject;

   }


   mutex::~mutex()
   {

   }


   void mutex::unlock()
   {

#ifdef MUTEX_DEBUG

      m_strThread.empty();
      m_itask = -1;
      ::output_debug_string("");

#endif

      if (!m_handle)
      {

         throw ::exception(error_wrong_state);

      }

      if (!::ReleaseMutex(m_handle))
      {

         auto last_error = GetLastError();

         auto estatus = failed_last_error_status(last_error);

         throw ::exception(estatus);

      }


   }


   void wait_until_mutex_does_not_exist(::particle * pparticle, const char * lpszName)
   {

      ::pointer<mutex>pmutex = __new(mutex(pparticle, false, "Global\\::ca::account::ca2_spa::7807e510-5579-11dd-ae16-0800200c7784"));

      if (::get_last_status() == ::error_already_exists)
      {

         while (::get_last_status() == ::error_already_exists && ::task_get_run())
         {

            pmutex.release();

            preempt(200_ms);

            pmutex = __new(mutex(pparticle, false, "Global\\::ca::account::ca2_spa::7807e510-5579-11dd-ae16-0800200c7784"));

         }

      }

   }



} // namespace acme_windows_commons




