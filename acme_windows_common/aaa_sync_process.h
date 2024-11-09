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

      __defer_construct(m_peventCompletion);

   }
public:


   ::payload                                    m_var;
   ::pointer<manual_reset_happening>         m_peventCompletion;


   virtual ~sync_future() {}


   inline virtual void operator()(const ::payload & payload) override
   {

      m_var = payload;

      m_peventCompletion->SetEvent();

   }

   inline synchronization_result wait(const ::duration& duration = duration::infinite())
   {

      return m_peventCompletion->wait(duration);

   }


};


inline ::pointer<sync_future>__sync_future()
{

   return __allocate sync_future();

}



