#include "framework.h"


//class thread_proc_runner :
//   virtual public matter
//{
//public:
//
//
//   ::thread_proc     m_proc;
//   void *            m_p;
//
//
//   thread_proc_runner(::thread_proc proc, void * p) :
//      m_proc(proc),
//      m_p(p)
//   {
//
//   }
//
//   virtual unsigned int thread_proc();
//
//
//};









//// very close to the operating system
//CLASS_DECL_ACME htask create_thread(
//thread_proc proc,
//void * p,
//::enum_priority epriority,
//unsigned int nStackSize,
//unsigned int uiCreateFlags,
//LPSECURITY_ATTRIBUTES psa,
//
//itask * puiId)
//{
//
//   thread_proc_runner * pdata = ___new thread_proc_runner(proc, p);
//
//   return pdata->create_thread(epriority, nStackSize, uiCreateFlags, psa, puiId);
//
//
//}





CLASS_DECL_ACME PPROC_SIMPLE g_axisoninitthread = nullptr;
CLASS_DECL_ACME PPROC_SIMPLE g_axisontermthread = nullptr;

//unsigned int thread_proc_runner::thread_proc()
//{
//
//   if (g_axisoninitthread)
//   {
//
//      g_axisoninitthread();
//
//   }
//
//   if (!on_init_thread())
//   {
//
//      return error_failed;
//
//   }
//
//   unsigned int u = m_proc(m_p);
//
//   try
//   {
//
//      if (g_axisontermthread)
//      {
//
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
//   return u;
//
//}


//thread_local ::task_pointer t_pthread;


//::thread* get_task()
//{
//   
//   return t_pthread; 
//
//}


//void set_thread(thread* pthread)
//{
//
//   t_pthread = pthread;
//
//}


//void thread_release()
//{
//
//   t_pthread.release();
//
//}



