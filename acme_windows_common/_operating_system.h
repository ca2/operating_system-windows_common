#pragma once



template < prototype_payload PAYLOAD >
inline void copy(PAYLOAD * ppayload, const DWORD * pdw)
{

   ppayload->operator = ((const ::u32 &)*pdw);

}


template < prototype_payload PAYLOAD >
inline void copy(PAYLOAD * ppayload, const long * pl)
{

   ppayload->operator = ((constint &)*pl);

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
//   *ppdw = (LPDWORD) (::u32*) *ppayload;
//
//}


template < prototype_payload PAYLOAD >
inline void copy(long * pl, const PAYLOAD * ppayload)
{

   *pl = (long)ppayload->long_long();

}


template < prototype_payload PAYLOAD >
inline void copy(DWORD * pdw, const PAYLOAD * ppayload)
{

   *pdw = ppayload->::u32();

}



#ifdef WINDOWS

typedef ::u32 itask;
typedef ::u32 thread_data_index;

#else

using thread_data_index = ::u32;

#endif


#ifdef WINDOWS


typedef iptr htask;

//typedef ::u32 itask;


#define null_hthread ((htask)0)
#define null_ithread ((itask)0)


inline ::i32 ithread_equals(itask a, itask b) { return a == b; }

#else



#define htask pthread_t

#define itask pthread_t

#define null_hthread ((htask)0)
#define null_ithread ((itask)0)

inline ::i32 ithread_equals(itask a, itask b) { return pthread_equal(a, b); }


#endif

