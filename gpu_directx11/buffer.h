#pragma once


#include "bred/gpu/buffer.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 buffer :
      virtual public ::gpu::buffer
   {
   public:


      //__creatable_from_base(buffer, ::gpu::buffer);


      buffer();
      ~buffer() override;


      void gpu_read() override;
      void gpu_write() override;
      


      //// Utility function to read RGBA pixel data from a GPU texture
      //void ReadBackTextureRGBA(
      //   //memory& memory,
      //   //ID3D11Device* device,
      //   //ID3D11DeviceContext* context,
      //   ID3D11Texture2D* offscreenTexture);
      //   //UINT* outWidth,
      //   //UINT* outHeight)
    

   };


} // namespace gpu_directx11



