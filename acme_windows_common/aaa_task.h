#pragma once


typedef pointer_array < ::matter > object_array;
typedef map < itask, ::pointer<task >>task_map;
typedef map < task *, itask > task_id_map;


class CLASS_DECL_ACME task :
   virtual public object
{

public:


   int                                 m_bAvoidProcedureFork : 1;
   int                                 m_bitIsRunning : 1;
   int                                 m_bIsPredicate : 1; // Is helper thread (as opposite to a "main" thread)
   int                                 m_bitCoInitialize : 1;


   htask                           m_htask;
   itask                           m_itask;
   string                              m_strTaskName;
   string                              m_strTaskTag;
   ::pointer<::object>   m_pobjectParent;

   ::pointer<::matter>           m_pmatter;
   ::pointer<manual_reset_happening>      m_pevSleep;

#ifdef WINDOWS
   HRESULT                          m_hresultCoInitialize;
#endif


   task();
   virtual ~task();


   virtual string get_tag() const;
   virtual string thread_get_name() const;


   virtual ::task * get_task() override;
   virtual const char * get_task_tag() override;


   virtual bool set_thread_name(const ::string & pszName);


#ifdef WINDOWS


#else

   static void* s_os_task(void* p);

#endif

   virtual void add_notify(::matter* pmatter);
   virtual void erase_notify(::matter* pmatter);


   virtual void task_caller_on_init();

   virtual bool on_get_thread_name(string & strThreadName);


   virtual void init_task();
   virtual void term_task();
   virtual void do_task() override;
   virtual void on_task() override;


   virtual void begin_task(
      ::enum_priority epriority = e_priority_normal,
      unsigned int nStackSize = 0,
      unsigned int dwCreateFlags = 0);


   virtual void start(
      ::matter* pmatter,
      ::enum_priority epriority = e_priority_normal,
      unsigned int nStackSize = 0,
      unsigned int dwCreateFlags = 0);




   //template < typename METHOD >
   //inline static ::task_pointer __task(METHOD method)
   //{

   //   auto pmethod = method(method);

   //   auto ptask = start(pmethod);

   //   return ptask;

   //}


   static ::task_pointer launch(
      ::matter* pmatter,
      ::enum_priority epriority = e_priority_normal,
      unsigned int nStackSize = 0,
      unsigned int dwCreateFlags = 0);


   virtual ::object * thread_parent();


   virtual bool is_thread() const override;
   virtual bool task_get_run() const;

   virtual bool task_active() const;
   virtual bool is_running() const;

   //virtual bool set_thread_name(const ::string & pszThreadName);

   virtual bool is_predicate() const { return !m_pobject || m_pobject.get() == this; }

   //virtual void set_thread_run(bool bRun = true);

   //virtual void finish() override;

   virtual void kick_idle() override;


};


using task_array = pointer_array < task >;




