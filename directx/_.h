// Created by camilo on 2021-06-05 23:48 BRT <3ThomasBS_!!
#pragma once


#include "aura/_.h"
#include "aura_windows_common/_.h"


#include "acme_windows_common/comptr.h"


#include "acme/_operating_system.h"


//#include <winapifamily.h>
//#include <windows.h>
//#include <shlwapi.h>
//#include <wrl/client.h>
#include <DXGI1_2.h>
#include <D3D11.h>
#include <D3D11_1.h>
#include <Dxgi1_3.h>
#include <Initguid.h>
#include <DXGIDebug.h>


// Windows Desktop Thread Local Storage for Draw2d Direct2d plugin


interface ID3D11Device;
interface ID3D11DeviceContext;
interface ID3D11Device1;
interface IDXGIDevice;


#if defined(_directx_project)
#define CLASS_DECL_DIRECTX  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_DIRECTX  CLASS_DECL_IMPORT
#endif


namespace directx
{


   class directx;


#if defined(_DEBUG)


   CLASS_DECL_DIRECTX void defer_dxgi_debug_initialize();
   CLASS_DECL_DIRECTX void dxgi_debug_terminate();


#endif


   CLASS_DECL_DIRECTX void defer_initialize(::particle * pparticle);
   CLASS_DECL_DIRECTX void finalize();


} // namespace directx


#if defined(_DEBUG)


CLASS_DECL_DIRECTX void directx_debug();


#endif



