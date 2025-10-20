// From github:/tristancalderbank/OpenGL-PBR-Renderer/specular_map.cpp by
// camilo on 2025-09-26 19:55 <3ThomasBorregaardSorensen!!
// From gpu_directx11 by camilo on 2025-10-16 17:38 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "specular_map.h"
#include "brdf_convolution_framebuffer.h"
#include "bred/graphics3d/render_system.h"
#include "bred/graphics3d/scene_base.h"
#include "bred/graphics3d/skybox.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context.h"
#include "bred/gpu/context_lock.h"
#include "bred/gpu/device.h"
#include "gpu_directx11/_gpu_directx11.h"
#include "gpu_directx11/context.h"
#include "gpu_directx11/texture.h"
#include "gpu/cube.h"
#include "gpu/gltf/_constant.h"
//#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "gpu/context.h"
//#include "::gpu::gltf.h"
//#include "cube.h"
//#include "fullscreenquad.h"
//#include "timer.h"
#include "mipmap_cubemap_framebuffer.h"
#include "gpu/full_screen_quad.h"

#include "gpu/ibl/_hlsl.h"


namespace gpu_directx11
{


   namespace ibl
   {

      glm::vec3 rot180Y(::glm::vec3 v);

      specular_map::specular_map()
      {


      }


      specular_map::~specular_map()
      {

      }



      ::block specular_map::embedded_prefiltered_env_map_vert()
      {

         return hlsl_embedded_prefiltered_env_map_vert();

      }



      ::block specular_map::embedded_prefiltered_env_map_frag()
      {
         
         return hlsl_embedded_prefiltered_env_map_frag();

      }


      ::block specular_map::embedded_brdf_convolution_frag()
      {

         return hlsl_embedded_brdf_convolution_frag();

      }

      ::block specular_map::embedded_brdf_convolution_vert()
      {
         
         return hlsl_embedded_brdf_convolution_vert(); 
      
      }



      // void specular_map::initialize_specular_map(const ::string &engineRoot, const unsigned int environmentCubemapId)
      // {
      //
      //    initialize_environmentCubemapId(environmentCubemapId);
      //
      //    // pre-filtered env map
      //    ::string prefilteredEnvMapVertexShaderPath = engineRoot + "/src/ibl/shaders/specularenv.vert";
      //    ::string prefilteredEnvMapFragmentShaderPath = engineRoot + "/src/ibl/shaders/specularenv.frag";
      //
      //    prefilteredEnvMapShader = std::make_unique<Shader>(prefilteredEnvMapVertexShaderPath.c_str(),
      //                                                       prefilteredEnvMapFragmentShaderPath.c_str());
      //    prefilteredEnvMapFramebuffer = std::make_unique<mipmap_cubemap_framebuffer>(
      //       prefilteredEnvMapWidth, prefilteredEnvMapHeight);
      //
      //    // BRDF convolution
      //    ::string brdfConvolutionVertexShaderPath = engineRoot + "/src/ibl/shaders/brdfconvolution.vert";
      //    ::string brdfConvolutionFragmentShaderPath = engineRoot + "/src/ibl/shaders/brdfconvolution.frag";
      //
      //    brdfConvolutionShader = std::make_unique<Shader>(brdfConvolutionVertexShaderPath.c_str(),
      //                                                     brdfConvolutionFragmentShaderPath.c_str());
      //    brdfConvolutionFramebuffer = std::make_unique<brdf_convolution_framebuffer>(
      //       m_uBrdfConvolutionMapWidth, brdfConvolutionMapHeight);
      //
      // }


      void specular_map::computePrefilteredEnvMap(::gpu::command_buffer * pgpucommandbuffer)
      {
         //return;
         //Timer timer;

         ::gpu::context_lock contextlock(m_pgpucontext);

         //auto pgpucommandbuffer = m_pgpucontext->beginSingleTimeCommands(m_pgpucontext->m_pgpudevice->graphics_queue());

         glm::mat4 model = ::gpu::gltf::mIndentity4;
         glm::mat4 cameraAngles[] = {
            // Swap +X/-X
            glm::lookAt(::gpu::gltf::origin, rot180Y(-::gpu::gltf::unitX), -::gpu::gltf::unitY), // DX +X face
            glm::lookAt(::gpu::gltf::origin, rot180Y(::gpu::gltf::unitX), -::gpu::gltf::unitY), // DX -X face

            // +Y/-Y (may also need flipping depending on your loader)
            glm::lookAt(::gpu::gltf::origin, rot180Y(::gpu::gltf::unitY), ::gpu::gltf::unitZ),
            glm::lookAt(::gpu::gltf::origin, rot180Y(-::gpu::gltf::unitY), -::gpu::gltf::unitZ),

            // +Z/-Z
            glm::lookAt(::gpu::gltf::origin, rot180Y(::gpu::gltf::unitZ), -::gpu::gltf::unitY),
            glm::lookAt(::gpu::gltf::origin, rot180Y(-::gpu::gltf::unitZ), -::gpu::gltf::unitY)};

         glm::mat4 projection = glm::perspective(
            glm::radians(90.0f), // 90 degrees to cover one face
            1.0f, // its a square
            0.1f,
            2.0f);

         auto pskybox = m_pscene->current_skybox();

         //auto prenderable = pskybox->m_prenderable;

         auto ptexture = pskybox->m_ptexture;

         //auto pcube = øcreate < ::gpu::cube >();
         //::cast < ::gpu_gpu::context > pcontext = m_pgpucontext;
         auto pcube = øcreate<::gpu::cube>();
         pcube->initialize_gpu_cube(m_pgpucontext);
         ::cast<::gpu_directx11::texture> ptextureSkybox = ptexture;
         ::cast<::gpu_directx11::context> pcontext = m_pgpucontext;

         ::cast<::gpu_directx11::texture> ptextureEnvMap = m_pframebufferPrefilteredEnvMap->m_ptexture;
         ::cast<::gpu_directx11::ibl::mipmap_cubemap_framebuffer> pframebufferEnvMap = m_pframebufferPrefilteredEnvMap;

         //m_pframebufferPrefilteredEnvMap->bind();
         //m_pshaderPrefilteredEnvMap->_bind();
         m_pshaderPrefilteredEnvMap->_bind(nullptr, ::gpu::e_scene_none);
         m_pshaderPrefilteredEnvMap->bind_source(nullptr, ptextureSkybox);
         //m_pshaderPrefilteredEnvMap->set_int("environmentCubemap", 0);

         for (auto iMip = 0; iMip < m_iPrefilteredEnvMapMipCount; iMip++)
         {
            m_pframebufferPrefilteredEnvMap->set_current_mip(iMip);

            //glViewport(0, 0, m_pframebufferPrefilteredEnvMap->getWidth(), m_pframebufferPrefilteredEnvMap->getHeight());
            //GLCheckError("");
            // each mip level has increasing roughness
            float roughness = (float)iMip / (float)(m_iPrefilteredEnvMapMipCount - 1);
            m_pshaderPrefilteredEnvMap->set_float("roughness", roughness);

            // render to each side of the cubemap
            for (auto iFace = 0; iFace < 6; iFace++)
            {
               m_pshaderPrefilteredEnvMap->setModelViewProjectionMatrices(model, cameraAngles[iFace], projection);
               m_pframebufferPrefilteredEnvMap->set_cube_face(iFace);

               //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
               //GLCheckError("");
               ::int_rectangle rectangle;
               rectangle.set(0, 0, m_pframebufferPrefilteredEnvMap->m_ptexture->m_sizeMip.cx(),
                             m_pframebufferPrefilteredEnvMap->m_ptexture->m_sizeMip.cy());
               // Bind RTV
               auto prendertargetview = ptextureEnvMap->render_target_view(iFace, iMip);
                                                //->m_pt[face * MIP_COUNT + mip]
                                                //.Get();
               pcontext->m_pcontext->OMSetRenderTargets(1, &prendertargetview, nullptr);

               m_pgpucontext->set_viewport(pgpucommandbuffer, rectangle);
               //glBindTexture(GL_TEXTURE_CUBE_MAP, ptextureSkybox->m_gluTextureID);
               //GLCheckError("");
               //m_pshaderPrefilteredEnvMap->set_int("environmentCubemap", 0);
               //pcube->draw(pgpucommandbuffer);
               ::graphics3d::render_system rendersystemScope;
               rendersystemScope.m_erendersystem = ::graphics3d::e_render_system_skybox_ibl;
               pgpucommandbuffer->m_prendersystem = &rendersystemScope;
               m_pshaderPrefilteredEnvMap->push_properties(pgpucommandbuffer);
               pcube->bind(pgpucommandbuffer);
               pcube->draw(pgpucommandbuffer);
               pcube->unbind(pgpucommandbuffer);
               pgpucommandbuffer->m_prendersystem = nullptr;

            }
         }

         ////timer.logDifference("Rendered specular pre-filtered environment map");
         //// timer.logDifference("Rendered diffuse irradiance map");
         //GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

         //if (status != GL_FRAMEBUFFER_COMPLETE)
         //{

         //   warning() << "Framebuffer incomplete! with status " << status;
         //}

         //glBindFramebuffer(GL_FRAMEBUFFER, 0);
         //GLCheckError("");

      }


      //unsigned int specular_map::getPrefilteredEnvMapId()
      //{

      //   ::cast <gpu_directx11::ibl::mipmap_cubemap_framebuffer> pframebuffer = m_pframebufferPrefilteredEnvMap;

      //   return pframebuffer->getCubemapTextureId();

      //}


      void specular_map::computeBrdfConvolutionMap()
      {
         //return;
         ::gpu::ibl::specular_map::computeBrdfConvolutionMap();

         ::cast<::gpu_directx11::context> pcontext = m_pgpucontext;
         ::cast<::gpu_directx11::device> pdevice = m_pgpucontext->m_pgpudevice;
         ::cast<::gpu_directx11::texture> ptexture = m_pbrdfconvolutionframebuffer->m_ptexture;

         ID3D11RenderTargetView *rtvs[8];
         UINT numRTVs = 8;
         pcontext->m_pcontext->OMGetRenderTargets(numRTVs, rtvs, nullptr);

         if (rtvs[0] == ptexture->m_prendertargetview)
         {

            information() << "seems ok";

         }
         else
         {

            information() << "well...";

         }

         D3D11_VIEWPORT vp;
         UINT num = 1;
         pcontext->m_pcontext->RSGetViewports(&num, &vp);
         if (vp.Width > 0 && vp.Height > 0)
         {

            information() << "a viewport";
         }
         else
         {

            information() << "empty viewport";
         }

         ::comptr<ID3D11Texture2D> pd3d11textureStaging;
         D3D11_TEXTURE2D_DESC desc = {};
         ptexture->m_ptextureOffscreen->GetDesc(&desc);
         desc.Usage = D3D11_USAGE_STAGING;
         desc.BindFlags = 0;
         desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

         pdevice->m_pdevice->CreateTexture2D(&desc, nullptr, &pd3d11textureStaging);
         pcontext->m_pcontext->CopyResource(pd3d11textureStaging, ptexture->m_ptextureOffscreen);

         
         // 4. Map (readback)
         D3D11_MAPPED_SUBRESOURCE mapped = {};
         HRESULT hr = pcontext->m_pcontext->Map(pd3d11textureStaging, 0, D3D11_MAP_READ, 0, &mapped);
         ::defer_throw_hresult(hr);
         //{
         //   printf("Failed to map staging texture: 0x%08X\n", hr);
         //   return;
         //}

         //// 5. Print the first few values
         ////    Adjust according to your format: assuming DXGI_FORMAT_R16G16_FLOAT
         //uint8_t *rowPtr = reinterpret_cast<uint8_t *>(mapped.pData);
         //for (UINT y = 0; y < minimum(desc.Height, 4u); y++) // print first 4 rows
         //{
         //   auto *pixel = reinterpret_cast<const uint16_t *>(rowPtr); // 16-bit floats
         //   for (UINT x = 0; x < minimum(desc.Width, 4u); x++)
         //   {
         //      // Convert half to float (quick approximate if no helper)
         //      float r = DirectX::PackedVector::XMConvertHalfToFloat(pixel[0]);
         //      float g = DirectX::PackedVector::XMConvertHalfToFloat(pixel[1]);
         //      printf("(%f, %f) ", r, g);
         //      pixel += 2; // R16G16
         //   }
         //   printf("\n");
         //   rowPtr += mapped.RowPitch;
         //}
         // 5. Print the first few values
         //    for DXGI_FORMAT_R32G32_FLOAT
         float *rowPtr = (float*)(mapped.pData);
         for (UINT y = 0; y < minimum(desc.Height, 4u); y++) // print first 4 rows
         {
            auto *pixel = rowPtr; // 32-bit floats
            for (UINT x = 0; x < minimum(desc.Width, 4u); x++)
            {
               // Convert half to float (quick approximate if no helper)
               float r = pixel[0];
               float g = pixel[1];
               informationf("(%f, %f) ", r, g);
               pixel += 2; // R32G32
            }
            //printf("\n");
            rowPtr += mapped.RowPitch;
         }

         // 6. Unmap
         pcontext->m_pcontext->Unmap(pd3d11textureStaging, 0);

         information() << "end computerBrdfConvolutionMap";

      }


      //unsigned int specular_map::getBrdfConvolutionMapId()
      //{

      //   ::cast <::gpu_directx11::ibl::brdf_convolution_framebuffer> pframebuffer = m_pbrdfconvolutionframebuffer;

      //   return pframebuffer->getColorTextureId();

      //}


   } // namespace ibl


} // namespace gpu_directx11
