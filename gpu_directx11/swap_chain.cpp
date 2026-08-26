// Created by camilo on 2025-06-10 18:26 <3ThomasBorregaardSørensen!!
#include "platform.h"
#include "window_attachment.h"
#include "renderer.h"
#include "shader.h"
#include "swap_chain.h"
#include "texture.h"
#include "windowing_win32/window.h"
#include "bred/gpu/binding.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context_lock.h"
#include "bred/gpu/model_buffer.h"
#include "bred/gpu/texture_site.h"
#include "operating_system-windows_common/directx11/directx11.h"


CLASS_DECL_DIRECTX11 bool IsRenderDocAttached();


namespace gpu_directx11
{


   swap_chain::swap_chain()
   {

   }


   swap_chain::~swap_chain()
   {

   }


   void swap_chain::initialize_swap_chain_window(::gpu::context * pgpucontext, ::acme::windowing::window * pwindow)
   {

      ::gpu::swap_chain::initialize_swap_chain_window(pgpucontext, pwindow);

      ::cast < ::gpu_directx11::context > pdx11gpucontext = pgpucontext;

      ::cast < ::gpu_directx11::device > pdx11gpudevice = pgpucontext->m_pgpudevice;

      ::cast < ::windowing_win32::window > pwin32window = pwindow;

      auto r = pwin32window->get_window_rectangle();

      if (!pdx11gpudevice->m_pdxgifactory2)
      {

         HRESULT hr = CreateDXGIFactory2(
            DXGI_CREATE_FACTORY_DEBUG,
            __interface_of(pdx11gpudevice->m_pdxgifactory2));

         ::defer_throw_hresult(hr);

      }

      m_pdxgidevice_2 = pdx11gpudevice->_get_dxgi_device();


      if (::directx11::from_gpu_device(pgpucontext->m_pgpudevice)->use_composition())
      {

//         MessageBox(nullptr, L"CreateSwapChainForComposition", L"CreateSwapChainForComposition", MB_OK);

      DXGI_SWAP_CHAIN_DESC1 dxgiswapchaindesc1 = {};
      dxgiswapchaindesc1.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      dxgiswapchaindesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      dxgiswapchaindesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
      dxgiswapchaindesc1.BufferCount = 2;
      dxgiswapchaindesc1.SampleDesc.Count = 1;
      dxgiswapchaindesc1.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;

      RECT rect = {};
      GetWindowRect((HWND)pwin32window->_HWND(), &rect);
      dxgiswapchaindesc1.Width = rect.right - rect.left;
      dxgiswapchaindesc1.Height = rect.bottom - rect.top;



      HRESULT hrCreateSwapChainForComposition =
         pdx11gpudevice->m_pdxgifactory2->CreateSwapChainForComposition(
            m_pdxgidevice_2,
            &dxgiswapchaindesc1,
            nullptr, // Don’t restrict
            &m_pdxgiswapchain1);

      ::defer_throw_hresult(hrCreateSwapChainForComposition);
      
      }
      else
      {

         //ComPtr<IDXGIDevice> dxgiDevice;
         //device->QueryInterface(&dxgiDevice);

         //ComPtr<IDXGIAdapter> adapter;
         //dxgiDevice->GetAdapter(&adapter);

         //ComPtr<IDXGIFactory> dxgiFactory;
         //adapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);


         ::comptr<IDXGIAdapter> adapter;
         m_pdxgidevice_2->GetAdapter(&adapter);

         ::comptr<IDXGIFactory> dxgiFactory;
         adapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

         //MessageBox(nullptr, L"CreateSwapChainForHwnd", L"CreateSwapChainForHwnd", MB_OK);
         //DXGI_SWAP_CHAIN_DESC sd = {};
         //sd.BufferCount = 1;
         RECT rect = {};
         GetWindowRect((HWND) pwin32window->_HWND(), &rect);
         //sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
         //sd.BufferDesc.RefreshRate.Numerator = 60;
         //sd.BufferDesc.RefreshRate.Denominator = 1;
         //sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
         //sd.OutputWindow = ;
         // Create a DXGI_SWAP_CHAIN_DESC1
         DXGI_SWAP_CHAIN_DESC1 desc = {};
         desc.Width = rect.right - rect.left;
         desc.Height = rect.bottom - rect.top;
         desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
         desc.Stereo = FALSE;
         desc.SampleDesc.Count = 1;
         desc.SampleDesc.Quality = 0;
         desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
         desc.BufferCount = 2; // or more (2 is typical)
         desc.Scaling = DXGI_SCALING_STRETCH;
         desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
         desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
         desc.Flags = 0; // Do NOT use DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING if avoiding DComp

         // Create the swap chain for an HWND
         //ComPtr<IDXGISwapChain1> swapChain1;
         HRESULT hrCreateSwapChainForHwnd = pdx11gpudevice->m_pdxgifactory2->CreateSwapChainForHwnd(
            m_pdxgidevice_2, (HWND) pwin32window->_HWND(),
            &desc,
            nullptr,       // No fullscreen desc
            nullptr,       // No output restriction
            &m_pdxgiswapchain1);

         ::defer_throw_hresult(hrCreateSwapChainForHwnd);

      }

      m_pdxgiswapchain1.as(m_pdxgiswapchain3);

      ///m_pswapchain->initialize_gpu_swap_chain(this, pwindow);


      ::directx11::swap_chain::initialize_swap_chain_window(pgpucontext, pwindow);

   }


   void swap_chain::initialize_gpu_swap_chain(::gpu::renderer * pgpurenderer)
   {

      ::gpu::swap_chain::m_pgpurenderer = pgpurenderer;

      m_bSwapChainInitialized = true;

   }


   //void swap_chain::_update_swap_chain()
   //{

   //   ::directx11::swap_chain::_update_swap_chain();

   //}


   void swap_chain::present(::gpu::texture_site *pgputexturesite, ::gpu::command_buffer *pgpucommandbuffer)
   {

      ::cast < renderer > pgpurenderer = ::gpu::swap_chain::m_pgpurenderer;

      ::cast < ::gpu_directx11::context > pgpucontext = pgpurenderer->m_pgpucontext;

      ::gpu::context_lock context_lock(pgpucontext);

      ID3D11RenderTargetView* nullRTV[1] = { nullptr };

      pgpucontext->m_pd3d11devicecontext->OMSetRenderTargets(1, nullRTV, nullptr);

      m_size = pgpucontext->size();

      ::cast < ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;

      UINT uBackBufferIndex = 0;

      if (m_pdxgiswapchain3)
      {

         uBackBufferIndex = m_pdxgiswapchain3->GetCurrentBackBufferIndex();

      }

      auto pgpuwindowattachment = ::gpu::window_attachment::get(m_pgpurenderer->m_pgpucontext);

      pgpuwindowattachment->m_iCurrentImage = uBackBufferIndex;

      m_iSwapChainIndex = uBackBufferIndex;

      defer_construct_newø(m_ptexturesiteaSwapChain);

      auto & ptexturesiteSwapChain = m_ptexturesiteaSwapChain->atø(pgpuwindowattachment->m_iCurrentImage);

      defer_construct_newø(ptexturesiteSwapChain);

      ::pointer < ::gpu_directx11::texture > ptextureSwapChain = ptexturesiteSwapChain->gpu_texture();

      if (!ptextureSwapChain)
      {

         constructø(ptexturesiteSwapChain->m_pgputextureSite);

         ptextureSwapChain = ptexturesiteSwapChain->m_pgputextureSite;

         ptextureSwapChain->m_textureflags.m_bRenderTarget= true;

         ptextureSwapChain->m_textureflags.m_bShaderResource = false;

         ptextureSwapChain->_initialize_gpu_texture(
            pgpucontext,
            m_pdxgiswapchain1,
            (::u32) pgpuwindowattachment->m_iCurrentImage);

         //m_pdxgiswapchain1->GetBuffer(0, __interface_of(ptextureSwapChain));

      }
//
//      if (!m_pblendstateDisabled)
//      {
//
//         D3D11_BLEND_DESC blendDesc = { 0 };
//         blendDesc.RenderTarget[0].BlendEnable = FALSE;  // Disable blending
//         blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
//
//         HRESULT hr = pgpudevice->m_pd3d11device->CreateBlendState(&blendDesc, &m_pblendstateDisabled);
//         ::defer_throw_hresult(hr);
//
//      }
//      
//      {
//
//         ::f32 blendFactor[4] = { 0, 0, 0, 0 }; // Not used
//         UINT sampleMask = 0xFFFFFFFF;
//         pgpucontext->m_pcontext->OMSetBlendState(m_pblendstateDisabled, blendFactor, sampleMask);
//
//      }
//      
//      //if (!m_prendertargetviewSwapChain)
//      //{
//      // 
//      //   ::cast < ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;
//
//      //   pgpudevice->m_pd3d11device->CreateRenderTargetView(
//      //      ptextureSwapChain, nullptr, &m_prendertargetviewSwapChain);
//
//      //}
//
//      //ID3D11RenderTargetView* rendertargetviewa[] = 
//      //{
//      //   ptextureSwapChain->m_prendertargetview
//      //};
//      //
//      //pgpucontext->m_pcontext->OMSetRenderTargets(1, rendertargetviewa, nullptr);
//
//      // 2. Set viewport
//
//      if (!m_pshaderPresent)
//      {
//
//
//         const_char_pointer fullscreen_vertex_shader = R"hlsl(
//struct VSOut {
//    float4 pos : SV_POSITION;
//    float2 uv  : TEXCOORD0;
//};
//
//VSOut main(uint vid : SV_VertexID) {
//    float2 verts[3] = {
//        float2(-1, -1),
//        float2(-1, +3),
//        float2(+3, -1)
//    };
//
//    VSOut o;
//    o.pos = float4(verts[vid], 0, 1);
//
//    // Map clip-space [-1..1] to texture-space [0..1]
//    float2 uv = 0.5f * (verts[vid] + float2(1.0f, 1.0f));
//    uv.y = 1.0f - uv.y;
//    o.uv = uv;
//    return o;
//})hlsl";
//
//         const_char_pointer fullscreen_pixel_shader = R"hlsl(
//Texture2D tex : register(t0);
//SamplerState samp : register(s0);
//
//float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target {
//    return tex.Sample(samp, uv);
//}
//)hlsl";
//
//         construct_newø(m_pshaderPresent);
//
//         m_pshaderPresent->initialize_shader_with_block(
//            pgpurenderer,
//            as_block(fullscreen_vertex_shader),
//            as_block(fullscreen_pixel_shader));  
//
//      }
      //::gpu::context_lock contextlock(this);

      //dummy_model_buffer();

      //{

      //present_shader();

      //auto pcontext = gpu_context();

      //auto pgputexturesite = pcontext->m_pgpurenderer->m_pgpurendertarget2->current_texture(::gpu::current_layer(), true);

      if (0)
      {

         ::cast < ::gpu_directx11::texture > ptexture = pgputexturesite->gpu_texture();
         //D3D11_RECT rect = {};
         //rect.left = 100;
         //rect.top = 100;
         //rect.right = 200;
         //rect.bottom = 200;

         //::f32 clearColor[4] = { 0.5f * 0.5f,0.75f * 0.5f, 0.95f * 0.5f, 0.5f };

         ::f32 clearColor[4] = { 0.1f ,0.1f, 0.1f, 0.1f };

         auto prendertargetview = ptexture->m_prendertargetview;

         //::f32 clearColor[4] = { 1.f ,1.f, 1.f, 1.f };

         pgpucontext->m_pd3d11devicecontextDeferred->ClearRenderTargetView(prendertargetview, clearColor);

         //pcontext->m_pd3d11devicecontext1->ClearView(prendertargetview, clearColor, NULL, 0);

      }

      ::gpu::swap_chain::present(pgputexturesite, pgpucommandbuffer);

      //pgpucontext->m_pcontext->OMSetDepthStencilState(pgpucontext->depth_stencil_state_disabled(), 0);

      //m_pshaderBlend3->bind(nullptr, ptexturesiteSwapChain);
      //m_pshaderBlend3->bind_source(nullptr, pgputexturesite, 0);
      ////pgpucontext->m_pcontext->VSSetShader(m_pvertexshaderFullscreen, nullptr, 0);
      ////pgpucontext->m_pcontext->PSSetShader(m_ppixelshaderFullscreen, nullptr, 0);

      ////pgpucontext->m_pcontext->PSSetShaderResources(
      ////   0, 1, ptextureSwapChain->m_pshaderresourceview.pp());
      ////pgpucontext->m_pcontext->PSSetSamplers(
      ////   0, 1, ptextureSwapChain->m_psamplerstate.pp());

      //pgpucontext->clear(ptexturesiteSwapChain->gpu_texture(), ::color::transparent);

      ////D3D11_VIEWPORT vp = {};
      ////vp.TopLeftX = 0;
      ////vp.TopLeftY = 0;
      ////vp.Width = static_cast<::f32>(m_size.cx);
      ////vp.Height = static_cast<::f32>(m_size.cy);
      ////vp.MinDepth = 0.0f;
      ////vp.MaxDepth = 1.0f;
      ////pgpucontext->m_pcontext->RSSetViewports(1, &vp);


      ////D3D11_RECT rectScissor;
      ////rectScissor.left = 0;
      ////rectScissor.top = 0;
      ////rectScissor.right = m_size.cx;
      ////rectScissor.bottom = m_size.cy;

      ////pgpucontext->m_pcontext->RSSetScissorRects(1, &rectScissor);


      ////pgpucontext->m_pcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      ////pgpucontext->m_pcontext->Draw(3, 0);

      //auto rectangleImpact = pgpucontext->output_placement();

      //auto sizeRaw = pgpucontext->raw_size();

      //auto size = rectangleImpact.size();

      //pgpucommandbuffer->set_viewport(size, size);

      //pgpucommandbuffer->set_scissor(size, size);



      //m_pshaderBlend3->set_impact_quad(rectangleImpact, sizeRaw);

      //m_pshaderBlend3->push_properties(pgpucommandbuffer);

      //pgpucommandbuffer->draw(m_pmodelbufferDummy);


      //pgpucontext->defer_unbind_shader();

      ////m_pshaderPresent->unbind(nullptr);


      ////FLOAT colorRGBA2[] = { 0.5f * 0.5f,0.75f * 0.5f, 0.95f * 0.5f, 0.5f };

      ////pgpucontext->m_pcontext->ClearRenderTargetView(ptextureSwapChain->m_prendertargetview, colorRGBA2);

      if (0)
      {

         D3D11_RECT rect = {};
         rect.left = 100;
         rect.top = 100;
         rect.right = 200;
         rect.bottom = 200;

         ::f32 clearColor[4] = { 0.5f * 0.5f,0.75f * 0.5f, 0.95f * 0.5f, 0.5f };

         pgpucontext->m_pd3d11devicecontext1->ClearView(ptextureSwapChain->m_prendertargetview, clearColor, &rect, 1);

      }

      //auto pgpuwindowattachment = ::gpu::window_attachment::get(this);

      auto iFrameCount = pgpuwindowattachment->get_frame_count();

        m_iCurrentSwapChainFrame = (m_iCurrentSwapChainFrame + 1) % iFrameCount;

        //auto iCurrentFrame3 = get_swap_chain()->m_iCurrentSwapChainFrame;


      ////{

      ////   ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
      ////   pgpucontext->m_pcontext->PSSetShaderResources(0, 1, nullSRV);
      ////   ID3D11RenderTargetView* nullRTV[1] = { nullptr };
      ////   pgpucontext->m_pcontext->OMSetRenderTargets(1, nullRTV, nullptr);
      ////   ID3D11SamplerState* nullSampler[1] = { nullptr };
      ////   pgpucontext->m_pcontext->PSSetSamplers(0, 1, nullSampler);

      ////}


   }


   void swap_chain::swap_buffers()
   {

      HRESULT hresult = m_pdxgiswapchain1->Present(1, 0);

      ::defer_throw_hresult(hresult);

   }

   
   //::gpu::texture* swap_chain::current_texture()
   //{

   //   if (!m_textureaSwapChain)
   //   {

   //      construct_newø(m_textureaSwapChain);

   //      m_textureaSwapChain->m_bRenderTarget = true;

   //      m_textureaSwapChain->m_bShaderResourceView = false;

   //      m_textureaSwapChain->_initialize_gpu_texture(
   //         m_pgpurenderer,
   //         m_pdxgiswapchain1);

   //      //m_pdxgiswapchain1->GetBuffer(0, __interface_of(m_textureaSwapChain));


   //   }

   //   return m_textureaSwapChain;

   //}



} // namespace gpu_directx11




