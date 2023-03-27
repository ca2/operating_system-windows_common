/*
Copyright (ca) 2001
Author: Konstantin Boukreev
E-mail: konstantin@mail.primorye.ru
Created: 25.12.2001 15:18:21
Version: 1.0.0

Permission to use, copy, modify, distribute and sell this software
and its documentation for any purpose is hereby granted without fee,
provided that the above copyright notice appear in all copies and
that both that copyright notice and this permission notice appear
in supporting documentation.  Konstantin Boukreev makes no representations
about the suitability of this software for any purpose.
It is provided "as is" without express or implied warranty.

*/
#include "framework.h"


#if defined(LINUX) || defined(ANDROID)

//#include <pthread.h>

#endif


#ifdef __APPLE__

void InstallUncaughtExceptionHandler();

#endif


#ifdef EXCEPTION_TRANSLATOR_USE_SIGNAL
//#include <signal.h>
#define SIG(psig) ((::exception::sig_companion *) psig)
#define m_sig (*SIG(m_psig))
#endif


#ifdef WINDOWS_DESKTOP

// 0x40010005 control C
// 0xc0000135 Dll Not Found
// 0xc0000142 Dll Initialization failed
// 0xc06d007e Module Not Found
// 0xc06d007f Procedure Not Found
// 0x40010008 control-Break



LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(
   LPTOP_LEVEL_EXCEPTION_FILTER pTopLevelExceptionFilter)

{
   __UNREFERENCED_PARAMETER(pTopLevelExceptionFilter);

   return nullptr;
}

bool PreventSetUnhandledExceptionFilter()
{
   /*HMODULE hKernel32 = LoadLibrary("kernel32.dll");
   if (hKernel32 == nullptr) return false;
   void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
   if(pOrgEntry ==  nullptr) return false;
   uchar newJump[ 100 ];
   u32 dwOrgEntryAddr = (u32) pOrgEntry;
   dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far
   void *pNewFunc = &MyDummySetUnhandledExceptionFilter;
   u32 dwNewEntryAddr = (u32) pNewFunc;
   u32 dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;
   newJump[ 0 ] = 0xE9;  // JMP absolute
   ::memory_copy(&newJump[ 1 ], &dwRelativeAddr, sizeof(pNewFunc));
   SIZE_T bytesWritten;
   bool bRet = WriteProcessMemory(GetCurrentProcess(),
   pOrgEntry, newJump, sizeof(pNewFunc) + 1, &bytesWritten);
   return bRet;*/
   return false;
}

#endif



//namespace exception
//{


#ifdef EXCEPTION_TRANSLATOR_USE_SIGNAL


   struct sig_companion
   {

      struct sigaction m_saSeg;
      struct sigaction m_saFpe;
      struct sigaction m_saPipe;
      struct sigaction m_saSegOld;
      struct sigaction m_saFpeOld;
      struct sigaction m_saPipeOld;

   };


#endif


//#else

#ifdef LINUX
   void filter_sigsegv(i32 signal, siginfo_t * psiginfo, void * pc)
   {

      //      sigset_t set;
      //      sigemptyset(&set);
      //      sigaddset(&set, SIGSEGV);
      //      pthread_sigmask(SIG_UNBLOCK, &set, nullptr);

      throw ::exception(standard_access_violation(signal, psiginfo, pc));

   }


   void filter_sigfpe(i32 signal, siginfo_t * psiginfo, void * pc)
   {

      //sigset_t set;
      //sigemptyset(&set);
      //sigaddset(&set, SIGSEGV);
      //sigprocmask(SIG_UNBLOCK, &set, nullptr);

      throw ::exception(standard_sigfpe(signal, psiginfo, pc));

   }


   void filter_sigpipe(i32 signal, siginfo_t * psiginfo, void * pc)
   {

      //      sigset_t set;
      //      sigemptyset(&set);
      //      sigaddset(&set, SIGSEGV);
      //      sigprocmask(SIG_UNBLOCK, &set, nullptr);

      //       throw ::exception(standard_sigfpe(nullptr, signal, psiginfo, pc));

   }


#endif


   /*

      the_standard_translator the_standard_translator::s_thesetranslator;

      the_standard_translator::the_standard_translator()
      {

         attach();

      }

      the_standard_translator::~the_standard_translator()
      {

         detach();

      }

   */

//
//} // namespace exception


extern bool g_bExiting;


#ifndef WINDOWS


void * standard_exception::siginfodup(void * psiginfo)
{

   siginfo_t * psiginfoDup = new siginfo_t;

   *psiginfoDup = *(siginfo_t *)psiginfo;

   return psiginfoDup;

}


void standard_exception::siginfofree(void * psiginfo)
{

   delete (siginfo_t *)psiginfo;

}


u32 standard_exception::code() const
{

   return ((siginfo_t *)m_psiginfo)->si_code;

}


void * standard_exception::address() const
{

   return ((siginfo_t *)m_psiginfo)->si_addr;

}


const void * standard_exception::info() const
{

   return m_psiginfo;

}


const char * standard_exception::name() const
{

   //return ::exception_translator::name(code());
   return "";

}


const char * standard_exception::description() const
{

   //return ::exception_translator::description(code());
   return "";

}


#ifndef ANDROID


const ucontext_t * standard_exception::context() const
{

   return &m_ucontext;

}


#endif
#endif


