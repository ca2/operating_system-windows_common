// Extracted from aura/graphics/draw2d/device_lock, changed and Organized by camilo on 2021-09-04 05:46 BRT <3ThomasBS__!!
#pragma once

//#error
//
//#ifdef _UWP
//
//#include "acme/operating_system/windows_common/comptr.h"
//#include <d2d1_1.h>
//
//#endif
//


namespace direct2d
{


   class CLASS_DECL_DIRECT2D multithreading_lock
   {
   public:


      comptr < ID2D1Multithread > m_D2DMultithread;

      //device_lock(ID2D1Multithread * pinteraction);
      //~device_lock();

      multithreading_lock(ID2D1Multithread * pmultithreading) :
         m_D2DMultithread(pmultithreading)
   {

//#ifdef _UWP
//
//      m_psystem = pinteraction->m_psystem;
//
//      ::pointer<::aura::system>psystem = m_psystem;
//
//      auto pdraw2d = psystem->draw2d();
//
//      auto pmultithread = pdraw2d->direct2d()->m_d2dMultithread.Get();
//
         if (m_D2DMultithread.Get())
         {
            m_D2DMultithread->Enter();

         }
//
//#endif

   }


   ~multithreading_lock()
   {

//#ifdef _UWP
//
//      //m_D2DMultithread->Leave();
//
//      ::pointer<::aura::system>psystem = m_psystem;
//
//      auto pdraw2d = psystem->draw2d();
//
//      auto pmultithread = pdraw2d->direct2d()->m_d2dMultithread.Get();
//
      if (m_D2DMultithread.Get())
      {
         m_D2DMultithread->Leave();

      }
//
//#endif

   }


   };


   //class CLASS_DECL_AURA lock :
   //   public synchronous_lock
   //{
   //public:

   //   static ::mutex * g_pmutex;

   //   lock() :
   //      synchronous_lock(g_pmutex)
   //   {

   //   }

   //   virtual ~lock()
   //   {

   //   }

   //};


} // namespace direct2d


