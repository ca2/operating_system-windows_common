// Created by camilo on 2025-06-04 23:11 <3ThomasBorregaardSørensen!!
#include "framework.h"
//#include "direct2d_draw2d_swap_chain_device.h"
//#include "draw2d_direct2d/graphics.h"
//#include "direct2d/direct2d.h"
//#include "aura/windowing/window.h"
//#include "gpu_directx11/context.h"
//#include "gpu_directx11/renderer.h"
//#include "gpu_directx11/swap_chain_render_target_view.h"
////#include "gpu_directx11/texture.h"
//#include "bred/gpu/graphics.h"
//
//
//namespace gpu_directx11
//{
//
//
//   //class CLASS_DECL_DRAW2D_DIRECT2D_DIRECTX12 direct2d_draw2d_swap_chain_gpu_device :
//   //   virtual public ::particle
//   //{
//   //public:
//
//
//
//   //   ::comptr <IDXGIDevice> m_pdxgidevice;
//
//   //   ::comptr<ID2D1Device> m_pd2d1device;
//
//   //   ::comptr<ID2D1DeviceContext> m_pd2d1context;
//
//   //   ::comptr<IDXGISurface> m_pdxgisurface;
//
//   //   ::comptr<ID2D1Bitmap1> m_pd2d1bitmap;
//
//
//   direct2d_draw2d_swap_chain_gpu_device::direct2d_draw2d_swap_chain_gpu_device()
//   {
//
//   }
//
//
//   direct2d_draw2d_swap_chain_gpu_device::~direct2d_draw2d_swap_chain_gpu_device()
//   {
//
//
//   }
//
//
//   void direct2d_draw2d_swap_chain_gpu_device::initialize_draw2d_connector_for_swap_chain(::windowing::window * pwindow)
//   {
//
//      //m_pgraphics = pgraphics;
//
//      //m_pgpucontext = pgraphics->m_pgpucontext;
//
//      //D3D11_TEXTURE2D_DESC desc = {};
//      //desc.Width = g_width;
//      //desc.Height = g_height;
//      //desc.MipLevels = 1;
//      //desc.ArraySize = 1;
//      //desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
//      //desc.SampleDesc.Count = 1;
//      //desc.Usage = D3D11_USAGE_DEFAULT;
//      //desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
//
//      //g_device->CreateTexture2D(&desc, nullptr, &g_d2dSharedTex);
//      //g_device->CreateShaderResourceView(g_d2dSharedTex.Get(), nullptr, &g_srv);
//
//      //ComPtr<IDXGIDevice> dxgiDevice;
//      //g_device.As(&dxgiDevice);
//
//      //D2D1_FACTORY_OPTIONS opts = {};
//      //D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, opts, &g_d2dFactory);
//      //g_d2dFactory->CreateDevice(dxgiDevice.Get(), &g_d2dDevice);
//
//      //comptr < ID2D1DeviceContext > pdevicecontextTemplate;
//
//      //HRESULT  hr = m_pgraphics->m_pdevice->CreateDeviceContext(
//      //   //D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
//      //   D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
//      //   &m_pdevicecontext);
//
//      //if (FAILED(hr))
//      //{
//
//      //   throw ::hresult_exception(hr);
//
//      //}
//
//      // g_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &g_d2dContext);
//      ::cast < ::gpu_directx11::device > pdevice = m_pgpucontext->m_pgpudevice;
//      ::cast < ::gpu_directx11::renderer > prenderer = m_pgpucontext->m_pgpurenderer;
//      ///auto pcommandqueue = prenderer->m_pcommandqueue;
//
//
//      //d3d11Device.As(&d3d11On12); // Query interface
//
//      HRESULT hr;
//
//      UINT createDeviceFlags = 0;
//#ifdef _DEBUG
//      createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
//
//      D3D_DRIVER_TYPE driverTypes[] =
//      {
//         D3D_DRIVER_TYPE_HARDWARE,
//         D3D_DRIVER_TYPE_WARP,
//         D3D_DRIVER_TYPE_REFERENCE,
//      };
//      UINT numDriverTypes = ARRAYSIZE(driverTypes);
//
//      D3D_FEATURE_LEVEL featureLevels[] =
//      {
//         D3D_FEATURE_LEVEL_11_1,
//         D3D_FEATURE_LEVEL_11_0,
//         D3D_FEATURE_LEVEL_10_1,
//         D3D_FEATURE_LEVEL_10_0,
//      };
//      UINT numFeatureLevels = ARRAYSIZE(featureLevels);
//
//      for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
//      {
//         m_edrivertype = driverTypes[driverTypeIndex];
//         hr = D3D11CreateDevice(nullptr, m_edrivertype, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
//            D3D11_SDK_VERSION, &m_pd3d11device, &m_efeaturelevel, &m_pd3d11context);
//
//         if (hr == E_INVALIDARG)
//         {
//            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
//            hr = D3D11CreateDevice(nullptr, m_edrivertype, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
//               D3D11_SDK_VERSION, &m_pd3d11device, &m_efeaturelevel, &m_pd3d11context);
//         }
//
//         if (SUCCEEDED(hr))
//            break;
//      }
//      if (FAILED(hr))
//         throw ::hresult_exception(hr, "Directx Device Creation Failed!");
//
//#if _DEBUG
//      hr = m_pdevice->QueryInterface(IID_PPV_ARGS(&d3d11debug));
//      if (FAILED(hr))
//         OutputDebugString(TEXT("Failed to retrieve DirectX 11 debug interface.\n"));
//#endif
//
//      //::defer_throw_hresult(m_pd3d11device.As(&m_pd3d11on12)); // Query interface
//
//
//      //::comptr<ID3D11Resource> wrappedRTV;
//      D3D11_RESOURCE_FLAGS flags = {};
//      flags.BindFlags = D3D11_BIND_RENDER_TARGET;
//
//      ::defer_throw_hresult(m_pd3d11on12->CreateWrappedResource(
//         prenderer->m_prendertargetview->current_texture()->m_presource,
//         &flags,
//         D3D12_RESOURCE_STATE_RENDER_TARGET,
//         D3D12_RESOURCE_STATE_PRESENT,
//         __interface_of(m_presourceWrappedRTV)
//      ));
//
//
//      resources[0] = { m_presourceWrappedRTV.m_p };
//      m_pd3d11on12->AcquireWrappedResources(resources, _countof(resources));
//
//      ::defer_throw_hresult(m_pd3d11device.as(m_pd3d11on12)); // Query interface
//
//      
//      ::defer_throw_hresult(m_presourceWrappedRTV.as(m_pdxgisurface)); // Get IDXGISurface
//
//
//      //D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
//      //   D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
//      //   D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
//      //);
//
//      //ComPtr<ID2D1Bitmap1> d2dTargetBitmap;
//      //d2dDeviceContext->CreateBitmapFromDxgiSurface(
//      //   dxgiSurface.Get(),
//      //   &bitmapProps,
//      //   &d2dTargetBitmap
//      //);
//
//      //// Set as render target
//      //d2dDeviceContext->SetTarget(d2dTargetBitmap.Get());
//
//
//      //::cast < gpu_directx12::renderer > pgpurendererOutput = m_pgraphics->end_draw_renderer_output();
//
//      //::cast < gpu_directx12::swap_chain_render_target_view > pswapchainrendertargetview = pgpurendererOutput->m_prendertargetview;
//
//      //pswapchainrendertargetview->m_ptextureShared.as(m_pdxgisurface);
//
//      //D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
//      //   D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
//      //   D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
//      //);
//
//      //::cast < ::gpu_directx12::device > pgpudevice = m_pgpucontext->m_pgpudevice;
//
//      //m_pd2d1devicecontext = ::direct2d_directx12::get()->create_d2d1_device_context(nullptr, {});
//
//      //m_pd2d1devicecontext->CreateBitmapFromDxgiSurface(m_pdxgisurface, bitmapProps, &m_pd2d1bitmap);
//      //
//      //m_pd2d1devicecontext->SetTarget(m_pd2d1bitmap);
//
//      //g_d2dContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &props, &g_d2dBitmap);
//      //g_d2dContext->SetTarget(g_d2dBitmap.Get());
//
//      //m_pgpucontext = pgraphics->m_pgpucontext;
//      
//      //m_pgpucontext = pgpucontextParam;
//
//      //::direct2d_directx12_lock lock;
//
//      //auto pdirect2d_directx12 = ::direct2d_directx12::get();
//
//      ////::user::interaction* puserinteraction = m_puserinteraction;
//
//      //::user::interaction* puserinteraction = nullptr;
//
//      //if (!puserinteraction)
//      //{
//
//      //   puserinteraction = dynamic_cast <::user::interaction*>(m_papplication->m_pacmeuserinteractionMain.m_p);
//
//      //}
//
//      //auto pwindow = puserinteraction->window();
//
//      //auto rectanglePlacement = pwindow->get_window_rectangle();
//
//      //m_pd2d1device = pdirect2d_directx12->d2d1_device(pwindow, rectanglePlacement);
//
//      //comptr < ID2D1DeviceContext > pdevicecontextTemplate;
//
//      //m_pd2d1device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pd2d1context);
//
//      //ComPtr<IDXGISurface> dxgiSurface;
//      //sharedTexture.As(&dxgiSurface);
//
//      //D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
//      //   D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
//      //   D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
//      //);
//
//      //ComPtr<ID2D1Bitmap1> d2dBitmap;
//      //d2dContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &bitmapProps, &d2dBitmap);
//
//      //d2dContext->SetTarget(d2dBitmap.Get());
//                  //ComPtr<ID2D1DeviceContext> d2dContext;
//            //d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dContext);
//            //::cast < ::gpu_directx12::device > pgpudevice = m_pgpucontext->m_pgpudevice;
//
//   }
//
//
//   void direct2d_draw2d_swap_chain_gpu_device::endDraw(::draw2d_gpu::graphics * pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc)
//   {
//
//      ::cast < ::draw2d_direct2d::graphics > pgraphicsDirect2d = pgraphics;
//
//      comptr < ID2D1Bitmap > pd2d1bitmap;
//
//      pgraphicsDirect2d->m_pbitmaprendertarget->GetBitmap(&pd2d1bitmap);
//
//      pgraphicsDirect2d->m_pdevicecontext->SetTarget(nullptr);
//
//      m_pd2d1devicecontext->BeginDraw();
//
//      D2D1_RECT_F rectfDib1;
//
//      // Save original transform
//      D2D1_MATRIX_3X2_F originalTransform;
//      m_pd2d1devicecontext->GetTransform(&originalTransform);
//
//      // Flip vertically: scale Y by -1 and translate down
//      D2D1_MATRIX_3X2_F flipY =
//         D2D1::Matrix3x2F::Scale(1.0f, -1.0f) *
//         D2D1::Matrix3x2F::Translation(0.0f, m_pgpucontext->m_rectangle.height());
//
//      m_pd2d1devicecontext->SetTransform(flipY * originalTransform);
//
//
//      rectfDib1.left = 0;
//      rectfDib1.top = 0;
//      rectfDib1.right = m_pgpucontext->m_rectangle.width();
//      rectfDib1.bottom = m_pgpucontext->m_rectangle.height();
//      D2D1_COLOR_F color;
//
//      copy(color, ::rgba(0.75, 0.75, 0.40, 0.5));
//
//      comptr< ID2D1SolidColorBrush> pd2d1solidcolorbrush1;
//
//      m_pd2d1devicecontext->CreateSolidColorBrush(color, &pd2d1solidcolorbrush1);
//      //D2D1_RECT_F r1;
//
//      //r1.left = 500;
//      //r1.top = 300;
//      //r1.right = 600;
//      //r1.bottom = 400;
//
//      //m_pd2d1devicecontext->FillRectangle(r1, pd2d1solidcolorbrush1);
//
//
//
//      m_pd2d1devicecontext->DrawImage(
//         pd2d1bitmap,
//         D2D1::Point2F(0.f, 0.f),
//         rectfDib1,
//         D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
//         D2D1_COMPOSITE_MODE_SOURCE_COPY);
//
//      // Restore transform
//      m_pd2d1devicecontext->SetTransform(originalTransform);
//      //D2D1_RECT_F r2;
//
//      //r2.left = 100;
//      //r2.top = 100;
//      //r2.right = 200;
//      //r2.bottom = 500;
//
//      //comptr< ID2D1SolidColorBrush> pd2d1solidcolorbrush;
//
//      //D2D1_COLOR_F color1;
//
//      //copy(color1, ::rgba(0.5, 0.75, 0.90, 0.5));
//
//      //m_pd2d1devicecontext->CreateSolidColorBrush(color1, &pd2d1solidcolorbrush);
//
//      //m_pd2d1devicecontext->DrawRectangle(r2, pd2d1solidcolorbrush);
//
//
//      m_pd2d1devicecontext->EndDraw();
//
//      m_pgraphics->m_pdevicecontext->SetTarget(pd2d1bitmap);
//
//      ::cast < gpu_directx12::renderer > pgpurendererOutput = m_pgraphics->end_draw_renderer_output();
//
//      ::cast < gpu_directx12::swap_chain_render_target_view > pswapchainrendertargetview = pgpurendererOutput->m_prendertargetview;
//
//      pswapchainrendertargetview->endDraw();
//
//
//      //d2dDeviceContext->BeginDraw();
////// ... Direct2D drawing calls ...
//      m_pd2d1context->EndDraw();
//
//      m_pd3d11on12->ReleaseWrappedResources(resources, _countof(resources));
//      m_pd3d11context->Flush(); // Ensure rendering is completed
//
//
//
//   }
//
//
//} // namespace gpu_directx12
//
//
//
