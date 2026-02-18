#pragma once

#if defined(LINUX) || defined(APPLEOS)
#include <sys/types.h>
#include <sys/sem.h>
#endif



class CLASS_DECL_ACME happening :
   public synchronization_object
{
public:

#if defined(LINUX) || defined(APPLEOS) || defined(ANDROID)

   /// Private Mutexes
   bool              m_bManualEvent;
   bool              m_bSignaled;  // meaningful only when m_bManualEvent
   int               m_iSignalId;  // meaningful only when m_bManualEvent
   void *            m_pcond; // pthread_cond_t
   void *            m_mutex; // pthread_mutex_t;


#endif


#if defined(LINUX) || defined(APPLEOS)

   /// Named Mutexes
   int               m_sem;

#endif


   happening(char * sz = nullptr,bool bInitiallyOwn = false, bool bManualReset = false, const ::scoped_string & scopedstrNAme = nullptr, sync_options * psyncoptions = nullptr);

   virtual ~happening();

   // using event_base::lock;
   virtual bool lock(const duration & durationTimeout = duration::infinite());

   using synchronization_object::unlock;
   virtual bool unlock();

   //virtual HSYNC hsync() const;

   bool set_happening();
   bool set_happening() {return set_happening(); }
   //bool pulse_happening();
   bool reset_happening();

   /**
   * \brief	Session independent waitables (windows version)
   */

   ///  \brief		waits for an happening forever
   virtual synchronization_result wait ();

   ///  \brief		waits for an happening for a specified time
   ///  \lparam		duration time period to wait for an happening
   ///  \return	waiting action result as WaitResult
   virtual synchronization_result wait (const duration & duration);

   virtual bool is_signaled() const;


};



