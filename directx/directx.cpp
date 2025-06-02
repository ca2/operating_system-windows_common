// Created by camilo on 2021-06-05 23:48 BRT <3ThomasBS_!!
#include "framework.h"
#include "directx.h"
#include "acme_windows_common/hresult_exception.h"


namespace directx
{


   directx::directx()
   {

   }


   directx::~directx()
   {

#if defined(_DEBUG)

      ::directx::dxgi_debug_terminate();

#endif

   }


   void directx::initialize(::particle * pparticle)
   {

      // This flag adds support for surfaces with a different color channel ordering
      // than the API default. It is required for compatibility with Direct2D.
      unsigned int creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(__DEBUG)

      // If the project is in a debug build, enable debugging via SDK Layers with this flag.
      creationFlags |= D3D11_CREATE_DEVICE_DEBUG;

#endif

      // This array defines the set of DirectX hardware feature levels this app will support.
      // Note the ordering should be preserved.
      // Don't forget to declare your application's minimum required feature level in its
      // description.  All applications are assumed to support 9.1 unless otherwise stated.
      D3D_FEATURE_LEVEL featureLevels[] =
      {

         D3D_FEATURE_LEVEL_11_1,
         D3D_FEATURE_LEVEL_11_0,
         D3D_FEATURE_LEVEL_10_1,
         D3D_FEATURE_LEVEL_10_0,
         D3D_FEATURE_LEVEL_9_3,
         D3D_FEATURE_LEVEL_9_2,
         D3D_FEATURE_LEVEL_9_1

      };

      // Create the Direct3D 11 API device object and a corresponding context.
      comptr<ID3D11Device> device;

      comptr<ID3D11DeviceContext> context;

      HRESULT hr = D3D11CreateDevice(
         nullptr,                    // Specify nullptr to use the default adapter.
         D3D_DRIVER_TYPE_HARDWARE,
         0,
         creationFlags,              // Set debug and Direct2D compatibility flags.
         featureLevels,              // List of feature levels this app can support.
         ARRAYSIZE(featureLevels),
         D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Metro style apps.
         &device,                    // Returns the Direct3D device created.
         &m_featurelevel,            // Returns feature level of device created.
         &context                    // Returns the device immediate context.
      );

      ::defer_throw_hresult(hr);

      // Get the Direct3D 11.1 API device and context interfaces.
      ::defer_throw_hresult(device.as(m_pdevice));

      ::defer_throw_hresult(device.as(m_pdevice1));

      ::defer_throw_hresult(context.as(m_pdevicecontext));

      ::defer_throw_hresult(context.as(m_pdevicecontext1));

      // Get the underlying DXGI device of the Direct3D device.
      ::defer_throw_hresult(device.as(m_pdxgidevice));

#if defined(_DEBUG)

      ::directx::defer_dxgi_debug_initialize();

#endif

      //return ::success;

   }


   ID3D11Device * directx::draw_get_d3d11_device()
   {

      return m_pdevice;

   }


   ID3D11Device1 * directx::draw_get_d3d11_device1()
   {

      return m_pdevice1;

   }


   ID3D11DeviceContext * directx::draw_get_d3d11_device_context()
   {

      return m_pdevicecontext;

   }


   ID3D11DeviceContext1 * directx::draw_get_d3d11_device_context1()
   {

      return m_pdevicecontext1;

   }


   IDXGIDevice * directx::draw_get_dxgi_device()
   {

      return m_pdxgidevice;

   }


   typedef HRESULT WINAPI FN_DXGIGetDebugInterface(REFIID riid, void** ppDebug);


   typedef FN_DXGIGetDebugInterface* PFN_DXGIGetDebugInterface;


   //CLASS_DECL_DIRECTX void defer_initialize(::particle * pparticle)
   //{

   //   //if (::is_set(directx::s_pdirectx))
   //   //{

   //   //   return;

   //   //}

   //   directx::s_pdirectx = ___new class directx;

   //   directx::s_pdirectx->initialize(pparticle);

   //}


   //CLASS_DECL_DIRECTX void terminate()
   //{

   //   ::acme::del(directx::s_pdirectx);

   //}


} // namespace directx



