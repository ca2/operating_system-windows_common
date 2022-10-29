// From acme/platform/exclusive.h by camilo on 2022-10-28 15:25 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "exclusive.h"
#include "acme/operating_system.h"
#include "acme/filesystem/filesystem/acme_directory.h"
#include "acme/platform/node.h"
#include "acme/platform/system.h"
#include "acme/primitive/primitive/object.h"


namespace acme_windows_common
{


   //#ifdef WINDOWS
   //

   exclusive::exclusive(::particle * pparticle, string strId ARG_SEC_ATTRS)
   {

      m_strId = strId;

      m_bResourceException = false;

      try
      {

         m_pmutex = pparticle->acmenode()->create_global_named_mutex(pparticle, false, strId);

         //m_pmutex = //__new(::pointer < ::mutex >(pparticle, false, strId ADD_PARAM_SEC_ATTRS));

         //m_dwLastError = ::GetLastError();

      }
      catch (const ::exception &)
      {

         //m_bResourceException = true;

         try
         {

            m_pmutex = __new(::pointer < ::mutex >(pparticle, false, strId));

            m_dwLastError = ::GetLastError();

         }
         catch (const ::exception &)
         {

            m_bResourceException = true;

         }

      }

   }


   //#else
   //
   //
   //   exclusive::exclusive(::particle * pparticle, string strId ARG_SEC_ATTRS)
   //   {
   //
   //      initialize(pparticle);
   //
   //      m_strId = strId;
   //
   //      ::file::path path;
   //
   //      if(strId.begins_ci("Local\\"))
   //      {
   //
   //         path = pparticle->acmedirectory()->home() / ".config/acme";
   //
   //      }
   //      else
   //      {
   //
   //         path = "/var/tmp/acme";
   //
   //      }
   //
   //      path /= strId;
   //
   //      acmedirectory()->create(path.folder());
   //
   //      m_iFile = open(path, O_WRONLY | O_CREAT, 0777);
   //
   //      m_iLock = lockf(m_iFile, F_TLOCK, 0);
   //
   //   }
   //
   //
   //#endif
   //

   exclusive::~exclusive()
   {

   }


   bool exclusive::exclusive_fails() const
   {

      return m_bResourceException || m_pmutex->already_exists();

      //#ifdef WINDOWS
      //
      //      return m_dwLastError == ERROR_ALREADY_EXISTS || m_bResourceException;
      //
      //#else
      //
      //      return m_iLock < 0;
      //
      //#endif

   }


} // namespace acme_windows_common



