// Created by camilo on 2021-06-05 23:48 BRT <3ThomasBS_!!
#include "framework.h"


namespace directx
{


   class directx * directx::s_pdirectx = nullptr;


   directx::directx()
   {

   }


   directx::~directx()
   {

      ::directx::dxgi_debug_terminate();

   }


   ::e_status directx::initialize(::object * pobject)
   {

      // This flag adds support for surfaces with a different color channel ordering
      // than the API default. It is required for compatibility with Direct2D.
      ::u32 creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

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
      Microsoft::WRL::ComPtr<ID3D11Device> device;

      Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

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

      ::directx::throw_if_failed(hr);

      // Get the Direct3D 11.1 API device and context interfaces.
      ::directx::throw_if_failed(device.As(&m_pd3device));

      ::directx::throw_if_failed(device.As(&m_pd3device1));

      ::directx::throw_if_failed(context.As(&m_pd3devicecontext));

      ::directx::throw_if_failed(context.As(&m_pd3devicecontext1));

      // Get the underlying DXGI device of the Direct3D device.
      ::directx::throw_if_failed(device.As(&m_pdxgidevice));

      
      ::directx::defer_dxgi_debug_initialize();


      return ::success;

   }


   ID3D11Device * directx::draw_get_d3d11_device()
   {

      return m_pd3device.Get();

   }


   ID3D11Device1 * directx::draw_get_d3d11_device1()
   {

      return m_pd3device1.Get();

   }


   ID3D11DeviceContext * directx::draw_get_d3d11_device_context()
   {

      return m_pd3devicecontext.Get();

   }


   ID3D11DeviceContext1 * directx::draw_get_d3d11_device_context1()
   {

      return m_pd3devicecontext1.Get();

   }


   IDXGIDevice * directx::draw_get_dxgi_device()
   {

      return m_pdxgidevice.Get();

   }


   typedef HRESULT WINAPI FN_DXGIGetDebugInterface(REFIID riid, void** ppDebug);


   typedef FN_DXGIGetDebugInterface* PFN_DXGIGetDebugInterface;


   CLASS_DECL_DIRECTX void defer_initialize()
   {

      if (::is_set(directx::s_pdirectx))
      {

         return;

      }

      directx::s_pdirectx = new class directx;

   }


   CLASS_DECL_DIRECTX void terminate()
   {

      ::acme::del(directx::s_pdirectx);

   }


} // namespace directx



