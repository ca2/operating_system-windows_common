#pragma once


class sync_future;


inline ::pointer<sync_future>__sync_future();



class CLASS_DECL_ACME sync_future : 
   virtual public matter
{
protected:

   friend ::pointer<sync_future>__sync_future();

   sync_future()
   {

      __defer_construct(m_phappeningCompletion);

   }
public:


   ::payload                                    m_var;
   ::pointer<manual_reset_happening>         m_phappeningCompletion;


   virtual ~sync_future() {}


   inline virtual void operator()(const ::payload & payload) override
   {

      m_var = payload;

      m_phappeningCompletion->set_happening();

   }

   inline synchronization_result wait(const ::duration& duration = duration::infinite())
   {

      return m_phappeningCompletion->wait(duration);

   }


};


inline ::pointer<sync_future>__sync_future()
{

   return øallocate sync_future();

}



