// Created by camilo on 2025-08-16 01:15 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/frame_storage.h"


namespace gpu_directx11
{



   class CLASS_DECL_GPU_DIRECTX11 frame_storage :
      virtual public ::gpu::frame_storage
   {
   public:

      //class CLASS_DECL_GPU_DIRECTX11 buffer
      //{
      //public:

      //   ID3D11Buffer * m_pbuffer;
      //   int m_iBufferOffset;
      //   int m_iSizeMapped;



      //};


      array_base < ::comptr<ID3D11Buffer> > m_buffera;

      frame_storage();
      ~frame_storage() override;


      void map_allocate(::gpu::memory_buffer* pbuffer, int size) override;


   };



} // namespace gpu_directx11



