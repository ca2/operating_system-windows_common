#pragma once



template < primitive_payload PAYLOAD >
inline void copy(PAYLOAD * ppayload, const DWORD * pdw)
{

   ppayload->operator = ((const unsigned int &)*pdw);

}


template < primitive_payload PAYLOAD >
inline void copy(PAYLOAD * ppayload, const long * pl)
{

   ppayload->operator = ((constint &)*pl);

}


//inline void copy(payload & payload, const DWORD & dw)
//{
//
//   payload.operator = ((const unsigned int &)dw);
//
//}


//inline void __copy(LPDWORD * ppdw, const payload * ppayload)
//{
//
//   *ppdw = (LPDWORD) (unsigned int*) *ppayload;
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

   *pdw = ppayload->unsigned int();

}



#ifdef WINDOWS

typedef unsigned int itask_t;
typedef unsigned int thread_data_index;

#else

using thread_data_index = unsigned int;

#endif


#ifdef WINDOWS


typedef iptr htask_t;

//typedef unsigned int itask_t;


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

