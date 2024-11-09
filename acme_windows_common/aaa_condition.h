#pragma once


namespace windows
{


   class CLASS_DECL_ACME condition :
      virtual public ::condition
   {
   public:

#if defined(LINUX) || defined(APPLEOS) || defined(SOLARIS)

      iptr           m_hsync;
      bool              m_bManualEvent;
      bool              m_bSignaled;  // meaningful only when m_bManualEvent
      int           m_iSignalId;  // meaningful only when m_bManualEvent

#elif defined(ANDROID)

      bool              m_bSignaled;
      int               m_iHold;
      pthread_mutex_t   m_mutex;
      pthread_cond_t    m_cond;

#else

      CRITICAL_SECTION     m_sect;
      CONDITION_VARIABLE   m_var;

#endif


      condition();
      virtual ~condition();


      using synchronization_object::lock;
      virtual bool lock(const duration & durationTimeout = duration::infinite());

      using synchronization_object::unlock;
      virtual bool unlock();

      virtual void * get_os_data() const;

      bool pulse();
      bool set_happening();
      bool set_happening() { return set_happening(); }
      //bool pulse_happening();
      bool reset_happening();


      /**
      * \brief	Session independent waitables (windows version)
      */

      ///  \brief		waits for an happening forever
      virtual synchronization_result wait();

      ///  \brief		waits for an happening for a specified time
      ///  \lparam		duration time period to wait for an happening
      ///  \return	waiting action result as WaitResult
      virtual synchronization_result wait(const duration & duration);

      bool is_signaled() const;

   };


} // namespace windows



