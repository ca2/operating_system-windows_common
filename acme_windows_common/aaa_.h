#pragma once


//using hfile = HFILE;

//#include <objidl.h>

struct lib_main_int
{

   int         m_iAny = 0;
   ::duration    m_durationProcessAttach = 0;
   ::duration    m_durationProcessDetach = 0;
   ::duration    m_durationThreadAttach = 0;
   ::duration    m_durationThreadDetach = 0;

};

#ifndef CUBE

CLASS_DECL_ACME int_bool _001DefaultDllMain(hinstance hinstance, unsigned int dwReason, void * pReserved, int iLibMainDebugBox = 0, const lib_main_int & intDelayMs = {});

#endif


#include "bstring.h"


//
//
//CLASS_DECL_ACME int_bool is_windows_98_or_lesser();
//
//CLASS_DECL_ACME int_bool is_windows_nt();
//
//CLASS_DECL_ACME int_bool is_windows_nt_lesser_than_2000();
//CLASS_DECL_ACME int_bool is_windows_2000_or_greater();
//CLASS_DECL_ACME int_bool is_windows_xp_or_greater();
//CLASS_DECL_ACME int_bool is_windows_vista_or_greater();
CLASS_DECL_ACME int_bool is_windows_7_or_lower();
CLASS_DECL_ACME int_bool is_windows_8_or_greater();
//
CLASS_DECL_ACME int_bool is_windows_native_unicode();
//
//
//
//


#include "parallelization.h"




CLASS_DECL_ACME bool defer_init_winsock();




#include "comptr.h"



