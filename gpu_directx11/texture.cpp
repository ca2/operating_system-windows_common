// Created by camilo on 2025-06-08 18:14 < 3ThomasBorregaardSørensen!!
#include "framework.h"
#include "texture.h"
#include "renderer.h"


namespace gpu_directx11
{


   texture::texture()
   {

      new_texture.set_new_texture();
      //m_bCreateRenderTargetView = false;
      m_bShaderResourceView = false;

   }


   texture::~texture()
   {

   }


   void texture::initialize_gpu_texture(::gpu::renderer* prenderer, const ::int_rectangle& rectangleTarget) //, bool bCreateRenderTargetView, bool bCreateShaderResourceView)
   {

      ::gpu::texture::initialize_gpu_texture(prenderer, rectangleTarget);

      // 1. Create offscreen render target texture
      D3D11_TEXTURE2D_DESC texDesc = {};
      texDesc.Width = rectangleTarget.width();
      texDesc.Height = rectangleTarget.height();
      texDesc.MipLevels = 1;
      texDesc.ArraySize = 1;
      texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      texDesc.SampleDesc.Count = 1;
      texDesc.Usage = D3D11_USAGE_DEFAULT;
      texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

      ::cast < ::gpu_directx11::device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      auto pdevice = pgpudevice->m_pdevice;

      HRESULT hrCreateTexture = pdevice->CreateTexture2D(&texDesc, nullptr, &m_ptextureOffscreen);

      if (FAILED(hrCreateTexture))
      {

         throw ::hresult_exception(hrCreateTexture, "Failed to create offscreen texture");

      }

      if(m_bRenderTarget)
      {
         create_render_target_view();
      }

      if( m_bShaderResourceView)
      {
         create_shader_resource_view();
      }  

      //HRESULT hrCreateRenderTargetView = pdevice->CreateRenderTargetView(m_ptextureOffscreen, nullptr, &m_prendertargetview);

      //if (FAILED(hrCreateRenderTargetView))
      //{

      //   throw ::hresult_exception(hrCreateRenderTargetView, "Failed to create offscreen render target view");

      //}

      //HRESULT hrCreateShaderResourceView = pdevice->CreateShaderResourceView(m_ptextureOffscreen, nullptr, &m_pshaderresourceview);

      //if (FAILED(hrCreateShaderResourceView))
      //{

      //   throw ::hresult_exception(hrCreateShaderResourceView, "Failed to create offscreen shader resource view");

      //}
      D3D11_SAMPLER_DESC samp = {};
      samp.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
      samp.AddressU = samp.AddressV = samp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
      pdevice->CreateSamplerState(&samp, &m_psamplerstate);

      new_texture.set_new_texture();

      //if (bCreateRenderTargetView)
      //{

      //   //// 2. Create RTV descriptor heap
      //   //D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
      //   //rtvHeapDesc.NumDescriptors = 1;
      //   //rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
      //   //rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
      //   //HRESULT hrCreateDescriptorHeap = pdevice->m_pdevice->CreateDescriptorHeap(&rtvHeapDesc, __interface_of(m_pheapRenderTargetView));

      //   //pdevice->defer_throw_hresult(hrCreateDescriptorHeap);

      //   //// 3. Create RTV
      //   //m_handleRenderTargetView = m_pheapRenderTargetView->GetCPUDescriptorHandleForHeapStart();
      //   CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

      //   pdevice->m_pdevice->CreateRenderTargetView(m_presource, nullptr, m_handleRenderTargetView);

      //}

      //if (bCreateShaderResourceView)
      //{

      //   //// 4. Create SRV descriptor heap
      //   //D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
      //   //srvHeapDesc.NumDescriptors = 1;
      //   //srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
      //   //srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
      //   //
      //   //HRESULT hrCreateDescriptorHeap = pdevice->m_pdevice->CreateDescriptorHeap(&srvHeapDesc, __interface_of(m_pheapShaderResourceView));

      //   //pdevice->defer_throw_hresult(hrCreateDescriptorHeap);

      //   //// 5. Create SRV
      //   //D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
      //   //srvDesc.Format = format;
      //   //srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
      //   //srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
      //   //srvDesc.Texture2D.MostDetailedMip = 0;
      //   //srvDesc.Texture2D.MipLevels = 1;

      //   //m_handleShaderResourceView = m_pheapShaderResourceView->GetCPUDescriptorHandleForHeapStart();

      //   //pdevice->m_pdevice->CreateShaderResourceView(m_presource, &srvDesc, m_handleShaderResourceView);

      //}

   }


   void texture::_initialize_gpu_texture(::gpu::renderer* prenderer, IDXGISwapChain1* pdxgiswapchain1)
   {

      m_pgpurenderer = prenderer;

      ::cast < ::gpu_directx11::device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      auto pdevice = pgpudevice->m_pdevice;

      HRESULT hrCreateTexture = pdxgiswapchain1->GetBuffer(0, __interface_of(m_ptextureOffscreen));

      if (FAILED(hrCreateTexture))
      {

         throw ::hresult_exception(hrCreateTexture, "Failed to create offscreen texture");

      }

      if (m_bRenderTarget)
      {

         create_render_target_view();

      }

      if (m_bShaderResourceView)
      {

         create_shader_resource_view();

      }

      //HRESULT hrCreateShaderResourceView = pdevice->CreateShaderResourceView(m_ptextureOffscreen, nullptr, &m_pshaderresourceview);

      //if (FAILED(hrCreateShaderResourceView))
      //{

      //   throw ::hresult_exception(hrCreateShaderResourceView, "Failed to create offscreen shader resource view");

      //}
      D3D11_SAMPLER_DESC samp = {};
      samp.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
      samp.AddressU = samp.AddressV = samp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
      pdevice->CreateSamplerState(&samp, &m_psamplerstate);

      new_texture.set_new_texture();

      //if (bCreateRenderTargetView)
      //{

      //   //// 2. Create RTV descriptor heap
      //   //D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
      //   //rtvHeapDesc.NumDescriptors = 1;
      //   //rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
      //   //rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
      //   //HRESULT hrCreateDescriptorHeap = pdevice->m_pdevice->CreateDescriptorHeap(&rtvHeapDesc, __interface_of(m_pheapRenderTargetView));

      //   //pdevice->defer_throw_hresult(hrCreateDescriptorHeap);

      //   //// 3. Create RTV
      //   //m_handleRenderTargetView = m_pheapRenderTargetView->GetCPUDescriptorHandleForHeapStart();
      //   CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

      //   pdevice->m_pdevice->CreateRenderTargetView(m_presource, nullptr, m_handleRenderTargetView);

      //}

      //if (bCreateShaderResourceView)
      //{

      //   //// 4. Create SRV descriptor heap
      //   //D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
      //   //srvHeapDesc.NumDescriptors = 1;
      //   //srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
      //   //srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
      //   //
      //   //HRESULT hrCreateDescriptorHeap = pdevice->m_pdevice->CreateDescriptorHeap(&srvHeapDesc, __interface_of(m_pheapShaderResourceView));

      //   //pdevice->defer_throw_hresult(hrCreateDescriptorHeap);

      //   //// 5. Create SRV
      //   //D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
      //   //srvDesc.Format = format;
      //   //srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
      //   //srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
      //   //srvDesc.Texture2D.MostDetailedMip = 0;
      //   //srvDesc.Texture2D.MipLevels = 1;

      //   //m_handleShaderResourceView = m_pheapShaderResourceView->GetCPUDescriptorHandleForHeapStart();

      //   //pdevice->m_pdevice->CreateShaderResourceView(m_presource, &srvDesc, m_handleShaderResourceView);

      //}

   }


   void texture::create_render_target_view()
   {

      //m_bRenderTarget = true;

      ::cast < ::gpu_directx11::device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      HRESULT hrCreateRenderTargetView = pgpudevice->m_pdevice->CreateRenderTargetView(m_ptextureOffscreen, nullptr, &m_prendertargetview);

      if (FAILED(hrCreateRenderTargetView))
      {

         throw ::hresult_exception(hrCreateRenderTargetView, "Failed to create offscreen render target view");

      }

   }


   void texture::create_shader_resource_view()
   {

      ::cast < ::gpu_directx11::device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      HRESULT hrCreateShaderResourceView = pgpudevice->m_pdevice->CreateShaderResourceView(m_ptextureOffscreen, nullptr, &m_pshaderresourceview);
      
      if (FAILED(hrCreateShaderResourceView))
      {
         
         throw ::hresult_exception(hrCreateShaderResourceView, "Failed to create offscreen shader resource view");

      }

   }


   //void texture::_new_state(ID3D12GraphicsCommandList* pcommandlist, D3D12_RESOURCE_STATES estateNew)
   //{

   //   if (m_estate != estateNew)
   //   {

   //      // Transition to copy source
   //      D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
   //         m_presource,
   //         m_estate, // Adjust if needed
   //         estateNew
   //      );

   //      pcommandlist->ResourceBarrier(1, &barrier);

   //      m_estate = estateNew;

   //      //pcommandBuffer->submit_command_buffer();

   //   }

   //}


   //class texture::d3d11* texture::d3d11()
   //{

   //   if (!m_pd3d11)
   //   {


   //   }

   //   return m_pd3d11;

   //}


   void texture::blend(::gpu::texture* ptexture)
   {

      

   }


} // namespace gpu_directx11




