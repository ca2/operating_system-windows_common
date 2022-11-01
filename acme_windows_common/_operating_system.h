#pragma once



template < primitive_payload PAYLOAD >
inline void copy(PAYLOAD * ppayload, const DWORD * pdw)
{

   ppayload->operator = ((const ::u32 &)*pdw);

}


template < primitive_payload PAYLOAD >
inline void copy(PAYLOAD * ppayload, const long * pl)
{

   ppayload->operator = ((const::i32 &)*pl);

}


//inline void copy(payload & payload, const DWORD & dw)
//{
//
//   payload.operator = ((const ::u32 &)dw);
//
//}


//inline void __copy(LPDWORD * ppdw, const payload * ppayload)
//{
//
//   *ppdw = (LPDWORD) (u32*) *ppayload;
//
//}


template < primitive_payload PAYLOAD >
inline void copy(long * pl, const PAYLOAD * ppayload)
{

   *pl = (long)ppayload->i64();

}


template < primitive_payload PAYLOAD >
inline void copy(DWORD * pdw, const PAYLOAD * ppayload)
{

   *pdw = ppayload->u32();

}



#ifdef WINDOWS

typedef u32 itask_t;
typedef ::u32 thread_data_index;

#else

using thread_data_index = ::u32;

#endif


#ifdef WINDOWS


typedef iptr htask_t;

//typedef ::u32 itask_t;


#define null_hthread ((htask_t)0)
#define null_ithread ((itask_t)0)


inline int ithread_equals(itask_t a, itask_t b) { return a == b; }

#else



#define htask_t pthread_t

#define itask_t pthread_t

#define null_hthread ((htask_t)0)
#define null_ithread ((itask_t)0)

inline int ithread_equals(itask_t a, itask_t b) { return pthread_equal(a, b); }


#endif

