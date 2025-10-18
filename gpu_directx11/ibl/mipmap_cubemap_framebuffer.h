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


         // unsigned int m_iWidth, m_iHeight;
         // unsigned int m_iMipWidth, m_iMipHeight;
          

         // unsigned int m_uFramebufferId;
         // unsigned int m_uDepthRenderbufferId;
         // unsigned int m_uCubemapTextureId;

         //::array_base<::array_base<::comptr<ID3D11RenderTargetView>>> m_rendertargetviewa;


         mipmap_cubemap_framebuffer();


         ~mipmap_cubemap_framebuffer() override;


         //virtual void initialize_mipmap_cubemap_framebuffer(unsigned int width, unsigned int height);

         void on_initialize_mipmap_cubemap_framebuffer() override;

         void bind() override;
         UINT mipWidthForLevel(UINT baseWidth, UINT level) const;
         UINT mipHeightForLevel(UINT baseHeight, UINT level) const;
         size_t rtvIndex(unsigned int face, unsigned int mip) const;

         /**
          * Set the mip level to render with.
          * @param mipLevel
          */
         void setMipLevel(unsigned int level) override;


         // /**
         //  * Get the current width based on the mip level.
         //  * @return
         //  */
         // unsigned int getWidth();
         //
         //
         // /**
         //  * Get the current height based on the mip level.
         //  * @return
         //  */
         // unsigned int getHeight();


         /**
          * Set which cube face texture to render to.
          * @param index
          */
         void setCubeFace(unsigned int faceIndex) override;


         //;
         //;
         //unsigned int getCubemapTextureId();

         void createCubemapTextureAndViews();
         void createDepthForCurrentMip();
      };


   } // namespace ibl


} // namespace gpu_directx11
