#include "framework.h"
#include "acme/os/_os.h"


synchronization_array::synchronization_array()
{

}


synchronization_array::synchronization_array(const ::synchronization_array & array) :
   matter(array),
   m_hsyncaCache(array.m_hsyncaCache),
   m_synchronizationa(array.m_synchronizationa)
{

   memory_copy(m_byteaSyncIndex, array.m_byteaSyncIndex, sizeof(m_byteaSyncIndex));

}


synchronization_array::~synchronization_array()
{

}


synchronization_array & synchronization_array::operator = (const synchronization_array & synca)
{

   if(this != &synca)
   {

      m_hsyncaCache = synca.m_hsyncaCache;
      memory_copy(&m_byteaSyncIndex, synca.m_byteaSyncIndex, sizeof(m_byteaSyncIndex));
      m_synchronizationa = synca.m_synchronizationa;

   }

   return *this;

}


void	synchronization_array::clear()
{

   m_synchronizationa.clear();

   m_hsyncaCache.clear();

}


::collection::count synchronization_array::size() const
{

   return m_synchronizationa.get_size();

}


bool synchronization_array::is_empty() const
{

   return m_synchronizationa.is_empty();

}


bool synchronization_array::add_item(synchronization_object * psync)
{

   if (m_synchronizationa.size() >= MAXIMUM_WAIT_OBJECTS)
   {

      return false;

   }

   HSYNC hsync = psync->hsync();

   if (hsync != nullptr && hsync != INVALID_HSYNC_VALUE)
   {

      m_byteaSyncIndex[m_hsyncaCache.get_size()] = (unsigned char) m_synchronizationa.get_size();

      m_hsyncaCache.add(hsync);

   }

   m_synchronizationa.add(psync);


   return true;

}




bool synchronization_array::add(const synchronization_array& synca)
{

   if (m_hsyncaCache.size() + synca.m_hsyncaCache.get_size() > MAXIMUM_WAIT_OBJECTS)
   {

      return false;

   }

   for (auto& psync : synca.m_synchronizationa)
   {

      add_item(psync);

   }

   return true;

}


void synchronization_array::erase(class synchronization_object * psync)
{

   m_synchronizationa.erase(psync);

   HSYNC hsync = psync->hsync();

   if (hsync != nullptr && hsync != INVALID_HSYNC_VALUE)
   {

      m_hsyncaCache.erase(hsync);

   }

}


void synchronization_array::erase(::collection::index index)
{

   if (index >= m_synchronizationa.size())
   {

      throw ::exception(range_error("synchronization_array::erase: index out of bounds"));

   }

   synchronization_object* psync = m_synchronizationa[index];

   erase(psync);

}


void synchronization_array::wait()
{

   wait(true, duration::infinite());

}


synchronization_result synchronization_array::wait(const duration & duration)
{

   return wait(false, duration);

}


synchronization_result synchronization_array::wait(bool waitForAll, const duration & duration, bool bWaitMessageQueue)
{

   if (is_empty())
   {

      return synchronization_result(e_synchronization_result_error);

   }

   unsigned int winResult;

   if (m_synchronizationa.size() == m_hsyncaCache.size())
   {

#ifdef WINDOWS_DESKTOP

      if (bWaitMessageQueue)
      {

         auto ::duration = duration.u32_millis();

         winResult = ::MsgWaitForMultipleObjectsEx((unsigned int)m_hsyncaCache.size(), m_hsyncaCache.get_data(), ::duration, QS_ALLEVENTS, waitForAll ? MWMO_WAITALL : 0);

      }
      else
#endif
      {

         auto ::duration = duration.u32_millis();

         unsigned int uCount = (unsigned int)m_hsyncaCache.size();

         auto psynca = m_hsyncaCache.get_data();

         winResult = ::WaitForMultipleObjectsEx(uCount, psynca, waitForAll, ::duration, bWaitMessageQueue);

      }

      return synchronization_result(winResult, m_hsyncaCache.size());

   }


   for(auto & psync : m_synchronizationa)
   {

      psync->init_wait();

   }

   auto start = ::duration::now();

   bool FoundExternal=false;

   do
   {

      if (start.elapsed() > duration)
      {

         winResult = WAIT_TIMEOUT;

      }

      do
      {

#ifdef WINDOWS_DESKTOP
         if (bWaitMessageQueue)
         {

            winResult = ::MsgWaitForMultipleObjectsEx((unsigned int)m_hsyncaCache.size(), m_hsyncaCache.get_data(),  __os(duration - start.elapsed()), QS_ALLEVENTS, waitForAll ? MWMO_WAITALL : 0);

         }
         else
#endif
         {

            winResult = ::WaitForMultipleObjectsEx((unsigned int)m_hsyncaCache.size(), m_hsyncaCache.get_data(), waitForAll, __os(duration - start.elapsed()), true);

         }

      }
      while (winResult == WAIT_IO_COMPLETION);

      // TODO

      //if(m_synchronizationa.has_element() && winResult!=WAIT_TIMEOUT && winResult!=WAIT_FAILED)
      //{

      //   synchronization_result result=synchronization_result(winResult,m_synchronizationa.size());

      //   if(waitForAll)
      //   {
      //
      //      for (auto& psync : m_synchronizationa)
      //      {

      //         psync->wait();

      //      }

      //   }
      //   else
      //   {

      //      index position = result.abandoned() ? result.abandoned_index() : result.signaled_index();

      //      position = maximum(0, position- m_hsynca.get_size());

      //      m_synchronizationa[position]->wait();

      //      for ( ++position; position<m_synchronizationa.size(); ++position )
      //      {
      //         if(m_waitableelementa[position].m_psynccallback)
      //         {
      //            int res = ::WaitForSingleObjectEx(m_synchronizationa[position], 0, false);

      //            if ( res != WAIT_TIMEOUT )
      //               m_waitableelementa[position].m_psynccallback->on_sync(m_waitableelementa[position].m_psync);
      //         }
      //         else if(!FoundExternal)
      //         {
      //            int res = ::WaitForSingleObjectEx(m_synchronizationa[position], 0, false);

      //            if ( res != WAIT_TIMEOUT )
      //            {
      //               winResult= (unsigned int) (res + position);
      //               FoundExternal=true;
      //            }
      //         }
      //      }
      //   }
      //}
      //else if(m_countCallback==0)
      //   FoundExternal=true;

   }
   while (winResult != WAIT_TIMEOUT && winResult != WAIT_FAILED);
   //while (winResult!=WAIT_TIMEOUT && winResult!= WAIT_FAILED && FoundExternal==false);

   ////MBO: erase happenings to avoid double signalization
   //comparable_array < waitable_element >::iterator it;
   //for (it = m_waitableelementa.begin(); it != m_waitableelementa.end(); ++it)
   //   (*it).m_psync->exit_wait();

   return synchronization_result(winResult,m_hsyncaCache.size());

}


synchronization_result synchronization_array::contains( const synchronization_result& result ) const
{

   throw todo();

   //if ( !result.abandoned() && !result.signaled() )
   //   throw ::exception(range_error("no matter signaled"));

   //index position = result.abandoned() ? result.abandoned_index() : result.signaled_index();

   //position = maximum(0)
   //for ( ++position; position<m_synchronizationa.get_size(); ++position )
   //{
   //   if(!m_waitableelementa[position].m_psynccallback)
   //   {
   //      int res = ::WaitForSingleObjectEx(m_synchronizationa[position], 0, false);
   //      if ( res == WAIT_TIMEOUT )
   //         continue;
   //      return synchronization_result( static_cast<int>(position), m_synchronizationa.get_size() );
   //   }
   //}

   //return synchronization_result( e_synchronization_result_error );

}


