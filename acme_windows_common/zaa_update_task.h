#pragma once


typedef huge_integer UPDATE_SERIAL();
using PFN_UPDATE_SERIAL = UPDATE_SERIAL *;


CLASS_DECL_ACME int os_get_system_update_poll_time(const :: atom & atom);


struct CLASS_DECL_ACME update_task :
   virtual public ::matter
{
protected:

   
   friend class matter;

   
   element_array m_elementa;


   static ::pointer<update_task>& task(huge_integer iUpdate);

   virtual void add(::matter * pmatter);
   virtual void erase(::matter * pmatter);

   static void _add(huge_integer iUpdate, ::matter* pmatter);

   static void _erase(huge_integer iUpdate, ::matter* pmatter);

   static void _erase(::matter* pmatter);

public:


   static critical_section * g_pcs;
   static ::i64_map < ::pointer<update_task >>* g_pmap;
   static bool g_bDestroyAll;


   bool                    m_bModified;
   int                     m_iMillisSleep;
   huge_integer                   m_iUpdate;
   huge_integer                   m_iSerial;


   update_task();
   virtual ~update_task();

   virtual void notify();


   static void set_modified(huge_integer iUpdate);



   virtual void run() override;

   static void post_destroy_all();

   inline bool is_ending() { synchronous_lock synchronouslock(synchronization()); return m_elementa.is_empty();};
   inline int poll_millis() { return os_get_system_update_poll_time(m_iUpdate);};

   static inline bool should_poll(int iMillis)
   {

      return iMillis >= 100;

   }


};


//template < typename PAYLOAD >
//class update_task :
//   virtual public ::update_task
//{
//public:
//
//
//   PAYLOAD         m_payload;
//   HAS_CHANGED *   m_pfnHasChanged;
//
//   update_task(huge_integer iUpdate, HAS_CHANGED * pfnHasChanged, int iMillisSleep = 300) :
//      update_task(iUpdate, iMillisSleep),
//      m_pfnHasChanged(pfnHasChanged)
//   {
//
//   }
//
//
//
//
//
//};





