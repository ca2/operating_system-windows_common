// From github:/tristancalderbank/OpenGL-PBR-Renderer/cubemap_framebuffer.cpp by
// camilo on 2025-09-26 19:52 <3ThomasBorregaardSorensen!!
// From gpu_directx11 by camilo on 2025-10-16 17:37 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "cubemap_framebuffer.h"
#include "bred/gpu/context_lock.h"
#include "bred/gpu/shader.h"
#include "gpu_directx11/context.h"
//#include <glad/glad.h>

#include "texture.h"


namespace gpu_directx11
{


   namespace ibl
   {


      cubemap_framebuffer::cubemap_framebuffer()
      {


      }


      cubemap_framebuffer::~cubemap_framebuffer()
      {


      }


      void cubemap_framebuffer::on_initialize_cubemap_framebuffer()
      {

         ::gpu::context_lock contextlock(m_pgpucontext);

         ::cast < gpu_directx11::texture>ptexture = m_ptexture;

         ::cast<gpu_directx11::context> pcontext = m_pgpucontext;

         //ptexture->m_mipsLevel = (uint32_t)(floor(::log2((double)::maximum(ptexture->m_rectangleTarget.width(),
                                                                           //ptexture->m_rectangleTarget.height()))) +
           //                                 1.0);
                                                                              ptexture->m_mipsLevel = -1;
         ptexture->m_bTransferSrc = true;
         ptexture->m_bWithDepth = false;
         ptexture->m_bSrgb = true;
         ptexture->initialize_image_texture(m_pgpucontext->m_pgpurenderer, ptexture->m_rectangleTarget, true, {},
                                            ::gpu::texture::e_type_cube_map);

        

         //// framebuffer
         //glGenFramebuffers(1, &ptexture->m_gluFbo);
         //GLCheckError("");
         //glBindFramebuffer(GL_FRAMEBUFFER, ptexture->m_gluFbo);
         //GLCheckError("");

         //// depth buffer
         //glGenRenderbuffers(1, &ptexture->m_gluDepthStencilRBO);
         //GLCheckError("");
         //ptexture->m_gluType = GL_TEXTURE_CUBE_MAP;
         //glBindRenderbuffer(GL_RENDERBUFFER, ptexture->m_gluDepthStencilRBO);
         //GLCheckError("");
         //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, ptexture->m_rectangleTarget.width(), ptexture->m_rectangleTarget.height());
         //GLCheckError("");

         //// attach the depth buffer
         //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ptexture->m_gluDepthStencilRBO);
         //GLCheckError("");

         //// cubemap
         //glGenTextures(1, &ptexture->m_gluTextureID);
         //GLCheckError("");
         //glBindTexture(ptexture->m_gluType, ptexture->m_gluTextureID);
         //GLCheckError("");

         //int width = ptexture->m_rectangleTarget.width();
         //int height = ptexture->m_rectangleTarget.height();

         //// specify/allocate each face for the cubemap
         //for (auto i = 0; i < 6; i++)
         //{

         //   glTexImage2D(
         //      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
         //      0,
         //      GL_RGB16F,
         //      width,
         //      height,
         //      0,
         //      GL_RGB,
         //      GL_FLOAT,
         //      nullptr);
         //   GLCheckError("");

         //}

         //glTexParameteri(ptexture->m_gluType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         //GLCheckError("");
         //glTexParameteri(ptexture->m_gluType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
         //GLCheckError("");
         //glTexParameteri(ptexture->m_gluType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
         //GLCheckError("");
         //glTexParameteri(ptexture->m_gluType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         //GLCheckError("");
         //glTexParameteri(ptexture->m_gluType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         //GLCheckError("");

         //GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

         //if (status != GL_FRAMEBUFFER_COMPLETE)
         //{

         //   warning() << "Framebuffer incomplete! with status " << status;
         //}

         //glBindFramebuffer(GL_FRAMEBUFFER, 0);
         //GLCheckError("");

      }


      void cubemap_framebuffer::bind(::gpu::command_buffer *pgpucommandbuffer)
      {
         
         /*::cast < gpu_directx11::texture>ptexture = m_ptexture;
         glBindFramebuffer(GL_FRAMEBUFFER, ptexture->m_gluFbo);
         GLCheckError("");*/
      }


      void cubemap_framebuffer::generateMipmap()
      {
         //::cast < gpu_directx11::texture>ptexture = m_ptexture;
         //glBindTexture(GL_TEXTURE_CUBE_MAP, ptexture->m_gluTextureID);
         //GLCheckError("");
         //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
         //GLCheckError("");
         //glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
         //GLCheckError("");
         ::cast<gpu_directx11::texture> ptexture = m_ptexture;
         ::cast<gpu_directx11::context> pcontext = m_pgpucontext;

           // Now generate mipmaps using DirectX
         pcontext->m_pcontext->GenerateMips(ptexture->m_pshaderresourceview);

      }


      void cubemap_framebuffer::setCubeFace(unsigned int index, ::gpu::shader * pgpushader)
      {
         ::cast < gpu_directx11::texture>ptexture = m_ptexture;
         ::cast<gpu_directx11::context> pcontext = m_pgpucontext;
         ::cast<gpu_directx11::device> pdevice = pcontext->m_pgpudevice;
         ////glFramebufferTexture2D(
         //  // GL_FRAMEBUFFER,
         //   //GL_COLOR_ATTACHMENT0,
         //   //GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
         //   //ptexture->m_gluTextureID,
         //   //0);
         //   D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
         //rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // match texture format
         //rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
         //rtvDesc.Texture2DArray.MipSlice = 0;
         //rtvDesc.Texture2DArray.FirstArraySlice = index; // which cube face
         //rtvDesc.Texture2DArray.ArraySize = 1; // just one face
         //ptexture->m_rendertargetviewa.set_size(6);
         //HRESULT hr = pdevice->m_pdevice->CreateRenderTargetView(ptexture->m_ptextureOffscreen, &rtvDesc, &ptexture->m_rendertargetviewa[index]);
         //defer_throw_hresult(hr);

                  ID3D11RenderTargetView *rtvPtr = nullptr;
         if (index < ptexture->m_rendertargetviewa.size() && ptexture->m_rendertargetviewa[index])
            rtvPtr = ptexture->m_rendertargetviewa[index];


         // --- Bind the cube face as render target ---
         pcontext->m_pcontext->OMSetRenderTargets(1, &rtvPtr, nullptr);

         //GLCheckError("");
         //
         ////if (m_strSamplerUniform.has_character())
         ////{
         ////   pgpushader->set_int(m_strSamplerUniform, 0);
         ////}

      }


      //unsigned int cubemap_framebuffer::getCubemapTextureId()
      //{
      //   ::cast < gpu_directx11::texture>ptexture = m_ptexture;
      //   return ptexture->m_gluTextureID;
      //}


   } // namespace ibl


} // namespace gpu_directx11
