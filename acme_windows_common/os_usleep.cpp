#include "framework.h"
#include "acme/operating_system.h"
#include "acme/operating_system/time.h"



//bool os_usleep::sleep(unsigned long usec)
//{
//
//#ifdef WINDOWS
//   LARGE_INTEGER ft;
//
//   ft.QuadPart = -(10 * (::i64)usec);
//   SetWaitableTimer(m_hTimer, &ft, 0, NULL, NULL, 0);
//   WaitForSingleObject(m_hTimer, U32_INFINITE_TIMEOUT);
//
//   return true;
//
//#else
//
//   usleep((::u32)usec);
//
//   return true;
//
//#endif
//
//}
