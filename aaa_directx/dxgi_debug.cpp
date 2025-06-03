// Created by camilo on 2021-06-05 23:48 BRT <3ThomasBS_!!
#include "framework.h"
#include "dxgi_debug.h"


#if defined(_DEBUG)


namespace directx
{


   class dxgi_debug* dxgi_debug::s_pdxgidebug = nullptr;


   dxgi_debug::dxgi_debug()
   {

// #ifdef UNIVERSAL_WINDOWS

      HRESULT hr = DXGIGetDebugInterface1(0, __interface_of(m_pdxgidebug1));

      HRESULT hrQueryInterface = m_pdxgidebug1.as(m_pdxgidebug);

// #else

//       comptr<IDXGIInfoQueue> dxgiInfoQueue;

//       typedef HRESULT(WINAPI* LPDXGIGETDEBUGINTERFACE)(REFIID, void**);

//       HMODULE hmoduleDXGIDebug = LoadLibraryExW(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

//       if (hmoduleDXGIDebug)
//       {

//          auto pfnDXGIGetDebugInterface = reinterpret_cast<LPDXGIGETDEBUGINTERFACE>(
//             reinterpret_cast<void*>(GetProcAddress(hmoduleDXGIDebug, "DXGIGetDebugInterface")));

//          if (SUCCEEDED(pfnDXGIGetDebugInterface(__interface_of(m_pdxgidebug))))
//          {

//          }

//          //d.Attach(dxgiGetDebugInterface);
//          //if (SUCCEEDED(dxgiGetDebugInterface(IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
//          //{
//          // dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
//          //dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
//          //}

//       }

// #endif

   }


   void dxgi_debug::debug()
   {

      ::output_debug_string("directx::dxgi_debug : IDXGIDebug::ReportLiveObjects");

      m_pdxgidebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);

   }


   CLASS_DECL_DIRECTX void defer_dxgi_debug_initialize()
   {

      if (::is_set(dxgi_debug::s_pdxgidebug))
      {

         return;

      }

      dxgi_debug::s_pdxgidebug = ___new dxgi_debug;

   }


   CLASS_DECL_DIRECTX void dxgi_debug_terminate()
   {

      ::acme::del(dxgi_debug::s_pdxgidebug);

   }


} // namespace directx


CLASS_DECL_DIRECTX void directx_debug()
{

   ::directx::dxgi_debug::s_pdxgidebug->debug();

}
//
//
//
//CLASS_DECL_DIRECT2D void directx_debug()
//{
//
//   g_pdxgidebug->debug();
//
//}


#endif



