// created by Camilo 2021-01-31 05:26 BRT <3CamiloSasukeThomasBorregaardSoerensen
#pragma once


namespace windows
{


   class CLASS_DECL_APEX_WINDOWS_COMMON interprocess_communication_base :
      virtual public inteprocess_channel::base
   {
   public:


      HWND              m_hwnd;
      string            m_strBaseChannel;


      interprocess_communication_base();
      virtual ~interprocess_communication_base();


      HWND get_hwnd() const { return (HWND) m_hwnd; }
      void set_hwnd(HWND hwnd) { m_hwnd = hwnd; }

   };


   class CLASS_DECL_APEX_WINDOWS_COMMON interprocess_caller :
      virtual public interprocess_communication_base,
      virtual public inteprocess_channel::caller
   {
   public:


      interprocess_caller();
      virtual ~interprocess_caller();



#if defined(UNIVERSAL_WINDOWS)
      bool open(const ::scoped_string & scopedstrChannel);
#else
      bool open(const ::scoped_string & scopedstrChannel, launcher * plauncher = nullptr);
#endif
      bool close();


      bool send(const ::scoped_string & scopedstrMessage, duration durationTimeout);
      bool send(int message, void * pdata, int len, duration durationTimeout);


      bool is_tx_ok();

   };


   class rx_private;


   class CLASS_DECL_APEX_WINDOWS_COMMON interprocess_handler :
      virtual public interprocess_communication_base,
      virtual public inteprocess_channel::handler
   {
   public:


      interprocess_handler();
      virtual ~interprocess_handler();


      bool create(const ::scoped_string & scopedstrChannel);
      bool destroy();


      virtual void * on_interprocess_receive(::inteprocess::handler * prx, const ::scoped_string & scopedstrMessage);
      virtual void * on_interprocess_receive(::inteprocess::handler * prx, int message, void * pdata, memsize len);
      virtual void * on_interprocess_post(::inteprocess::handler * prx, long long a, long long b);


      virtual bool on_idle();


      LRESULT message_queue_proc(UINT message, WPARAM wparam, LPARAM lparam);


      bool is_rx_ok();


   };


//   class CLASS_DECL_APEX_WINDOWS_COMMON inteprocess_channel :
//      virtual public interprocess_communication_base,
//      virtual public ::inteprocess_channel::inteprocess_channel
//   {
//   public:
//
//
//      inteprocess_channel();
//      virtual ~inteprocess_channel();
//
//
//#if defined(UNIVERSAL_WINDOWS)
//      bool open_ab(const ::scoped_string & scopedstrChannel, const ::scoped_string & scopedstrModule);
//      bool open_ba(const ::scoped_string & scopedstrChannel, const ::scoped_string & scopedstrModule);
//#elif defined(WINDOWS)
//      bool open_ab(const ::scoped_string & scopedstrChannel, const ::scoped_string & scopedstrModule, launcher * plauncher = nullptr);
//      bool open_ba(const ::scoped_string & scopedstrChannel, const ::scoped_string & scopedstrModule, launcher * plauncher = nullptr);
//#else
//      bool open_ab(const ::scoped_string & scopedstrChannel, launcher * plauncher = nullptr);
//      bool open_ba(const ::scoped_string & scopedstrChannel, launcher * plauncher = nullptr);
//#endif
//
//
//      bool close();
//
//
//      virtual void restart_apex_ipc();
//
//      //bool ensure_tx(const ::scoped_string & scopedstrMessage, duration durationTimeout = one_hour());
//      //bool ensure_tx(int message, void * pdata, int len, duration durationTimeout = one_hour());
//
//
//      bool is_rx_tx_ok();
//
//
//   };


   CLASS_DECL_APEX_WINDOWS_COMMON string app_install(string strPlatform = "");


} // namespace windows



