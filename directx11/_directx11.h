// Created by camilo on 2025-06-11 19:44 <3ThomasBorregaardSørensen!!
#pragma once



#ifndef ACME_OPERATING_SYSTEM
#undef USUAL_OPERATING_SYSTEM_SUPPRESSIONS
#include "acme/_operating_system.h"
#endif


#include "acme_windows_common/comptr.h"

#include <D3D11.h>
#include <dxgidebug.h>
#include <DXGI1_2.h>
#include <Dxgi1_3.h>


namespace directx11
{


   class directx11;


#if defined(_DEBUG)


   CLASS_DECL_DIRECTX11 void defer_dxgi_debug_initialize();
   CLASS_DECL_DIRECTX11 void dxgi_debug_terminate();


#endif


} // namespace directx11


#if defined(_DEBUG)


CLASS_DECL_DIRECTX11 void directx11_debug();


#endif


namespace directx11
{


   CLASS_DECL_DIRECTX11::string errorString(HRESULT errorCode);


} // namespace directx11

