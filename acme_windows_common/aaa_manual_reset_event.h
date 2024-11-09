//*****************************************************************************
//
//      Class:          manual_reset_happening
//      Author:         Kenny Kerr
//      Date created:   10 April 2004
//      Description:    Notifies one or more waiting threads that an happening has
//                      occurred.
//
//*****************************************************************************


#pragma once

#define DECLARE_REUSABLE(TYPE) \
TYPE * m_pnext; \
::factory_item::reusable_factory_item < TYPE, TYPE > * m_pfactory; \
virtual void delete_this() { if(m_pfactory) m_pfactory->return_back(this); else delete this;}

class CLASS_DECL_ACME manual_reset_happening :
   public ::happening
{
public:


   DECLARE_REUSABLE(manual_reset_happening);


   manual_reset_happening(char * sz = nullptr, bool bInitiallyOwn = false);


   void reuse() { reset_happening(); }

};


class CLASS_DECL_ACME long_counter :
   public manual_reset_happening
{
public:


   interlocked_long m_long;

   
   long_counter(long lCount) : m_long(lCount) {}


   long operator ++()
   {

      long l = --m_long;

      if (l <= 0)
      {

         set_happening();

      }

      return l;

   }

   long operator ++(int)
   {

      long l = m_long;
   
      ++(*this);

      return l;

   }

};


