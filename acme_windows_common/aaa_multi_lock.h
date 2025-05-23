#pragma once


class CLASS_DECL_ACME multi_lock
{
public:


   synchronization_array          m_synchronizationa;
   bits < unsigned long long >        m_bitsLocked;


   multi_lock(const synchronization_array & synca,bool bInitialLock = false);
   multi_lock(::collection::count c, const synchronization_array & synca, bool bInitialLock = false);
   ~multi_lock();


   void construct(const synchronization_array & synca,bool bInitialLock = false);


   synchronization_result lock(const duration & tickTimeout = duration::infinite(), bool bWaitForAll = true, unsigned int dwWakeMask = 0);
   bool unlock();
   bool unlock(int lCount, int * lPrevCount = nullptr);
   bool IsLocked(::collection::index iItem);

};
