#pragma once


namespace windows
{


   class CLASS_DECL_APEX_WINDOWS_COMMON directory_context :
      virtual public ::directory_context
   {
   public:


      ::pointer<file_system>     m_pfilesystem;
      ::pointer<directory_system>      m_pdirsystem;


      directory_context();
      virtual ~directory_context();


      virtual void initialize(::particle * pparticle) override;

      virtual void init_system() override;

      using ::directory_context::ls;
      // rls fetchs should set a meaningful m_iRelative value at each returned path
      virtual ::file::listing & ls(::file::listing & path);
      virtual ::file::listing & ls_relative_name(::file::listing & path);


      virtual bool  is_impl(const ::file::path & path) override;
      virtual bool  is_inside(const ::file::path & pathFolder, const ::file::path & path);
      virtual bool  is_inside_time(const ::file::path & path);
      virtual bool  name_is(const ::file::path & path);
      virtual bool  has_subdir(const ::file::path & path);

      virtual ::file::listing & root_ones(::file::listing & listing);
      virtual bool mk(const ::file::path & path);
      virtual bool rm(const ::file::path & path, bool bRecursive = true);


      virtual ::file::path name(const ::file::path & path);

      virtual ::file::path time();
      virtual ::file::path stage();
      virtual ::file::path stageapp();
      virtual ::file::path netseed();
      //virtual ::file::path matter();

      virtual ::file::path module();
      virtual ::file::path ca2module();
      virtual ::file::path time_square(const ::string & strPrefix = nullptr, const ::string & strSuffix = nullptr);
      virtual ::file::path time_log();


      virtual ::file::path trash_that_is_not_trash(const ::file::path & path);



      virtual ::file::path appdata();
      virtual ::file::path commonappdata_root();

      //virtual ::file::path usersystemappdata(const ::string & pcszPrefix);

      //virtual ::file::path userappdata();
      //virtual ::file::path userdata();
      //virtual ::file::path userfolder();
      //virtual ::file::path default_os_user_path_prefix();
      //virtual ::file::path default_userappdata(const string & pcszPrefix,const string & lpcszLogin );

      //virtual ::file::path default_userdata(const string & pcszPrefix,const string & lpcszLogin);

      //virtual ::file::path default_userfolder(const string & pcszPrefix,const string & lpcszLogin);

      virtual ::file::path userquicklaunch();
      virtual ::file::path userprograms();

      virtual ::file::path commonprograms();




      virtual ::file::path document() override;
      virtual ::file::path desktop() override;
      virtual ::file::path download() override;


      virtual ::file::path music() override;
      virtual ::file::path video() override;
      virtual ::file::path image() override;


      virtual ::file::path onedrive() override;


   };


} // namespace windows




