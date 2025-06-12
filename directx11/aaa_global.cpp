#include "framework.h"
#include "acme/operating_system.h"
//#include "graphics.h"


#include <winapifamily.h>
#include <shlwapi.h>
#include <wrl/client.h>
#include <D2d1_1.h>


#include <DXGI1_2.h>
#include <Dwrite.h>
#include <D3D11.h>
#include <D3D11_1.h>
#include <Dxgi1_3.h>
#include <Initguid.h>
#include <DXGIDebug.h>

#include "global.h"


#define d2d1_fax_options D2D1_FACTORY_OPTIONS // fax of merde
#define d2d1_thread_model D2D1_FACTORY_TYPE_MULTI_THREADED // ???? muliple performance multi thread hidden option there exists cost uses?

//
//namespace draw2d
//{
//
//
//   ::mutex * lock::g_pmutex = nullptr;
//
//} // namespace draw2d
//
//

// void dpi_os_initialize();



namespace directx11
{



   comptr<IDWriteFactory>      plugin::s_pwritefactory;
   comptr<ID2D1Factory1>       plugin::s_pd2factory;


    //plugin * g_pdirectx11plugin = nullptr;
   //CLASS_DECL_DIRECTX11 void directx11_finalize()
   //{

   //   //delete g_pdirectx11plugin;

   //   delete g_pdxgidebug;

   //}


   //CLASS_DECL_DIRECTX11 void defer_directx11_initialize()
   //{


   //}


   void plugin::initialize(::particle * pparticle)
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

      ::dx::throw_if_failed(hr);


      // Get the Direct3D 11.1 API device and context interfaces.
      ::dx::throw_if_failed(
      device.As(&m_pd3device)
      );

      ::dx::throw_if_failed(
      device.As(&m_pd3device1)
      );

      ::dx::throw_if_failed(
      context.As(&m_pd3devicecontext)
      );

      ::dx::throw_if_failed(
      context.As(&m_pd3devicecontext1)
      );

      // Get the underlying DXGI device of the Direct3D device.
      ::dx::throw_if_failed(
      device.As(&m_pdxgidevice)
      );

      // Create the Direct2D device object and a corresponding context.
      ::dx::throw_if_failed(
      d2d1_factory1()->CreateDevice(m_pdxgidevice.Get(), &m_pd2device)
      );


      d2d1_factory1()->QueryInterface(IID_PPV_ARGS(&m_d2dMultithread));

      return ::success;

   }


   //IDWriteFactory * plugin::dwrite_factory(bool bCreate)
   //{

   //   if (s_pwritefactory != nullptr || !bCreate)
   //   {

   //      return s_pwritefactory.Get();

   //   }

   //   HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &s_pwritefactory);

   //   if (FAILED(hr))
   //   {

   //      return nullptr;

   //   }

   //   return s_pwritefactory.Get();

   //}


   //ID2D1Factory1 * plugin::d2d1_factory1(bool bCreate)
   //{


   //   if (s_pd2factory != nullptr || !bCreate)
   //   {

   //      return s_pd2factory.Get();

   //   }

   //   d2d1_fax_options options;

   //   memory_set(&options, 0, sizeof(options));

   //   HRESULT hr = ::D2D1CreateFactory(d2d1_thread_model, __uuidof(ID2D1Factory1), &options, &s_pd2factory);

   //   if (FAILED(hr))
   //   {

   //      return nullptr;

   //   }

   //   //dpi_initialize(::draw2d::device_lock::g_pfactory);

   //   return s_pd2factory.Get();

   //}


   //ID3D11Device * plugin::draw_get_d3d11_device()
   //{

   //   return m_pd3device.Get();

   //}


   //ID3D11Device1 * plugin::draw_get_d3d11_device1()
   //{

   //   return m_pd3device1.Get();

   //}


   //ID3D11DeviceContext * plugin::draw_get_d3d11_device_context()
   //{

   //   return m_pd3devicecontext.Get();

   //}


   //ID3D11DeviceContext1 * plugin::draw_get_d3d11_device_context1()
   //{

   //   return m_pd3devicecontext1.Get();

   //}

   //IDXGIDevice * plugin::draw_get_dxgi_device()
   //{

   //   return m_pdxgidevice.Get();

   //}

   //ID2D1Device * plugin::draw_get_d2d1_device()
   //{

   //   return m_pd2device.Get();

   //}


} // namespace draw2d_directx11



