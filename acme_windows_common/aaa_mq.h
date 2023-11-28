#pragma once


struct mq_message 
{

   ::atom        m_atom;
   MESSAGE     m_message;

   mq_message(const ::atom & atom) :
      m_atom(atom)
   {
      m_message.message = atom.umessage();

   }
   mq_message(const mq_message& message)
   {

      memcpy(this, &message, sizeof(mq_message));

   }

   mq_message(const MESSAGE& message) :
      m_atom((enum_message) message.message)
   {

      memcpy(&m_message, &message, sizeof(MESSAGE));

   }

   mq_message& operator = (const mq_message& message)
   {

      if(this != &message) memcpy(this, &message, sizeof(mq_message));

      return *this;

   }

   mq_message& operator = (const MESSAGE& message)
   {


      m_atom = (enum_message)message.message;

      memcpy(&m_message, &message, sizeof(MESSAGE));

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
   manual_reset_event      m_eventNewMessage;
   itask_t                 m_itask;
   bool                    m_bKickIdle;
   bool                    m_bQuit;


   message_queue();
   virtual ~message_queue();

   int_bool peek_message(MESSAGE * pMsg, oswindow oswindow, ::u32 wMsgFilterMin, ::u32 wMsgFilterMax, ::u32 wRemoveMsg);
   int_bool get_message(MESSAGE * pMsg, oswindow oswindow, ::u32 wMsgFilterMin, ::u32 wMsgFilterMax);
   int_bool post_message(oswindow oswindow, const ::atom & atom, WPARAM wParam, LPARAM lParam);
   int_bool post_message(const mq_message & message);


};


CLASS_DECL_ACME message_queue * aaa_get_message_queue(itask_t atom, bool bCreate);
CLASS_DECL_ACME void aaa_clear_message_queue(itask_t idthread);

void _c_simple_message_loop();





