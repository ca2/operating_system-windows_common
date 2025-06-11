// Created by camilo on 2025-06-10 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "direct2d_draw2d_swap_chain.h"
#include "draw2d_direct2d/graphics.h"
#include "direct2d/direct2d.h"
#include "aura/windowing/window.h"
#include "gpu_directx11/context.h"
#include "gpu_directx11/renderer.h"
#include "gpu_directx11/swap_chain_render_target_view.h"
#include "aura/graphics/gpu/graphics.h"
#include "windowing_win32/window.h"



namespace gpu_directx11
{


   //class CLASS_DECL_DRAW2D_DIRECT2D_DIRECTX12 direct2d_draw2d_swap_chain :
   //   virtual public ::particle
   //{
   //public:



   //   ::comptr <IDXGIDevice> m_pdxgidevice;

   //   ::comptr<ID2D1Device> m_pd2d1device;

   //   ::comptr<ID2D1DeviceContext> m_pd2d1context;

   //   ::comptr<IDXGISurface> m_pdxgisurface;

   //   ::comptr<ID2D1Bitmap1> m_pd2d1bitmap;


   direct2d_draw2d_swap_chain::direct2d_draw2d_swap_chain()
   {

   }


   direct2d_draw2d_swap_chain::~direct2d_draw2d_swap_chain()
   {


   }

   IDXGISurface* direct2d_draw2d_swap_chain::_get_dxgi_surface()
   {

      auto r = m_pwindow->get_window_rectangle();

      if (texDesc.Width != r.width()
         || texDesc.Height != r.height())
      {


         texDesc.Width = r.width();
         texDesc.Height = r.height();
         texDesc.MipLevels = 1;
         texDesc.ArraySize = 1;
         texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
         texDesc.SampleDesc.Count = 1;
         texDesc.Usage = D3D11_USAGE_DEFAULT;
         texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
         texDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

         m_pd3d11device->CreateTexture2D(&texDesc, nullptr, &m_ptextureShared);

         m_ptextureShared.as(m_pdxgisurface_2);

      }

      return m_pdxgisurface_2;

   }



   void direct2d_draw2d_swap_chain::initialize_direct2d_draw2d_gpu_swap_chain(::gpu::device* pgpudevice, ::windowing::window* pwindow)
   {

      m_pgpudevice = pgpudevice;

      ::cast < ::windowing_win32::window > pwin32window = pwindow;

      auto r = pwindow->get_window_rectangle();

      //// Swap chain description
      //DXGI_SWAP_CHAIN_DESC dxgiswapchaindesc = {};
      //dxgiswapchaindesc.BufferDesc.Width = r.width();
      //dxgiswapchaindesc.BufferDesc.Height = r.height();
      //dxgiswapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      //dxgiswapchaindesc.BufferCount = 2;
      //dxgiswapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      //dxgiswapchaindesc.OutputWindow = pwin32window->m_hwnd;
      //dxgiswapchaindesc.SampleDesc.Count = 1;
      //dxgiswapchaindesc.Windowed = TRUE;
      //dxgiswapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
      //dxgiswapchaindesc.Flags = 0;

      //comptr<IDXGISwapChain> swapchain;
      //comptr<ID3D11Device> device;

      //comptr<ID3D11DeviceContext> context;

      //HRESULT hr = D3D11CreateDeviceAndSwapChain(
      //   nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
      //   D3D11_CREATE_DEVICE_BGRA_SUPPORT,
      //   nullptr, 0,
      //   D3D11_SDK_VERSION, 
      //   &dxgiswapchaindesc,
      //   &swapchain,
      //   &device,
      //   &m_featurelevel,
      //   &context
      //);


      auto& pdevice = m_pgpudevice->m_pdevice;
      auto& pcontext = m_pgpudevice->m_pdevicecontext;
      auto& pdevice1 = m_pgpudevice->m_pdevice1;
      auto& pdxgidevice = m_pgpudevice->m_pdxgidevice;
      auto& pdxgifactory2 = m_pgpudevice->m_pdxgifactory2;
      auto& pdxgiswapchain1 = m_pdxgiswapchain1;

      ::defer_throw_hresult(D3D11CreateDevice(nullptr,    // Adapter
         D3D_DRIVER_TYPE_HARDWARE,
         nullptr,    // Module
         D3D11_CREATE_DEVICE_BGRA_SUPPORT,
         nullptr, 0, // Highest available feature level
         D3D11_SDK_VERSION,
         &pdevice,
         nullptr,    // Actual feature level
         nullptr));  // Device context
      ::defer_throw_hresult(pdevice.as(pdevice1));
      ::defer_throw_hresult(pdevice.as(pdxgidevice));

      ::defer_throw_hresult(CreateDXGIFactory2(
         DXGI_CREATE_FACTORY_DEBUG,
         __interface_of(pdxgifactory2)));

      DXGI_SWAP_CHAIN_DESC1 dxgiswapchaindesc1 = {};
      dxgiswapchaindesc1.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      dxgiswapchaindesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      dxgiswapchaindesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
      dxgiswapchaindesc1.BufferCount = 2;
      dxgiswapchaindesc1.SampleDesc.Count = 1;
      dxgiswapchaindesc1.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
      
      RECT rect = {};
      GetWindowRect(pwin32window->m_hwnd, &rect);
      dxgiswapchaindesc1.Width = rect.right - rect.left;
      dxgiswapchaindesc1.Height = rect.bottom - rect.top;

      HRESULT hrCreateSwapChainForComposition = 
         pdxgifactory2->CreateSwapChainForComposition(
            pdxgidevice,
            &dxgiswapchaindesc1,
            nullptr, // Don’t restrict
            &pdxgiswapchain1);

      ::defer_throw_hresult(hrCreateSwapChainForComposition);


      //assert(SUCCEEDED(hr));
      //::defer_throw_hresult(hr);

      //::defer_throw_hresult(swapchain.as(m_pdxgiswapchain));

      // Get the Direct3D 11.1 API device and context interfaces.
      //::defer_throw_hresult(device.as(m_pdevice));

      pdevice->GetImmediateContext(&pcontext);

      


#if defined(_DEBUG)

      ::directx11::defer_dxgi_debug_initialize();

#endif

      pdxgiswapchain1->GetBuffer(0, __interface_of(m_ptextureBackBuffer));

      pdevice->CreateRenderTargetView(m_ptextureBackBuffer, nullptr, &m_prendertargetviewBackBuffer);

      auto& pdcompositiondevice = m_pdcompositiondevice;
      auto& pdcompositiontarget = m_pdcompositiontarget;
      auto& pdcompositionvisual = m_pdcompositionvisual;

      ::defer_throw_hresult(DCompositionCreateDevice(
         pdxgidevice,
         __interface_of(pdcompositiondevice)));


      ::defer_throw_hresult(pdcompositiondevice->CreateTargetForHwnd(pwin32window->m_hwnd,
         true, // Top most
         &pdcompositiontarget));

      ::defer_throw_hresult(pdcompositiondevice->CreateVisual(&pdcompositionvisual));
      ::defer_throw_hresult(pdcompositionvisual->SetContent(pdxgiswapchain1));
      ::defer_throw_hresult(pdcompositiontarget->SetRoot(pdcompositionvisual));
      ::defer_throw_hresult(pdcompositiondevice->Commit());

      m_pd3d11device = pdevice;
      m_pd3d11context = pcontext;
      m_pdxgidevice_2 = pdxgidevice;

      //::comptr<ID3D11Texture2D> m_ptextureShared;


//
//      ////m_pgraphics = pgraphics;
//
//      ////m_pgpucontext = pgraphics->m_pgpucontext;
//
//      ////D3D11_TEXTURE2D_DESC desc = {};
//      ////desc.Width = g_width;
//      ////desc.Height = g_height;
//      ////desc.MipLevels = 1;
//      ////desc.ArraySize = 1;
//      ////desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
//      ////desc.SampleDesc.Count = 1;
//      ////desc.Usage = D3D11_USAGE_DEFAULT;
//      ////desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
//
//      ////g_device->CreateTexture2D(&desc, nullptr, &g_d2dSharedTex);
//      ////g_device->CreateShaderResourceView(g_d2dSharedTex.Get(), nullptr, &g_srv);
//
//      ////ComPtr<IDXGIDevice> dxgiDevice;
//      ////g_device.As(&dxgiDevice);
//
//      ////D2D1_FACTORY_OPTIONS opts = {};
//      ////D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, opts, &g_d2dFactory);
//      ////g_d2dFactory->CreateDevice(dxgiDevice.Get(), &g_d2dDevice);
//
//      ////comptr < ID2D1DeviceContext > pdevicecontextTemplate;
//
//      ////HRESULT  hr = m_pgraphics->m_pdevice->CreateDeviceContext(
//      ////   //D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
//      ////   D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
//      ////   &m_pdevicecontext);
//
//      ////if (FAILED(hr))
//      ////{
//
//      ////   throw ::hresult_exception(hr);
//
//      ////}
//
//      //// g_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &g_d2dContext);
//      //::cast < ::gpu_directx11::device > pdevice = m_pgpucontext->m_pgpudevice;
//      //::cast < ::gpu_directx11::renderer > prenderer = m_pgpucontext->m_pgpurenderer;
//      /////auto pcommandqueue = prenderer->m_pcommandqueue;
//
//
//      ////d3d11Device.As(&d3d11On12); // Query interface
//
//      //::defer_throw_hresult(D3D11On12CreateDevice(
//      //   pdevice->m_pdevice,
//      //   0,
//      //   nullptr,
//      //   0,
//      //   reinterpret_cast<IUnknown**>(&m_pcommandqueue),
//      //   1,
//      //   0,
//      //   &m_pd3d11device,
//      //   &m_pd3d11context,
//      //   nullptr
//      //));
//
//      ////::defer_throw_hresult(m_pd3d11device.As(&m_pd3d11on12)); // Query interface
//
//
//      ////::comptr<ID3D11Resource> wrappedRTV;
//      //D3D11_RESOURCE_FLAGS flags = {};
//      //flags.BindFlags = D3D11_BIND_RENDER_TARGET;
//
//      //::defer_throw_hresult(m_pd3d11on12->CreateWrappedResource(
//      //   prenderer->m_prendertargetview->current_texture()->m_presource,
//      //   &flags,
//      //   D3D12_RESOURCE_STATE_RENDER_TARGET,
//      //   D3D12_RESOURCE_STATE_PRESENT,
//      //   __interface_of(m_presourceWrappedRTV)
//      //));
//
//
//      //resources[0] = { m_presourceWrappedRTV.m_p };
//      //m_pd3d11on12->AcquireWrappedResources(resources, _countof(resources));
//
//      //::defer_throw_hresult(m_pd3d11device.as(m_pd3d11on12)); // Query interface
//
//
//      //::defer_throw_hresult(m_presourceWrappedRTV.as(m_pdxgisurface)); // Get IDXGISurface
//
//
//      ////D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
//      ////   D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
//      ////   D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
//      ////);
//
//      ////ComPtr<ID2D1Bitmap1> d2dTargetBitmap;
//      ////d2dDeviceContext->CreateBitmapFromDxgiSurface(
//      ////   dxgiSurface.Get(),
//      ////   &bitmapProps,
//      ////   &d2dTargetBitmap
//      ////);
//
//      ////// Set as render target
//      ////d2dDeviceContext->SetTarget(d2dTargetBitmap.Get());
//
//
//      ////::cast < gpu_directx12::renderer > pgpurendererOutput = m_pgraphics->end_draw_renderer_output();
//
//      ////::cast < gpu_directx12::swap_chain_render_target_view > pswapchainrendertargetview = pgpurendererOutput->m_prendertargetview;
//
//      ////pswapchainrendertargetview->m_ptextureShared.as(m_pdxgisurface);
//
//      ////D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
//      ////   D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
//      ////   D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
//      ////);
//
//      ////::cast < ::gpu_directx12::device > pgpudevice = m_pgpucontext->m_pgpudevice;
//
//      ////m_pd2d1devicecontext = ::direct2d_directx12::get()->create_d2d1_device_context(nullptr, {});
//
//      ////m_pd2d1devicecontext->CreateBitmapFromDxgiSurface(m_pdxgisurface, bitmapProps, &m_pd2d1bitmap);
//      ////
//      ////m_pd2d1devicecontext->SetTarget(m_pd2d1bitmap);
//
//      ////g_d2dContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &props, &g_d2dBitmap);
//      ////g_d2dContext->SetTarget(g_d2dBitmap.Get());
//
//      ////m_pgpucontext = pgraphics->m_pgpucontext;
//
//      ////m_pgpucontext = pgpucontextParam;
//
//      ////::direct2d_directx12_lock lock;
//
//      ////auto pdirect2d_directx12 = ::direct2d_directx12::get();
//
//      //////::user::interaction* puserinteraction = m_puserinteraction;
//
//      ////::user::interaction* puserinteraction = nullptr;
//
//      ////if (!puserinteraction)
//      ////{
//
//      ////   puserinteraction = dynamic_cast <::user::interaction*>(m_papplication->m_pacmeuserinteractionMain.m_p);
//
//      ////}
//
//      ////auto pwindow = puserinteraction->window();
//
//      ////auto rectanglePlacement = pwindow->get_window_rectangle();
//
//      ////m_pd2d1device = pdirect2d_directx12->d2d1_device(pwindow, rectanglePlacement);
//
//      ////comptr < ID2D1DeviceContext > pdevicecontextTemplate;
//
//      ////m_pd2d1device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pd2d1context);
//
//      ////ComPtr<IDXGISurface> dxgiSurface;
//      ////sharedTexture.As(&dxgiSurface);
//
//      ////D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
//      ////   D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
//      ////   D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
//      ////);
//
//      ////ComPtr<ID2D1Bitmap1> d2dBitmap;
//      ////d2dContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &bitmapProps, &d2dBitmap);
//
//      ////d2dContext->SetTarget(d2dBitmap.Get());
//      //            //ComPtr<ID2D1DeviceContext> d2dContext;
//      //      //d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dContext);
//      //      //::cast < ::gpu_directx12::device > pgpudevice = m_pgpucontext->m_pgpudevice;
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
//
//      if (FAILED(hr))
//      {
//
//         throw ::hresult_exception(hr, "Directx Device Creation Failed!");
//
//      }
//
//#if _DEBUG
//      
//      hr = m_pd3d11device->QueryInterface(__interface_of(m_pd3d11debug));
//      
//      if (FAILED(hr))
//      {
//
//         OutputDebugString(TEXT("Failed to retrieve DirectX 11 debug interface.\n"));
//
//      }
//
//#endif
//



      //::defer_throw_hresult(m_pd3d11device.As(&m_pd3d11on12)); // Query interface


      ////::comptr<ID3D11Resource> wrappedRTV;
      //D3D11_RESOURCE_FLAGS flags = {};
      //flags.BindFlags = D3D11_BIND_RENDER_TARGET;

      //::defer_throw_hresult(m_pd3d11on12->CreateWrappedResource(
      //   prenderer->m_prendertargetview->current_texture()->m_presource,
      //   &flags,
      //   D3D12_RESOURCE_STATE_RENDER_TARGET,
      //   D3D12_RESOURCE_STATE_PRESENT,
      //   __interface_of(m_presourceWrappedRTV)
      //));


      //resources[0] = { m_presourceWrappedRTV.m_p };
      //m_pd3d11on12->AcquireWrappedResources_(resources, _countof(resources));

      //::defer_throw_hresult(m_pd3d11device.as(m_pd3d11on12)); // Query interface

      //
      //::defer_throw_hresult(m_presourceWrappedRTV.as(m_pdxgisurface)); // Get IDXGISurface


      //D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
      //   D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
      //   D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
      //);

      //::comptr<ID2D1Bitmap1> d2dTargetBitmap;
      //d2dDeviceContext->CreateBitmapFromDxgiSurface(
      //   dxgiSurface.Get(),
      //   &bitmapProps,
      //   &d2dTargetBitmap
      //);

      //// Set as render target
      //d2dDeviceContext->SetTarget(d2dTargetBitmap.Get());
//m_pdxgisurface

      //::cast < gpu_directx12::renderer > pgpurendererOutput = m_pgraphics->end_draw_renderer_output();

      //::cast < gpu_directx12::swap_chain_render_target_view > pswapchainrendertargetview = pgpurendererOutput->m_prendertargetview;

      //pswapchainrendertargetview->m_ptextureShared.as(m_pdxgisurface);

      //D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
      //   D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
      //   D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
      //);

      //::cast < ::gpu_directx12::device > pgpudevice = m_pgpucontext->m_pgpudevice;

      //m_pd2d1devicecontext = ::direct2d_directx12::get()->create_d2d1_device_context(nullptr, {});

      //m_pd2d1devicecontext->CreateBitmapFromDxgiSurface(m_pdxgisurface, bitmapProps, &m_pd2d1bitmap);
      //
      //m_pd2d1devicecontext->SetTarget(m_pd2d1bitmap);

      //g_d2dContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &props, &g_d2dBitmap);
      //g_d2dContext->SetTarget(g_d2dBitmap.Get());

      //m_pgpucontext = pgraphics->m_pgpucontext;
      //
      //m_pgpucontext = pgpucontextParam;

      //::direct2d_directx12_lock lock;

      //auto pdirect2d_directx12 = ::direct2d_directx12::get();

      ////::user::interaction* puserinteraction = m_puserinteraction;

      //::user::interaction* puserinteraction = nullptr;

      //if (!puserinteraction)
      //{

      //   puserinteraction = dynamic_cast <::user::interaction*>(m_papplication->m_pacmeuserinteractionMain.m_p);

      //}

      //auto pwindow = puserinteraction->window();

      //auto rectanglePlacement = pwindow->get_window_rectangle();

      //m_pd2d1device = pdirect2d_directx12->d2d1_device(pwindow, rectanglePlacement);

      //comptr < ID2D1DeviceContext > pdevicecontextTemplate;

      //m_pd2d1device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pd2d1context);

      //ComPtr<IDXGISurface> dxgiSurface;
      //sharedTexture.As(&dxgiSurface);

      //D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
      //   D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
      //   D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
      //);

      //ComPtr<ID2D1Bitmap1> d2dBitmap;
      //d2dContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &bitmapProps, &d2dBitmap);

      //d2dContext->SetTarget(d2dBitmap.Get());
      //            ComPtr<ID2D1DeviceContext> d2dContext;
      //      d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dContext);
      //      ::cast < ::gpu_directx12::device > pgpudevice = m_pgpucontext->m_pgpudevice;


   }


   void direct2d_draw2d_swap_chain::endDraw(::draw2d_gpu::graphics* pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc)
   {

      ::draw2d_direct2d::swap_chain::endDraw(pgraphics, puserinteraction, prendererSrc);


   }

   void direct2d_draw2d_swap_chain::present()
   {
      
      m_pd3d11context->OMSetRenderTargets(1, m_prendertargetviewBackBuffer.pp(), nullptr);

		// 2. Set viewport
		D3D11_VIEWPORT vp = {};
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.Width = static_cast<float>(texDesc.Width);
		vp.Height = static_cast<float>(texDesc.Height);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
      m_pd3d11context->RSSetViewports(1, &vp);

      if (!m_pvertexshaderFullscreen
         || !m_ppixelshaderFullscreen
         || !m_pshaderresourceviewShader
         || !m_psamplerstateLinear)
      {

         _update_swap_chain(m_pd3d11device);

      }

		// 3. Bind shaders
      m_pd3d11context->VSSetShader(m_pvertexshaderFullscreen, nullptr, 0);
      m_pd3d11context->PSSetShader(m_ppixelshaderFullscreen, nullptr, 0);

		// 4. Bind SRV and sampler
      m_pd3d11context->PSSetShaderResources(0, 1, m_pshaderresourceviewShader.pp());
      m_pd3d11context->PSSetSamplers(0, 1, m_psamplerstateLinear.pp());

		// 5. Draw fullscreen triangle (no vertex buffer needed)
      m_pd3d11context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      m_pd3d11context->Draw(3, 0);

		m_pdxgiswapchain1->Present(1, 0);

   }


} // namespace gpu_directx11



