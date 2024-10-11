#include "framework.h"
#include "task.h"
#ifdef LINUX
#include "acme/os/ansios/_pthread.h"
#endif


task::task()
{

   m_bitCoInitialize = false;
   m_bitIsRunning = false;
   m_bIsPredicate = true;
   m_htask = null_hthread;
   m_itask = 0;

}


task::~task()
{


}


string task::get_tag() const
{

   return m_strTaskTag;

}



string task::thread_get_name() const
{

   return m_strTaskName;

}


::task * task::get_task()
{

   return this;

}


const char * task::get_task_tag()
{

   return m_strTaskTag;

}


//::object * task::calc_parent_thread()
//{
//
//   return ::get_task();
//
//}


bool task::set_thread_name(const ::string & pszThreadName)
{

   m_strTaskName = pszThreadName;

   if (m_strTaskTag.is_empty() && m_strTaskName.has_char())
   {

      m_strTaskTag = m_strTaskName;

   }

   if (!::set_thread_name(m_htask, pszThreadName))
   {

      return false;

   }

   return true;

}


bool task::task_get_run() const
{

   return !m_bSetFinish;

}


bool task::task_active() const
{

   return m_htask != (htask_t) 0;

}


bool task::is_running() const
{

   return m_bitIsRunning;

}


::object * task::thread_parent()
{

   return __object(m_pobjectParent);

}


bool task::is_thread() const
{

   return false;

}


#ifdef WINDOWS
DWORD WINAPI task::s_os_task(void* p)
#else
void* task::s_os_task(void* p)
#endif
{

   try
   {

      ::task* pthread = (::task*) p;

      ::set_task(pthread REFERENCING_DEBUGGING_COMMA_P_FUNCTION_LINE(pthread));

      pthread->release(REFERENCING_DEBUGGING_P_FUNCTION_LINE(pthread));

      pthread->do_task();

#if REFERENCING_DEBUGGING

      if (pthread->m_countReference > 1)
      {

         __check_pending_releases(pthread);

      }

#endif

      ::thread_release(REFERENCING_DEBUGGING_P_NOTE(pthread, ""));

   }
   catch (...)
   {

   }

   return 0;

}


void task::add_notify(::matter* pmatter)
{

   synchronous_lock synchronouslock(mutex());

   notify_array().add_item(pmatter REFERENCING_DEBUGGING_COMMA_THIS_FUNCTION_FILE_LINE);

}


void task::erase_notify(::matter* pmatter)
{

   synchronous_lock synchronouslock(mutex());

   if (m_pnotifya)
   {

      m_pnotifya->erase_item(pmatter REFERENCING_DEBUGGING_COMMA_THIS);

   }

}


bool task::on_get_thread_name(string & strThreadName)
{

   if (m_strTaskTag.has_char())
   {

      //::set_thread_name(m_strTaskTag);

      strThreadName = m_strTaskTag;

   }
   else
   {

      //::set_thread_name(type_name());

      strThreadName = type_name();

   }

   return true;

}


void task::task_caller_on_init()
{

   return ::success;

}


void task::init_task()
{

   string strThreadName;

   if (on_get_thread_name(strThreadName))
   {

      set_thread_name(strThreadName);

   }

   if (string(type_name()).contains("synth_thread"))
   {

      output_debug_string("synth_thread thread::thread_proc");

   }
   else if (string(type_name()).case_insensitive_ends("out"))
   {

      output_debug_string("synth_thread thread::out");

   }

}


void task::term_task()
{

   try
   {

      destroy();

   }
   catch (...)
   {

   }

   synchronous_lock synchronouslock(mutex());

   //if (m_pnotifya)
   //{

   //   auto notifya = *m_pnotifya;

   //   synchronouslock.unlock();

   //   for (auto & pmatter : notifya)
   //   {

   //      pmatter->task_erase(this);

   //      pmatter->task_on_term(this);

   //   }

   //   synchronouslock.lock();

   //}

   //if (m_pthreadParent)
   //{

   //   m_pthreadParent->task_on_term(this);

   //   m_pthreadParent->task_erase(this);

   //   //m_pthreadParent->kick_idle();

   //}

}


void task::do_task()
{

   init_task();

   auto estatus = on_task();

   term_task();

   return estatus;

}


void task::on_task()
{

   ::e_status estatus = ::success;

   while (!m_bSetFinish)
   {

      ::matter* pmatter;

      {

         synchronous_lock synchronouslock(mutex());

         pmatter = m_pmatter.m_p;

         m_bitIsRunning = pmatter != nullptr;

         if (!m_bitIsRunning)
         {

            break;

         }

         m_atom = pmatter->type_name();

         set_thread_name(m_atom);

         m_pmatter.m_p = nullptr;

      }

      pmatter->on_task();

   }

   return estatus;

}


void task::start(
   ::matter* pmatter,
   ::enum_priority epriority,
   u32 nStackSize,
   u32 uCreateFlags)
{

   m_pmatter = pmatter;

   m_atom = pmatter->type_name();

   return begin_task(epriority, nStackSize, uCreateFlags);

}


void task::begin_task(
   ::enum_priority epriority,
   u32 nStackSize,
   u32 uCreateFlags)
{

   if (m_atom.is_empty())
   {

      if (m_pmatter)
      {

         m_atom = m_pmatter->type_name();

      }
      else
      {

         m_atom = type_name();

      }

   }

   if (m_atom.is_empty() || m_atom == "task" || m_atom == "thread")
   {

      throw ::exception(error_bad_argument);

      return ::error_failed;

   }

   auto estatus = task_caller_on_init();

   if (!estatus)
   {

      return estatus;

   }

   //if (m_pobjectParent && m_bIsPredicate)
   //{

   //   //auto pthreadParent = calc_parent_thread();

   //   m_pobjectParent->task_add(this);


   //   if (pthreadParent)
   //   {

   //

   //   }

   //}

   // __task_procedure() should release this (pmatter)
   increment_reference_count(REFERENCING_DEBUGGING_THIS_FUNCTION_FILE_LINE);

#ifdef WINDOWS

   DWORD dwThread = 0;

   m_htask = ::CreateThread(nullptr, nStackSize, &::task::s_os_task, (LPVOID)(task*)this, uCreateFlags, &dwThread);

   m_itask = dwThread;

#else

   pthread_attr_t taskAttr;

   pthread_attr_init(&taskAttr);

   if (nStackSize > 0)
   {

      pthread_attr_setstacksize(&taskAttr, nStackSize); // Set the stack size_i32 of the task

   }

   pthread_attr_setdetachstate(&taskAttr, PTHREAD_CREATE_DETACHED); // Set task to detached state. No need for pthread_join

   pthread_create(
      &m_htask,
      &taskAttr,
      &task::s_os_task,
      this);

#endif

   if (!m_htask)
   {

      return ::error_failed;

   }

   return ::success;

}


::task_pointer task::launch(::matter * pmatter, ::enum_priority epriority, ::u32 nStackSize, u32 uCreateFlags)
{

   auto ptask = __allocate task();

   ptask->start(pmatter, epriority, nStackSize, uCreateFlags);

   return ptask;

}


//
//bool task::set_task_name(const ::string & pszThreadName)
//{
//
//   if (!::set_task_name(m_htask, pszThreadName))
//   {
//
//      return false;
//
//   }
//
//   return true;
//
//}


//
//void task::set_task_run(bool bRun)
//{
//
//   m_bRunThisThread = bRun;
//
//}


void task::kick_idle()
{


}


CLASS_DECL_ACME bool __task_sleep(task* task)
{

   while (task->task_get_run())
   {

      sleep(100_ms);

   }

   return false;

}


CLASS_DECL_ACME bool __task_sleep(task* pthread, const class ::wait & wait)
{

   if (wait.m_iMillisecond < 1000)
   {

      if (!pthread->task_get_run())
      {

         return false;

      }

      sleep(::duration(wait.m_iMillisecond));

      return pthread->task_get_run();

   }

   auto iTenths = wait.m_i::durations / 10;

   auto iMillis = wait.m_i::durations % 10;

   try
   {

      ::pointer<manual_reset_event>spev;

      {

         synchronous_lock synchronouslock(pthread->mutex());

         if (pthread->m_pevSleep.is_null())
         {

            pthread->m_pevSleep = __allocate manual_reset_event();

            pthread->m_pevSleep->ResetEvent();

         }

         spev = pthread->m_pevSleep;

      }

      if (!pthread->task_get_run())
      {

         return false;

      }

      //while(iTenths > 0)
      //{

      pthread->m_pevSleep->wait(wait);

      if (!pthread->task_get_run())
      {

         return false;

      }

      //iTenths--;

   //}

   }
   catch (...)
   {

   }

   return pthread->task_get_run();

}


CLASS_DECL_ACME bool __task_sleep(::task* pthread, synchronization_object* psync)
{

   try
   {

      while (pthread->task_get_run())
      {

         if (psync->wait(100).succeeded())
         {

            break;

         }

      }

   }
   catch (...)
   {

   }

   return pthread->task_get_run();

}


CLASS_DECL_ACME bool __task_sleep(task* pthread, const class ::wait & wait, synchronization_object* psync)
{

   if (wait.m_iMillisecond < 1000)
   {

      if (!pthread->task_get_run())
      {

         return false;

      }

      psync->wait(::duration(wait.m_iMillisecond));

      return pthread->task_get_run();

   }

   auto iTenths = wait.m_iMillisecond / 100;

   auto iMillis = wait.m_iMillisecond % 100;

   try
   {

      {

         pthread->m_pevSleep->wait(100);

         if (!pthread->task_get_run())
         {

            return false;

         }

         iTenths--;

      }

   }
   catch (...)
   {

   }

   return pthread->task_get_run();

}


CLASS_DECL_ACME bool task_sleep(const class ::wait & wait, synchronization_object* psync)
{

   auto pthread = ::get_task();

   if (::is_null(pthread))
   {

      if (::is_null(psync))
      {

         if (wait.is_infinite())
         {

         }
         else
         {

            ::sleep(::duration);

         }

      }
      else
      {

         if (__os(::duration) == U32_INFINITE_TIMEOUT)
         {

            return psync->lock();

         }
         else
         {

            return psync->lock(::duration);

         }

      }

      return true;

   }

   if (::is_null(psync))
   {

      if (__os(::duration) == U32_INFINITE_TIMEOUT)
      {

         return __task_sleep(pthread);

      }
      else
      {

         return __task_sleep(pthread, ::duration);

      }

   }
   else
   {

      if (__os(::duration) == U32_INFINITE_TIMEOUT)
      {

         return __task_sleep(pthread, psync);

      }
      else
      {

         return __task_sleep(pthread, ::duration, psync);

      }

   }
}



