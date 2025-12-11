// From github:/tristancalderbank/OpenGL-PBR-Renderer/specular_map.cpp by
// camilo on 2025-09-26 19:55 <3ThomasBorregaardSorensen!!
// From gpu_directx11 by camilo on 2025-10-16 17:38 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "specular_map.h"
//#include "brdf_convolution_framebuffer.h"
#include "bred/graphics3d/_functions.h"
#include "bred/graphics3d/engine.h"
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
//#include "gpu/cube.h"
#include "gpu/gltf/_constant.h"
#include "gpu/context.h"
#include "gpu/timer.h"
//#include "mipmap_cubemap_framebuffer.h"
#include "gpu/full_screen_quad.h"
#include "gpu/ibl/_hlsl.h"


namespace gpu_directx11
{


   namespace ibl
   {


      specular_map::specular_map()
      {


      }


      specular_map::~specular_map()
      {

      }



      ::memory specular_map::prefiltered_environment_map_vert_memory()
      {

         return hlsl_prefiltered_environment_map_vert_memory();

      }


      ::memory specular_map::prefiltered_environment_map_frag_memory()
      {
         
         return hlsl_prefiltered_environment_map_frag_memory();

      }


      ::memory specular_map::brdf_convolution_frag_memory()
      {

         return hlsl_brdf_convolution_frag_memory();

      }


      ::memory specular_map::brdf_convolution_vert_memory()
      {
         
         return hlsl_brdf_convolution_vert_memory();
      
      }


      void specular_map::computePrefilteredEnvMap(::gpu::command_buffer * pgpucommandbuffer)
      {

         ::gpu::ibl::specular_map::computePrefilteredEnvMap(pgpucommandbuffer);

      //   ::gpu::Timer timer;

      //   ::gpu::context_lock contextlock(m_pgpucontext);

      //   //auto pgpucommandbuffer = m_pgpucontext->beginSingleTimeCommands(m_pgpucontext->m_pgpudevice->graphics_queue());
      //   
      //   using namespace graphics3d;
      //   
      //   floating_matrix4 model = mIndentity4;

      //   floating_matrix4 cameraAngles[] = 
      //   {
      //      lookAt(origin, unitX, -unitY),
      //      lookAt(origin, -unitX, -unitY),
      //      lookAt(origin, unitY, -unitZ), 
      //      lookAt(origin, -unitY, unitZ),
      //      lookAt(origin, -unitZ, -unitY),
      //      lookAt(origin, unitZ, -unitY)
      //   };

      //   floating_matrix4 projection = m_pgpucontext->m_pengine->perspective(
      //      90f_degrees, // 90 degrees to cover one face
      //      1.0f, // its a square
      //      0.1f,
      //      2.0f);

      //   //projection[1][1] *= -1.0f; // flip y

      //   auto pskybox = m_pscene->current_skybox();

      //   auto ptexture = pskybox->m_ptexture;

      //   auto prenderableCube = m_pgpucontext->m_pengine->shape_factory()->create_cube_001(m_pgpucontext,  32.f);
      //   //
      //   //pcube->initialize_gpu_cube(m_pgpucontext);

      //   ::cast<::gpu_directx11::texture> ptextureSkybox = ptexture;

      //   ::cast<::gpu_directx11::context> pcontext = m_pgpucontext;

      //   ::cast<::gpu_directx11::texture> ptextureEnvMap = m_pframebufferPrefilteredEnvMap->m_ptexture;
      //   
      //   ::cast<::gpu_directx11::ibl::mipmap_cubemap_framebuffer> pframebufferEnvMap = m_pframebufferPrefilteredEnvMap;

      //   m_pshaderPrefilteredEnvMap->_bind(nullptr, ::gpu::e_scene_none);

      //   m_pshaderPrefilteredEnvMap->bind_source(nullptr, ptextureSkybox);

      //   for (auto iMip = 0; iMip < m_iPrefilteredEnvMapMipCount; iMip++)
      //   {

      //      m_pframebufferPrefilteredEnvMap->set_current_mip(iMip);

      //      // each mip level has increasing roughness
      //      float roughness = (float)iMip / (float)(m_iPrefilteredEnvMapMipCount - 1);

      //      m_pshaderPrefilteredEnvMap->set_float("roughness", roughness);

      //      // render to each side of the cubemap
      //      for (auto iFace = 0; iFace < 6; iFace++)
      //      {
      //         
      //         m_pshaderPrefilteredEnvMap->setModelViewProjection(model, cameraAngles[iFace], projection);
      //         
      //         m_pframebufferPrefilteredEnvMap->set_cube_face(iFace);

      //         ::int_rectangle rectangle;

      //         rectangle.set(0, 0, 
      //            m_pframebufferPrefilteredEnvMap->m_ptexture->m_sizeMip.cx,
      //            m_pframebufferPrefilteredEnvMap->m_ptexture->m_sizeMip.cy);

      //         auto prendertargetview = ptextureEnvMap->render_target_view(iFace, iMip);

      //         ID3D11RenderTargetView *bindRTV[1] = {prendertargetview};

      //         pcontext->m_pcontext->OMSetRenderTargets(1, bindRTV, nullptr);

      //         m_pgpucontext->set_viewport(pgpucommandbuffer, rectangle);

      //         ::graphics3d::render_system rendersystemScope;

      //         rendersystemScope.m_erendersystem = ::graphics3d::e_render_system_skybox_ibl;

      //         pgpucommandbuffer->m_prendersystem = &rendersystemScope;

      //         m_pshaderPrefilteredEnvMap->push_properties(pgpucommandbuffer);

      //         prenderableCube->bind(pgpucommandbuffer);

      //         prenderableCube->draw(pgpucommandbuffer);

      //         prenderableCube->unbind(pgpucommandbuffer);

      //         pgpucommandbuffer->m_prendersystem = nullptr;

      //         ID3D11RenderTargetView *nullRTV[1] = {nullptr};

      //         pcontext->m_pcontext->OMSetRenderTargets(1, nullRTV, nullptr);

      //      }

      //   }

      //   pcontext->m_pcontext->OMSetRenderTargets(0, nullptr, nullptr);

      //   pcontext->m_pcontext->Flush();

      //   timer.logDifference("Rendered specular pre-filtered environment map");

      }


      void specular_map::computeBrdfConvolutionMap()
      {

      //   ::gpu::Timer timer;

         ::gpu::ibl::specular_map::computeBrdfConvolutionMap();

      //   ::cast<::gpu_directx11::context> pcontext = m_pgpucontext;

      //   ::cast<::gpu_directx11::device> pdevice = m_pgpucontext->m_pgpudevice;

      //   ::cast<::gpu_directx11::texture> ptexture = m_pframebufferBrdfConvolution->m_ptexture;

      //   ID3D11RenderTargetView *rtvs[8];
      //   UINT numRTVs = 8;
      //   pcontext->m_pcontext->OMGetRenderTargets(numRTVs, rtvs, nullptr);

      //   if (rtvs[0] == ptexture->m_prendertargetview)
      //   {

      //      information() << "seems ok";

      //   }
      //   else
      //   {

      //      information() << "well...";

      //   }

      //   D3D11_VIEWPORT vp;
      //   UINT num = 1;
      //   pcontext->m_pcontext->RSGetViewports(&num, &vp);
      //   if (vp.Width > 0 && vp.Height > 0)
      //   {

      //      information() << "a viewport";
      //   }
      //   else
      //   {

      //      information() << "empty viewport";
      //   }

      //   ::comptr<ID3D11Texture2D> pd3d11textureStaging;
      //   D3D11_TEXTURE2D_DESC desc = {};
      //   ptexture->m_ptextureOffscreen->GetDesc(&desc);
      //   desc.Usage = D3D11_USAGE_STAGING;
      //   desc.BindFlags = 0;
      //   desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

      //   pdevice->m_pdevice->CreateTexture2D(&desc, nullptr, &pd3d11textureStaging);
      //   pcontext->m_pcontext->CopyResource(pd3d11textureStaging, ptexture->m_ptextureOffscreen);

      //   
      //   // 4. Map (readback)
      //   D3D11_MAPPED_SUBRESOURCE mapped = {};
      //   HRESULT hr = pcontext->m_pcontext->Map(pd3d11textureStaging, 0, D3D11_MAP_READ, 0, &mapped);
      //   ::defer_throw_hresult(hr);
      //   //{
      //   //   printf("Failed to map staging texture: 0x%08X\n", hr);
      //   //   return;
      //   //}

      //   //// 5. Print the first few values
      //   ////    Adjust according to your format: assuming DXGI_FORMAT_R16G16_FLOAT
      //   //uint8_t *rowPtr = reinterpret_cast<uint8_t *>(mapped.pData);
      //   //for (UINT y = 0; y < minimum(desc.Height, 4u); y++) // print first 4 rows
      //   //{
      //   //   auto *pixel = reinterpret_cast<const uint16_t *>(rowPtr); // 16-bit floats
      //   //   for (UINT x = 0; x < minimum(desc.Width, 4u); x++)
      //   //   {
      //   //      // Convert half to float (quick approximate if no helper)
      //   //      float r = DirectX::PackedVector::XMConvertHalfToFloat(pixel[0]);
      //   //      float g = DirectX::PackedVector::XMConvertHalfToFloat(pixel[1]);
      //   //      printf("(%f, %f) ", r, g);
      //   //      pixel += 2; // R16G16
      //   //   }
      //   //   printf("\n");
      //   //   rowPtr += mapped.RowPitch;
      //   //}
      //   // 5. Print the first few values
      //   //    for DXGI_FORMAT_R32G32_FLOAT
      //   float *rowPtr = (float*)(mapped.pData);
      //   for (UINT y = 0; y < minimum(desc.Height, 4u); y++) // print first 4 rows
      //   {
      //      auto *pixel = rowPtr; // 32-bit floats
      //      for (UINT x = 0; x < minimum(desc.Width, 4u); x++)
      //      {
      //         // Convert half to float (quick approximate if no helper)
      //         float r = pixel[0];
      //         float g = pixel[1];
      //         informationf("(%f, %f) ", r, g);
      //         pixel += 2; // R32G32
      //      }
      //      //printf("\n");
      //      rowPtr += mapped.RowPitch;
      //   }

      //   // 6. Unmap
      //   pcontext->m_pcontext->Unmap(pd3d11textureStaging, 0);

      //   timer.logDifference("Computed Brdf Convolution Map");

      }

     
   } // namespace ibl


} // namespace gpu_directx11
