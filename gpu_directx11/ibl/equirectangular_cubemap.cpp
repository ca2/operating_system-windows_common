// From github:/tristancalderbank/OpenGL-PBR-Renderer/equirectangular_cubemap.cpp by
// camilo on 2025-09-26 19:53 <3ThomasBorregaardSorensen!!
// From gpu_directx11 by camilo on 2025-10-16 17:37 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "equirectangular_cubemap.h"
#include "bred/gpu/context.h"
#include "bred/graphics3d/_functions.h"
#include "bred/graphics3d/skybox.h"
#include "bred/gpu/device.h"
#include "bred/gpu/texture.h"
#include "bred/graphics3d/engine.h"
#include "gpu/timer.h"
#include "gpu/gltf/_constant.h"
#include "gpu_directx11/_gpu_directx11.h"
#include "gpu_directx11/context.h"
#include "cubemap_framebuffer.h"
#include "hdri_cube.h"
#include "gpu/ibl/_hlsl.h"


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


      void equirectangular_cubemap::compute()
      {
         
         ::gpu::Timer timer;

         auto pgpucommandbuffer = m_pgpucontext->beginSingleTimeCommands(m_pgpucontext->m_pgpudevice->graphics_queue());
         
         using namespace graphics3d;

         floating_matrix4 model = mIndentity4;

         floating_matrix4 cameraAngles[] = {
            lookAt(origin, unitX, -unitY),
            lookAt(origin, -unitX, -unitY),
            lookAt(origin, unitY, -unitZ),
            lookAt(origin, -unitY, unitZ),
            lookAt(origin, -unitZ, -unitY),
            lookAt(origin, unitZ, -unitY)};

         floating_matrix4 projection =
            m_pgpucontext->m_pengine->perspective(
            90f_degrees, // 90 degrees to cover one face
            1.0f, // its a square
            0.1f, 2.0f);
         
         ::cast<::gpu_directx11::context> pcontext = m_pgpucontext;

         m_pshaderHdri->_bind(nullptr, ::gpu::e_scene_none);

         m_pshaderHdri->bind_source(nullptr, m_phdricube->m_ptextureHdr);

         // render to each side of the cubemap
         for (auto i = 0; i < 6; i++)
         {

            pcontext->start_debug_happening(pgpucommandbuffer, "equirectangular_cubemap face " + ::as_string(i + 1));
            
            m_pshaderHdri->setModelViewProjection(model, cameraAngles[i], projection);
            
            m_pframebuffer->setCubeFace(i, m_pshaderHdri);

            m_pshaderHdri->push_properties(pgpucommandbuffer);

            ::int_rectangle rectangle;

            rectangle.right = m_pframebuffer->m_ptexture->width();
            rectangle.bottom = m_pframebuffer->m_ptexture->height();

            m_pgpucontext->set_viewport(pgpucommandbuffer, rectangle);

            m_phdricube->draw(pgpucommandbuffer);

            pcontext->end_debug_happening(pgpucommandbuffer);

         }

         pcontext->start_debug_happening(pgpucommandbuffer, "equirectangular_cubemap generateMipmap");

         pcontext->m_pcontext->Flush();

         m_pframebuffer->generateMipmap();

         pcontext->end_debug_happening(pgpucommandbuffer);

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
