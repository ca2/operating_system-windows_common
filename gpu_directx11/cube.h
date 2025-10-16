// From github:/tristancalderbank/OpenGL-PBR-Renderer/Cube.h by
// camilo on 2025-09-26 23:31 <3ThomasBorregaardSorensen!!
// From gpu_directx11 by camilo on 2025-10-16 17:57 <3ThomasBorregaardSorensen!!
#pragma once


 #include "gpu/cube.h"


 namespace gpu_directx11
 {
    /**
     * A unit cube mesh with only position coordinates.
     */
    class cube :
    virtual public ::gpu::cube
    {
    public:


       unsigned int m_uVAO, m_uVBO;

       cube();
       ~cube() override;



       virtual void initialize_gpu_cube(::gpu::context* pgpucontext);


       void draw(::gpu::command_buffer *pcommandbuffer) override;

       //void Draw();

    //private:
       ///void loadVertexData();

    //private:

      //  std::vector<float> mVertices = {
      //     // positions
      //     -1.0f,  1.0f, -1.0f,
      //     -1.0f, -1.0f, -1.0f,
      //      1.0f, -1.0f, -1.0f,
      //      1.0f, -1.0f, -1.0f,
      //      1.0f,  1.0f, -1.0f,
      //     -1.0f,  1.0f, -1.0f,
      //
      //     -1.0f, -1.0f,  1.0f,
      //     -1.0f, -1.0f, -1.0f,
      //     -1.0f,  1.0f, -1.0f,
      //     -1.0f,  1.0f, -1.0f,
      //     -1.0f,  1.0f,  1.0f,
      //     -1.0f, -1.0f,  1.0f,
      //
      //      1.0f, -1.0f, -1.0f,
      //      1.0f, -1.0f,  1.0f,
      //      1.0f,  1.0f,  1.0f,
      //      1.0f,  1.0f,  1.0f,
      //      1.0f,  1.0f, -1.0f,
      //      1.0f, -1.0f, -1.0f,
      //
      //     -1.0f, -1.0f,  1.0f,
      //     -1.0f,  1.0f,  1.0f,
      //      1.0f,  1.0f,  1.0f,
      //      1.0f,  1.0f,  1.0f,
      //      1.0f, -1.0f,  1.0f,
      //     -1.0f, -1.0f,  1.0f,
      //
      //     -1.0f,  1.0f, -1.0f,
      //      1.0f,  1.0f, -1.0f,
      //      1.0f,  1.0f,  1.0f,
      //      1.0f,  1.0f,  1.0f,
      //     -1.0f,  1.0f,  1.0f,
      //     -1.0f,  1.0f, -1.0f,
      //
      //     -1.0f, -1.0f, -1.0f,
      //     -1.0f, -1.0f,  1.0f,
      //      1.0f, -1.0f, -1.0f,
      //      1.0f, -1.0f, -1.0f,
      //     -1.0f, -1.0f,  1.0f,
      //      1.0f, -1.0f,  1.0f
      // };
    };


 } // namespace gpu_directx11



