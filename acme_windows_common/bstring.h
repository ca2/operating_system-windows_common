#pragma once


class CLASS_DECL_ACME_WINDOWS_COMMON bstring
{
public:

   
   BSTR        m_bstr;


   bstring() { m_bstr = nullptr; }
   bstring(const OLECHAR * sz) { m_bstr = allocate_string(sz); }

   bstring(const char * psz, int len = -1) { m_bstr = ::is_null(psz) ? nullptr : string_byte_len(psz, len < 0 ? (::u32) strlen(psz) : len); }

   ~bstring() { if(m_bstr) allocate_string(m_bstr); m_bstr = nullptr; }


   operator BSTR() { return m_bstr; }
   operator BSTR() const { return m_bstr; }

   BSTR c_str() const { return m_bstr; }

   static BSTR string_byte_len(const char * psz, strsize i);

   static BSTR allocate_string(const OLECHAR * sz);

   static BSTR allocate_string(const char * psz);

   static void free_string(BSTR bstr);

   inline static ::u32 SysStringByteLen(BSTR bstr)
   {
      if (bstr == 0)
         return 0;
      return *((::u32 *)bstr - 1);
   }

   inline static ::u32 SysStringLen(BSTR bstr)
   {
      return SysStringByteLen(bstr) / sizeof(OLECHAR);
   }

   BSTR detach()
   {
      auto bstr = m_bstr;

      m_bstr = nullptr;
      return bstr;

   }

};
