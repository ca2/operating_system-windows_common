// Created by camilo on 2025-06-08 18:14 < 3ThomasBorregaardSørensen!!
#include "framework.h"
#include "texture.h"
#include "renderer.h"
#include "acme/graphics/image/pixmap.h"
#include "aura/graphics/image/image.h"


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


   void texture::initialize_image_texture(::gpu::renderer* prenderer, const ::int_rectangle& rectangleTarget, bool bWithDepth, const ::pointer_array < ::image::image >& imagea, enum_type etype)
   {

      if (m_rectangleTarget == rectangleTarget
         && m_pgpurenderer == prenderer)
      {

         return;

      }

      auto sizeCurrent = m_rectangleTarget.size();

      ::gpu::texture::initialize_image_texture(prenderer, rectangleTarget, bWithDepth, imagea, etype);

      if (sizeCurrent == m_rectangleTarget.size())
      {

         return;

      }


      m_texture2ddesc = {};
      // 1. Create offscreen render target texture
      m_texture2ddesc.Width = m_rectangleTarget.width();
      m_texture2ddesc.Height = m_rectangleTarget.height();
      if (m_etype == e_type_cube_map)
      {

         if (m_texture2ddesc.Width != m_texture2ddesc.Height)
         {

            throw ::exception(error_wrong_state, "width and height should be the same for a cube map texture");

         }

      }

      m_texture2ddesc.MipLevels = 1;
      if (m_etype == e_type_cube_map)
      {
         m_texture2ddesc.ArraySize =6;
      }
      else
      {
         m_texture2ddesc.ArraySize = 1;

      }
      m_texture2ddesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      m_texture2ddesc.SampleDesc.Count = 1;
      m_texture2ddesc.Usage = D3D11_USAGE_DEFAULT;
      if (m_etype == e_type_cube_map)
      {

         m_texture2ddesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

      }
      m_texture2ddesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
      if (m_bRenderTarget)
      {
         m_texture2ddesc.BindFlags |= D3D11_BIND_RENDER_TARGET;

      }

      ::cast < ::gpu_directx11::device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      auto pdevice = pgpudevice->m_pdevice;

      D3D11_SUBRESOURCE_DATA data[6];

      if (imagea.has_element())
      {

         memset(data, 0, sizeof(D3D11_SUBRESOURCE_DATA) * 6);

         if (m_etype == e_type_cube_map)
         {
            
            if (imagea.size() != 6)
            {

               throw ::exception(error_wrong_state);

            }

            for (int i = 0; i < 6; ++i)
            {

               auto pimage = imagea[i];

               data[i].pSysMem = pimage->data();  // Your RGBA image data per face

               data[i].SysMemPitch = pimage->m_iScan;

            }

         }
         else
         {

            memset(data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
            
            auto pimage = imagea.first();
            data[0].pSysMem = pimage->data();
            data[0].SysMemPitch = pimage->m_iScan;


         }

      }

      HRESULT hrCreateTexture = pdevice->CreateTexture2D(
         &m_texture2ddesc, 
         imagea.has_element() ? data : nullptr,
         &m_ptextureOffscreen);

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

      if (m_etype & ::gpu::texture::e_type_depth)
      {

         create_depth_resources();

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
      samp.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
      samp.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
      samp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
      samp.ComparisonFunc = D3D11_COMPARISON_NEVER;
      samp.MinLOD = 0;
      samp.MaxLOD = D3D11_FLOAT32_MAX;
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

      if (m_bRenderTarget)
      {

         ::cast < ::gpu_directx11::device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

         HRESULT hrCreateRenderTargetView = pgpudevice->m_pdevice->CreateRenderTargetView(
            m_ptextureOffscreen, nullptr, &m_prendertargetview);

         if (FAILED(hrCreateRenderTargetView))
         {

            throw ::hresult_exception(hrCreateRenderTargetView, "Failed to create offscreen render target view");

         }

      }

   }


   void texture::create_shader_resource_view()
   {

      ::cast < ::gpu_directx11::device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
      srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // Must match or be compatible
      if (m_etype == e_type_cube_map)
      {
         srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
      }
      else
      {
         srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
      }
      srvDesc.Texture2D.MostDetailedMip = 0;
      srvDesc.Texture2D.MipLevels = 1;


      HRESULT hrCreateShaderResourceView = pgpudevice->m_pdevice->CreateShaderResourceView(
         m_ptextureOffscreen,
         &srvDesc, &m_pshaderresourceview);

      if (FAILED(hrCreateShaderResourceView))
      {

         throw ::hresult_exception(hrCreateShaderResourceView, "Failed to create offscreen shader resource view");

      }

      D3D11_SAMPLER_DESC sampDesc = {};
      sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
      sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
      sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
      sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
      pgpudevice->m_pdevice->CreateSamplerState(&sampDesc, &m_psamplerstate);

      new_texture.set_new_texture();


   }


   void texture::create_depth_resources()
   {

      auto size = m_rectangleTarget.size();

      D3D11_TEXTURE2D_DESC depthDesc = {};
      depthDesc.Width = size.cx();
      depthDesc.Height = size.cy();
      depthDesc.MipLevels = 1;
      depthDesc.ArraySize = 1;
      int MorePrecisionNoStencil = 1;
      if (MorePrecisionNoStencil)
      {
         depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
      }
      else
      {
         depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
      }
      depthDesc.SampleDesc.Count = 1;
      depthDesc.Usage = D3D11_USAGE_DEFAULT;
      depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
      ::cast < ::gpu_directx11::device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      auto pdevice = pgpudevice->m_pdevice;

      HRESULT hrCreateTexture = pdevice->CreateTexture2D(&depthDesc, nullptr, &m_ptextureDepthStencil);

      if (FAILED(hrCreateTexture))
      {

         throw ::hresult_exception(hrCreateTexture);

      }
      D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

      if (MorePrecisionNoStencil)
      {

         dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
         dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
      }
      HRESULT hrCreateDepthStencilView = pdevice->CreateDepthStencilView(
         m_ptextureDepthStencil,
         MorePrecisionNoStencil ? &dsvDesc : nullptr, &m_pdepthstencilview);

      if (FAILED(hrCreateDepthStencilView))
      {

         throw ::hresult_exception(hrCreateDepthStencilView);

      }

      //ID3D11DepthStencilState* depthStencilState = nullptr;

      D3D11_DEPTH_STENCIL_DESC dsDesc = {};
      dsDesc.DepthEnable = TRUE;
      dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
      dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

      HRESULT hrCreateDepthStencilState = pdevice->CreateDepthStencilState(&dsDesc, &m_pdepthstencilstate);

      if (FAILED(hrCreateDepthStencilState))
      {

         throw ::hresult_exception(hrCreateDepthStencilState);

      }



      //VkFormat depthFormat = findDepthFormat();

      //m_formatDepth = depthFormat;

      //VkExtent2D extent = getExtent();

      //depthImages.resize(imageCount());
      //depthImageMemorys.resize(imageCount());
      //depthImageViews.resize(imageCount());

      //for (int i = 0; i < depthImages.size(); i++) 
      //{

      //   VkImageCreateInfo imageInfo{};
      //   imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
      //   imageInfo.imageType = VK_IMAGE_TYPE_2D;
      //   imageInfo.extent.width = extent.width;
      //   imageInfo.extent.height = extent.height;
      //   imageInfo.extent.depth = 1;
      //   imageInfo.mipLevels = 1;
      //   imageInfo.arrayLayers = 1;
      //   imageInfo.format = depthFormat;
      //   imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
      //   imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      //   imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
      //   imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      //   imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      //   imageInfo.flags = 0;

      //   m_pgpucontext->createImageWithInfo(
      //      imageInfo,
      //      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      //      depthImages[i],
      //      depthImageMemorys[i]);

      //   VkImageViewCreateInfo viewInfo{};
      //   viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      //   viewInfo.image = depthImages[i];
      //   viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      //   viewInfo.format = depthFormat;
      //   viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
      //   viewInfo.subresourceRange.baseMipLevel = 0;
      //   viewInfo.subresourceRange.levelCount = 1;
      //   viewInfo.subresourceRange.baseArrayLayer = 0;
      //   viewInfo.subresourceRange.layerCount = 1;

      //   if (vkCreateImageView(m_pgpucontext->logicalDevice(), &viewInfo, nullptr, &depthImageViews[i]) != VK_SUCCESS) 
      //   {
      //      throw ::exception(error_failed,"failed to create texture image view!");
      //   }

      //}


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


   IDXGISurface* texture::__get_dxgi_surface()
   {

      if (m_pdxgisurface)
      {

         return m_pdxgisurface;

      }

      auto hr = m_ptextureOffscreen.as(m_pdxgisurface);

      ::defer_throw_hresult(hr);

      return m_pdxgisurface;

   }


   void texture::set_pixels(const ::int_rectangle& rectangle, const void* data)
   {


   }


} // namespace gpu_directx11




