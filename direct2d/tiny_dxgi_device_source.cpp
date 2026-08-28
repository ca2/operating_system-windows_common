// Created by camilo on 2026-08-28 19:23 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#include "platform.h"
#include "tiny_dxgi_device_source.h"
#include "acme/_operating_system.h"
#include "operating_system-windows_common/acme_windows_common/dxgi_device_source.h"
#include <windows.h>

#include <d3d11.h>
#include <dxgi.h>

#include <d2d1_1.h>
#include <d2d1_1helper.h>

//#include <wrl/client.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "d2d1.lib")

//using Microsoft::WRL::ComPtr;
namespace direct2d
{


   class tiny_dxgi_device_source :
      virtual public ::dxgi_device_source
   {
   public:

      ::comptr<ID3D11Device> m_pd3d11device;
      ::comptr<IDXGIDevice> m_pdxgidevice;

      //ComPtr<ID2D1Factory1> m_d2dFactory;
      //ComPtr<ID2D1Device> m_d2dDevice;
      //ComPtr<ID2D1DeviceContext> m_d2dContext;

      //ComPtr<ID2D1Bitmap1> m_bitmapTarget;
      //ComPtr<ID2D1Bitmap1> m_bitmapRead;

      //UINT m_width{};
      //UINT m_height{};

      tiny_dxgi_device_source()
      {


      }


      ~tiny_dxgi_device_source() override
      {


      }


      IDXGIDevice * _get_dxgi_device() override
      {

         UINT flags =
            D3D11_CREATE_DEVICE_BGRA_SUPPORT;

         HRESULT hr =
            D3D11CreateDevice(
               nullptr,
               D3D_DRIVER_TYPE_HARDWARE,
               nullptr,
               flags,
               nullptr,
               0,
               D3D11_SDK_VERSION,
               &m_pd3d11device,
               nullptr,
               nullptr);          // no ID3D11DeviceContext needed

         if (FAILED(hr))
         {

            //
            // Optional software fallback.
            //

            hr =
               D3D11CreateDevice(
                  nullptr,
                  D3D_DRIVER_TYPE_WARP,
                  nullptr,
                  flags,
                  nullptr,
                  0,
                  D3D11_SDK_VERSION,
                  &m_pd3d11device,
                  nullptr,
                  nullptr);

            if (FAILED(hr))
            {

               ::defer_throw_hresult(hr);

               return nullptr;

            }

         }


         hr = m_pd3d11device.as(m_pdxgidevice);

         if (FAILED(hr))
         {

            ::defer_throw_hresult(hr);

            return nullptr;

         }


         return m_pdxgidevice;

      }


//         D2D1_FACTORY_OPTIONS factoryOptions{};
//
//#if defined(_DEBUG)
//
//         factoryOptions.debugLevel =
//            D2D1_DEBUG_LEVEL_INFORMATION;
//
//#endif
//
//
//         hr =
//            D2D1CreateFactory(
//               D2D1_FACTORY_TYPE_SINGLE_THREADED,
//               __uuidof(ID2D1Factory1),
//               &factoryOptions,
//               reinterpret_cast<void **>(
//                  m_d2dFactory.GetAddressOf()));
//
//         if (FAILED(hr))
//         {
//
//            return hr;
//
//         }
//
//
//         hr =
//            m_d2dFactory->CreateDevice(
//               m_dxgiDevice.Get(),
//               &m_d2dDevice);
//
//         if (FAILED(hr))
//         {
//
//            return hr;
//
//         }
//
//
//         hr =
//            m_d2dDevice->CreateDeviceContext(
//               D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
//               &m_d2dContext);
//
//         return hr;
//
//      }
   };


} // namespace direct2d




void tiny_dxgi_device_source_factory(::factory::factory * pfactory)
{
   pfactory->add_factory_item<::direct2d::tiny_dxgi_device_source, ::dxgi_device_source>();
}