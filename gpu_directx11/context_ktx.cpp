// Created by camilo on 2025-12-18 23:11 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include <DirectXMath.h>
#include "acme/platform/application.h"
#include "acme_windows_common/dxgi_surface_bindable.h"
#include "approach.h"
#include "aura/graphics/image/image.h"
#include "aura/user/user/interaction.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context_lock.h"
#include "bred/gpu/frame.h"
#include "bred/gpu/graphics.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/types.h"
#include "bred/graphics3d/types.h"
#include "context.h"
#include "device.h"
#include "gpu_directx11/descriptors.h"
#include "initializers.h"
#include "memory_buffer.h"
#include "offscreen_render_target_view.h"
#include "physical_device.h"
#include "program.h"
#include "renderer.h"
#include "shader.h"
#include "swap_chain.h"
#include "swap_chain_render_target_view.h"
#include "texture.h"

#include "block.h"
#include <glad/glad.h>
//#include <algorithm>
#include <assert.h>
#include <d3d11.h>
#include <ktx.h>
//#include <vector>
#include <vulkan/vulkan.h> // for VkFormat enum only


using namespace directx11;


namespace gpu_directx11
{


   struct Dx11Texture
   {
      ID3D11Texture2D *texture = nullptr;
      ID3D11ShaderResourceView *srv = nullptr;
   };

   static DXGI_FORMAT VkFormatToDxgi(VkFormat vk)
   {
      switch (vk)
      {
         case VK_FORMAT_R8G8B8A8_UNORM:
            return DXGI_FORMAT_R8G8B8A8_UNORM;
         case VK_FORMAT_R8G8B8A8_SRGB:
            return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

         case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
            return DXGI_FORMAT_BC1_UNORM;
         case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
            return DXGI_FORMAT_BC1_UNORM_SRGB;
         case VK_FORMAT_BC3_UNORM_BLOCK:
            return DXGI_FORMAT_BC3_UNORM;
         case VK_FORMAT_BC3_SRGB_BLOCK:
            return DXGI_FORMAT_BC3_UNORM_SRGB;
         case VK_FORMAT_BC7_UNORM_BLOCK:
            return DXGI_FORMAT_BC7_UNORM;
         case VK_FORMAT_BC7_SRGB_BLOCK:
            return DXGI_FORMAT_BC7_UNORM_SRGB;

         default:
            assert(false && "Unsupported KTX/Vk format");
            return DXGI_FORMAT_UNKNOWN;
      }
   }
#define GL_COMPRESSED_RGBA_BPTC_UNORM 0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM 0x8E8D
#define GL_R8 33321
   DXGI_FORMAT GlInternalFormatToDxgi(GLenum internalFormat, GLenum format, GLenum type)
   {
      switch (internalFormat)
      {
         case GL_R8:
            return DXGI_FORMAT_R8_UNORM;
         case GL_RGBA8:
            return DXGI_FORMAT_R8G8B8A8_UNORM;
         case GL_RGBA16F:
            return DXGI_FORMAT_R16G16B16A16_FLOAT;
         case GL_SRGB8_ALPHA8:
            return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

         case GL_COMPRESSED_RGBA_BPTC_UNORM:
            return DXGI_FORMAT_BC7_UNORM;

         case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM:
            return DXGI_FORMAT_BC7_UNORM_SRGB;

         default:
            assert(false && "Unsupported KTX1 GL format");
            return DXGI_FORMAT_UNKNOWN;
      }
   }

   Dx11Texture CreateDx11TextureFromKtx(ID3D11Device *device, const ktxTexture *ktxTex)
   {
      assert(device);
      assert(ktxTex);
      assert(ktxTex->pData);

      Dx11Texture out = {};

      bool isKtx2 = ktxTex->classId == ktxTexture2_c;
      ktxTexture1 *ktxTex1 = nullptr;
      ktxTexture2 *ktxTex2 = nullptr;
      if (isKtx2)
      {

         ktxTex2 = (ktxTexture2 *)ktxTex;
      }
      else
      {

         ktxTex1 = (ktxTexture1 *)ktxTex;
      }
      const bool isCubemap = ktxTex->isCubemap;
      const uint32_t faceCount = isCubemap ? 6 : 1;
      DXGI_FORMAT format;
      if (ktxTex2)
      {
        format = VkFormatToDxgi((VkFormat)ktxTex2->vkFormat);
      }
      else
      {
         // KTX1 path (GL format based)
         format = GlInternalFormatToDxgi(ktxTex1->glInternalformat, ktxTex1->glFormat, ktxTex1->glType);
      }

      const uint32_t mipCount = ktxTex->numLevels;
      const uint32_t layerCount = ktxTex->numLayers * faceCount;

      // ------------------------------------------------------------------
      // Create texture description
      // ------------------------------------------------------------------
      D3D11_TEXTURE2D_DESC desc = {};
      desc.Width = ktxTex->baseWidth;
      desc.Height = ktxTex->baseHeight;
      desc.MipLevels = mipCount;
      desc.ArraySize = layerCount;
      desc.Format = format;
      desc.SampleDesc.Count = 1;
      desc.Usage = D3D11_USAGE_DEFAULT;
      desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
      desc.CPUAccessFlags = 0;
      desc.MiscFlags = isCubemap ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;

      // ------------------------------------------------------------------
      // Fill subresource data
      // DX11 subresource index:
      //   sub = mip + arraySlice * mipCount
      // ------------------------------------------------------------------
      ::array_base<D3D11_SUBRESOURCE_DATA> subresources;
      subresources.resize(mipCount * layerCount);

      for (uint32_t layer = 0; layer < ktxTex->numLayers; ++layer)
      {
         for (uint32_t face = 0; face < faceCount; ++face)
         {
            for (uint32_t mip = 0; mip < mipCount; ++mip)
            {
               ktx_size_t offset = 0;
               KTX_error_code err = ktxTexture_GetImageOffset((ktxTexture *) ktxTex, mip, layer, face, &offset);
               assert(err == KTX_SUCCESS);

               const uint32_t arraySlice = layer * faceCount + face;

               const uint32_t subresource = mip + arraySlice * mipCount;

               D3D11_SUBRESOURCE_DATA &sr = subresources[subresource];

               sr.pSysMem = (const uint8_t *)ktxTex->pData + offset;

               sr.SysMemPitch = ktxTexture_GetRowPitch((ktxTexture *) ktxTex, mip);

               sr.SysMemSlicePitch = 0;
            }
         }
      }

      // ------------------------------------------------------------------
      // Create texture
      // ------------------------------------------------------------------
      HRESULT hr = device->CreateTexture2D(&desc, subresources.data(), &out.texture);
      assert(SUCCEEDED(hr));

      // ------------------------------------------------------------------
      // Create SRV
      // ------------------------------------------------------------------
      D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
      srvDesc.Format = desc.Format;

      if (isCubemap)
      {
         srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
         srvDesc.TextureCube.MipLevels = mipCount;
      }
      else if (desc.ArraySize > 1)
      {
         srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
         srvDesc.Texture2DArray.MipLevels = mipCount;
         srvDesc.Texture2DArray.ArraySize = desc.ArraySize;
      }
      else
      {
         srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
         srvDesc.Texture2D.MipLevels = mipCount;
      }

      hr = device->CreateShaderResourceView(out.texture, &srvDesc, &out.srv);
      assert(SUCCEEDED(hr));

      return out;
   }


   void context::load_ktxTexture(::gpu::texture * pgputexture, void * p_ktxTexture)
   {

      auto pktxtexture = (ktxTexture *)p_ktxTexture;

      ::cast<::gpu_directx11::texture> ptexture = pgputexture;

      ::cast<device> pdevice = m_pgpudevice;



      auto tex = CreateDx11TextureFromKtx(pdevice->m_pd3d11device1, pktxtexture);

      ptexture->m_ptextureOffscreen = tex.texture;
      ptexture->m_pshaderresourceview = tex.srv;
      ptexture->m_textureattributes.m_etexture = ::gpu::e_texture_image;
      ptexture->set_ok_flag();

   }


   //// Minimal format mapping (extend as needed)
   //static DXGI_FORMAT GlOrVkToDxgi(const ktxTexture *tex)
   //{

   //   VkFormat vkformat = ((ktxTexture2 *)tex)->vkFormat;
   //   // KTX2 path
   //   if (vkformat != VK_FORMAT_UNDEFINED)
   //   {
   //      switch (vkformat)
   //      {
   //         
   //         case VK_FORMAT_R8G8B8A8_UNORM:
   //            return DXGI_FORMAT_R8G8B8A8_UNORM;
   //         case VK_FORMAT_R8G8B8A8_SRGB:
   //            return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
   //         case VK_FORMAT_BC7_UNORM_BLOCK:
   //            return DXGI_FORMAT_BC7_UNORM;
   //         case VK_FORMAT_BC7_SRGB_BLOCK:
   //            return DXGI_FORMAT_BC7_UNORM_SRGB;
   //         default:
   //            assert(false);
   //            return DXGI_FORMAT_UNKNOWN;
   //      }
   //   }

   //   // KTX1 path
   //   switch (((ktxTexture1 *)tex)->glInternalformat)
   //   {
   //      case 0x8058: /* GL_RGBA8 */
   //         return DXGI_FORMAT_R8G8B8A8_UNORM;
   //      case 0x8C43: /* GL_SRGB8_ALPHA8 */
   //         return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
   //      case 0x8E8C: /* GL_COMPRESSED_RGBA_BPTC_UNORM */
   //         return DXGI_FORMAT_BC7_UNORM;
   //      case 0x8E8D: /* GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM */
   //         return DXGI_FORMAT_BC7_UNORM_SRGB;
   //      default:
   //         assert(false);
   //         return DXGI_FORMAT_UNKNOWN;
   //   }
   //}

   //Dx11Texture CreateDx11CubemapFromKtx(ID3D11Device *device, const ktxTexture *ktxTex)
   //{
   //   assert(device);
   //   assert(ktxTex);
   //   assert(ktxTex->isCubemap);

   //   Dx11Texture out = {};

   //   const uint32_t mipCount = ktxTex->numLevels;
   //   const uint32_t layerCount = ktxTex->numLayers; // cubemap arrays supported
   //   const uint32_t faceCount = 6;

   //   DXGI_FORMAT format = GlOrVkToDxgi(ktxTex);

   //   // ------------------------------------------------------------
   //   // Texture description
   //   // ------------------------------------------------------------
   //   D3D11_TEXTURE2D_DESC desc = {};
   //   desc.Width = ktxTex->baseWidth;
   //   desc.Height = ktxTex->baseHeight;
   //   desc.MipLevels = mipCount;
   //   desc.ArraySize = layerCount * faceCount;
   //   desc.Format = format;
   //   desc.SampleDesc.Count = 1;
   //   desc.Usage = D3D11_USAGE_DEFAULT;
   //   desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
   //   desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

   //   // ------------------------------------------------------------
   //   // Subresource data
   //   // DX11 subresource index:
   //   //   mip + arraySlice * mipCount
   //   // ------------------------------------------------------------
   //   ::array_base<D3D11_SUBRESOURCE_DATA> sub;
   //   sub.resize(desc.ArraySize * mipCount);

   //   for (uint32_t layer = 0; layer < layerCount; ++layer)
   //   {
   //      for (uint32_t face = 0; face < faceCount; ++face)
   //      {
   //         for (uint32_t mip = 0; mip < mipCount; ++mip)
   //         {
   //            ktx_size_t offset = 0;
   //            KTX_error_code err = ktxTexture_GetImageOffset((ktxTexture *) ktxTex, mip, layer, face, &offset);
   //            assert(err == KTX_SUCCESS);

   //            uint32_t arraySlice = layer * faceCount + face;
   //            uint32_t subIndex = mip + arraySlice * mipCount;

   //            sub[subIndex].pSysMem = (const uint8_t *)ktxTex->pData + offset;

   //            sub[subIndex].SysMemPitch = ktxTexture_GetRowPitch((ktxTexture *)ktxTex, mip);

   //            sub[subIndex].SysMemSlicePitch = 0;
   //         }
   //      }
   //   }

   //   // ------------------------------------------------------------
   //   // Create texture
   //   // ------------------------------------------------------------
   //   HRESULT hr = device->CreateTexture2D(&desc, sub.data(), &out.texture);
   //   assert(SUCCEEDED(hr));

   //   // ------------------------------------------------------------
   //   // Create SRV
   //   // ------------------------------------------------------------
   //   D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
   //   srvDesc.Format = desc.Format;
   //   srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
   //   srvDesc.TextureCube.MipLevels = mipCount;

   //   hr = device->CreateShaderResourceView(out.texture, &srvDesc, &out.srv);
   //   assert(SUCCEEDED(hr));

   //   return out;
   //}

   void context::load_ktxTexture_cube_map(::gpu::texture * pgputexture, void * p_ktxTexture)
   {
      auto pktxtexture = (ktxTexture *)p_ktxTexture;

      ::cast<::gpu_directx11::texture> ptexture = pgputexture;

      ::cast<device> pdevice = m_pgpudevice;


      auto tex = CreateDx11TextureFromKtx(pdevice->m_pd3d11device1, pktxtexture);

      ptexture->m_ptextureOffscreen = tex.texture;
      ptexture->m_pshaderresourceview = tex.srv;
      ptexture->m_textureattributes.m_etexture = ::gpu::e_texture_cube_map;
      ptexture->set_ok_flag();

   }

} // namespace gpu_directx11
 
