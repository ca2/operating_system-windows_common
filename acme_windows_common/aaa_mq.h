#pragma once


struct mq_message 
{

   ::atom        id();
   MESSAGE     m_message;

   mq_message(const ::atom & atom) :
      id()(atom)
   {
      m_message.message = atom.umessage();

   }
   mq_message(const mq_message& message)
   {

      memory_copy(this, &message, sizeof(mq_message));

   }

   mq_message(const MESSAGE& message) :
      id()((enum_message) message.message)
   {

      memory_copy(&m_message, &message, sizeof(MESSAGE));

   }

   mq_message& operator = (const mq_message& message)
   {

      if(this != &message) memory_copy(this, &message, sizeof(mq_message));

      return *this;

   }

   mq_message& operator = (const MESSAGE& message)
   {


      id() = (enum_message)message.message;

      memory_copy(&m_message, &message, sizeof(MESSAGE));

      return *this;

   }

};


class CLASS_DECL_ACME mq_message_array :
   public raw_array < mq_message >
{


};


class CLASS_DECL_ACME message_queue :
   virtual public matter
{
public:


   mq_message_array        m_messagea;
   manual_reset_happening      m_happeningNewMessage;
   itask                 m_itask;
   bool                    m_bKickIdle;
   bool                    m_bQuit;


   message_queue();
   virtual ~message_queue();

   int_bool peek_message(MESSAGE * pMsg, ::acme::windowing::window * pacmewindowingwindow, unsigned int wMsgFilterMin, unsigned int wMsgFilterMax, unsigned int wRemoveMsg);
   int_bool get_message(MESSAGE * pMsg, ::acme::windowing::window * pacmewindowingwindow, unsigned int wMsgFilterMin, unsigned int wMsgFilterMax);
   int_bool post_message(::acme::windowing::window * pacmewindowingwindow, ::enum_message emessage, ::wparam wparam, ::lparam lparam);
   int_bool post_message(const mq_message & message);


};


CLASS_DECL_ACME message_queue * aaa_get_message_queue(itask atom, bool bCreate);
CLASS_DECL_ACME void aaa_clear_message_queue(itask idthread);

void _c_simple_message_loop();





