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
         key(HKEY hkey, const ::scoped_string & scopedstrSubKey, bool bCreate = false) { _open(hkey, pcszSubKey, bCreate); }
         virtual ~key();

      
         operator HKEY();

         void open(HKEY hkey, const ::scoped_string & scopedstrSubKey, bool bCreate = false);
         void _open(HKEY hkey, const ::scoped_string & scopedstrSubKey, bool bCreate = false);

         
         void defer_create(HKEY hkey, const ::scoped_string & scopedstrSubKey);
         inline void _defer_create(HKEY hkey, const ::scoped_string & scopedstrSubKey) { return _open(hkey, pcszSubKey, true); }


         void close();

         
         void value(void * pvalue, const ::scoped_string & scopedstrValueName, unsigned int & dwType, unsigned int & cbValue);
         void _value(void * pvalue, const ::scoped_string & scopedstrValueName, unsigned int& dwType, unsigned int& cbValue);

         void _set_value(const void* pvalue, const ::scoped_string & scopedstrValueName, unsigned int dwType, unsigned int cbValue);


         void value_type_and_size(const ::scoped_string & scopedstrValueName, unsigned int & dwType, unsigned int & cbValue);
         void _value_type_and_size(const ::scoped_string & scopedstrValueName, unsigned int& dwType, unsigned int& cbValue) { return _value(nullptr, pcszValueName, dwType, cbValue); }


         void get(const ::scoped_string & scopedstrValueName, unsigned int & dwValue);
         void _get(const ::scoped_string & scopedstrValueName, unsigned int & dwValue);

         
         void get(const ::scoped_string & scopedstrValueName, string & strValue);
         void _get(const ::scoped_string & scopedstrValueName, string &strValue);


         void get(const ::scoped_string & scopedstrValueName, memory & mem);
         void _get(const ::scoped_string & scopedstrValueName, memory & mem);


         inline ::payload get(const ::scoped_string & scopedstrValueName);


         void set(const ::scoped_string & scopedstrValueName, unsigned int dwValue);
         void _set(const ::scoped_string & scopedstrValueName, unsigned int dwValue);


         void set(const ::scoped_string & scopedstrValueName, const ::scoped_string & scopedstrValue);
         void _set(const ::scoped_string & scopedstrValueName, const ::scoped_string & scopedstrValue);


         void set(const ::scoped_string & scopedstrValueName, const ::scoped_string & scopedstrValue);
         void _set(const ::scoped_string & scopedstrValueName, const ::scoped_string & scopedstrValue);


         void set(const ::scoped_string & scopedstrValueName, const memory & mem);
         void _set(const ::scoped_string & scopedstrValueName, const memory & mem);


         void delete_value(const ::scoped_string & scopedstrValueName);
         void _delete_value(const ::scoped_string & scopedstrValueName);


         void delete_key();
         void _delete_key();

         
         void ls_key(string_array_base & stra);
         inline auto ls_key() { string_array_base stra; ls_key(stra); return stra; }
         void _ls_key(string_array_base & stra);


         void ls_value(string_array_base & stra);
         inline auto ls_value() { string_array_base stra; ls_value(stra); return stra; }
         void _ls_value(string_array_base & stra);


      };

      registry();
      virtual ~registry();

      


      
   };


} // namespace windows



