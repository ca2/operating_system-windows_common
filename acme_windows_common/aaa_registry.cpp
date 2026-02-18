#include "framework.h"
#include "acme/operating_system.h"
#include "acme/node/windows/_windows.h"
#include "acme/const/atom.h"


namespace windows
{


   registry::registry()
   {

   }

   registry::~registry()
   {

   }

   
   ::payload registry::key::get(const ::scoped_string & scopedstrValueName)
   {

      unsigned int dwType;

      unsigned int cbValue;

      auto estatus = _value_type_and_size(pcszValueName, dwType, cbValue);

      if (::failed(estatus))
      {

         return estatus;

      }

      if (dwType == REG_DWORD)
      {

         unsigned int uValue;

         auto estatus = _value(&uValue, pcszValueName, dwType, cbValue);

         if(!estatus)
         {

            return estatus;

         }

         return uValue;

      }
      else if (dwType == REG_SZ)
      {

         wstring wstrValue;
       
         auto pwszValue = wstrValue.get_buffer(cbValue);

         auto estatus = _value(pwszValue, pcszValueName, dwType, cbValue);

         if(!estatus)
         {

            return estatus;

         }
         
         wstrValue.release_buffer();

         return string(wstrValue);

      }
      else if (dwType == REG_BINARY)
      {

         memory mem;

         mem.set_size(cbValue);

         auto estatus = _value(mem.get_data(), pcszValueName, dwType, cbValue);
         
         if (!estatus)
         {

            return estatus;

         }

         return mem;

      }

      return ::error_wrong_type;

   }


   registry::key::key()
   {

      m_hkey = nullptr;

   }


   registry::key::~key()
   {

      if(m_hkey != nullptr)
      {

         close();

      }

   }


   void registry::key::open(HKEY hkey, const ::scoped_string & scopedstrSubKey, bool bCreate)
   { 
      
      __defer_throw_estatus(_open(hkey, pcszSubKey, bCreate));
   
   }


   void registry::key::_open(HKEY hkey, const ::scoped_string & scopedstrSubKey, bool bCreate)
   {

      LSTATUS lstatus;

      if(bCreate)
      {

         lstatus = ::RegCreateKeyW(hkey, wstring(pcszSubKey), &m_hkey);

         if (lstatus != ERROR_SUCCESS)
         {

            return ::error_failed;

         }

      }
      else
      {

         lstatus = ::RegOpenKeyW(hkey, wstring(pcszSubKey), &m_hkey);
         
         if (lstatus != ERROR_SUCCESS)
         {

            return ::error_open_failed;

         }

      }

      return ::success;

   }


   void registry::key::value(void * pvalue, const ::scoped_string & scopedstrValueName, unsigned int & dwType, unsigned int & cbValue)
   { 
      
      auto estatus = _value(pvalue, pcszValueName, dwType, cbValue); 

      __defer_throw_estatus(estatus);
   
   }


   void registry::key::defer_create(HKEY hkey, const ::scoped_string & scopedstrSubKey) 
   { 
      
      __defer_throw_estatus(_defer_create(hkey, pcszSubKey));
   
   }

   
   void registry::key::close()
   {

      if(m_hkey != nullptr)
      {

         if(ERROR_SUCCESS == ::RegCloseKey(m_hkey))
         {

            m_hkey = nullptr;

         }

      }

   }



   void registry::key::_value(void* pvalue, const ::scoped_string & scopedstrValueName, unsigned int& dwType, unsigned int& cbValue)
   {

      if (ERROR_SUCCESS != ::RegQueryValueExW(m_hkey, wstring(pcszValueName), nullptr, (LPDWORD) &dwType, (unsigned char*)pvalue, (LPDWORD) &cbValue))
      {

         return error_failed;

      }

      return ::success;

   }


   void registry::key::_get(const ::scoped_string & scopedstrValueName, unsigned int & dwValue)
   {
      
      unsigned int dwType;

      unsigned int cbValue;
      
      auto estatus = _value_type_and_size(pcszValueName, dwType, cbValue);

      if (::failed(estatus))
      {

         return estatus;

      }

      if (dwType != REG_DWORD)
      {

         return error_wrong_type;

      }

      cbValue = sizeof(dwValue);

      estatus = _value(&dwValue, pcszValueName, dwType, cbValue);

      if (::failed(estatus))
      {

         return estatus;

      }

      return success;

   }


   void registry::key::_get(const ::scoped_string & scopedstrValueName, string &str)
   {

      unsigned int dwType;

      unsigned int cbValue;

      auto estatus = _value_type_and_size(pcszValueName, dwType, cbValue);

      if (::failed(estatus))
      {

         return estatus;

      }

      if (dwType != REG_SZ)
      {

         return error_wrong_type;

      }

      wstring wstr;

      auto pwsz = wstr.get_buffer(cbValue);

      estatus = _value(pwsz, pcszValueName, dwType, cbValue);

      wstr.release_buffer();

      str = wstr;

      if(!estatus)
      {
         
         return estatus;

      }

      return ::success;

   }


   void registry::key::_get(const ::scoped_string & scopedstrValueName, memory & mem)
   {

      unsigned int dwType;

      unsigned int cbValue;

      auto estatus = _value_type_and_size(pcszValueName, dwType, cbValue);

      if (::failed(estatus))
      {

         return estatus;

      }

      if (dwType != REG_BINARY)
      {

         return error_wrong_type;

      }

      mem.set_size(cbValue);

      estatus = _value(mem.get_data(), pcszValueName, dwType, cbValue);

      if (!estatus)
      {

         mem.truncate(0);

         return estatus;

      }      
      
      return ::success;

   }

   
   void registry::key::_set_value(const void* pvalue, const ::scoped_string & scopedstrValueName, unsigned int dwType, unsigned int cbValue)
   {

      auto lstatus = RegSetValueExW(m_hkey, wstring(pcszValueName), 0, dwType, (const unsigned char *) pvalue, cbValue);

      if (lstatus != ERROR_SUCCESS)
      {

         return ::error_failed;

      }

      return ::success;

   }


   void registry::key::value_type_and_size(const ::scoped_string & scopedstrValueName, unsigned int & dwType, unsigned int & cbValue)
   {

      auto estatus = _value_type_and_size(pcszValueName, dwType, cbValue);

      __defer_throw_estatus(estatus);

   }


   void registry::key::_set(const ::scoped_string & scopedstrValueName, const ::scoped_string & scopedstrValue)
   {

      wstring wstr(strValue);

      return _set_value(wstr.c_str(), pcszValueName, REG_SZ, (unsigned int) wstr.get_length_in_bytes_with_null_terminator());

   }


   void registry::key::_set(const ::scoped_string & scopedstrValueName, const ::scoped_string & scopedstrValue)
   {

      return _set(pcszValueName, string(pszValue));

   }


   void registry::key::_set(const ::scoped_string & scopedstrValueName, const memory & memValue)
   {

      return _set_value(memValue.get_data(), pcszValueName, REG_BINARY, (unsigned int) memValue.get_size());

   }


   void registry::key::_set(const ::scoped_string & scopedstrValueName, unsigned int dwValue)
   {

      return _set_value(&dwValue, pcszValueName, REG_DWORD, sizeof(dwValue));

   }


   void registry::key::get(const ::scoped_string & scopedstrValueName, unsigned int & dwValue)
   { 
      
      auto estatus = _get(pcszValueName, dwValue); 

      __defer_throw_estatus(estatus);
   
   }


   void registry::key::get(const ::scoped_string & scopedstrValueName, string & strValue)
   { 
      
      auto estatus = _get(pcszValueName, strValue); 

      __defer_throw_estatus(estatus);
   
   }


   void registry::key::get(const ::scoped_string & scopedstrValueName, memory & mem)
   { 
      
      auto estatus = _get(pcszValueName, mem); 

      __defer_throw_estatus(estatus);
   
   }


   void registry::key::set(const ::scoped_string & scopedstrValueName, unsigned int dwValue)
   {
      
      auto estatus = _set(pcszValueName, dwValue); 

      __defer_throw_estatus(estatus);

   
   }


   void registry::key::set(const ::scoped_string & scopedstrValueName, const ::scoped_string & scopedstrValue)
   { 

      auto estatus = _set(pcszValueName, strValue);
      
      __defer_throw_estatus(estatus);
   
   }


   void registry::key::set(const ::scoped_string & scopedstrValueName, const ::scoped_string & scopedstrValue)
   {

      auto estatus = _set(pcszValueName, pszValue);

      __defer_throw_estatus(estatus);

   }


   void registry::key::set(const ::scoped_string & scopedstrValueName, const memory & mem)
   { 

      auto estatus = _set(pcszValueName, mem);
      
      __defer_throw_estatus(estatus);
   
   }


   void registry::key::delete_value(const ::scoped_string & scopedstrValueName)
   { 
      
      auto estatus = _delete_value(pcszValueName); 

      __defer_throw_estatus(estatus);
   
   }


   void registry::key::delete_key()
   { 

      auto estatus = _delete_key();
      
      __defer_throw_estatus(estatus);
   
   }


   void registry::key::ls_key(string_array_base & stra)
   { 

      auto estatus = _ls_key(stra);
      
      __defer_throw_estatus(estatus);
   
   }


   void registry::key::ls_value(string_array_base & stra)
   {

      auto estatus = _ls_value(stra);

      __defer_throw_estatus(estatus);

   }


   void registry::key::_delete_value(const ::scoped_string & scopedstrValueName)
   {

      wstring wstr(pcszValueName);

      if (ERROR_SUCCESS != ::RegDeleteValueW(m_hkey, (WCHAR *) wstr.c_str()))
      {

         return error_failed;

      }

      return success;

   }


   void registry::key::_delete_key()
   {

      ASSERT(false);
      // please verify if
      // using nullptr for the value parameter
      // deletes the key.
      if (ERROR_SUCCESS != ::RegDeleteKey(m_hkey, nullptr))
      {

         return error_failed;

      }

      return ::success;

   }


   void registry::key::_ls_key(string_array_base & stra)
   {

      DWORD dwMaxSubKeyLen;

      RegQueryInfoKey(
      m_hkey,
      nullptr,
      nullptr,
      nullptr,
      nullptr,
      &dwMaxSubKeyLen,
      nullptr,
      nullptr,
      nullptr,
      nullptr,
      nullptr,
      nullptr);
      int iSize = maximum(dwMaxSubKeyLen, 1024u);
      wchar_t *buf = (wchar_t *) malloc(iSize * 2);
      int iKey = 0;
      while(::RegEnumKeyW(m_hkey, iKey, buf, iSize) == ERROR_SUCCESS)
      {
         stra.add(string(buf));
         iKey++;
      }
      free(buf);
      return iKey;
   }


   registry::key::operator HKEY()
   {
      return m_hkey;
   }

   void registry::key::_ls_value(string_array_base & stra)
   {
      
      unsigned int dwMaxValueNameLen = 16384;

      wstring hwstr;
      
      auto pwsz=hwstr.get_buffer(dwMaxValueNameLen * 2);

      int l;

      DWORD dwIndex = 0;

      DWORD dwLen = dwMaxValueNameLen;

      while(ERROR_SUCCESS == (l = RegEnumValueW(
                                  m_hkey,
                                  dwIndex,
         pwsz,
                                  &dwLen,
                                  nullptr,
                                  nullptr,
                                  nullptr,
                                  nullptr)))
      {
         
         stra.add(string(pwsz, dwLen));

         dwIndex++;

         dwLen = dwMaxValueNameLen;

      }
      
      return ::success;

   }


} // namespace windows


