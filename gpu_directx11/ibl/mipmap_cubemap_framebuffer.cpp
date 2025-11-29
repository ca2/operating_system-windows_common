// From github:/tristancalderbank/OpenGL-PBR-Renderer/mipmap_cubemap_framebuffer.h by
// camilo on 2025-09-26 19:54 <3ThomasBorregaardSorensen!!
// From gpu_directx11 by camilo on 2025-10-16 17:38 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "mipmap_cubemap_framebuffer.h"
#include "bred/gpu/context_lock.h"
#include "gpu_directx11/context.h"

#include "texture.h"


namespace gpu_directx11
{


   namespace ibl
   {
      


      mipmap_cubemap_framebuffer::mipmap_cubemap_framebuffer()
      {


      }


      mipmap_cubemap_framebuffer::~mipmap_cubemap_framebuffer()
      {


      }


      void mipmap_cubemap_framebuffer::on_initialize_mipmap_cubemap_framebuffer()
      {

         ::gpu::context_lock contextlock(m_pgpucontext);

         // cast to your DX11 texture wrapper
         ::cast<::gpu_directx11::texture> ptexture = m_ptexture;

         // ---- Obtain device/context from your gpu context
         // ADAPT THIS to how your m_pgpucontext exposes device/context.
         // Common names: m_pgpucontext->m_pd3dDevice and m_pgpucontext->m_pd3dDeviceContext
         ID3D11Device *device = nullptr;
         ID3D11DeviceContext *context = nullptr;

         // === ADAPT: replace these with your actual members/accessors ===
         // Example assumed members (change if your code differs):
         // device = m_pgpucontext->m_pd3dDevice.Get();
         // context = m_pgpucontext->m_pd3dDeviceContext.Get();
         //
         // If your gpu context exposes methods, use them:
         // device = m_pgpucontext->device();
         // context = m_pgpucontext->context();
         //
         // For now attempt common member names and fall back gracefully.

         // try some likely fields (non-owning raw pointers)
         //if (m_pgpucontext != nullptr)
         //{
         //   // attempt to access common fields via pointers (these lines are safe no-op if not present at compile-time)
         //   // You need to adapt these lines to your project if you have different names.
         //   // The following lines are placeholders and likely need to be changed:
         //   device = m_pgpucontext->m_pd3dDevice;
         //   context = m_pgpucontext->m_pd3dDeviceContext;
         //}

         //if (device == nullptr || context == nullptr)
         //{
         //   // Could not obtain device/context; you must adapt above lines.
         //   OutputDebugStringA("mipmap_cubemap_framebuffer (DX11): device/context is null — adapt code to your gpu "
         //                      "context's device/context members.\n");
         //   return;
         //}

         //// Query base texture size from your texture wrapper. ADAPT if your texture member names differ.
         //// I assume ptexture has m_rectangleTarget.width()/height() like your GL code.
         //m_baseWidth = static_cast<UINT>(ptexture->m_rectangleTarget.width());
         //m_baseHeight = static_cast<UINT>(ptexture->m_rectangleTarget.height());

         //// compute mip count: you may already have ptexture->m_iMipCount or similar
         // We'll compute full mip chain if not provided

         if (ptexture->m_iMipCount <= 1)
         {
            ptexture->m_iMipCount =
               1u + static_cast<UINT>(std::floor(std::log2(static_cast<float>(
                       std::max(ptexture->m_rectangleTarget.width(), ptexture->m_rectangleTarget.height())))));
         }
         if (ptexture->m_iMipCount < 1)
            ptexture->m_iMipCount = 1;

         // Create the cubemap texture + SRV + RTVs
         createCubemapTextureAndViews();

         // set current mip to what base class may have (if any). We'll default to 0.
         //m_uCurrentMip = static_cast<unsigned int>(
           // ptexture->m_iMipCount); // ADAPT if your texture class stores current mip differently
         if (ptexture->m_iCurrentMip >= ptexture->m_iMipCount)
            ptexture->m_iCurrentMip = 0;

         if (m_ptexture->m_bWithDepth)
         {

            // create depth for the current mip level
            createDepthForCurrentMip();
         }

      }

      
      void mipmap_cubemap_framebuffer::createCubemapTextureAndViews()
      {
         ::gpu::context_lock contextlock(m_pgpucontext);
         ::cast<::gpu_directx11::context> pgpucontext = m_pgpucontext;

         ::cast<::gpu_directx11::device> pgpudevice = pgpucontext->m_pgpudevice;

         //ID3D11Device *device = pgpucontext->m_pd3dDevice;
         ::cast<::gpu_directx11::texture> ptexture = m_ptexture;
         ///ID3D11Device *device = m_pgpucontext->m_pd3dDevice;
         /*if (!device)
         {
            OutputDebugStringA("createCubemapTextureAndViews: device null\n");
            return;
         }*/

         // Describe texture2D as an array of 6 textures (cube) with mipmaps
         D3D11_TEXTURE2D_DESC desc = {};
         desc.Width = ptexture->width();
         desc.Height = ptexture->height();
         desc.MipLevels = ptexture->m_iMipCount;
         desc.ArraySize = 6; // cube
         desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; // use 16-bit float RGBA similar to GL_RGB16F (use RGBA because
                                                       // DX doesn't have RGB)
         desc.SampleDesc.Count = 1;
         desc.Usage = D3D11_USAGE_DEFAULT;
         desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
         desc.CPUAccessFlags = 0;
         desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

         HRESULT hr = pgpudevice->m_pdevice->CreateTexture2D(&desc, nullptr, &ptexture->m_ptextureOffscreen);
         if (FAILED(hr) || !ptexture->m_ptextureOffscreen)
         {
            OutputDebugStringA("Failed CreateTexture2D for cubemap.\n");
            return;
         }

         // Create SRV (TEXTURECUBE)
         D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
         srvDesc.Format = desc.Format;
         srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
         srvDesc.TextureCube.MipLevels = desc.MipLevels;
         srvDesc.TextureCube.MostDetailedMip = 0;

         hr = pgpudevice->m_pdevice->CreateShaderResourceView(
            ptexture->m_ptextureOffscreen, 
            &srvDesc,
            &ptexture->m_pshaderresourceview);

         if (FAILED(hr) || !ptexture->m_pshaderresourceview)
         {
            
            OutputDebugStringA("Failed CreateShaderResourceView for cubemap.\n");
            
            defer_throw_hresult(hr);

            return;

         }

         // Create RTVs: one per face per mip level
         ptexture->m_rendertargetviewa.clear();
         ptexture->m_rendertargetviewa.set_size(6 * ptexture->m_iMipCount);

         for (int iFace = 0; iFace < 6; ++iFace)
         {
            
            for (int iMip = 0; iMip < ptexture->m_iMipCount; ++iMip)
            {
               
               D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};

               rtvDesc.Format = desc.Format;
               // We create RTV for a single array slice (face) and a single mip slice
               rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
               rtvDesc.Texture2DArray.MipSlice = (UINT)iMip;
               rtvDesc.Texture2DArray.FirstArraySlice = (UINT)iFace;
               rtvDesc.Texture2DArray.ArraySize = 1;

               auto iRenderTargetView = ptexture->render_target_view_index(iFace, iMip);

               auto &rtv = ptexture->m_rendertargetviewa[iRenderTargetView];

               HRESULT hr = pgpudevice->m_pdevice->CreateRenderTargetView(
                  ptexture->m_ptextureOffscreen, &rtvDesc, &rtv);

               ::defer_throw_hresult(hr);

            }

         }

      }

      
      void mipmap_cubemap_framebuffer::createDepthForCurrentMip()
      {

         ::gpu::context_lock contextlock(m_pgpucontext);

         ::cast<::gpu_directx11::texture> ptexture = m_ptexture;
               ::cast<::gpu_directx11::context> pgpucontext = m_pgpucontext;

               ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;


               ID3D11Device *device = pgpudevice->m_pdevice;

         ID3D11DeviceContext *context = pgpucontext->m_pcontext;

         //ID3D11Device *device =pgpucontext->m_pd3dDevice;

             // Release previous depth resources
         ptexture->m_pdepthstencilview.Release();
         ptexture->m_ptextureDepth.release();

         auto w = ptexture->mip_width();
         auto h = ptexture->mip_height();

         D3D11_TEXTURE2D_DESC desc = {};
         desc.Width = w;
         desc.Height = h;
         desc.MipLevels = 1;
         desc.ArraySize = 1;
         desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // depth24 + stencil
         desc.SampleDesc.Count = 1;
         desc.Usage = D3D11_USAGE_DEFAULT;
         desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
         desc.CPUAccessFlags = 0;
         desc.MiscFlags = 0;

         ødefer_construct(ptexture->m_ptextureDepth);

         ::cast<::gpu_directx11::texture> ptextureDepth = ptexture->m_ptextureDepth;

         HRESULT hr = pgpudevice->m_pdevice->CreateTexture2D(&desc, nullptr, &ptextureDepth->m_ptextureOffscreen);
         if (FAILED(hr) || !ptextureDepth->m_ptextureDepth)
         {
            OutputDebugStringA("Failed CreateTexture2D for depth.\n");
            return;
         }

         hr = device->CreateDepthStencilView(ptextureDepth->m_ptextureOffscreen, nullptr, &ptextureDepth->m_pdepthstencilview);
         if (FAILED(hr) || !ptextureDepth->m_pdepthstencilview)
         {
            OutputDebugStringA("Failed CreateDepthStencilView.\n");
            return;
         }
      }


      void mipmap_cubemap_framebuffer::bind()
      {
      
         ::gpu::context_lock contextlock(m_pgpucontext);

         ::cast<::gpu_directx11::texture> ptexture = m_ptexture;

         ::cast<::gpu_directx11::context> pgpucontext = m_pgpucontext;

         ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

         // choose the RTV for current face + mip
         size_t idx = ptexture->current_render_target_view_index();

         ID3D11RenderTargetView *rtvPtr = nullptr;

         if (idx < ptexture->m_rendertargetviewa.size() && ptexture->m_rendertargetviewa[idx])
         {

            rtvPtr = ptexture->m_rendertargetviewa[idx];

         }

         // Bind the single RTV and the current DSV
         pgpucontext->m_pcontext->OMSetRenderTargets(rtvPtr ? 1 : 0, rtvPtr ? &rtvPtr : nullptr, ptexture->m_pdepthstencilview);

         // set viewport to match current mip size
         D3D11_VIEWPORT vp = {};
         vp.TopLeftX = 0.0f;
         vp.TopLeftY = 0.0f;
         vp.Width = (float) ptexture->mip_width();
         vp.Height = (float)ptexture->mip_height();
         vp.MinDepth = 0.0f;
         vp.MaxDepth = 1.0f;
         pgpucontext->m_pcontext->RSSetViewports(1, &vp);

      }


      void mipmap_cubemap_framebuffer::set_current_mip(int iMip)
      {
         
         ::gpu::ibl::mipmap_cubemap_framebuffer::set_current_mip(iMip); // call base (as original)

         ::cast<::gpu_directx11::texture> ptexture = m_ptexture;

         if (iMip >= m_ptexture->m_iMipCount)
         {

            iMip = 0;

         }

         ptexture->m_iCurrentMip = iMip;

         // Recreate depth buffer for new mip size (similar to GL glRenderbufferStorage)
         //createDepthForCurrentMip();

         // Optionally: if you want to change SRV/RTV usage or other states per mip, do it here.
      }

      void mipmap_cubemap_framebuffer::set_cube_face(int iFace)
      {
         ::gpu::ibl::mipmap_cubemap_framebuffer::set_cube_face(iFace);
         if (iFace >= 6)
            iFace = 0;
         m_ptexture->m_iCurrentFace = iFace;
         // We don't actually rebind here — bind() will pick up current face & mip
      }

      //unsigned int mipmap_cubemap_framebuffer::getCubemapTextureId()
      //{
      //   // There is no GLuint id in D3D11. You can return a pointer cast if you want:
      //   // return static_cast<unsigned int>(reinterpret_cast<uintptr_t>(m_texCube.Get()));
      //   // But returning 0 to indicate "not applicable" keeps it safe.
      //   return 0u;
      //}


   } // namespace ibl


} // namespace gpu_directx11



