#include "framework.h"
#include "apex/operating_system.h"
// Thank you TBS!!


VOID CALLBACK apex_timer_TimerRoutine(PVOID pParam, BOOLEAN TimerOrWaitFired);




//bool timer::impl_start()
//{
//
//   if (!m_bInit)
//   {
//
//      return false;
//
//   }
//
//   begin();
//   //synchronous_lock synchronouslock(mutex());
//
//   //increment_reference_count(REFERENCING_DEBUGGING_ARGS);
//
//   //if(!CreateTimerQueueTimer(&m_hTimer,m_hTimerQueue,(WAITORTIMERCALLBACK)apex_timer_TimerRoutine,this,m_dwMillis,0,WT_EXECUTEONLYONCE | WT_EXECUTELONGFUNCTION))
//   //{
//
//   //   release();
//
//   //   return false;
//
//   //}
//
//   return true;
//
//}


//void timer::impl_init()
//{
//
//   synchronous_lock synchronouslock(mutex());
//
//   m_hTimer = nullptr;
//
//   m_hTimerQueue = CreateTimerQueue();
//
//   if(nullptr == m_hTimerQueue)
//   {
//
//      throw ::exception(::exception("integer_exception -1"));
//
//   }
//
//}
//
//
//void timer::impl_term()
//{
//
//   synchronous_lock synchronouslock(mutex());
//
//   if(m_hTimerQueue != nullptr && m_hTimerQueue != INVALID_HANDLE_VALUE)
//   {
//
//      DeleteTimerQueue(m_hTimerQueue);
//
//      m_hTimerQueue = nullptr;
//
//   }
//
//}
//
//
//void timer::impl_stop()
//{
//
//   synchronous_lock synchronouslock(mutex());
//
//   if(m_hTimerQueue != nullptr && m_hTimer != nullptr)
//   {
//
//      DeleteTimerQueueTimer(m_hTimerQueue, m_hTimer, nullptr);
//
//      m_hTimer = nullptr;
//
//   }
//
//}
//
//
//
//bool timer::impl_restart()
//{
//
//   synchronous_lock synchronouslock(mutex());
//
//   DeleteTimerQueueTimer(m_hTimerQueue, m_hTimer, nullptr);
//
//   increment_reference_count(REFERENCING_DEBUGGING_ARGS);
//
//   if(!CreateTimerQueueTimer(&m_hTimer,m_hTimerQueue,(WAITORTIMERCALLBACK)apex_timer_TimerRoutine,this,m_dwMillis,0,WT_EXECUTEONLYONCE | WT_EXECUTELONGFUNCTION))
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
//

bool on_init_timer_thread();


//VOID CALLBACK apex_timer_TimerRoutine(PVOID pParam, BOOLEAN TimerOrWaitFired)
//{
//
//   auto ptimer = __move_transfer((::timer *)pParam);
//
//   if (!g_bAura)
//   {
//
//      output_debug_string("there is timer on (apex_timer_TimerRoutine) and apex is going away (!g_bAura)\n");
//
//      return;
//
//   }
//
//   if (g_axisoninitthread)
//   {
//
//      g_axisoninitthread();
//
//   }
//
//   on_init_timer_thread();
//
//   //on_init_thread();
//
//   //if (!on_init_thread())
//   //{
//   //   ::acme::del(ptimer);
//   //   //return -34;
//   //   return;
//
//   //}
//
//   bool bOk = false;
//
//   try
//   {
//
//      if (ptimer && ptimer->get_context_thread() && ptimer->get_context_thread()->is_set_finish())
//      {
//
//         output_debug_string("time context thread is set finish");
//
//      }
//      else
//      {
//
//         ptimer->call_on_timer();
//
//         bOk = true;
//
//      }
//
//   }
//   catch (::exception &)
//   {
//
//   }
//   catch (...)
//   {
//
//   }
//
//   try
//   {
//
//      if (g_axisontermthread)
//      {
//         g_axisontermthread();
//
//      }
//
//      on_term_thread();
//
//   }
//   catch (...)
//   {
//
//   }
//
//
//}


