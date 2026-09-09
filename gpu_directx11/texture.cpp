// Created by camilo on 2025-06-08 18:14 < 3ThomasBorregaardSørensen!!
#include "platform.h"
#include "texture.h"
#include <stb/stb_image.h>
#include "acme/graphics/image/pixmap.h"
#include "aura/graphics/image/image.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/command_buffer_lease.h"
#include "bred/gpu/context_lock.h"
#include "bred/gpu/render_target.h"
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

            m_d3d11texture2ddesc = {};
      // 1. Create offscreen render target texture
      m_d3d11texture2ddesc.Width = this->raw_width();
      m_d3d11texture2ddesc.Height = this->raw_height();
      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {

         if (m_d3d11texture2ddesc.Width != m_d3d11texture2ddesc.Height)
         {

            throw ::exception(error_wrong_state, "width and height should be the same for a cube map_base texture");
         }
      }
      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {
         m_d3d11texture2ddesc.ArraySize = 6;
      }
      else
      {
         m_d3d11texture2ddesc.ArraySize = 1;
      }
      if (m_textureattributes.m_iChannelCount == 2)
      {
         if (m_textureattributes.m_iFloat >= 1)
         {

            m_d3d11texture2ddesc.Format = DXGI_FORMAT_R32G32_FLOAT;
         }
         else
         {
            m_d3d11texture2ddesc.Format = DXGI_FORMAT_R8G8_UNORM;
         }
      }
      else if (m_textureattributes.m_iFloat >= 1)
      {
         m_d3d11texture2ddesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
      }
      else
      {
         m_d3d11texture2ddesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      }
      m_d3d11texture2ddesc.SampleDesc.Count = 1;
      m_d3d11texture2ddesc.Usage = D3D11_USAGE_DEFAULT;
      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {

         m_d3d11texture2ddesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
      }
      if (m_textureattributes.m_iMipCount < 0 
         || 
         (m_textureattributes.maximum_mip_count() > 1
            && m_textureattributes.m_iMipCount == m_textureattributes.maximum_mip_count()))
      {


         m_d3d11texture2ddesc.MipLevels = 0;
         m_d3d11texture2ddesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
         m_d3d11texture2ddesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
         m_d3d11texture2ddesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
      }
      else if (m_textureattributes.m_iMipCount > 1)
      {

         m_d3d11texture2ddesc.MipLevels = m_textureattributes.m_iMipCount;
         m_d3d11texture2ddesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
         m_d3d11texture2ddesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
         m_d3d11texture2ddesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
      }
      else
      {

         m_d3d11texture2ddesc.MipLevels = 1;
      }

      m_d3d11texture2ddesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
      if (m_textureflags.m_bRenderTarget)
      {
         m_d3d11texture2ddesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
      }

      // if (imagea.is_empty() && !m_bRenderTarget)
      //{

      //   m_d3d11texture2ddesc.Usage = D3D11_USAGE_DYNAMIC;
      //   m_d3d11texture2ddesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;

      //}

      ::cast<::gpu_directx11::device> pgpudevice = m_pgpucontext->m_pgpudevice;

      auto pdevice = pgpudevice->m_pd3d11device;

      D3D11_SUBRESOURCE_DATA subresourcedata[6]{};

      ::memory memory;


      if (texturedata.is_pixmap_array())
      {

         const auto &imagea = texturedata.pixmapa();

         if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
         {

            if (imagea.size() != 6)
            {

               throw ::exception(error_wrong_state);
            }

            for (::i32 i = 0; i < 6; ++i)
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
      else if (texturedata.is_raw_scoped_pixmap())
      {

         auto pixmap = texturedata.raw_scoped_pixmap();

         subresourcedata[0].SysMemPitch = pixmap.m_iScan;
         if (pixmap.m_bTopLeft)
         {
            subresourcedata[0].pSysMem = pixmap.image32();
         }
         else
         {

            memory.set_size(pixmap.scan_area_in_bytes());

            auto pimage32 = (::image32_t *)memory.data();

            pimage32->y_swap_copy(&pixmap);

            subresourcedata[0].pSysMem = pimage32;


         }

      }

      HRESULT hrCreateTexture =
         pdevice->CreateTexture2D(&m_d3d11texture2ddesc, subresourcedata[0].pSysMem ? subresourcedata : nullptr, &m_pd3d11texture2d);

      if (FAILED(hrCreateTexture))
      {

         throw ::hresult_exception(hrCreateTexture, "Failed to create offscreen texture");

      }

      // GPU bitmaps pass a single scoped pixmap, not a pixmap array. Atlas
      // growth passes the previous texture. Neither is CreateTexture2D data.
      //if (texturedata.is_raw_scoped_pixmap() || texturedata.is_gpu_texture())
      if (texturedata.is_gpu_texture())
      {

         _set_data(texturedata);

      }

   }


   void texture::_set_data(const ::gpu::texture_data & texturedata)
   {

      if (texturedata.is_empty())
      {

         return;

      }

      if (texturedata.is_raw_scoped_pixmap())
      {

         const auto & pixmap = texturedata.raw_scoped_pixmap();
         if (m_d3d11texture2ddesc.Format != DXGI_FORMAT_B8G8R8A8_UNORM
            || m_d3d11texture2ddesc.ArraySize != 1)
         {

            throw ::exception(error_bad_argument, "Icon pixmap requires a BGRA 2D texture");

         }

         write_pixels(false, pixmap.data(), pixmap.size(), pixmap.m_iScan, 4, {});
         return;

      }

      if (!texturedata.is_gpu_texture())
      {

         throw ::exception(error_not_supported, "Unsupported DirectX 11 texture data update");

      }

      ::cast<texture> psource = texturedata.gpu_texture();
      if (!psource || !psource->m_pd3d11texture2d || !m_pd3d11texture2d)
      {

         throw ::exception(error_bad_argument);

      }

      D3D11_TEXTURE2D_DESC sourceDesc{}, targetDesc{};
      psource->m_pd3d11texture2d->GetDesc(&sourceDesc);
      m_pd3d11texture2d->GetDesc(&targetDesc);
      if (psource->m_pgpucontext->m_pgpudevice != m_pgpucontext->m_pgpudevice
         || sourceDesc.Format != targetDesc.Format
         || sourceDesc.ArraySize != 1 || targetDesc.ArraySize != 1
         || sourceDesc.MipLevels != 1 || targetDesc.MipLevels != 1
         || sourceDesc.SampleDesc.Count != 1 || targetDesc.SampleDesc.Count != 1)
      {

         throw ::exception(error_bad_argument, "Atlas preservation requires compatible single-level 2D textures");

      }

      if (psource->m_pd3d11texture2d.m_p == m_pd3d11texture2d.m_p)
      {

         return;

      }

      psource->wait_fence();
      ::cast<context> pmain = m_pgpucontext->m_pgpudevice->main_gpu_context();
      ::gpu::context_lock contextlock(pmain);

      // The destination can be larger than the old atlas. CopyResource would
      // require equal dimensions; copy just their common pixel rectangle.
      D3D11_BOX box{0, 0, 0,
         minimum(sourceDesc.Width, targetDesc.Width),
         minimum(sourceDesc.Height, targetDesc.Height), 1};
      pmain->m_pd3d11devicecontext->CopySubresourceRegion(
         m_pd3d11texture2d, 0, 0, 0, 0, psource->m_pd3d11texture2d, 0, &box);

   }


   // void texture::initialize_texture(::gpu::renderer *prenderer, const ::i32_rectangle &rectangleTarget,
   //                                        bool bWithDepth, const ::pointer_array<::image::image> * pimagea,
   //                                        enum_type etype)
   // {
   //
   //    if (m_pd3d11texture2d && m_rectangleTarget == rectangleTarget && m_pgpurenderer == prenderer)
   //    {
   //
   //       return;
   //    }
   //
   //    auto sizeCurrent = m_rectangleTarget.size();
   //
   //    ::gpu::texture::initialize_image_texture(prenderer, rectangleTarget, bWithDepth, pimagea, etype);
   //
   //    if (m_pd3d11texture2d && sizeCurrent == m_rectangleTarget.size())
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
   //    // HRESULT hrCreateRenderTargetView = pdevice->CreateRenderTargetView(m_pd3d11texture2d, nullptr,
   //    // &m_pd3d11rendertargetview);
   //
   //    // if (FAILED(hrCreateRenderTargetView))
   //    //{
   //
   //    //   throw ::hresult_exception(hrCreateRenderTargetView, "Failed to create offscreen render target view");
   //
   //    //}
   //
   //    // HRESULT hrCreateShaderResourceView = pdevice->CreateShaderResourceView(m_pd3d11texture2d, nullptr,
   //    // &m_pd3d11shaderresourceview);
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
   //    pdevice->CreateSamplerState(&samp, &m_pd3d11samplerstate);
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


   void texture::_initialize_gpu_texture(
      ::gpu::context *pgpucontext,
      IDXGISwapChain1 *pdxgiswapchain1,
      UINT uBuffer)
   {

      //m_pgpurenderer = prenderer;

      m_pgpucontext = pgpucontext;

      ::cast<::gpu_directx11::device> pgpudevice = m_pgpucontext->m_pgpudevice;

      auto pdevice = pgpudevice->m_pd3d11device;

      HRESULT hrCreateTexture = pdxgiswapchain1->GetBuffer(uBuffer, __interface_of(m_pd3d11texture2d));

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

      // HRESULT hrCreateShaderResourceView = pdevice->CreateShaderResourceView(m_pd3d11texture2d, nullptr,
      // &m_pd3d11shaderresourceview);

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

            if (m_d3d11rendertargetview2a.is_empty())
            {
               m_d3d11rendertargetview2a.set_size(m_textureattributes.m_iMipCount);
               for (::i32 iMip = 0; iMip < m_textureattributes.m_iMipCount; iMip++)
               {
                  m_d3d11rendertargetview2a[iMip].set_size(6);

                  for (::i32 i = 0; i < 6; i++)
                  {
                     D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
                     rtvDesc.Format = m_d3d11texture2ddesc.Format;
                     rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
                     rtvDesc.Texture2DArray.MipSlice = iMip;
                     rtvDesc.Texture2DArray.FirstArraySlice = i;
                     rtvDesc.Texture2DArray.ArraySize = 1;

                     HRESULT hr = pgpudevice->m_pd3d11device->CreateRenderTargetView(m_pd3d11texture2d, &rtvDesc,
                                                                                &m_d3d11rendertargetview2a[iMip][i]);
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
            // rtvDesc.Format = m_d3d11texture2ddesc.Format;
            // rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
            // rtvDesc.Texture2DArray.MipSlice = 0;
            // rtvDesc.Texture2DArray.FirstArraySlice = 0;
            // rtvDesc.Texture2DArray.ArraySize = 1;


            //            HRESULT hrCreateRenderTargetView =
            //             pgpudevice->m_pd3d11device->CreateRenderTargetView(m_pd3d11texture2d, &rtvDesc,
            //             &m_pd3d11rendertargetview);

            HRESULT hrCreateRenderTargetView =
               pgpudevice->m_pd3d11device->CreateRenderTargetView(m_pd3d11texture2d, nullptr, &m_pd3d11rendertargetview);
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
      srvDesc.Format = m_d3d11texture2ddesc.Format; // Must match or be compatible
      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {
         srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
      }
      else
      {
         srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
      }
      srvDesc.Texture2D.MostDetailedMip = 0;
      if (m_d3d11texture2ddesc.MiscFlags & D3D11_RESOURCE_MISC_GENERATE_MIPS)
         srvDesc.Texture2D.MipLevels = -1;
      else
         srvDesc.Texture2D.MipLevels = 1;


      HRESULT hrCreateShaderResourceView =
         pgpudevice->m_pd3d11device->CreateShaderResourceView(m_pd3d11texture2d, &srvDesc, &m_pd3d11shaderresourceview);

      if (FAILED(hrCreateShaderResourceView))
      {

         throw ::hresult_exception(hrCreateShaderResourceView, "Failed to create offscreen shader resource view");
      }

      D3D11_SAMPLER_DESC sampDesc = {};
      sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
      sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
      sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
      sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
      pgpudevice->m_pd3d11device->CreateSamplerState(&sampDesc, &m_pd3d11samplerstate);

      new_texture.set_new_texture();
   }


   void texture::create_depth_resources()
   {

      if (!m_pd3d11texture2dDepthStencil)
      {
         auto size = m_textureattributes.m_size;

         D3D11_TEXTURE2D_DESC depthDesc = {};
         depthDesc.Width = size.cx;
         depthDesc.Height = size.cy;
         depthDesc.MipLevels = 1;
         depthDesc.ArraySize = 1;
         ::i32 MorePrecisionNoStencil = 1;
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

         HRESULT hrCreateTexture = pdevice->CreateTexture2D(&depthDesc, nullptr, &m_pd3d11texture2dDepthStencil);

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
            m_pd3d11texture2dDepthStencil, MorePrecisionNoStencil ? &dsvDesc : nullptr, &m_pd3d11depthstencilview);

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

      // for (::i32 i = 0; i < depthImages.size(); i++)
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


   ////void texture::blend(::gpu::texture *ptexture) {}

   //bool read_texture_pixels(
   //ID3D11Device * device,
   //ID3D11DeviceContext * context,
   //ID3D11Texture2D * texture,
   //std::vector<uint8_t> & pixels,
   //UINT & width,
   //UINT & height,
   //UINT & rowSize)
   //{
   //   if (!device || !context || !texture)
   //   {
   //      return false;
   //   }


   //   return true;
   //}

   void texture::read_pixels(::gpu::command_buffer * pgpucommandbuffer, ::pixmap_t * ppixmap, const ::i32_point & pointOutput)
   {

      if (!pgpucommandbuffer || !pgpucommandbuffer->m_pgpucommandbufferlease ||
          !ppixmap || ppixmap->size().is_empty() || !ppixmap->m_pimage32 ||
          (::i64)ppixmap->m_iScan < (::i64)ppixmap->width() * 4 ||
          !m_pd3d11texture2d || pointOutput.x < 0 || pointOutput.y < 0)
         throw ::exception(error_bad_argument, "DirectX 11 readback requires a writable pixmap and pending command lease.");

      ::cast<::gpu_directx11::context> pgpucontext = m_pgpucontext;
      ::cast<::gpu_directx11::context> commandContext =
         pgpucommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;
      if (!pgpucontext || !commandContext || commandContext->m_pgpudevice != pgpucontext->m_pgpudevice)
         throw ::exception(error_bad_argument, "DirectX 11 readback command buffer belongs to another device.");

      ::gpu::context_lock commandContextLock(commandContext);

      D3D11_TEXTURE2D_DESC desc{};

      m_pd3d11texture2d->GetDesc(&desc);

      if (desc.SampleDesc.Count != 1 ||
          (desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM && desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM_SRGB &&
           desc.Format != DXGI_FORMAT_R8G8B8A8_UNORM && desc.Format != DXGI_FORMAT_R8G8B8A8_UNORM_SRGB))
         throw ::exception(error_not_supported, "DirectX 11 readback requires a resolved 32-bit color texture.");

      const auto iMip = maximum(0, m_iCurrentMip);
      const auto iLayer = maximum(0, m_iCurrentLayer);
      if ((::u64)iMip >= desc.MipLevels || iMip >= 32 || (::u64)iLayer >= desc.ArraySize)
         throw ::exception(error_bad_argument, "Invalid DirectX 11 readback subresource.");

      const auto mipWidth = maximum(1u, desc.Width >> iMip);
      const auto mipHeight = maximum(1u, desc.Height >> iMip);
      if ((::i64)pointOutput.x + ppixmap->width() > mipWidth ||
          (::i64)pointOutput.y + ppixmap->height() > mipHeight)
         throw ::exception(error_bad_argument, "DirectX 11 readback region exceeds the source texture.");

      const auto subresource = D3D11CalcSubresource(iMip, iLayer, desc.MipLevels);

      D3D11_TEXTURE2D_DESC stagingDesc{};

      if (m_pd3d11texture2dStaging)
      {

         m_pd3d11texture2dStaging->GetDesc(&stagingDesc);

         if (stagingDesc.Width != desc.Width
            || stagingDesc.Height != desc.Height
            || stagingDesc.Format != desc.Format
            || stagingDesc.MipLevels != desc.MipLevels
            || stagingDesc.ArraySize != desc.ArraySize)
         {

            m_pd3d11texture2dStaging.release();

         }

      }

      if (!m_pd3d11texture2dStaging)
      {

         stagingDesc = desc;

         // This example assumes 4 bytes/pixel, such as:
         // DXGI_FORMAT_R8G8B8A8_UNORM
         // DXGI_FORMAT_B8G8R8A8_UNORM
         auto rowSize = desc.Width * 4;

         stagingDesc.Usage = D3D11_USAGE_STAGING;
         stagingDesc.BindFlags = 0;
         stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
         stagingDesc.MiscFlags = 0;

         ::cast<::gpu_directx11::device> pgpudevice = pgpucontext->m_pgpudevice;

         auto pdevice = pgpudevice->m_pd3d11device;

         HRESULT hrCreateTexture2D = pdevice->CreateTexture2D(
            &stagingDesc,
            nullptr,
            &m_pd3d11texture2dStaging);

         if (FAILED(hrCreateTexture2D))
         {

            throw ::hresult_exception(hrCreateTexture2D);

         }

      }

      commandContext->m_pd3d11devicecontext->CopyResource(
         m_pd3d11texture2dStaging,
         m_pd3d11texture2d);

      // Execute the deferred copy before Map on the immediate context. Map
      // waits for submitted GPU work, but cannot submit a deferred command list.
      pgpucommandbuffer->m_pgpucommandbufferlease->commit();

      HRESULT hrMap = E_FAIL;

      //pgpucontext->m_pgpudevice->main_gpu_context()->send([&]()
         {
         ::cast < ::gpu_directx11::context > pgpucontextMain = pgpucontext->m_pgpudevice->main_gpu_context();
         ::gpu::context_lock contextlock(pgpucontextMain);

            D3D11_MAPPED_SUBRESOURCE mapped{};

            

            hrMap = pgpucontextMain->m_pd3d11devicecontext->Map(
               m_pd3d11texture2dStaging,
               subresource,
               D3D11_MAP_READ,
               0,
               &mapped);

            if (FAILED(hrMap))
            {
               //return;
               throw ::hresult_exception(hrMap);
            }


            struct unmap_guard
            {
               ID3D11DeviceContext * context;
               ID3D11Resource * resource;
               UINT subresource;
               ~unmap_guard() { context->Unmap(resource, subresource); }
            } unmap{pgpucontextMain->m_pd3d11devicecontext, m_pd3d11texture2dStaging, subresource};

            const auto sourcePixels = (const unsigned char *)mapped.pData
               + (size_t)pointOutput.y * mapped.RowPitch + (size_t)pointOutput.x * 4;
            ppixmap->copy(ppixmap->size(), (const ::image32_t *)sourcePixels, mapped.RowPitch);

      }//);

      //if (FAILED(hrMap))
      //{
        // throw ::hresult_exception(hrMap);
      //}


      //stagingTexture->Release();


   }


   IDXGISurface *texture::__get_dxgi_surface()
   {

      if (m_pdxgisurface)
      {

         return m_pdxgisurface;
      }

      auto hr = m_pd3d11texture2d.as(m_pdxgisurface);

      ::defer_throw_hresult(hr);

      return m_pdxgisurface;
   }


   void texture::set_pixels(bool bSync, const ::i32_rectangle &rectangle, const void *data)
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
      
      box.back = 1;

      UINT rowPitch = rectangle.width() * 4;

      {
         ::cast < ::gpu_directx11::context > pgpucontextMain = pgpucontext->m_pgpudevice->main_gpu_context();
         ::gpu::context_lock contextlock(pgpucontextMain);

         // Upload the sub-region
         pgpucontextMain->m_pd3d11devicecontext->UpdateSubresource(m_pd3d11texture2d, // destination texture
                                                    0, // subresource (mip 0, array slice 0)
                                                    &box, // region to update
                                                    data, // source pixels (must be tightly packed)
                                                    rowPitch, // bytes per row
                                                    0 // bytes per slice (not used for 2D textures)


         );

      }
      // HRESULT hrMap = pgpucontext->m_pcontext->Map(
      //    m_pd3d11texture2d, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

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

      // pgpucontext->m_pcontext->Unmap(m_pd3d11texture2d, 0);
   }


   //void texture::set_pixels(::gpu::command_buffer * pgpucommandbuffer, const ::i32_rectangle & rectangle, const void * data)
   //{

   //   set_pixels(pgpucommandbuffer, rectangle, data);

   //}


   void texture::create_with_image_data(::gpu::context *pgpucontext, const ::i32_size & size,
                                            ::i32 channels, bool bSrgb, const void *pdata, ::gpu::enum_texture etexture)
   {
      m_pgpucontext = pgpucontext;
      auto width = size.width();
      auto height = size.height();
      auto imagedata = (::u8 *)pdata;

      // m_etexture = etype;
      m_textureattributes.m_size = size;

      m_textureflags.m_bWithDepth = false;

      ::cast<::gpu_directx11::context> pcontext = m_pgpucontext;
      ::cast<::gpu_directx11::device> pgpudevice = pgpucontext->m_pgpudevice;

      auto pdevice = pgpudevice->m_pd3d11device;

      // --- Create Texture2D ---
      m_d3d11texture2ddesc = {};

      D3D11_TEXTURE2D_DESC &texDesc = m_d3d11texture2ddesc;
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

      //::i32 h = height;
      //::i32 w = width;
      //auto p = rgbaData;

      // ::i32 halfw = w / 2;
      // for (size_t y = 0; y < h; y++)
      // {
      //    auto pline = p + y * width * 4;
      //    for (size_t x = 0; x < halfw; x++)
      //    {
      //       swap(((::u32 *)pline)[x], ((::u32 *)pline)[w - x - 1]);
      //    }
      // }

      // ::i32 halfh = h / 2;
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

      HRESULT hr = pdevice->CreateTexture2D(&texDesc, &initData, &m_pd3d11texture2d);
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

      // HRESULT hrCreateRenderTargetView = pdevice->CreateRenderTargetView(m_pd3d11texture2d, nullptr,
      // &m_pd3d11rendertargetview);

      // if (FAILED(hrCreateRenderTargetView))
      //{

      //   throw ::hresult_exception(hrCreateRenderTargetView, "Failed to create offscreen render target view");

      //}

      // HRESULT hrCreateShaderResourceView = pdevice->CreateShaderResourceView(m_pd3d11texture2d, nullptr,
      // &m_pd3d11shaderresourceview);

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
      pdevice->CreateSamplerState(&samp, &m_pd3d11samplerstate);

      new_texture.set_new_texture();
   }


   void texture::create_hdr_texture_on_memory(::gpu::context *pgpucontext, const ::block &block)
   {

      ::gpu::context_lock contextlock(pgpucontext);

      m_pgpucontext = pgpucontext;

      auto data = block.data();

      auto size = block.size();

      ::i32 width, height, channels;

      //stbi_set_flip_vertically_on_load(1);
      stbi_set_flip_vertically_on_load(0);
      auto imagedata = stbi_loadf_from_memory(data, (int) size, &width, &height, &channels, 0);
      stbi_set_flip_vertically_on_load(0);

      if (!imagedata)
      {


         warning() << "Failed to load texture data";

         stbi_image_free(imagedata);

         return;
      }

      //stbi_set_flip_vertically_on_load(0);

      // m_textureattributes.m_etexture = etype;
      m_textureattributes.m_size = { width, height };

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
      ::f32 *rgbaData = nullptr;
      if (channels != 4)
      {

         size_t pixelCount = (size_t)width * height;
         memory.set_size(pixelCount * 4 * sizeof(::f32));
         rgbaData = (::f32 *)memory.data();

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
      initData.SysMemPitch = width * 4 * sizeof(::f32); // 4 floats per pixel
      initData.SysMemSlicePitch = 0;

      HRESULT hr = pgpudevice->m_pd3d11device->CreateTexture2D(&texDesc, &initData, &m_pd3d11texture2d);
      defer_throw_hresult(hr);
      set_ok_flag();

      // m_gluType = GL_TEXTURE_2D;

      // glGenTextures(1, &m_gluTextureID);
      // GLCheckError("");
      // glBindTexture(m_gluType, m_gluTextureID);
      // GLCheckError("");

      // ::f32 *rgbaData = nullptr;
      // if (channels == 3)
      //{

      //   size_t pixelCount = (size_t)width * height;
      //   rgbaData = (::f32 *)malloc(pixelCount * 4 * sizeof(::f32));

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




   ID3D11RenderTargetView *texture::render_target_view(::i32 iFace, ::i32 iMip)
   {

      //auto iIndex = render_target_view_index(iFace, iMip);

      if (iMip < 0 || iMip >= m_d3d11rendertargetview2a.count())
      {

         throw ::exception(error_wrong_state);
      }
      if (iFace < 0 || iFace >= m_d3d11rendertargetview2a[iMip].count())
      {

         throw ::exception(error_wrong_state);
      }

      return m_d3d11rendertargetview2a[iMip][iFace];
   }


   void texture::generate_mipmap(::gpu::command_buffer *pgpucommandbuffer)
   {

      ::cast<gpu_directx11::context> pcontext = pgpucommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

      pcontext->m_pd3d11devicecontext->Flush();

      // Now generate mipmaps using DirectX
      pcontext->m_pd3d11devicecontext->GenerateMips(m_pd3d11shaderresourceview);

   }


   void texture::write_pixels(
      bool bSync,
      const void * pData,
      const ::i32_size & size,
      ::i32 iScan,
      ::i32 iBytesPerPixel,
      const ::i32_point & point)
   {

      if (!pData)
      {

         throw ::exception(error_null_pointer);

      }

      if (size.cx <= 0 || size.cy <= 0)
      {

         return;

      }

      if (iBytesPerPixel <= 0)
      {

         throw ::exception(error_bad_argument);

      }

      if (iScan < size.cx * iBytesPerPixel)
      {

         throw ::exception(error_bad_argument);

      }

      if (point.x < 0
         || point.y < 0
         || point.x + size.cx > width()
         || point.y + size.cy > height())
      {

         throw ::exception(error_bad_argument);

      }


      ::cast<::gpu_directx11::context> pcontext =
         m_pgpucontext;

      if (!pcontext)
      {

         throw ::exception(error_wrong_state);

      }


      // A texture can retain a pooled deferred context after its graphics
      // lease ends. This standalone upload has no command-buffer submission,
      // so record it on the protected immediate context, as set_pixels does.
      ::cast<context> pmain = pcontext->m_pgpudevice->main_gpu_context();
      ::gpu::context_lock contextlock(pmain);
      auto pd3d11devicecontext = pmain->m_pd3d11devicecontext;

      if (!pd3d11devicecontext)
      {

         throw ::exception(error_wrong_state);

      }

      if (!m_pd3d11texture2d)
      {

         throw ::exception(error_wrong_state);

      }


      //
      // D3D11_BOX coordinates are:
      //
      //    left   inclusive
      //    top    inclusive
      //    right  exclusive
      //    bottom exclusive
      //

      D3D11_BOX box{};

      box.left =
         (UINT)point.x;

      box.top =
         (UINT)point.y;

      box.front =
         0;

      box.right =
         (UINT)(point.x + size.cx);

      box.bottom =
         (UINT)(point.y + size.cy);

      box.back =
         1;


      //
      // Unlike Vulkan, UpdateSubresource accepts the CPU-side
      // source row pitch directly.
      //
      // Therefore padding at the end of each source row is
      // automatically handled by iScan.
      //
      // For a 2D texture SrcDepthPitch is unused, so 0 is fine.
      //

      pd3d11devicecontext->UpdateSubresource(
         m_pd3d11texture2d,
         0,             // destination subresource
         &box,
         pData,
         (UINT)iScan,   // source row pitch
         0);            // source depth pitch


      ////
      //// UpdateSubresource has already consumed the caller's CPU memory
      //// by the time it returns. Therefore bSync is NOT required merely
      //// to keep pData alive.
      ////
      //// However, if bSync means that the caller requires the upload
      //// to have actually completed on the GPU, wait for an event query.
      ////

      //if (bSync)
      //{

      //   comptr<ID3D11Device> pdevice;

      //   pd3d11devicecontext->GetDevice(
      //      &pdevice);

      //   if (!pdevice)
      //   {

      //      throw ::exception(error_wrong_state);

      //   }


      //   D3D11_QUERY_DESC querydesc{};

      //   querydesc.Query =
      //      D3D11_QUERY_EVENT;

      //   querydesc.MiscFlags =
      //      0;


      //   comptr<ID3D11Query> pquery;

      //   HRESULT hr =
      //      pdevice->CreateQuery(
      //         &querydesc,
      //         &pquery);

      //   ::defer_throw_hresult(hr);


      //   //
      //   // Everything submitted before End(query) must complete
      //   // before the event becomes signaled.
      //   //

      //   pd3d11devicecontext->End(
      //      pquery);


      //   BOOL bDone = FALSE;

      //   for (;;)
      //   {

      //      hr =
      //         pd3d11devicecontext->GetData(
      //            pquery,
      //            &bDone,
      //            sizeof(bDone),
      //            0);

      //      if (hr == S_OK)
      //      {

      //         break;

      //      }

      //      if (hr != S_FALSE)
      //      {

      //         ::defer_throw_hresult(hr);

      //      }

      //      //
      //      // Avoid burning an entire CPU core while waiting.
      //      //

      //      ::Sleep(0);

      //   }

      //}

   }


} // namespace gpu_directx11
