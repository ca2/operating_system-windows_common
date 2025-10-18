// From github:/tristancalderbank/OpenGL-PBR-Renderer/equirectangular_cubemap.cpp by
// camilo on 2025-09-26 19:53 <3ThomasBorregaardSorensen!!
// From gpu_directx11 by camilo on 2025-10-16 17:37 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "equirectangular_cubemap.h"
#include "bred/gpu/context.h"
#include "bred/graphics3d/skybox.h"
#include "bred/gpu/device.h"
#include "bred/gpu/texture.h"
#include <glm/gtc/matrix_transform.hpp>
//#include "glad/glad.h"
#include "glm/glm.hpp"

#include "gpu/gltf/_constant.h"
#include "gpu_directx11/_gpu_directx11.h"
#include "gpu_directx11/context.h"
// #include "timer.h"
#include "cubemap_framebuffer.h"
#include "hdri_cube.h"

#include "gpu/ibl/_hlsl.h"

//#include "shaders/hdricube.frag.h"
//#include "shaders/hdricube.vert.h"

namespace gpu_directx11
{


   namespace ibl
   {

      equirectangular_cubemap::equirectangular_cubemap()
      {


      }


      equirectangular_cubemap::~equirectangular_cubemap()
      {


      }


      ::block equirectangular_cubemap::embedded_ibl_hdri_cube_vert()
      {

         return hlsl_embedded_ibl_hdri_cube_vert();
      }


      ::block equirectangular_cubemap::embedded_ibl_hdri_cube_frag()
      {

         return hlsl_embedded_ibl_hdri_cube_frag();
      }


      // equirectangular_cubemap::equirectangular_cubemap(const ::string &engineRoot, const ::string &hdriPath) {
      //     ::string hdriVertexShaderPath = engineRoot + "/src/ibl/shaders/hdri_cube.vert";
      //     ::string hdriFragmentShaderPath = engineRoot + "/src/ibl/shaders/hdri_cube.frag";
      //
      //     hdriShader = std::make_unique<Shader>(hdriVertexShaderPath.c_str(), hdriFragmentShaderPath.c_str());
      //     hdri_cube = std::make_unique<hdri_cube>(hdriPath);
      //     framebuffer = std::make_unique<cubemap_framebuffer>(cubemapWidth, cubemapHeight);
      // }
      glm::vec3 rot180Y(::glm::vec3 v) { return glm::vec3(-v.x, v.y, -v.z); }

      void equirectangular_cubemap::compute()
      {
         // Timer timer;

         auto pgpucommandbuffer = m_pgpucontext->beginSingleTimeCommands(m_pgpucontext->m_pgpudevice->graphics_queue());
         glm::mat4 model = ::gpu::gltf::mIndentity4;
         //glm::mat4 cameraAngles[] = {glm::lookAt(::gpu::gltf::origin, ::gpu::gltf::unitX, -::gpu::gltf::unitY),
           ///                          glm::lookAt(::gpu::gltf::origin, -::gpu::gltf::unitX, -::gpu::gltf::unitY),
              //                       glm::lookAt(::gpu::gltf::origin, ::gpu::gltf::unitY, ::gpu::gltf::unitZ),
                ///                     glm::lookAt(::gpu::gltf::origin, -::gpu::gltf::unitY, -::gpu::gltf::unitZ),
                   //                  glm::lookAt(::gpu::gltf::origin, ::gpu::gltf::unitZ, -::gpu::gltf::unitY),
                     //                glm::lookAt(::gpu::gltf::origin, -::gpu::gltf::unitZ, -::gpu::gltf::unitY)};
         // Rotate targets around Y
         glm::mat4 cameraAngles[] = {
            // Swap +X/-X
            glm::lookAt(::gpu::gltf::origin, rot180Y( - ::gpu::gltf::unitX), -::gpu::gltf::unitY), // DX +X face
            glm::lookAt(::gpu::gltf::origin, rot180Y(::gpu::gltf::unitX), -::gpu::gltf::unitY), // DX -X face

            // +Y/-Y (may also need flipping depending on your loader)
            glm::lookAt(::gpu::gltf::origin, rot180Y(::gpu::gltf::unitY), ::gpu::gltf::unitZ),
            glm::lookAt(::gpu::gltf::origin, rot180Y(-::gpu::gltf::unitY), -::gpu::gltf::unitZ),

            // +Z/-Z
            glm::lookAt(::gpu::gltf::origin,rot180Y( ::gpu::gltf::unitZ), -::gpu::gltf::unitY),
            glm::lookAt(::gpu::gltf::origin, rot180Y(-::gpu::gltf::unitZ), -::gpu::gltf::unitY)};
         glm::mat4 projection = glm::perspective(glm::radians(90.0f), // 90 degrees to cover one face
                                                 1.0f, // its a square
                                                 0.1f, 2.0f);
         ::cast<::gpu_directx11::context> pcontext = m_pgpucontext;
         //glViewport(0, 0, m_uCubemapWidth, m_uCubemapHeight);

         // render the equirectangular HDR texture to a cubemap
         //m_pframebuffer->bind();
         m_pshaderHdri->_bind(nullptr, ::gpu::e_scene_none);
         m_pshaderHdri->bind_source(nullptr, m_phdricube->m_ptextureHdr);

         // render to each side of the cubemap
         for (auto i = 0; i < 6; i++)
         {

            pcontext->start_debug_happening("equirectangular_cubemap face " + ::as_string(i + 1));
            
            m_pshaderHdri->setModelViewProjectionMatrices(model, cameraAngles[i], projection);
            
            m_pframebuffer->setCubeFace(i, m_pshaderHdri);
            m_pshaderHdri->push_properties(pgpucommandbuffer);

            //pcontext->clear(m_p::color::transparent);

            //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //
            //GLCheckError("");

                // auto pshader = pcommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext->
            //   m_pshaderBound;

            //m_pshaderHdri->set_int("hdri", 0);
            //  Assume each cube face is size 'faceSize'
            D3D11_VIEWPORT viewport;
            viewport.TopLeftX = 0.0f; // Start at top-left corner
            viewport.TopLeftY = 0.0f;
            viewport.Width = static_cast<float>(m_pframebuffer->m_ptexture->width()); // Width of face
            viewport.Height = static_cast<float>(m_pframebuffer->m_ptexture->height()); // Height of face
            viewport.MinDepth = 0.0f; // Minimum depth
            viewport.MaxDepth = 1.0f; // Maximum depth

            // Set the viewport on the device context
            pcontext->m_pcontext->RSSetViewports(1, &viewport);

            m_phdricube->draw(pgpucommandbuffer);

            pcontext->end_debug_happening();

         }

         pcontext->start_debug_happening("equirectangular_cubemap generateMipmap");

         pcontext->m_pcontext->Flush();

         m_pframebuffer->generateMipmap();

         pcontext->end_debug_happening();

         // timer.logDifference("Rendered equirectangular cubemap");

         //GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

         //if (status != GL_FRAMEBUFFER_COMPLETE)
         //{

         //   printf("Framebuffer incomplete!\n");
         //}

         // timer.logDifference("Rendered specular brdf convolution map");

         //glBindFramebuffer(GL_FRAMEBUFFER, 0);
         //GLCheckError("");
      }


      //unsigned int equirectangular_cubemap::getCubemapId()
      //{

      //   ::cast<cubemap_framebuffer> pframebuffer = m_pframebuffer;
      //   return pframebuffer->getCubemapTextureId();
      //}


   } // namespace ibl


} // namespace gpu_directx11
