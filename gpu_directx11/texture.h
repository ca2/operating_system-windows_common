// Created by camilo on 2025-06-14 14:41 < 3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/texture.h"
//#include <d3d11.h>
//#include <d2d1_1.h>


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 texture :
      virtual public ::gpu::texture
   {
   public:


      struct
      {

         int m_bForOnAfterDoneFrameStep : 1;

         void set_new_texture()
         {
            memset(this, 0xff, sizeof(*this));

         }


      }new_texture;

      
      //::comptr<ID3D12Resource>         m_presource;
      ////::comptr<ID3D12DescriptorHeap>   m_pheapRenderTargetView;
      //D3D12_RESOURCE_STATES            m_estate;
      ////D3D12_CPU_DESCRIPTOR_HANDLE      m_handleRenderTargetView;
      //::comptr<ID3D12DescriptorHeap>   m_pheapShaderResourceView;
      //D3D12_CPU_DESCRIPTOR_HANDLE      m_handleShaderResourceView;

      comptr<ID3D11RenderTargetView>      m_prendertargetview;
      comptr<ID3D11Texture2D> m_ptextureOffscreen;
      comptr< ID3D11SamplerState> m_psamplerstate;
      comptr<ID3D11ShaderResourceView>m_pshaderresourceview;

      //class d3d11 :
      //   virtual public ::particle
      //{
      //public:


      //   //::comptr<ID3D12Resource> d3d12Resource;
      //   ::comptr<ID3D11Resource> wrappedResource;
      //   ::comptr<IDXGISurface> dxgiSurface;
      //   ::comptr<ID2D1Bitmap1> d2dBitmap;

      //   d3d11() {}
      //   ~d3d11() override {}



      //};


      //::pointer < d3d11 > m_pd3d11;

      texture();
      ~texture() override;


      void initialize_gpu_texture(::gpu::renderer* prenderer, const ::int_size & size);

      //void _new_state(ID3D12GraphicsCommandList* pcommandlist, D3D12_RESOURCE_STATES estate);

      //class d3d11* d3d11();

      void blend(::gpu::texture* ptexture, const ::int_rectangle& rectangleTarget) override;

   };


} // namespace gpu_directx11



