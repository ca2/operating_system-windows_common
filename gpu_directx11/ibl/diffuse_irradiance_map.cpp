// From github:/tristancalderbank/OpenGL-PBR-Renderer/diffuse_irradiance_map.cpp by
// camilo on 2025-09-26 19:53 <3ThomasBorregaardSorensen!!
// From gpu_directx11 by camilo on 2025-10-16 17:37 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "diffuse_irradiance_map.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context.h"
#include "bred/gpu/context_lock.h"
#include "bred/gpu/device.h"
#include "bred/graphics3d/_functions.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/renderable.h"
#include "bred/graphics3d/scene_base.h"
#include "bred/graphics3d/skybox.h"
#include "gpu/gltf/_constant.h"
//#include "gpu/cube.h"
#include "gpu/timer.h"
#include "gpu_directx11/_gpu_directx11.h"
#include "gpu_directx11/context.h"
#include "gpu_directx11/texture.h"
#include "bred/gpu/model_buffer.h"
#include "gpu/context.h"
#include "gpu/ibl/_hlsl.h"


namespace gpu_directx11
{


   namespace ibl
   {


      diffuse_irradiance_map::diffuse_irradiance_map()
      {


      }


      diffuse_irradiance_map::~diffuse_irradiance_map()
      {


      }


      ::block diffuse_irradiance_map::embedded_diffuse_irradiance_vert()
      {

         return hlsl_embedded_diffuse_irradiance_vert();

      }


      ::block diffuse_irradiance_map::embedded_diffuse_irradiance_frag()
      {

         return hlsl_embedded_diffuse_irradiance_frag();


      }


      void diffuse_irradiance_map::computeIrradianceMap(::gpu::command_buffer * pgpucommandbuffer)
      {

         ::gpu::context_lock contextlock(m_pgpucontext);

         ::gpu::Timer timer;

         //auto pgpucommandbuffer = m_pgpucontext->beginSingleTimeCommands(m_pgpucontext->m_pgpudevice->graphics_queue());
         
         using namespace graphics3d;

         floating_matrix4 model = mIndentity4;

         floating_matrix4 cameraAngles[] = 
         {
            lookAt(origin, unitX, -unitY), 
            lookAt(origin, -unitX, -unitY),
            lookAt(origin, unitY, -unitZ), 
            lookAt(origin, -unitY, unitZ),
            lookAt(origin, -unitZ, -unitY),
            lookAt(origin, unitZ, -unitY)
         };

         floating_matrix4 projection = m_pgpucontext->m_pengine->perspective(
            90f_degrees, // 90 degrees to cover one face
            1.0f, // its a square
            0.1f, 2.0f);

         auto prenderableCube = m_pgpucontext->m_pengine->shape_factory()->create_cube_001(m_pgpucontext, 32.f);

         //pcube->initialize_gpu_cube(m_pgpucontext);

         auto pskybox = m_pscene->current_skybox();

         auto prenderable = pskybox->m_prenderable;

         auto ptexture = pskybox->m_ptexture;

         ::cast<::gpu_directx11::texture> ptextureSkybox = ptexture;

         ::cast<::gpu_directx11::texture> ptextureIrradiance = m_pframebufferDiffuseIrradiance->m_ptexture;

         m_pshaderDiffuseIrradiance->_bind(pgpucommandbuffer, ::gpu::e_scene_none);

         m_pshaderDiffuseIrradiance->bind_source(nullptr, 
            ptextureSkybox);

         // render to each side of the cubemap
         for (auto i = 0; i < 6; i++)
         {

            m_pshaderDiffuseIrradiance->setModelViewProjection(model, cameraAngles[i], projection);

            m_pframebufferDiffuseIrradiance->set_cube_face(i, m_pshaderDiffuseIrradiance);

            m_pshaderDiffuseIrradiance->push_properties(pgpucommandbuffer);

            prenderableCube->bind(pgpucommandbuffer);

            prenderableCube->draw(pgpucommandbuffer);

            prenderableCube->unbind(pgpucommandbuffer);

         }

         ::cast<gpu_directx11::context> pcontext = m_pgpucontext;

         pcontext->m_pcontext->Flush();

         // Now generate mipmaps using DirectX
         pcontext->m_pcontext->GenerateMips(ptextureIrradiance->m_pshaderresourceview);

         timer.logDifference("Rendered diffuse irradiance map");

      }


      //unsigned int diffuse_irradiance_map::getCubemapId()
      //{

      //   ::cast <cubemap_framebuffer > pframebuffer = m_pdiffuseIrradianceFramebuffer;

      //   return pframebuffer->getCubemapTextureId();

      //}


   } // namespace ibl


} // namespace gpu_directx11
