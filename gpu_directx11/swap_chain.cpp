// Created by camilo on 2025-06-10 18:26 <3ThomasBorregaardSørensen!!
#include "framework.h"
//#include "lock.h"
#include "renderer.h"
#include "shader.h"
#include "swap_chain.h"
#include "texture.h"
#include "windowing_win32/window.h"
#include "bred/gpu/context_lock.h"
#include "bred/gpu/context_lock.h"


CLASS_DECL_DIRECTX11 bool IsRenderDocAttached();

namespace gpu_directx11
{



   swap_chain::swap_chain()
   {

   }


   swap_chain::~swap_chain()
   {

   }


   void swap_chain::initialize_swap_chain_window(::gpu::context * pgpucontext, ::acme::windowing::window* pwindow)
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


      if (IsRenderDocAttached())
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


         //DXGI_SWAP_CHAIN_DESC sd = {};
         //sd.BufferCount = 1;
         RECT rect = {};
         GetWindowRect(pwin32window->m_hwnd, &rect);
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
            m_pdxgidevice_2,
            pwin32window->m_hwnd,
            &desc,
            nullptr,       // No fullscreen desc
            nullptr,       // No output restriction
            &m_pdxgiswapchain1);

         ::defer_throw_hresult(hrCreateSwapChainForHwnd);

      }
      else
      {

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
            pdx11gpudevice->m_pdxgifactory2->CreateSwapChainForComposition(
               m_pdxgidevice_2,
               &dxgiswapchaindesc1,
               nullptr, // Don’t restrict
               &m_pdxgiswapchain1);

         ::defer_throw_hresult(hrCreateSwapChainForComposition);

      }

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


   void swap_chain::present(::gpu::texture * pgputexture)
   {

      ::cast < renderer > pgpurenderer = ::gpu::swap_chain::m_pgpurenderer;

      ::cast < ::gpu_directx11::context > pgpucontext = pgpurenderer->m_pgpucontext;

      ::gpu::context_lock context_lock(pgpucontext);

      ID3D11RenderTargetView* nullRTV[1] = { nullptr };

      pgpucontext->m_pcontext->OMSetRenderTargets(1, nullRTV, nullptr);

      m_size = pgpucontext->m_rectangle.size();

      if (!m_ptextureSwapChain)
      {

         øconstruct_new(m_ptextureSwapChain);


         m_ptextureSwapChain->m_textureflags.m_bRenderTarget= true;

         m_ptextureSwapChain->m_textureflags.m_bShaderResource = false;

         m_ptextureSwapChain->_initialize_gpu_texture(
            pgpucontext,
            m_pdxgiswapchain1);

         //m_pdxgiswapchain1->GetBuffer(0, __interface_of(m_ptextureSwapChain));

      }

      if (!m_pblendstateDisabled)
      {

         ::cast < ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;

         D3D11_BLEND_DESC blendDesc = { 0 };
         blendDesc.RenderTarget[0].BlendEnable = FALSE;  // Disable blending
         blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

         HRESULT hr = pgpudevice->m_pd3d11device->CreateBlendState(&blendDesc, &m_pblendstateDisabled);
         ::defer_throw_hresult(hr);

      }
      
      {

         float blendFactor[4] = { 0, 0, 0, 0 }; // Not used
         UINT sampleMask = 0xFFFFFFFF;
         pgpucontext->m_pcontext->OMSetBlendState(m_pblendstateDisabled, blendFactor, sampleMask);

      }
      
      //if (!m_prendertargetviewSwapChain)
      //{
      // 
      //   ::cast < ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;

      //   pgpudevice->m_pd3d11device->CreateRenderTargetView(
      //      m_ptextureSwapChain, nullptr, &m_prendertargetviewSwapChain);

      //}

      //ID3D11RenderTargetView* rendertargetviewa[] = 
      //{
      //   m_ptextureSwapChain->m_prendertargetview
      //};
      //
      //pgpucontext->m_pcontext->OMSetRenderTargets(1, rendertargetviewa, nullptr);

      // 2. Set viewport

      if (!m_pshaderPresent)
      {


         const_char_pointer fullscreen_vertex_shader = R"hlsl(
struct VSOut {
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

VSOut main(uint vid : SV_VertexID) {
    float2 verts[3] = {
        float2(-1, -1),
        float2(-1, +3),
        float2(+3, -1)
    };

    VSOut o;
    o.pos = float4(verts[vid], 0, 1);

    // Map clip-space [-1..1] to texture-space [0..1]
    float2 uv = 0.5f * (verts[vid] + float2(1.0f, 1.0f));
    uv.y = 1.0f - uv.y;
    o.uv = uv;
    return o;
})hlsl";

         const_char_pointer fullscreen_pixel_shader = R"hlsl(
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target {
    return tex.Sample(samp, uv);
}
)hlsl";

         øconstruct_new(m_pshaderPresent);

         m_pshaderPresent->initialize_shader_with_block(
            pgpurenderer,
            as_block(fullscreen_vertex_shader),
            as_block(fullscreen_pixel_shader));  

      }


      //pgpucontext->m_pcontext->OMSetDepthStencilState(pgpucontext->depth_stencil_state_disabled(), 0);

      m_pshaderPresent->bind(nullptr, m_ptextureSwapChain);
      m_pshaderPresent->bind_source(nullptr, pgputexture, 0);
      //pgpucontext->m_pcontext->VSSetShader(m_pvertexshaderFullscreen, nullptr, 0);
      //pgpucontext->m_pcontext->PSSetShader(m_ppixelshaderFullscreen, nullptr, 0);

      //pgpucontext->m_pcontext->PSSetShaderResources(
      //   0, 1, m_ptextureSwapChain->m_pshaderresourceview.pp());
      //pgpucontext->m_pcontext->PSSetSamplers(
      //   0, 1, m_ptextureSwapChain->m_psamplerstate.pp());

      D3D11_VIEWPORT vp = {};
      vp.TopLeftX = 0;
      vp.TopLeftY = 0;
      vp.Width = static_cast<float>(m_size.cx);
      vp.Height = static_cast<float>(m_size.cy);
      vp.MinDepth = 0.0f;
      vp.MaxDepth = 1.0f;
      pgpucontext->m_pcontext->RSSetViewports(1, &vp);


      D3D11_RECT rectScissor;
      rectScissor.left = 0;
      rectScissor.top = 0;
      rectScissor.right = m_size.cx;
      rectScissor.bottom = m_size.cy;

      pgpucontext->m_pcontext->RSSetScissorRects(1, &rectScissor);


      pgpucontext->m_pcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      pgpucontext->m_pcontext->Draw(3, 0);


      m_pshaderPresent->unbind(nullptr);


      //FLOAT colorRGBA2[] = { 0.5f * 0.5f,0.75f * 0.5f, 0.95f * 0.5f, 0.5f };

      //pgpucontext->m_pcontext->ClearRenderTargetView(m_ptextureSwapChain->m_prendertargetview, colorRGBA2);

      //D3D11_RECT rect = {};
      //rect.left = 100;
      //rect.top = 100;
      //rect.right = 200;
      //rect.bottom = 200;

      //float clearColor[4] = { 0.5f * 0.5f,0.75f * 0.5f, 0.95f * 0.5f, 0.5f }; 

      //pgpucontext->m_pcontext1->ClearView(m_ptextureSwapChain->m_prendertargetview, clearColor, &rect, 1);


      {

         ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
         pgpucontext->m_pcontext->PSSetShaderResources(0, 1, nullSRV);
         ID3D11RenderTargetView* nullRTV[1] = { nullptr };
         pgpucontext->m_pcontext->OMSetRenderTargets(1, nullRTV, nullptr);
         ID3D11SamplerState* nullSampler[1] = { nullptr };
         pgpucontext->m_pcontext->PSSetSamplers(0, 1, nullSampler);

      }


   }


   void swap_chain::swap_buffers()
   {

      m_pdxgiswapchain1->Present(1, 0);

   }

   
   //::gpu::texture* swap_chain::current_texture()
   //{

   //   if (!m_ptextureSwapChain)
   //   {

   //      øconstruct_new(m_ptextureSwapChain);

   //      m_ptextureSwapChain->m_bRenderTarget = true;

   //      m_ptextureSwapChain->m_bShaderResourceView = false;

   //      m_ptextureSwapChain->_initialize_gpu_texture(
   //         m_pgpurenderer,
   //         m_pdxgiswapchain1);

   //      //m_pdxgiswapchain1->GetBuffer(0, __interface_of(m_ptextureSwapChain));


   //   }

   //   return m_ptextureSwapChain;

   //}



} // namespace gpu_directx11




