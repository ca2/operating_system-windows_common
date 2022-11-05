// Created by camilo on 2022-11-04 00:31 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "standard_exception.h"


namespace acme_windows_common
{


   standard_exception::standard_exception(const ::e_status & estatus, const ::string & strMessage, const ::string & strDetails, EXCEPTION_POINTERS * ppointers) :
      exception(estatus, strMessage, strDetails),
      m_ppointers(ppointers)
   {

   }


   standard_exception::~standard_exception()
   {

   }


} // namespace acme_windows_common



