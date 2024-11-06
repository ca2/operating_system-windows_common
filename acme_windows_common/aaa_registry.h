#pragma once


namespace windows
{


   class CLASS_DECL_ACME_WINDOWS_COMMON registry :
      virtual public ::matter
   {
   public:


      class CLASS_DECL_ACME_WINDOWS_COMMON key
      {
      public:


         HKEY m_hkey;


         key();
         key(HKEY hkey, const ::string & pcszSubKey, bool bCreate = false) { _open(hkey, pcszSubKey, bCreate); }
         virtual ~key();

      
         operator HKEY();

         void open(HKEY hkey, const ::string & pcszSubKey, bool bCreate = false);
         void _open(HKEY hkey, const ::string & pcszSubKey, bool bCreate = false);

         
         void defer_create(HKEY hkey, const ::string & pcszSubKey);
         inline void _defer_create(HKEY hkey, const ::string & pcszSubKey) { return _open(hkey, pcszSubKey, true); }


         void close();

         
         void value(void * pvalue, const ::string & pcszValueName, unsigned int & dwType, unsigned int & cbValue);
         void _value(void * pvalue, const ::string & pcszValueName, unsigned int& dwType, unsigned int& cbValue);

         void _set_value(const void* pvalue, const ::string & pcszValueName, unsigned int dwType, unsigned int cbValue);


         void value_type_and_size(const ::string & pcszValueName, unsigned int & dwType, unsigned int & cbValue);
         void _value_type_and_size(const ::string & pcszValueName, unsigned int& dwType, unsigned int& cbValue) { return _value(nullptr, pcszValueName, dwType, cbValue); }


         void get(const ::string & pcszValueName, unsigned int & dwValue);
         void _get(const ::string & pcszValueName, unsigned int & dwValue);

         
         void get(const ::string & pcszValueName, string & strValue);
         void _get(const ::string & pcszValueName, string &strValue);


         void get(const ::string & pcszValueName, memory & mem);
         void _get(const ::string & pcszValueName, memory & mem);


         inline ::payload get(const ::string & pcszValueName);


         void set(const ::string & pcszValueName, unsigned int dwValue);
         void _set(const ::string & pcszValueName, unsigned int dwValue);


         void set(const ::string & pcszValueName, const ::string & strValue);
         void _set(const ::string & pcszValueName, const ::string & strValue);


         void set(const ::string & pcszValueName, const ::string & pszValue);
         void _set(const ::string & pcszValueName, const ::string & pszValue);


         void set(const ::string & pcszValueName, const memory & mem);
         void _set(const ::string & pcszValueName, const memory & mem);


         void delete_value(const ::string & pcszValueName);
         void _delete_value(const ::string & pcszValueName);


         void delete_key();
         void _delete_key();

         
         void ls_key(string_array & stra);
         inline auto ls_key() { string_array stra; ls_key(stra); return stra; }
         void _ls_key(string_array & stra);


         void ls_value(string_array & stra);
         inline auto ls_value() { string_array stra; ls_value(stra); return stra; }
         void _ls_value(string_array & stra);


      };

      registry();
      virtual ~registry();

      


      
   };


} // namespace windows



