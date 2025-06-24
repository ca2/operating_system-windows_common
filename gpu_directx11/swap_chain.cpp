// Created by camilo on 2025-06-10 18:26 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "renderer.h"
#include "shader.h"
#include "swap_chain.h"
#include "texture.h"
#include "windowing_win32/window.h"


const char* fullscreen_vertex_shader = R"hlsl(// fullscreen_vs.hlsl
      struct VSOut {
         float4 pos : SV_POSITION;
         float2 uv : TEXCOORD0;
      };

      VSOut main(uint vid : SV_VertexID) {
         float2 verts[3] = {
             float2(-1, -1),
             float2(-1, +3),
             float2(+3, -1)
         };
         float2 uvs[3] = {
             float2(0, 1),
             float2(0, -1),
             float2(2, 1)
         };

         VSOut o;
o.pos = float4(verts[vid], 0, 1);
float2 uv = 0.5 * (verts[vid] + 1.0);
o.uv = float2(uv.x, 1.0 - uv.y); // Flip Y
         return o;
      }
)hlsl";

const char* fullscreen_pixel_shader = R"hlsl(// fullscreen_ps.hlsl
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target {
    return tex.Sample(samp, uv);
}
)hlsl";


namespace gpu_directx11
{



   swap_chain::swap_chain()
   {

   }


   swap_chain::~swap_chain()
   {

   }


   void swap_chain::initialize_swap_chain_window(::gpu::device * pgpudevice, ::windowing::window* pwindow)
   {

      ::gpu::swap_chain::initialize_swap_chain_window(pgpudevice, pwindow);

      ::cast < ::gpu_directx11::device > pdx11gpudevice = pgpudevice;

      ::cast < ::windowing_win32::window > pwin32window = pwindow;

      auto r = pwin32window->get_window_rectangle();

      if (!pdx11gpudevice->m_pdxgifactory2)
      {

         HRESULT hr = CreateDXGIFactory2(
            DXGI_CREATE_FACTORY_DEBUG,
            __interface_of(pdx11gpudevice->m_pdxgifactory2));

         ::defer_throw_hresult(hr);

      }

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

      m_pdxgidevice_2 = pdx11gpudevice->_get_dxgi_device();

      HRESULT hrCreateSwapChainForComposition =
         pdx11gpudevice->m_pdxgifactory2->CreateSwapChainForComposition(
            m_pdxgidevice_2,
            &dxgiswapchaindesc1,
            nullptr, // Don’t restrict
            &m_pdxgiswapchain1);

      ::defer_throw_hresult(hrCreateSwapChainForComposition);

      ///m_pswapchain->initialize_gpu_swap_chain(this, pwindow);


      ::directx11::swap_chain::initialize_swap_chain_window(pgpudevice, pwindow);

   }


   void swap_chain::initialize_gpu_swap_chain(::gpu::renderer * pgpurenderer)
   {

      ::gpu::swap_chain::m_pgpurenderer = pgpurenderer;

      

   }


   //void swap_chain::_update_swap_chain()
   //{

   //   ::directx11::swap_chain::_update_swap_chain();

   //}


   void swap_chain::present(::gpu::texture * pgputexture)
   {

      ::cast < renderer > pgpurenderer = ::gpu::swap_chain::m_pgpurenderer;

      ::cast < ::gpu_directx11::context > pgpucontext = pgpurenderer->m_pgpucontext;

      m_size = pgpucontext->m_rectangle.size();

      if (!m_ptextureSwapChain)
      {

         __construct_new(m_ptextureSwapChain);

         m_ptextureSwapChain->m_bRenderTarget= true;

         m_ptextureSwapChain->m_bShaderResourceView = false;

         m_ptextureSwapChain->m_bDepthStencil = false;

         m_ptextureSwapChain->_initialize_gpu_texture(
            pgpurenderer,
            m_pdxgiswapchain1);

         //m_pdxgiswapchain1->GetBuffer(0, __interface_of(m_ptextureSwapChain));

      }

      if (!m_pblendstateDisabled)
      {

         ::cast < ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;

         D3D11_BLEND_DESC blendDesc = { 0 };
         blendDesc.RenderTarget[0].BlendEnable = FALSE;  // Disable blending
         blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

         HRESULT hr = pgpudevice->m_pdevice->CreateBlendState(&blendDesc, &m_pblendstateDisabled);
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

      //   pgpudevice->m_pdevice->CreateRenderTargetView(
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

         __construct_new(m_pshaderPresent);

         m_pshaderPresent->initialize_shader_with_block(
            pgpurenderer,
            as_block(fullscreen_vertex_shader),
            as_block(fullscreen_pixel_shader));  

      }

      m_pshaderPresent->bind(m_ptextureSwapChain, pgputexture);
      //pgpucontext->m_pcontext->VSSetShader(m_pvertexshaderFullscreen, nullptr, 0);
      //pgpucontext->m_pcontext->PSSetShader(m_ppixelshaderFullscreen, nullptr, 0);

      //pgpucontext->m_pcontext->PSSetShaderResources(
      //   0, 1, m_ptextureSwapChain->m_pshaderresourceview.pp());
      //pgpucontext->m_pcontext->PSSetSamplers(
      //   0, 1, m_ptextureSwapChain->m_psamplerstate.pp());

      D3D11_VIEWPORT vp = {};
      vp.TopLeftX = 0;
      vp.TopLeftY = 0;
      vp.Width = static_cast<float>(m_size.cx());
      vp.Height = static_cast<float>(m_size.cy());
      vp.MinDepth = 0.0f;
      vp.MaxDepth = 1.0f;
      pgpucontext->m_pcontext->RSSetViewports(1, &vp);


      pgpucontext->m_pcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      pgpucontext->m_pcontext->Draw(3, 0);


      m_pshaderPresent->unbind();


      //FLOAT colorRGBA2[] = { 0.5f * 0.5f,0.75f * 0.5f, 0.95f * 0.5f, 0.5f };

      //pgpucontext->m_pcontext->ClearRenderTargetView(m_ptextureSwapChain->m_prendertargetview, colorRGBA2);

      m_pdxgiswapchain1->Present(1, 0);

   }

   
   //::gpu::texture* swap_chain::current_texture()
   //{

   //   if (!m_ptextureSwapChain)
   //   {

   //      __construct_new(m_ptextureSwapChain);

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




