// From github:/tristancalderbank/OpenGL-PBR-Renderer/mipmap_cubemap_framebuffer.h by
// camilo on 2025-09-26 19:54 <3ThomasBorregaardSorensen!!
// From gpu_directx11 by camilo on 2025-10-16 17:37 <3ThomasBorregaardSorensen!!
#pragma once
#include "gpu/ibl/mipmap_cubemap_framebuffer.h"


namespace gpu_directx11
{


   namespace ibl
   {
      /**
       * Framebuffer for rendering to faces of a cubemap. The faces themselves are mipmapped.
       */
      class CLASS_DECL_GPU_DIRECTX11 mipmap_cubemap_framebuffer :
         virtual public ::gpu::ibl::mipmap_cubemap_framebuffer
      {
      public:


         // ::u32 m_iWidth, m_iHeight;
         // ::u32 m_iMipWidth, m_iMipHeight;
          

         // ::u32 m_uFramebufferId;
         // ::u32 m_uDepthRenderbufferId;
         // ::u32 m_uCubemapTextureId;

         //::array_base<::array_base<::comptr<ID3D11RenderTargetView>>> m_rendertargetviewa;


         mipmap_cubemap_framebuffer();


         ~mipmap_cubemap_framebuffer() override;


         //virtual void initialize_mipmap_cubemap_framebuffer(::u32 width, ::u32 height);

         void on_initialize_mipmap_cubemap_framebuffer() override;

         void bind() override;

         /**
          * Set the mip level to render with.
          * @param mipLevel
          */
         void set_current_mip(::i32 iCurrentMip) override;


         // /**
         //  * Get the current width based on the mip level.
         //  * @return
         //  */
         // ::u32 getWidth();
         //
         //
         // /**
         //  * Get the current height based on the mip level.
         //  * @return
         //  */
         // ::u32 getHeight();


         /**
          * Set which cube face texture to render to.
          * @param index
          */
         void set_cube_face(::i32 iFace) override;


         //;
         //;
         //::u32 getCubemapTextureId();

         void createCubemapTextureAndViews();
         void createDepthForCurrentMip();
      };


   } // namespace ibl


} // namespace gpu_directx11
