#pragma once


namespace windows
{


   class CLASS_DECL_APEX_WINDOWS_COMMON process :
      virtual public ::operating_system::process
   {
   public:


      PROCESS_INFORMATION        m_pi;
      STARTUPINFOW               m_si;


      process();
      virtual ~process();


      virtual bool create_child_process(const ::scoped_string & scopedstrCmdLine, bool bPiped, const ::scoped_string & scopedstrDir = nullptr, ::enum_priority epriority = ::e_priority_none) override;

      virtual bool has_exited() override;

      virtual bool synch_elevated(const ::scoped_string & scopedstrCmdLine,int iShow,const ::duration & durationTimeOut,bool * pbTimeOut) override;

      virtual bool kill() override;

   };


} // namespace windows
