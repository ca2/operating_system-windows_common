// Created by camilo on 2025-06-04 23:11 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "swap_chain.h"
#include "directx11.h"
#include "aura/windowing/window.h"
#include "bred/gpu/context.h"
#include "bred/gpu/renderer.h"
#include "bred/gpu/graphics.h"
#include "windowing_win32/_.h"
#include "acme/operating_system/windows_common/com/hresult_exception.h"
#include "windowing_win32/window.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

CLASS_DECL_DIRECTX11 bool IsRenderDocAttached()
{

   ///return true;
   
   if (GetModuleHandleA("renderdoc.dll") != nullptr)
   {

      return true;

   }
   else
   {

      return false;

      //return true;

   }

}


const_char_pointer fullscreen_vertex_shader = R"hlsl(// fullscreen_vs.hlsl
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

const_char_pointer fullscreen_pixel_shader = R"hlsl(// fullscreen_ps.hlsl
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target {
    return tex.Sample(samp, uv);
}
)hlsl";


namespace directx11
{


   swap_chain::swap_chain()
   {

   }


   swap_chain::~swap_chain()
   {


   }


   ID3D11Device* swap_chain::_get_d3d11_device()
   {

      return nullptr;

   }


   IDXGIDevice* swap_chain::_get_dxgi_device()
   {

      return m_pdxgidevice_2;

   }


   ::comptr < IDXGISurface> swap_chain::_create_dxgi_surface(const ::int_size& size)
   {

      ::comptr < IDXGISurface> pdxgisurface;
      //auto r = m_pwindow->get_window_rectangle();

      //if (texDesc.Width != r.width()
      //   || texDesc.Height != r.height())
      //{

      D3D11_TEXTURE2D_DESC texDesc = {};
      texDesc.Width = size.width();
      texDesc.Height = size.height();
      texDesc.MipLevels = 1;
      texDesc.ArraySize = 1;
      texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      texDesc.SampleDesc.Count = 1;
      texDesc.Usage = D3D11_USAGE_DEFAULT;
      texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
      texDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

      auto pd3d11device = _get_d3d11_device();

      ::comptr <ID3D11Texture2D> ptextureShared;

      pd3d11device->CreateTexture2D(&texDesc, nullptr, &ptextureShared);

      ptextureShared.as(pdxgisurface);

      //}

      return pdxgisurface;

   }


   void swap_chain::initialize_gpu_swap_chain(::gpu::renderer* pgpurenderer)
   {

      ::gpu::swap_chain::initialize_gpu_swap_chain(pgpurenderer);

      //HRESULT hrGetBuffer = m_pdxgiswapchain1->GetBuffer(
      //   0, __interface_of(m_ptextureBackBuffer));

      //::defer_throw_hresult(hrGetBuffer);

      //auto pd3d11device = _get_d3d11_device();

      //HRESULT hrCreateRenderTargetView = pd3d11device->CreateRenderTargetView(
      //   m_ptextureBackBuffer, nullptr, &m_prendertargetviewBackBuffer);

      //::defer_throw_hresult(hrCreateRenderTargetView);

      //_initialize_direct_composition(pwindow);

   }


   void swap_chain::initialize_swap_chain_window(::gpu::context* pgpucontext, ::acme::windowing::window* pwindow)
   {

      ::gpu::swap_chain::initialize_swap_chain_window(pgpucontext, pwindow);

      if (IsRenderDocAttached())
      {

         return;

      }

      ::cast < ::windowing_win32::window > pwin32window = pwindow;

      auto& pdcompositiondevice = m_pdcompositiondevice;
      auto& pdcompositiontarget = m_pdcompositiontarget;
      auto& pdcompositionvisual = m_pdcompositionvisual;

      auto pdxgidevice = _get_dxgi_device();

      HRESULT hrDCompositionCreateDevice = DCompositionCreateDevice(
         pdxgidevice,
         __interface_of(pdcompositiondevice));

      ::defer_throw_hresult(hrDCompositionCreateDevice);

      ::defer_throw_hresult(pdcompositiondevice->CreateTargetForHwnd(pwin32window->m_hwnd,
         true,
         &pdcompositiontarget));

      ::defer_throw_hresult(pdcompositiondevice->CreateVisual(&pdcompositionvisual));
      ::defer_throw_hresult(pdcompositionvisual->SetContent(m_pdxgiswapchain1));
      ::defer_throw_hresult(pdcompositiontarget->SetRoot(pdcompositionvisual));
      ::defer_throw_hresult(pdcompositiondevice->Commit());

   }


   ::string swap_chain::_fullscreen_vertex_shader_hlsl()
   {

      return fullscreen_vertex_shader;

   }


   ::string swap_chain::_fullscreen_pixel_shader_hlsl()
   {

      return fullscreen_pixel_shader;

   }


   //void swap_chain::_update_swap_chain()
   //{

   //   ID3D11Device* pdevice = _get_d3d11_device();

   //   comptr<ID3DBlob> vsBlob;
   //   comptr<ID3DBlob> psBlob;
   //   comptr<ID3DBlob> errorBlob;

   //   ::string strVertexShader(_fullscreen_vertex_shader_hlsl());
   //   
   //   HRESULT hr = D3DCompile(
   //      strVertexShader.c_str(), strVertexShader.size(),
   //      nullptr,                       // optional source name
   //      nullptr,                       // macro definitions
   //      nullptr,                       // include handler

   //      "main", "vs_5_0",
   //      0, 0,
   //      &vsBlob, &errorBlob
   //   );

   //   if (FAILED(hr)) 
   //   {

   //      if (errorBlob)
   //      {

   //         OutputDebugStringA((char*)errorBlob->GetBufferPointer());

   //      }

   //      return;

   //   }

   //   ::string strPixelShader(_fullscreen_pixel_shader_hlsl());

   //   hr = D3DCompile(
   //      strPixelShader.c_str(), strPixelShader.size(),
   //      nullptr,                       // optional source name
   //      nullptr,                       // macro definitions
   //      nullptr,                       // include handler
   //      "main", "ps_5_0",
   //      0, 0,
   //      &psBlob, &errorBlob
   //   );

   //   if (FAILED(hr)) 
   //   {

   //      if (errorBlob)
   //      {

   //         OutputDebugStringA((char*)errorBlob->GetBufferPointer());

   //      }

   //      return;

   //   }

   //   pdevice->CreateVertexShader(
   //      vsBlob->GetBufferPointer(),
   //      vsBlob->GetBufferSize(),
   //      nullptr,
   //      &m_pvertexshaderFullscreen
   //   );

   //   pdevice->CreatePixelShader(
   //      psBlob->GetBufferPointer(),
   //      psBlob->GetBufferSize(),
   //      nullptr,
   //      &m_ppixelshaderFullscreen
   //   );

   //   hr = pdevice->CreateShaderResourceView(
   //      m_ptextureShared, nullptr, &m_pshaderresourceviewShader);

   //   if (FAILED(hr)) 
   //   {

   //      OutputDebugStringA("Failed to create SRV from shared D2D texture\n");
   //      
   //      return;

   //   }

   //   D3D11_SAMPLER_DESC sampDesc = {};
   //   sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
   //   sampDesc.AddressU = sampDesc.AddressV = sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

   //   pdevice->CreateSamplerState(&sampDesc, &m_psamplerstateLinear);

   //}


} // namespace directx11



