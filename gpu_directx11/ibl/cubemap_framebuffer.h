// From github:/tristancalderbank/OpenGL-PBR-Renderer/cubemap_framebuffer.h by
// camilo on 2025-09-26 19:52 <3ThomasBorregaardSorensen!!
// From gpu_directx11 by camilo on 2025-10-16 17:36 <3ThomasBorregaardSorensen!!
#pragma once


#include "gpu/ibl/cubemap_framebuffer.h"


namespace gpu_directx11
{


   namespace ibl
   {
      /**
       * Framebuffer for rendering to faces of a cubemap.
       */
      class CLASS_DECL_GPU_DIRECTX11 cubemap_framebuffer :
         virtual public ::gpu::ibl::cubemap_framebuffer
      {
      public:



         ///int m_iWidth, m_iHeight;
         //unsigned int m_uFramebufferId;
         //unsigned int m_uDepthRenderbufferId;
         //unsigned int m_uCubemapTextureId;




         cubemap_framebuffer();


         ~cubemap_framebuffer() override;


         //virtual void initialize_cubemap_framebuffer(int width, int height);


         void on_initialize_cubemap_framebuffer() override;

         void bind(::gpu::command_buffer *pgpucommandbuffer) override;


         void generateMipmap();


         /**
          * Set which cube face texture to render to.
          * @param index
          */
         void set_cube_face(unsigned int index, ::gpu::shader * pgpushader) override;


         //unsigned int getCubemapTextureId();


      };


   } // namespace ibl


} // namespace gpu_directx11
