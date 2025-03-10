#pragma once


class CLASS_DECL_ACME single_lock :
   virtual public synchronization_object
{
public:


   ::pointer<synchronization_object>        m_psync;
   //HANDLE                  m_hobject;
   bool                    m_bAcquired;


   explicit single_lock(synchronization_object * pobject, bool bInitialLock = false);
   ~single_lock();

   synchronization_result wait();
   synchronization_result wait(const duration & duration);
   bool unlock();
   bool unlock(int lCount, int * lPrevCount = nullptr);
   bool IsLocked();


};
