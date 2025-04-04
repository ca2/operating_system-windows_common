#include "framework.h"


#if defined(LINUX) || defined(__APPLE__) || defined(ANDROID)
#include <sys/ipc.h>
#include "acme/os/ansios/_pthread.h"
#include <sys/time.h>
#include <time.h>
#include <sys/time.h>

#if defined(LINUX) || defined(__APPLE__)
#include <sys/sem.h>
#endif
#include "acme/os/ansios/_ansios.h"

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

void clock_getrealtime(struct timespec * pts)
{

#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time

   clock_serv_t cclock;
   mach_timespec_t mts;
   host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
   clock_get_time(cclock, &mts);
   mach_port_deallocate(mach_task_self(), cclock);
   pts->tv_sec = mts.tv_sec;
   pts->tv_nsec = mts.tv_nsec;

#else

   clock_gettime(CLOCK_REALTIME, pts);

#endif

}

#endif


happening::happening(char * sz, bool bInitiallyOwn, bool bManualReset, const ::string & pstrName,sync_options * psyncoptions)
{

#ifdef WINDOWS_DESKTOP

   m_hsync = ::CreateEventW(psaAttribute, bManualReset, bInitiallyOwn, wstring(pstrName));


   if (m_hsync == NULL)
   {

      throw ::exception(error_resource);

   }

#elif defined(UNIVERSAL_WINDOWS)

   unsigned int dwFlags = 0;

   if(bInitiallyOwn)
   {

      dwFlags |= CREATE_EVENT_INITIAL_SET;

   }

   if(bManualReset)
   {

      dwFlags |= CREATE_EVENT_MANUAL_RESET;

   }

   m_hsync = (HSYNC) ::CreateEventEx(psaAttribute, utf8_to_unicode(pstrName), dwFlags, DELETE | EVENT_MODIFY_STATE | SYNCHRONIZE);

   if (m_hsync == nullptr)
   {

      throw ::exception(error_resource);

   }

#elif defined(ANDROID)

   m_pcond = ___new pthread_cond_t;

   pthread_cond_init((pthread_cond_t *) m_pcond, nullptr);

   m_bManualEvent = bManualReset;

   if(m_bManualEvent)
   {

      m_bSignaled = bInitiallyOwn;
      pthread_mutexattr_t  attr;
      pthread_mutexattr_init(&attr);
      pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
      int rc;
      m_mutex = ___new pthread_mutex_t;
      if((rc = pthread_mutex_init((pthread_mutex_t *) m_mutex,&attr)))
      {
         throw ::exception(::exception("RC_OBJECT_NOT_CREATED"));
      }


   }
   else
   {
      m_mutex = nullptr;
   }

#else

   m_bManualEvent    = bManualReset;

   if(bManualReset)
   {

      m_mutex = ___new pthread_mutex_t;

      m_pcond = ___new pthread_cond_t;

      pthread_mutex_init((pthread_mutex_t *) m_mutex, 0);

      pthread_cond_init((pthread_cond_t *) m_pcond, 0);

      m_bSignaled = bInitiallyOwn;

   }
   else
   {

      m_mutex = nullptr;

      m_pcond = nullptr;

      if(pstrName != nullptr && *pstrName != '\0')
      {

         string strPath = "/::payload/tmp/ca2/ftok/happening/" + string(pstrName);

         m_sem = semget(ftok(strPath, 0), 1, 0666 | IPC_CREAT);

      }
      else
      {

         m_sem = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);

      }

      semun semctl_arg;

      if(bInitiallyOwn)
      {

         semctl_arg.val = 1;

      }
      else
      {

         semctl_arg.val = 0;

      }

      semctl((int) m_sem, 0, SETVAL, semctl_arg);

   }


#endif


}


happening::~happening()
{

#if defined(LINUX)

   semun ignored_argument;

   semctl(m_sem, 0, IPC_RMID, ignored_argument);

   ::acme::del((pthread_mutex_t * &)m_mutex);

   ::acme::del((pthread_cond_t * &)m_pcond);

#elif defined(ANDROID)

   if(m_pcond != nullptr)
   {
      delete (pthread_cond_t *)m_pcond;
   }
   if(m_mutex != nullptr)
   {
      delete (pthread_mutex_t *)m_mutex;
   }

#endif

}



bool happening::set_happening()
{

#ifdef WINDOWS

   if (m_hsync == nullptr)
   {

      ASSERT(false);

      return false;

   }

   try
   {

      return ::SetEvent((HANDLE)m_hsync) != false;

   }
   catch(...)
   {
   }

   return false;

#elif defined(ANDROID)

   pthread_mutex_lock((pthread_mutex_t *) m_mutex);

   if(m_bManualEvent)
   {

      m_bSignaled = true;

      m_iSignalId++;

      pthread_cond_broadcast((pthread_cond_t *) m_pcond);

   }
   else
   {

      pthread_cond_signal((pthread_cond_t *) m_pcond);

   }

   pthread_mutex_unlock((pthread_mutex_t *) m_mutex);

   return true;

#else

   if(m_bManualEvent)
   {

      pthread_mutex_lock((pthread_mutex_t *) m_mutex);

      m_bSignaled = true;

      m_iSignalId++;

      pthread_cond_broadcast((pthread_cond_t *) m_pcond);

      pthread_mutex_unlock((pthread_mutex_t *) m_mutex);

   }
   else
   {

      sembuf sb;

      sb.sem_op   = 1;
      sb.sem_num  = 0;
      sb.sem_flg  = m_bManualEvent ? 0 : SEM_UNDO;

      return semop((int) m_sem, &sb, 1) == 0;

   }

   return true;

#endif

}


//bool happening::pulse_happening()
//{
//
//#ifdef WINDOWS_DESKTOP
//
//
//   ASSERT(m_hsync != nullptr);
//
//   return ::pulse_happening(m_hsync) != false;
//
//#else
//
//   sembuf sb;
//
//   sb.sem_op   = 1;
//   sb.sem_num  = 0;
//   sb.sem_flg  = SEM_UNDO;
//
//   return semop((int) m_hsync, &sb, 1) == 0;
//
//#endif
//
//}



bool happening::reset_happening()
{

#ifdef WINDOWS

   try
   {

      if(m_hsync == NULL)
      {

         ::output_debug_string(L"error reset happening (1)");

         return false;

      }

      return ::ResetEvent((HANDLE)m_hsync) != false;

   }
   catch(...)
   {

      ::output_debug_string(L"error reset happening (2)");

   }

   return false;

#else

   if(m_bManualEvent)
   {

      pthread_mutex_lock((pthread_mutex_t *) m_mutex);

      m_bSignaled = false;

      pthread_mutex_unlock((pthread_mutex_t *) m_mutex);

   }
   else
   {

      throw ::exception(::exception("It does not make sense to Reset a Event that is Automatic. It can be only Pulsed/Broadcasted."));

   }

   return true;

#endif

}


synchronization_result happening::wait ()
{

   //throw ::exception(todo("thread"));
   //if(m_eobject & e_object_alertable_wait)
   //{

   //   auto pthread = ::get_task();

   //   if(pthread)
   //   {

   //      pthread->add_waiting_event(this);

   //   }

   //}

#ifdef WINDOWS

   while (true)
   {

      int iResult = ::WaitForSingleObjectEx(hsync(), 300, false);

      if(iResult == WAIT_OBJECT_0)
      {

         return synchronization_result(e_synchronization_result_signaled_base);

      }
      else if (iResult == WAIT_TIMEOUT)
      {

         if (!task_get_run())
         {

            return synchronization_result(synchronization_result::result_abandon0);

         }

      }
      else
      {

         return synchronization_result(e_synchronization_result_error);

      }

   }

#elif defined(ANDROID)

   pthread_mutex_lock((pthread_mutex_t *) m_mutex);

   if(m_bManualEvent)
   {

      int iSignal = m_iSignalId;

      while(!m_bSignaled && iSignal == m_iSignalId)
      {

         pthread_cond_wait((pthread_cond_t *) m_pcond, (pthread_mutex_t *) m_mutex);

      }

   }
   else
   {

      pthread_cond_wait((pthread_cond_t *) m_pcond, (pthread_mutex_t *) m_mutex);

   }

   pthread_mutex_unlock((pthread_mutex_t *) m_mutex);

#else

   if(m_bManualEvent)
   {

      pthread_mutex_lock((pthread_mutex_t *) m_mutex);

      int iSignal = m_iSignalId;

      while(!m_bSignaled && iSignal == m_iSignalId)
      {

         pthread_cond_wait((pthread_cond_t *) m_pcond, (pthread_mutex_t *) m_mutex);

      }

      pthread_mutex_unlock((pthread_mutex_t *) m_mutex);

   }
   else
   {

      sembuf sb;

      sb.sem_op   = -1;
      sb.sem_num  = 0;
      sb.sem_flg  = 0;

      semop((int) m_sem, &sb, 1);

   }

#endif

   //throw ::exception(todo("thread"));
   //if(m_eobject & e_object_alertable_wait)
   //{

   //   auto pthread = ::get_task();

   //   if(pthread)
   //   {

   //      pthread->add_waiting_event(this);

   //   }

   //}

   return synchronization_result(e_synchronization_result_signaled_base);

}

///  \brief		waits for an happening for a specified time
///  \lparam		duration time period to wait for an happening
///  \return	waiting action result as WaitResult
synchronization_result happening::wait (const duration & durationTimeout)
{

   synchronization_result result;

   //throw ::exception(todo("thread"));
   //if(durationTimeout > 1_s && m_eobject & e_object_alertable_wait)
   //{

   //   auto pthread = ::get_task();

   //   if(pthread)
   //   {

   //      pthread->add_waiting_event(this);

   //   }

   //}


#ifdef WINDOWS

   auto osduration = durationTimeout.u32_millis();

   result = synchronization_result((unsigned int) ::WaitForSingleObjectEx(hsync(), osduration,false));

#elif defined(ANDROID)

   pthread_mutex_lock((pthread_mutex_t *) m_mutex);

   timespec end;
   clock_gettime(CLOCK_REALTIME, &end);

   ((duration & ) durationTimeout).normalize();

   end.tv_sec +=durationTimeout.m_secs.m_i;
   end.tv_nsec +=durationTimeout.m_nanos.m_i;

   end.tv_sec += end.tv_nsec / (1000 * 1000 * 1000);
   end.tv_nsec %= 1000 * 1000 * 1000;

   if(m_bManualEvent)
   {

      int iSignal = m_iSignalId;

      while(!m_bSignaled && iSignal == m_iSignalId)
      {

         if(pthread_cond_timedwait((pthread_cond_t *) m_pcond, (pthread_mutex_t *) m_mutex, &end))
            break;

      }

   }
   else
   {

      timespec delay;
      delay.tv_sec = durationTimeout.m_secs.m_i;
      delay.tv_nsec = durationTimeout.m_nanos.m_i;
      pthread_cond_timedwait((pthread_cond_t *) m_pcond, (pthread_mutex_t *) m_mutex, &delay);

   }

   pthread_mutex_unlock((pthread_mutex_t *) m_mutex);

   result =  m_bSignaled ? synchronization_result(e_synchronization_result_signaled_base) : synchronization_result(e_synchronization_result_timed_out);


#else

   if(m_bManualEvent)
   {

      if(durationTimeout.is_pos_infinity())
      {

         pthread_mutex_lock((pthread_mutex_t *) m_mutex);

         int iSignal = m_iSignalId;

         //clock_gettime(CLOCK_REALTIME, &abstime);

         while(!m_bSignaled && iSignal == m_iSignalId)
         {

            int error = pthread_cond_wait((pthread_cond_t *) m_pcond, (pthread_mutex_t *) m_mutex);

            if(error != 0)
            {

               break;

            }

         }

         pthread_mutex_unlock((pthread_mutex_t *) m_mutex);

         if(m_bSignaled)
            result = synchronization_result(e_synchronization_result_signaled_base);
         else
            result = synchronization_result(e_synchronization_result_error);

      }
      else
      {
         timespec abstime;

         ((duration & ) durationTimeout).normalize();

         pthread_mutex_lock((pthread_mutex_t *) m_mutex);

         int iSignal = m_iSignalId;

         //clock_gettime(CLOCK_REALTIME, &abstime);

         clock_getrealtime(&abstime);

         abstime.tv_sec += durationTimeout.m_secs.m_i;

         abstime.tv_nsec += durationTimeout.m_nanos.m_i;

         while(abstime.tv_nsec > 1000 * 1000 * 1000)
         {

            abstime.tv_nsec -= 1000 * 1000 * 1000;

            abstime.tv_sec++;

         }

         while(!m_bSignaled && iSignal == m_iSignalId)
         {

            int error = pthread_cond_timedwait((pthread_cond_t *) m_pcond, (pthread_mutex_t *) m_mutex, &abstime);

            if(error == EBUSY || error == ETIMEDOUT)
            {

               pthread_mutex_unlock((pthread_mutex_t *) m_mutex);

               return synchronization_result(e_synchronization_result_timed_out);

            }

         }

         pthread_mutex_unlock((pthread_mutex_t *) m_mutex);

         if(m_bSignaled)
            result = synchronization_result(e_synchronization_result_signaled_base);
         else
            result = synchronization_result(e_synchronization_result_error);

      }

   }
   else
   {

      timespec delay;

      delay.tv_sec = 0;

      delay.tv_nsec = 1000 * 1000;

      unsigned int timeout = durationTimeout.u32_millis();

      auto start = ::duration::now();

      while(durationTimeout.is_pos_infinity() || start.elapsed() < timeout)
      {


         sembuf sb;

         sb.sem_op   = -1;
         sb.sem_num  = 0;
         sb.sem_flg  = IPC_NOWAIT;

         int ret = semop((int) m_sem, &sb, 1);

         if(ret < 0)
         {
            if(ret == EPERM || ret == EBUSY)
            {
               nanosleep(&delay, nullptr);
            }
            else
            {
               return synchronization_result(e_synchronization_result_error);
            }
         }
         else
         {
            return synchronization_result(e_synchronization_result_signaled_base);
         }

      }

      result= synchronization_result(e_synchronization_result_timed_out);

   }


#endif

   ////throw ::exception(todo("thread"));
   //if(durationTimeout > 1_s && m_eobject & e_object_alertable_wait)
   //{

   //   auto pthread = ::get_task();

   //   if(pthread)
   //   {

   //      pthread->erase_waiting_event(this);

   //   }

   //}

   return result;

}


//*****************************************************************************
//
//      Class:          manual_reset_happening
//      Author:         Kenny Kerr
//      Date created:   10 April 2004
//      Description:    Notifies one or more waiting threads that an happening has
//                      occurred.
//
//*****************************************************************************
//*****************************************************************************
//
//      Name:           Signaled
//      Description:    Determines whether the happening is currently signaled.
//
//*****************************************************************************
bool happening::is_signaled() const
{

#ifdef WINDOWS

   return WAIT_OBJECT_0 == ::WaitForSingleObjectEx((HANDLE)m_hsync,0,false);

#elif defined(ANDROID)

   if(m_bManualEvent)
   {

      return m_bSignaled;

   }
   else
   {

      return ((happening *) this)->wait(::duration(0)).signaled();

   }


#else

   if(m_bManualEvent)
   {

      return m_bSignaled;

   }
   else
   {

      sembuf sb;

      sb.sem_op   = -1;
      sb.sem_num  = 0;
      sb.sem_flg  = IPC_NOWAIT;

      int ret = semop((int) m_sem, &sb, 1);

      if(ret < 0)
      {
         if(ret == EPERM)
         {
            return true;
         }
         else
         {
            return false;
         }
      }
      else
      {
         return false;
      }

   }

#endif

}
//end**************************************************************************
//
//      Class:          manual_reset_happening
//      Author:         Kenny Kerr
//      Date created:   10 April 2004
//      Description:    Notifies one or more waiting threads that an happening has
//                      occurred.
//
//end**************************************************************************

bool happening::lock(const duration & durationTimeout)
{

   return wait(durationTimeout).succeeded();

//#ifdef WINDOWS
//
//   unsigned int dwRet = ::WaitForSingleObjectEx((HANDLE)m_hsync,durationTimeout.u32_millis(),false);
//
//   if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED)
//      return true;
//   else
//      return false;
//
//#elif defined(ANDROID)
//
//   pthread_mutex_lock((pthread_mutex_t *) m_mutex);
//
//   ((duration & ) durationTimeout).normalize();
//
//
//   if(m_bManualEvent)
//   {
//
//      int iSignal = m_iSignalId;
//
//      while(!m_bSignaled && iSignal == m_iSignalId)
//      {
//
//         timespec delay;
//         delay.tv_sec = durationTimeout.m_i;
//         delay.tv_nsec = durationTimeout.m_i;
//         if(pthread_cond_timedwait((pthread_cond_t *) m_pcond, (pthread_mutex_t *) m_mutex, &delay))
//            break;
//
//      }
//
//      return m_bSignaled;
//
//   }
//   else
//   {
//
//      timespec delay;
//      delay.tv_sec = durationTimeout.m_i;
//      delay.tv_nsec = durationTimeout.m_i;
//      pthread_cond_timedwait((pthread_cond_t *) m_pcond, (pthread_mutex_t *) m_mutex, &delay);
//
//      return is_locked();
//
//   }
//
//   pthread_mutex_unlock((pthread_mutex_t *) m_mutex);
//
//#else
//
//
//   timespec delay;
//
//
//   if(m_bManualEvent)
//   {
//
//      wait(durationTimeout);
//
//      return m_bSignaled;
//
//   }
//   else
//   {
//
//      unsigned int timeout = durationTimeout.u32_millis();
//
//      unsigned int start= ::duration::now();
//
//      while(start.elapsed() < timeout)
//      {
//
//         sembuf sb;
//
//         sb.sem_op   = -1;
//         sb.sem_num  = 0;
//         sb.sem_flg  = IPC_NOWAIT;
//
//         int ret = semop((int) m_hsync, &sb, 1);
//
//         if(ret < 0)
//         {
//            if(ret == EPERM)
//            {
//               nanosleep(&delay, nullptr);
//            }
//            else
//            {
//               return false;
//            }
//         }
//         else
//         {
//            return true;
//         }
//
//      }
//
//   }
//
//   return false;
//
//#endif

}


bool happening::unlock()
{

   return true;

}


//HSYNC happening::hsync() const
//{
//
//#ifdef WINDOWS_DESKTOP
//
//   return m_hsync;
//
//#else
//
//   return (happening *) this;
//
//#endif
//
//}



