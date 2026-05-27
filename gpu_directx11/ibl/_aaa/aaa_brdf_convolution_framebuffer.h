// From github:/tristancalderbank/OpenGL-PBR-Renderer/brdf_convolution_framebuffer.h by
// camilo on 2025-09-26 19:50 <3ThomasBorregaardSorensen!!
// From gpu_directx11 by camilo on 2025-10-16 17:36 <3ThomasBorregaardSorensen!!
#pragma once


//#include "gpu/ibl/brdf_convolution_framebuffer.h"


namespace gpu_directx11
{


   namespace ibl
   {

      /**
       * Framebuffer for rendering brdf convolution map.
       */
      class CLASS_DECL_GPU_DIRECTX11 brdf_convolution_framebuffer
          :
         virtual public ::gpu::ibl::brdf_convolution_framebuffer
      {

public:


         // ::u32 width, height;

         // ::u32 m_uFramebufferId;
         // ::u32 m_uDepthRenderbufferId;
         // ::u32 m_uColorTextureId;


         brdf_convolution_framebuffer();


         ~brdf_convolution_framebuffer() override;


         ///virtual void initialize_BrdfConvolutionFramebuffer(::u32 width, ::u32 height);

         void on_initialize_BrdfConvolutionFramebuffer() override;
         /**
          * Activate this pframebuffer for drawing.
          */
         void bind() override;


         /**
          * Get the width.
          * @return
          */
         //::u32 getWidth();


         /**
          * Get the height.
          * @return
          */
         //::u32 getHeight();


         /**
          * Get the GL color texture id.
          * @return
          */
         //::u32 getColorTextureId();


      };


   } // namespace ibl


} // namespace gpu_directx11
