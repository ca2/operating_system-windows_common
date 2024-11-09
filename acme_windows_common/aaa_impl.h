
#pragma once

#ifdef inline

inline int_bool semaphore::Unlock()
{ return Unlock(1, nullptr); }

inline int_bool CEvent::set_happening()
{ ASSERT(m_hObject != nullptr); return ::SetEvent(m_hObject); }
inline int_bool CEvent::pulse_happening()
{ ASSERT(m_hObject != nullptr); return ::pulse_happening(m_hObject); }
inline int_bool CEvent::reset_happening()
{ ASSERT(m_hObject != nullptr); return ::ResetEvent(m_hObject); }

inline CSingleLock::~CSingleLock()
{ Unlock(); }
inline int_bool CSingleLock::IsLocked()
{ return m_bAcquired; }

inline int_bool CMultiLock::IsLocked(unsigned int dwObject)
{
   ASSERT(dwObject < m_dwCount);
   return m_bLockedArray[dwObject];
}

inline int_bool critical_section::Init()
{
   __try
   {
      ::InitializeCriticalSection(&m_sect);
   }
   __except(STATUS_NO_MEMORY == GetExceptionCode())
   {
      return false;
   }

   return true;
}

inline critical_sectioncritical_section() : synchronization_object < HANDLE > (nullptr)
{
   int_bool bSuccess;

   bSuccess = Init();
   if (!bSuccess)
      throw ::exception(error_no_memory);
}

inline critical_section::operator CRITICAL_SECTION*()
{ return (CRITICAL_SECTION*) &m_sect; }
inline critical_section::~critical_section()
{ ::DeleteCriticalSection(&m_sect); }
inline int_bool critical_section::Lock()
{
   __try
   {
      ::EnterCriticalSection(&m_sect);
   }
   __except(STATUS_NO_MEMORY == GetExceptionCode())
   {
      throw ::exception(error_no_memory);
   }
   return true;
}
inline int_bool critical_section::Lock(unsigned int tickTimeout)
{ ASSERT(tickTimeout == U32_INFINITE_TIMEOUT); (void)tickTimeout; return Lock(); }
inline int_bool critical_section::Unlock()
{ ::LeaveCriticalSection(&m_sect); return true; }

#endif //inline







inline bool synchronization_result::abandoned() const
{ return m_iEvent <= (::collection::index)result_abandon0; }

inline ::collection::index synchronization_result::abandoned_index() const
{
   if ( !abandoned() )
      throw ::exception(range_error("abandoned index out of range"));
   return -(m_iEvent + (::collection::index)result_abandon0);
}

inline bool synchronization_result::error() const
{ return m_eresult == result_error; }

inline bool synchronization_result::bad_thread() const
{ return m_eresult == result_bad_thread; }

inline bool synchronization_result::timeout() const
{ return m_eresult == result_timeout; }

inline bool synchronization_result::signaled() const
{ return m_iEvent >= (::collection::index)result_event0; }

inline bool synchronization_result::succeeded() const
{
   return signaled();
}

inline ::collection::index synchronization_result::signaled_index() const
{
   if ( !signaled() )
      throw ::exception(range_error("signaled index out of range"));
   return m_iEvent;
}





//template <class T >
//inline synch_index_iterator::synch_index_iterator(synch_ptr_array < T > & ptra,bool bInitialLock):
//   single_lock(&ptra.m_mutex,true)
//{
//
//   m_pptra = &ptra;

//   init(&ptra.m_indexptra,bInitialLock);

//}










template < typename PRED >
inline bool predicate_Sleep(int iTime, PRED pred)
{

   if(iTime < 100)
   {

      sleep(100_ms);

   }
   else
   {

      iTime += 99;

      iTime /= 100;

      for(::collection::index i = 0; i < iTime; i++)
      {

         sleep(100_ms);

         if(!::task_get_run() || !pred())
         {

            break;

         }

      }

   }

   return ::task_get_run();

}


template < typename PRED >
void async_predicate(void (* pfnBranch )(::matter * pobjectTask, enum_priority), PRED pred, enum_priority epriority)
{

   auto pobjectTask = __routine(pred);

   pfnBranch(pobjectTask, epriority);

}



