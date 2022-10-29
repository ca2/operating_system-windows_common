#pragma once


#include "acme/primitive/string/string.h"


template < typename POINTER_TYPE >
class cotaskptr
{
public:


   POINTER_TYPE   m_p;

   cotaskptr()
   {

      m_p = nullptr;

   }


   ~cotaskptr()
   {

      free();

   }

   void alloc(SIZE_T size)
   {

      if (m_p != nullptr)
      {

         m_p = (POINTER_TYPE) CoTaskMemRealloc(m_p, size);

      }
      else
      {

         m_p = (POINTER_TYPE) CoTaskMemAlloc(size);

      }

   }

   bool is_null() const
   {

      return m_p == nullptr;

   }

   bool is_set() const
   {

      return m_p != nullptr;

   }

   void free()
   {

      if (m_p != nullptr)
      {

         ::CoTaskMemFree(m_p);

         m_p = nullptr;

      }

   }


   operator POINTER_TYPE()
   {

      return m_p;

   }

   operator const POINTER_TYPE() const
   {

      return m_p;

   }


   POINTER_TYPE operator ->()
   {

      return m_p;

   }


   const POINTER_TYPE operator ->() const
   {

      return m_p;

   }

   POINTER_TYPE * operator &()
   {

      free();

      return &m_p;

   }

   const POINTER_TYPE * operator &() const
   {

      return &m_p;

   }


};


template < typename POINTER_TYPE >
class sec_cotaskptr :
   public cotaskptr < POINTER_TYPE >
{
public:


   DWORD          m_size;


   sec_cotaskptr(DWORD size = 0)
   {

      m_size = size;

   }


   ~sec_cotaskptr()
   {

      free();

   }

   using cotaskptr < POINTER_TYPE > ::alloc;
   void alloc()
   {

      alloc(m_size);

   }

   void free()
   {

      if (this->m_p != nullptr)
      {

         if (m_size > 0)
         {

            SecureZeroMemory(this->m_p, m_size);

            m_size = 0;

         }

         ::cotaskptr < POINTER_TYPE > ::free();

      }

   }

};


template < typename TYPE >
class comptr_array
{
public:


   TYPE **           m_pp;
   UINT32            m_cCount;

   comptr_array(TYPE * * pp = nullptr, UINT32 cCount = 0) :
      m_pp(pp),
      m_cCount(cCount)
   {

   }


   ~comptr_array()
   {

      free();

   }

   bool is_null() const
   {

      return m_pp == nullptr;

   }

   bool is_set() const
   {

      return m_pp != nullptr;

   }

   virtual void free()
   {

      if (m_pp != nullptr)
      {

         for (UINT32 u = 0; u < m_cCount; u++)
         {

            if (m_pp[u])
            {

               m_pp[u]->Release();

            }

         }

         ::CoTaskMemFree(m_pp);

         m_pp = nullptr;

      }

   }


   operator TYPE * *()
   {

      return m_pp;

   }

   operator const TYPE * *() const
   {

      return m_pp;

   }


   TYPE * * operator ->()
   {

      return m_pp;

   }


   const TYPE * * operator ->() const
   {

      return m_pp;

   }

   TYPE * * * operator &()
   {

      free();

      return &m_pp;

   }

   const TYPE * * * operator &() const
   {

      return &m_pp;

   }


};

#define __comptr_array(a) &a.m_pp, &a.m_cCount

#define cotaskp(POINTER_TYPE) cotaskptr < POINTER_TYPE >



template < typename POINTER_TYPE >
inline auto __string(const cotaskptr < POINTER_TYPE > & ptr)
{

   return __string((const POINTER_TYPE)ptr);

}



//namespace str
//{
//

   inline ansistring & assign(ansistring & ansistrDst, const cotaskptr < PWSTR > & pwidesz)
   {

      ansistrDst.assign(pwidesz.operator const PWSTR());

      return ansistrDst;

   }


   inline widestring & assign(widestring & widestrDst, const cotaskptr < PSTR > & pansisz)
   {

      widestrDst.assign(pansisz.operator const PSTR());

      return widestrDst;

   }


   inline ansistring & assign(ansistring & ansistrDst, const cotaskptr < PSTR > & pansisz)
   {

      ansistrDst.assign(pansisz.operator const PSTR());

      return ansistrDst;

   }


   inline widestring & assign(widestring & widestrDst, const cotaskptr < PWSTR > & pwidesz)
   {

      widestrDst.assign(pwidesz.operator const PWSTR());

      return widestrDst;

   }
//
//
//} // namespace str



