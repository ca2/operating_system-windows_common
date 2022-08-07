// Created by camilo on 2021-06-05 23:48 BRT <3ThomasBS_!!
#pragma once


#include "aura_windows_common/_library.h"
#include "aura/operating_system.h"
#include "aura/user/user/_user.h"


#include <winapifamily.h>
#include <windows.h>
#include <shlwapi.h>
#include <wrl/client.h>
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


#if defined(_DEBUG)


namespace directx
{


   CLASS_DECL_DIRECTX void defer_dxgi_debug_initialize();
   CLASS_DECL_DIRECTX void dxgi_debug_terminate();


} // namespace dx


CLASS_DECL_DIRECTX void directx_debug();


#endif


//#include "object.h"


//#include "graphics.h"


#include "directx.h"



