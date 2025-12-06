// From github:/tristancalderbank/OpenGL-PBR-Renderer/hdri_cube.h by
// camilo on 2025-09-26 19:54 <3ThomasBorregaardSorensen!!
// From gpu_directx11 by camilo on 2025-10-16 17:36 <3ThomasBorregaardSorensen!!
#pragma once

 #include <vector>
 #include <string>
 #include <iostream>

 //#include "stb_image/stb_image.h"

 //#include "gpu/cube.h"
 #include "gpu/hdr_texture.h"
 #include "bred/gpu/shader.h"
 //#include "gpu/ibl/hdri_cube.h"


 namespace gpu_directx11
 {


    namespace ibl
    {

       /**
        * A unit cube model textured with an equirectangular HDR image.
        */
       class CLASS_DECL_GPU_DIRECTX11 hdri_cube
          :
          virtual public ::gpu::ibl::hdri_cube
       {
       public:


          //::pointer<::gpu::cube> m_pcube;
          //HDRTexture m_phdrtexture;


          hdri_cube();


          ~hdri_cube() override;



          void draw(::gpu::command_buffer *pcommandbuffer) override;


 //         virtual void initialize_hdri_cube(const ::scoped_string & scopedstrhdriPath);



          // void Draw(Shader &shader);
          //
          //
          // void initialize_hdri_cube(const ::string &hdriPath);

       };


    } // namespace ibl


 } // namespace gpu_directx11
