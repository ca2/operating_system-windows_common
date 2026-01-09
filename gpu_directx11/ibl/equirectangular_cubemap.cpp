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
#include "bred/platform/timer.h"
#include "gpu/model/_constant.h"
#include "gpu_directx11/_gpu_directx11.h"
#include "gpu_directx11/context.h"
//#include "cubemap_framebuffer.h"
#include "bred/gpu/command_buffer.h"
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


      void equirectangular_cubemap::compute_equirectangular_cubemap(::gpu::command_buffer * pgpucommandbuffer)
      {

         ::gpu::ibl::equirectangular_cubemap::compute_equirectangular_cubemap(pgpucommandbuffer);
         
         //::bred::Timer timer;

         //auto pgpucommandbuffer = m_pgpucontext->beginSingleTimeCommands(m_pgpucontext->m_pgpudevice->graphics_queue());
         //
         //using namespace graphics3d;

         //floating_matrix4 model = mIndentity4;

         //floating_matrix4 cameraAngles[] = {
         //   lookAt(origin, unitX, -unitY),
         //   lookAt(origin, -unitX, -unitY),
         //   lookAt(origin, -unitY, -unitZ),
         //   lookAt(origin, unitY, unitZ),
         //   lookAt(origin, unitZ, -unitY),
         //   lookAt(origin, -unitZ, -unitY)};

         //floating_matrix4 projection =
         //   m_pgpucontext->m_pengine->perspective(
         //   90f_degrees, // 90 degrees to cover one face
         //   1.0f, // its a square
         //   0.1f, 2.0f);
         //
         //::cast<::gpu_directx11::context> pcontext = m_pgpucontext;

         //ID3D11DeviceContext *pcontextImmediate1 = nullptr;
         //   
         //pcontext->m_pgpudevice->m_pdevice1->GetImmediateContext(&pcontextImmediate1);

         //auto pcontext2 = pcontext->m_pcontext;

         //m_pshaderHdri->_bind(nullptr, ::gpu::e_scene_none);

         //m_pshaderHdri->bind_source(nullptr, m_ptextureHdr);

         //// render to each side of the cubemap
         //for (auto i = 0; i < 6; i++)
         //{

         //   pcontext->start_debug_happening(pgpucommandbuffer, "equirectangular_cubemap face " + ::as_string(i + 1));
         //   
         //   m_pshaderHdri->setModelViewProjection(model, cameraAngles[i], projection);
         //   
         //   //m_pframebuffer->set_cube_face(i, m_pshaderHdri);

         //   m_ptextureCubemap->set_cube_face(i, m_pshaderHdri);

         //   m_pshaderHdri->push_properties(pgpucommandbuffer);

         //   ::int_rectangle rectangle;

         //   rectangle.right = m_ptextureCubemap->width();
         //   rectangle.bottom = m_ptextureCubemap->height();

         //   m_pgpucontext->set_viewport(pgpucommandbuffer, rectangle);

         //   m_prenderableCube->bind(pgpucommandbuffer);

         //   m_prenderableCube->draw(pgpucommandbuffer);

         //   m_prenderableCube->unbind(pgpucommandbuffer);

         //   pcontext->end_debug_happening(pgpucommandbuffer);

         //}

         //pcontext->start_debug_happening(pgpucommandbuffer, "equirectangular_cubemap generateMipmap");

         //pcontext->m_pcontext->Flush();

         //m_pframebuffer->generateMipmap();

         //pcontext->end_debug_happening(pgpucommandbuffer);

      }


   } // namespace ibl


} // namespace gpu_directx11
