// From acme/platform/exclusive.h by camilo on 2022-10-28 15:25 <3ThomasBorregaardSorensen!!
#pragma once


#include "acme/platform/exclusive.h"


namespace acme_windows_common
{


   class CLASS_DECL_ACME exclusive :
      virtual public ::acme::exclusive
   {
   public:


      //string                  m_strId;

      // #ifdef WINDOWS

             ::u32                   m_dwLastError;
             bool                    m_bResourceException;

      // #else

      //       int                     m_iFile;
      //       int                     m_iLock;

      // #endif

             ::pointer < ::mutex >     m_pmutex;


      exclusive(::particle * pparticle, string str ARG_SEC_ATTRS_DEF);
      ~exclusive() override;


      bool exclusive_fails() const override;


   };


} // namespace acme



