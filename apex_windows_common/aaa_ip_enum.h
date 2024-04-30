#pragma once

/*
Module : IPENUM.H
Purpose: Interface for an apex API class wrapper for IP address enumeration
Created: PJN / 21-04-1998
History: None

Copyright (ca) 1998 by PJ Naughter.


*/


namespace windows
{


   class CLASS_DECL_APEX_WINDOWS_COMMON ip_enum :
      virtual public ::net::ip_enum
   {
   public:


      bool     m_bWinsockInitialized;


      ip_enum();
      virtual ~ip_enum();


      //      virtual ::raw::count enumerate(ip_array & ipa);
      virtual ::raw::count enumerate(array < ::networking::address > & ipa) override;

   };


} // namespace windows



