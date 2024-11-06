#pragma once


class CLASS_DECL_ACME semaphore :
   virtual public synchronization_object
{
public:

   string            m_strName;

#if defined(ANDROID)

   int              m_lMaxCount;
   sem_t *           m_psem;

#elif defined(LINUX) || defined(APPLEOS)

   int              m_lMaxCount;
   int           m_hsync;

#endif

   semaphore(int lInitialCount = 1, int lMaxCount = 1, const ::string & pstrName=nullptr, sync_options * poptions = nullptr);

   virtual ~semaphore();


#if defined(APPLEOS) || defined(LINUX) || defined(ANDROID) || defined(SOLARIS)
//   using matter::lock;
   virtual synchronization_result wait(const duration & durationTimeout) override;
#endif

   virtual bool unlock() override;
   virtual bool unlock(int lCount, int * prevCount = nullptr) override;


};


