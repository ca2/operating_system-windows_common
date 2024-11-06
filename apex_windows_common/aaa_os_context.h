#pragma once


//CLASS_DECL_APEX_WINDOWS_COMMON HRESULT os_create_link(::file::path pathObj, ::file::path pathLink, string strDesc, ::file::path pathIcon = nullptr, int iIcon = 0);

namespace windows
{


   class CLASS_DECL_APEX_WINDOWS_COMMON os_context :
      virtual public ::os_context
   {
   public:


      os_context();
      virtual ~os_context();


      virtual string get_command_line() override;


      virtual bool reboot() override;
      virtual bool shutdown(bool bPowerOff) override;

      virtual void terminate_processes_by_title(const ::string & lpszName) override;
      //virtual ::file::path get_module_path(HMODULE hmodule) override;
      virtual bool get_pid_by_path(const ::string & lpszName, unsigned int & dwPid) override;
      virtual bool get_pid_by_title(const ::string & lpszName, unsigned int & dwPid) override;
      virtual void get_all_processes(unsigned_int_array & dwa) override;
      virtual ::file::path get_process_path(unsigned int dwPid) override;

      virtual int get_pid() override;


      virtual ::payload connection_settings_get_auto_detect() override;
      virtual ::payload connection_settings_get_auto_config_url() override;


      virtual bool local_machine_set_run(const ::string & pszKey, const ::string & pszCommand, bool bSet) override;
      virtual bool local_machine_set_run_once(const ::string & pszKey, const ::string & pszCommand, bool bSet) override;
      virtual bool current_user_set_run(const ::string & pszKey, const ::string & pszCommand, bool bSet) override;
      virtual bool current_user_set_run_once(const ::string & pszKey, const ::string & pszCommand, bool bSet) override;
      virtual bool defer_register_ca2_plugin_for_mozilla() override;

      virtual bool file_extension_get_open_with_list_keys(string_array & straKey, const ::string & pszExtension) override;
      virtual bool file_extension_get_open_with_list_commands(string_array & straCommand, const ::string & pszExtension) override;

      virtual bool file_association_set_default_icon(const ::string & pszExtension, const ::string & pszExtensionNamingClass, const ::string & pszIconPath) override;
      virtual bool file_association_set_shell_open_command(const ::string & pszExtension, const ::string & pszExtensionNamingClass, const ::string & pszCommand, const ::string & pszParam) override;
      virtual bool file_association_get_shell_open_command(const ::string & pszExtension, string & strExtensionNamingClass, string & strCommand, string & strParam) override;


      virtual bool open_in_ie(const ::string & pcsz);


      virtual bool file_open(::file::path path, string strParams = "", string strFolder = "") override;

      virtual bool browse_file_open(property_set & set) override;
      virtual bool browse_file_save(property_set & set) override;
      virtual bool browse_folder(property_set & set) override;
      virtual bool browse_file_or_folder(property_set & set) override;

      virtual void enable_service() override;
      virtual void disable_service() override;

      virtual void start_service() override;
      virtual void stop_service() override;

      virtual bool _getCredentialsForService(const string& strService, WCHAR* szUsername, WCHAR* szPassword);

      virtual bool enable_service(const ::string & strServiceName, const ::string & strDisplayName, const ::string & strCommand, const ::string & strUser = "", const ::string & strPass = "") override;
      virtual bool disable_service(const ::string & strServiceName) override;

      virtual bool start_service(const ::string & strServiceName) override;
      virtual bool stop_service(const ::string & strServiceName) override;

      virtual string calc_service_name();


      virtual bool resolve_link(::file::path & path, const ::string & strSource, string * pstrDirectory = nullptr, string * pstrParams = nullptr) override;

      virtual bool resolve_lnk_link(::file::path & path, const ::string & strSource, string * pstrDirectory = nullptr, string * pstrParams = nullptr);


      DECLSPEC_NO_RETURN void raise_exception(unsigned int dwExceptionCode, unsigned int dwExceptionFlags);

      virtual bool is_remote_session() override;

      virtual void set_file_status(const ::string & pszFileName, const ::file::file_status& status) override;



      virtual bool initialize_wallpaper_fileset(::file::set* pset, bool bAddSearch) override;

      virtual bool get_default_browser(string & strId, ::file::path & path, string & strParam) override;

      virtual bool register_user_auto_start(string strId, string strCommand, bool bRegister) override;

      virtual bool is_user_auto_start(string strId) override;

      virtual ::file::path get_app_path(const ::string & strApp) override;

      virtual bool set_default_browser() override;

      virtual bool add_default_program(string_array & straExtension, string_array & straMimeType) override;

      virtual void list_process(::file::path_array & patha, unsigned_int_array & uaPid) override;

      //virtual icon_pointer load_icon(const ::payload & payloadFile) override;


   };


} // namespace windows



