// Created by camilo on 2025-06-08 18:14 < 3ThomasBorregaardSørensen!!
#include "framework.h"
#include "texture.h"
#include <stb/stb_image.h>
#include "acme/graphics/image/pixmap.h"
#include "aura/graphics/image/image.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context_lock.h"
#include "renderer.h"


namespace gpu_directx11
{


   texture::texture()
   {

      new_texture.set_new_texture();
      // m_bCreateRenderTargetView = false;
      //m_bShaderResourceView = false;
   }


   texture::~texture() {}


   void texture::_create_texture(const ::gpu::texture_data & texturedata)
   {

            m_texture2ddesc = {};
      // 1. Create offscreen render target texture
      m_texture2ddesc.Width = this->width();
      m_texture2ddesc.Height = this->height();
      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {

         if (m_texture2ddesc.Width != m_texture2ddesc.Height)
         {

            throw ::exception(error_wrong_state, "width and height should be the same for a cube map_base texture");
         }
      }
      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {
         m_texture2ddesc.ArraySize = 6;
      }
      else
      {
         m_texture2ddesc.ArraySize = 1;
      }
      if (m_textureattributes.m_iChannelCount == 2)
      {
         if (m_textureattributes.m_iFloat >= 1)
         {

            m_texture2ddesc.Format = DXGI_FORMAT_R32G32_FLOAT;
         }
         else
         {
            m_texture2ddesc.Format = DXGI_FORMAT_R8G8_UNORM;
         }
      }
      else if (m_textureattributes.m_iFloat >= 1)
      {
         m_texture2ddesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
      }
      else
      {
         m_texture2ddesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      }
      m_texture2ddesc.SampleDesc.Count = 1;
      m_texture2ddesc.Usage = D3D11_USAGE_DEFAULT;
      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {

         m_texture2ddesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
      }
      if (m_textureattributes.m_iMipCount < 0 
         || 
         (m_textureattributes.maximum_mip_count() > 1
            && m_textureattributes.m_iMipCount == m_textureattributes.maximum_mip_count()))
      {


         m_texture2ddesc.MipLevels = 0;
         m_texture2ddesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
         m_texture2ddesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
         m_texture2ddesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
      }
      else if (m_textureattributes.m_iMipCount > 1)
      {

         m_texture2ddesc.MipLevels = m_textureattributes.m_iMipCount;
         m_texture2ddesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
         m_texture2ddesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
         m_texture2ddesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
      }
      else
      {

         m_texture2ddesc.MipLevels = 1;
      }

      m_texture2ddesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
      if (m_textureflags.m_bRenderTarget)
      {
         m_texture2ddesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
      }

      // if (imagea.is_empty() && !m_bRenderTarget)
      //{

      //   m_texture2ddesc.Usage = D3D11_USAGE_DYNAMIC;
      //   m_texture2ddesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;

      //}

      ::cast<::gpu_directx11::device> pgpudevice = m_pgpucontext->m_pgpudevice;

      auto pdevice = pgpudevice->m_pd3d11device;

      D3D11_SUBRESOURCE_DATA subresourcedata[6]{};

      if (texturedata.is_image_array())
      {

         const auto &imagea = texturedata.imagea();

         if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
         {

            if (imagea.size() != 6)
            {

               throw ::exception(error_wrong_state);
            }

            for (int i = 0; i < 6; ++i)
            {

               auto pimage = imagea[i];

               subresourcedata[i].pSysMem = pimage->data(); // Your RGBA image data per face

               subresourcedata[i].SysMemPitch = pimage->m_iScan;
            }
         }
         else
         {

            auto pimage = imagea.first();
            subresourcedata[0].pSysMem = pimage->data();
            subresourcedata[0].SysMemPitch = pimage->m_iScan;

         }

      }

      HRESULT hrCreateTexture =
         pdevice->CreateTexture2D(&m_texture2ddesc, subresourcedata[0].pSysMem ? subresourcedata : nullptr, &m_ptextureOffscreen);

      if (FAILED(hrCreateTexture))
      {

         throw ::hresult_exception(hrCreateTexture, "Failed to create offscreen texture");

      }

   }


   // void texture::initialize_texture(::gpu::renderer *prenderer, const ::int_rectangle &rectangleTarget,
   //                                        bool bWithDepth, const ::pointer_array<::image::image> * pimagea,
   //                                        enum_type etype)
   // {
   //
   //    if (m_ptextureOffscreen && m_rectangleTarget == rectangleTarget && m_pgpurenderer == prenderer)
   //    {
   //
   //       return;
   //    }
   //
   //    auto sizeCurrent = m_rectangleTarget.size();
   //
   //    ::gpu::texture::initialize_image_texture(prenderer, rectangleTarget, bWithDepth, pimagea, etype);
   //
   //    if (m_ptextureOffscreen && sizeCurrent == m_rectangleTarget.size())
   //    {
   //
   //       return;
   //    }
   //
   //
   //    create_image(pimagea);
   //
   //    if (m_bRenderTarget)
   //    {
   //
   //       create_render_target();
   //
   //    }
   //
   //    if (m_bShaderResourceView)
   //    {
   //
   //       create_shader_resource_view();
   //
   //    }
   //
   //    if (m_etexture & ::gpu::e_texture_depth)
   //    {
   //
   //       create_depth_resources();
   //
   //    }
   //
   //    // HRESULT hrCreateRenderTargetView = pdevice->CreateRenderTargetView(m_ptextureOffscreen, nullptr,
   //    // &m_prendertargetview);
   //
   //    // if (FAILED(hrCreateRenderTargetView))
   //    //{
   //
   //    //   throw ::hresult_exception(hrCreateRenderTargetView, "Failed to create offscreen render target view");
   //
   //    //}
   //
   //    // HRESULT hrCreateShaderResourceView = pdevice->CreateShaderResourceView(m_ptextureOffscreen, nullptr,
   //    // &m_pshaderresourceview);
   //
   //    // if (FAILED(hrCreateShaderResourceView))
   //    //{
   //
   //    //   throw ::hresult_exception(hrCreateShaderResourceView, "Failed to create offscreen shader resource view");
   //
   //    //}
   //
   //    ::cast<::gpu_directx11::device> pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;
   //
   //    auto pdevice = pgpudevice->m_pd3d11device;
   //
   //    D3D11_SAMPLER_DESC samp = {};
   //    samp.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
   //    samp.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
   //    samp.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
   //    samp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
   //    samp.ComparisonFunc = D3D11_COMPARISON_NEVER;
   //    samp.MinLOD = 0;
   //    samp.MaxLOD = D3D11_FLOAT32_MAX;
   //    pdevice->CreateSamplerState(&samp, &m_psamplerstate);
   //
   //    new_texture.set_new_texture();
   //
   //    // if (bCreateRenderTargetView)
   //    //{
   //
   //    //   //// 2. Create RTV descriptor heap
   //    //   //D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
   //    //   //rtvHeapDesc.NumDescriptors = 1;
   //    //   //rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
   //    //   //rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
   //    //   //HRESULT hrCreateDescriptorHeap = pdevice->m_pdevice->CreateDescriptorHeap(&rtvHeapDesc,
   //    //   __interface_of(m_pheapRenderTargetView));
   //
   //    //   //pdevice->defer_throw_hresult(hrCreateDescriptorHeap);
   //
   //    //   //// 3. Create RTV
   //    //   //m_handleRenderTargetView = m_pheapRenderTargetView->GetCPUDescriptorHandleForHeapStart();
   //    //   CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
   //
   //    //   pdevice->m_pdevice->CreateRenderTargetView(m_presource, nullptr, m_handleRenderTargetView);
   //
   //    //}
   //
   //    // if (bCreateShaderResourceView)
   //    //{
   //
   //    //   //// 4. Create SRV descriptor heap
   //    //   //D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
   //    //   //srvHeapDesc.NumDescriptors = 1;
   //    //   //srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
   //    //   //srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
   //    //   //
   //    //   //HRESULT hrCreateDescriptorHeap = pdevice->m_pdevice->CreateDescriptorHeap(&srvHeapDesc,
   //    //   __interface_of(m_pheapShaderResourceView));
   //
   //    //   //pdevice->defer_throw_hresult(hrCreateDescriptorHeap);
   //
   //    //   //// 5. Create SRV
   //    //   //D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
   //    //   //srvDesc.Format = format;
   //    //   //srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
   //    //   //srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
   //    //   //srvDesc.Texture2D.MostDetailedMip = 0;
   //    //   //srvDesc.Texture2D.MipLevels = 1;
   //
   //    //   //m_handleShaderResourceView = m_pheapShaderResourceView->GetCPUDescriptorHandleForHeapStart();
   //
   //    //   //pdevice->m_pdevice->CreateShaderResourceView(m_presource, &srvDesc, m_handleShaderResourceView);
   //
   //    //}
   // }


   void texture::_initialize_gpu_texture(::gpu::context *pgpucontext, IDXGISwapChain1 *pdxgiswapchain1)
   {

      //m_pgpurenderer = prenderer;

      m_pgpucontext = pgpucontext;

      ::cast<::gpu_directx11::device> pgpudevice = m_pgpucontext->m_pgpudevice;

      auto pdevice = pgpudevice->m_pd3d11device;

      HRESULT hrCreateTexture = pdxgiswapchain1->GetBuffer(0, __interface_of(m_ptextureOffscreen));

      if (FAILED(hrCreateTexture))
      {

         throw ::hresult_exception(hrCreateTexture, "Failed to create offscreen texture");
      }

      if (m_textureflags.m_bRenderTarget)
      {

         create_render_target();

      }

      if (m_textureflags.m_bShaderResource)
      {

         create_shader_resource_view();
      }

      // HRESULT hrCreateShaderResourceView = pdevice->CreateShaderResourceView(m_ptextureOffscreen, nullptr,
      // &m_pshaderresourceview);

      // if (FAILED(hrCreateShaderResourceView))
      //{

      //   throw ::hresult_exception(hrCreateShaderResourceView, "Failed to create offscreen shader resource view");

      //}

      // if (bCreateRenderTargetView)
      //{

      //   //// 2. Create RTV descriptor heap
      //   //D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
      //   //rtvHeapDesc.NumDescriptors = 1;
      //   //rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
      //   //rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
      //   //HRESULT hrCreateDescriptorHeap = pdevice->m_pdevice->CreateDescriptorHeap(&rtvHeapDesc,
      //   __interface_of(m_pheapRenderTargetView));

      //   //pdevice->defer_throw_hresult(hrCreateDescriptorHeap);

      //   //// 3. Create RTV
      //   //m_handleRenderTargetView = m_pheapRenderTargetView->GetCPUDescriptorHandleForHeapStart();
      //   CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

      //   pdevice->m_pdevice->CreateRenderTargetView(m_presource, nullptr, m_handleRenderTargetView);

      //}

      // if (bCreateShaderResourceView)
      //{

      //   //// 4. Create SRV descriptor heap
      //   //D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
      //   //srvHeapDesc.NumDescriptors = 1;
      //   //srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
      //   //srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
      //   //
      //   //HRESULT hrCreateDescriptorHeap = pdevice->m_pdevice->CreateDescriptorHeap(&srvHeapDesc,
      //   __interface_of(m_pheapShaderResourceView));

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


   void texture::create_render_target()
   {

      // m_bRenderTarget = true;
      ::cast<::gpu_directx11::device> pgpudevice = m_pgpucontext->m_pgpudevice;

      if (m_textureflags.m_bRenderTarget)
      {


         if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
         {

            if (m_rendertargetview2a.is_empty())
            {
               m_rendertargetview2a.set_size(m_textureattributes.m_iMipCount);
               for (int iMip = 0; iMip < m_textureattributes.m_iMipCount; iMip++)
               {
                  m_rendertargetview2a[iMip].set_size(6);

                  for (int i = 0; i < 6; i++)
                  {
                     D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
                     rtvDesc.Format = m_texture2ddesc.Format;
                     rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
                     rtvDesc.Texture2DArray.MipSlice = iMip;
                     rtvDesc.Texture2DArray.FirstArraySlice = i;
                     rtvDesc.Texture2DArray.ArraySize = 1;

                     HRESULT hr = pgpudevice->m_pd3d11device->CreateRenderTargetView(m_ptextureOffscreen, &rtvDesc,
                                                                                &m_rendertargetview2a[iMip][i]);
                     ::defer_throw_hresult(hr);
                     // if (FAILED(hr))
                     //{
                     //    OutputDebugStringA("CreateRenderTargetView failed!\n");
                     // }
                  }
               }
            }
         }
         else
         {
            // D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
            // rtvDesc.Format = m_texture2ddesc.Format;
            // rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
            // rtvDesc.Texture2DArray.MipSlice = 0;
            // rtvDesc.Texture2DArray.FirstArraySlice = 0;
            // rtvDesc.Texture2DArray.ArraySize = 1;


            //            HRESULT hrCreateRenderTargetView =
            //             pgpudevice->m_pd3d11device->CreateRenderTargetView(m_ptextureOffscreen, &rtvDesc,
            //             &m_prendertargetview);

            HRESULT hrCreateRenderTargetView =
               pgpudevice->m_pd3d11device->CreateRenderTargetView(m_ptextureOffscreen, nullptr, &m_prendertargetview);
            if (FAILED(hrCreateRenderTargetView))
            {

               throw ::hresult_exception(hrCreateRenderTargetView, "Failed to create offscreen render target view");
            }
         }
      }
   }


   void texture::create_shader_resource_view()
   {

      ::cast<::gpu_directx11::device> pgpudevice = m_pgpucontext->m_pgpudevice;

      D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
      // srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // Must match or be compatible
      srvDesc.Format = m_texture2ddesc.Format; // Must match or be compatible
      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {
         srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
      }
      else
      {
         srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
      }
      srvDesc.Texture2D.MostDetailedMip = 0;
      if (m_texture2ddesc.MiscFlags & D3D11_RESOURCE_MISC_GENERATE_MIPS)
         srvDesc.Texture2D.MipLevels = -1;
      else
         srvDesc.Texture2D.MipLevels = 1;


      HRESULT hrCreateShaderResourceView =
         pgpudevice->m_pd3d11device->CreateShaderResourceView(m_ptextureOffscreen, &srvDesc, &m_pshaderresourceview);

      if (FAILED(hrCreateShaderResourceView))
      {

         throw ::hresult_exception(hrCreateShaderResourceView, "Failed to create offscreen shader resource view");
      }

      D3D11_SAMPLER_DESC sampDesc = {};
      sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
      sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
      sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
      sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
      pgpudevice->m_pd3d11device->CreateSamplerState(&sampDesc, &m_psamplerstate);

      new_texture.set_new_texture();
   }


   void texture::create_depth_resources()
   {

      if (!m_ptextureDepthStencil)
      {
         auto size = m_textureattributes.m_rectangleTarget.size();

         D3D11_TEXTURE2D_DESC depthDesc = {};
         depthDesc.Width = size.cx;
         depthDesc.Height = size.cy;
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

         ::cast<::gpu_directx11::device> pgpudevice = m_pgpucontext->m_pgpudevice;

         auto pdevice = pgpudevice->m_pd3d11device;

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
            m_ptextureDepthStencil, MorePrecisionNoStencil ? &dsvDesc : nullptr, &m_pdepthstencilview);

         if (FAILED(hrCreateDepthStencilView))
         {

            throw ::hresult_exception(hrCreateDepthStencilView);
         }

      }

      // ID3D11DepthStencilState* depthStencilState = nullptr;

      // D3D11_DEPTH_STENCIL_DESC dsDesc = {};

      // dsDesc.DepthEnable = TRUE;
      // dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
      // dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

      // HRESULT hrCreateDepthStencilState = pdevice->CreateDepthStencilState(&dsDesc, &m_pdepthstencilstate);

      // if (FAILED(hrCreateDepthStencilState))
      //{

      //   throw ::hresult_exception(hrCreateDepthStencilState);

      //}

      // VkFormat depthFormat = findDepthFormat();

      // m_formatDepth = depthFormat;

      // VkExtent2D extent = getExtent();

      // depthImages.resize(imageCount());
      // depthImageMemorys.resize(imageCount());
      // depthImageViews.resize(imageCount());

      // for (int i = 0; i < depthImages.size(); i++)
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


   // void texture::_new_state(ID3D12GraphicsCommandList* pcommandlist, D3D12_RESOURCE_STATES estateNew)
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


   // class texture::d3d11* texture::d3d11()
   //{

   //   if (!m_pd3d11)
   //   {


   //   }

   //   return m_pd3d11;

   //}


   //void texture::blend(::gpu::texture *ptexture) {}


   IDXGISurface *texture::__get_dxgi_surface()
   {

      if (m_pdxgisurface)
      {

         return m_pdxgisurface;
      }

      auto hr = m_ptextureOffscreen.as(m_pdxgisurface);

      ::defer_throw_hresult(hr);

      return m_pdxgisurface;
   }


   void texture::set_pixels(const ::int_rectangle &rectangle, const void *data)
   {

      // D3D11_MAPPED_SUBRESOURCE mapped{};

      //::cast < ::gpu_directx11::device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      ::cast<::gpu_directx11::context> pgpucontext = m_pgpucontext;

      // Define the box region to update (in texel coordinates)
      D3D11_BOX box{};
      box.left = rectangle.left;
      box.top = rectangle.top;
      box.front = 0;
      box.right = rectangle.right;
      box.bottom = rectangle.bottom;
      ;
      box.back = 1;

      UINT rowPitch = rectangle.width() * 4;

      // Upload the sub-region
      pgpucontext->m_pcontext->UpdateSubresource(m_ptextureOffscreen, // destination texture
                                                 0, // subresource (mip 0, array slice 0)
                                                 &box, // region to update
                                                 data, // source pixels (must be tightly packed)
                                                 rowPitch, // bytes per row
                                                 0 // bytes per slice (not used for 2D textures)
      );
      // HRESULT hrMap = pgpucontext->m_pcontext->Map(
      //    m_ptextureOffscreen, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

      //::defer_throw_hresult(hrMap);
      //
      // auto pimage32 = (::image32_t*)mapped.pData;
      //
      // pimage32->copy(rectangle, mapped.RowPitch,(const ::image32_t *) data, rectangle.width() * 4);

      ////// Copy pixel rows into mapped.pData
      ////for (UINT row = 0; row < height; ++row)
      ////{
      ////   memcpy(
      ////      (BYTE*)mapped.pData + (y + row) * mapped.RowPitch + x * bytesPerPixel,
      ////      srcData + row * width * bytesPerPixel,
      ////      width * bytesPerPixel
      ////   );
      ////}

      // pgpucontext->m_pcontext->Unmap(m_ptextureOffscreen, 0);
   }


   void texture::initialize_with_image_data(::gpu::context *pgpucontext, const ::int_rectangle &rectangleTarget,
                                            int channels, bool bSrgb, const void *pdata, ::gpu::enum_texture etexture)
   {
      m_pgpucontext = pgpucontext;
      auto width = rectangleTarget.width();
      auto height = rectangleTarget.height();
      auto imagedata = (unsigned char *)pdata;

      // m_etexture = etype;
      m_textureattributes.m_rectangleTarget = rectangleTarget;

      m_textureflags.m_bWithDepth = false;

      ::cast<::gpu_directx11::context> pcontext = m_pgpucontext;
      ::cast<::gpu_directx11::device> pgpudevice = pgpucontext->m_pgpudevice;

      auto pdevice = pgpudevice->m_pd3d11device;

      // --- Create Texture2D ---
      m_texture2ddesc = {};

      D3D11_TEXTURE2D_DESC &texDesc = m_texture2ddesc;
      texDesc.Width = width;
      texDesc.Height = height;
      texDesc.MipLevels = 1;
      texDesc.ArraySize = 1;
      texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      texDesc.SampleDesc.Count = 1;
      texDesc.SampleDesc.Quality = 0;
      texDesc.Usage = D3D11_USAGE_DEFAULT;
      texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
      texDesc.CPUAccessFlags = 0;
      texDesc.MiscFlags = 0;

      ::memory memory;

      auto rgbaData = ::image32_t::create_copy_of(memory, {width, height}, 0, pdata, channels);

      rgbaData->swap_red_blue(width, height);

      //int h = height;
      //int w = width;
      //auto p = rgbaData;

      // int halfw = w / 2;
      // for (size_t y = 0; y < h; y++)
      // {
      //    auto pline = p + y * width * 4;
      //    for (size_t x = 0; x < halfw; x++)
      //    {
      //       swap(((unsigned int *)pline)[x], ((unsigned int *)pline)[w - x - 1]);
      //    }
      // }

      // int halfh = h / 2;
      //::memory memoryLine;
      // memoryLine.set_size(width * 4);
      // p = buffer;
      // for (size_t y = 0; y < halfh; y++)
      //{
      //    memcpy(memoryLine.data(), p + y * width * 4, memoryLine.size());
      //    memcpy(p + y * width * 4, p + (h - 1 - y) * width * 4, memoryLine.size());
      //    memcpy(p + (h - 1 - y) * width * 4, memoryLine.data(), memoryLine.size());
      // }

      // Fill subresource data
      D3D11_SUBRESOURCE_DATA initData = {};
      initData.pSysMem = (const void *)rgbaData;
      initData.SysMemPitch = width * 4;
      initData.SysMemSlicePitch = 0;

      HRESULT hr = pdevice->CreateTexture2D(&texDesc, &initData, &m_ptextureOffscreen);
      defer_throw_hresult(hr);
      set_ok_flag();

      if (m_textureflags.m_bRenderTarget)
      {

         create_render_target();
      }

      if (m_textureflags.m_bShaderResource)
      {

         create_shader_resource_view();
      }

      if (m_textureattributes.m_etexture & ::gpu::e_texture_depth)
      {

         create_depth_resources();
      }

      // HRESULT hrCreateRenderTargetView = pdevice->CreateRenderTargetView(m_ptextureOffscreen, nullptr,
      // &m_prendertargetview);

      // if (FAILED(hrCreateRenderTargetView))
      //{

      //   throw ::hresult_exception(hrCreateRenderTargetView, "Failed to create offscreen render target view");

      //}

      // HRESULT hrCreateShaderResourceView = pdevice->CreateShaderResourceView(m_ptextureOffscreen, nullptr,
      // &m_pshaderresourceview);

      // if (FAILED(hrCreateShaderResourceView))
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
   }


   void texture::initialize_hdr_texture_on_memory(::gpu::context *pgpucontext, const ::block &block)
   {

      ::gpu::context_lock contextlock(pgpucontext);

      m_pgpucontext = pgpucontext;

      auto data = block.data();

      auto size = block.size();

      int width, height, channels;

      //stbi_set_flip_vertically_on_load(1);
      stbi_set_flip_vertically_on_load(0);
      auto imagedata = stbi_loadf_from_memory(data, size, &width, &height, &channels, 0);
      stbi_set_flip_vertically_on_load(0);

      if (!imagedata)
      {


         warning() << "Failed to load texture data";

         stbi_image_free(imagedata);

         return;
      }

      //stbi_set_flip_vertically_on_load(0);

      // m_textureattributes.m_etexture = etype;
      m_textureattributes.m_rectangleTarget = ::int_rectangle(::int_size(width, height));

      m_textureflags.m_bWithDepth = false;

      ::cast<::gpu_directx11::context> pcontext = pgpucontext;
      ::cast<::gpu_directx11::device> pgpudevice = pgpucontext->m_pgpudevice;
      // --- Create Texture2D ---
      D3D11_TEXTURE2D_DESC texDesc = {};
      texDesc.Width = width;
      texDesc.Height = height;
      texDesc.MipLevels = 1;
      texDesc.ArraySize = 1;
      texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
      texDesc.SampleDesc.Count = 1;
      texDesc.SampleDesc.Quality = 0;
      texDesc.Usage = D3D11_USAGE_DEFAULT;
      texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
      texDesc.CPUAccessFlags = 0;
      texDesc.MiscFlags = 0;
      memory memory;
      float *rgbaData = nullptr;
      if (channels != 4)
      {

         size_t pixelCount = (size_t)width * height;
         memory.set_size(pixelCount * 4 * sizeof(float));
         rgbaData = (float *)memory.data();

         if (channels == 3)
         {

            for (size_t i = 0; i < pixelCount; ++i)
            {
               rgbaData[i * 4 + 0] = imagedata[i * 3 + 0];
               rgbaData[i * 4 + 1] = imagedata[i * 3 + 1];
               rgbaData[i * 4 + 2] = imagedata[i * 3 + 2];
               rgbaData[i * 4 + 3] = 1.0f; // synthesized alpha
            }
         }
         else if (channels == 1)
         {

            for (size_t i = 0; i < pixelCount; ++i)
            {
               rgbaData[i * 4 + 0] = imagedata[i];
               rgbaData[i * 4 + 1] = imagedata[i];
               rgbaData[i * 4 + 2] = imagedata[i];
               rgbaData[i * 4 + 3] = 1.0f; // synthesized alpha
            }
         }
         else
         {

            throw ::exception(error_wrong_state, "Currently unsupported number of channels");
         }
         channels = 4;
      }

      // Fill subresource data
      D3D11_SUBRESOURCE_DATA initData = {};
      initData.pSysMem = (const void *)(rgbaData ? rgbaData : imagedata);
      initData.SysMemPitch = width * 4 * sizeof(float); // 4 floats per pixel
      initData.SysMemSlicePitch = 0;

      HRESULT hr = pgpudevice->m_pd3d11device->CreateTexture2D(&texDesc, &initData, &m_ptextureOffscreen);
      defer_throw_hresult(hr);
      set_ok_flag();

      // m_gluType = GL_TEXTURE_2D;

      // glGenTextures(1, &m_gluTextureID);
      // GLCheckError("");
      // glBindTexture(m_gluType, m_gluTextureID);
      // GLCheckError("");

      // float *rgbaData = nullptr;
      // if (channels == 3)
      //{

      //   size_t pixelCount = (size_t)width * height;
      //   rgbaData = (float *)malloc(pixelCount * 4 * sizeof(float));

      //   for (size_t i = 0; i < pixelCount; ++i)
      //   {
      //      rgbaData[i * 4 + 0] = imagedata[i * 3 + 0];
      //      rgbaData[i * 4 + 1] = imagedata[i * 3 + 1];
      //      rgbaData[i * 4 + 2] = imagedata[i * 3 + 2];
      //      rgbaData[i * 4 + 3] = 1.0f; // synthesized alpha
      //   }
      //   channels = 4;
      //}

      //// glTexImage2D(m_gluType, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, imagedata);
      // glTexImage2D(m_gluType, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, rgbaData ? rgbaData : imagedata);
      // GLCheckError("");

      // glTexParameteri(m_gluType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      // GLCheckError("");
      // glTexParameteri(m_gluType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      // GLCheckError("");
      // glTexParameteri(m_gluType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      // GLCheckError("");
      // glTexParameteri(m_gluType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // GLCheckError("");


      //stbi_image_free(imagedata);

      // if (rgbaData)
      //{

      //   free(rgbaData);
      //}
   }




   ID3D11RenderTargetView *texture::render_target_view(int iFace, int iMip)
   {

      //auto iIndex = render_target_view_index(iFace, iMip);

      if (iMip < 0 || iMip >= m_rendertargetview2a.count())
      {

         throw ::exception(error_wrong_state);
      }
      if (iFace < 0 || iFace >= m_rendertargetview2a[iMip].count())
      {

         throw ::exception(error_wrong_state);
      }

      return m_rendertargetview2a[iMip][iFace];
   }


   void texture::generate_mipmap(::gpu::command_buffer *pgpucommandbuffer)
   {

      ::cast<gpu_directx11::context> pcontext = pgpucommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

      pcontext->m_pcontext->Flush();

      // Now generate mipmaps using DirectX
      pcontext->m_pcontext->GenerateMips(m_pshaderresourceview);

   }


} // namespace gpu_directx11
