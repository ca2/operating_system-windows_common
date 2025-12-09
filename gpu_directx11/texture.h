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

      D3D11_TEXTURE2D_DESC m_texture2ddesc;


      comptr<ID3D11RenderTargetView>      m_prendertargetview;
      comptr<ID3D11Texture2D> m_ptextureOffscreen;
      comptr< ID3D11SamplerState> m_psamplerstate;
      comptr<ID3D11ShaderResourceView>m_pshaderresourceview;

      ::array_base <::comptr<ID3D11RenderTargetView>> m_rendertargetviewa;

      comptr<ID3D11Texture2D> m_ptextureDepthStencil;
      comptr<ID3D11DepthStencilView>m_pdepthstencilview;
      //comptr <ID3D11DepthStencilState>m_pdepthstencilstate;


      comptr < IDXGISurface > m_pdxgisurface;


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

      //bool m_bCreateRenderTargetView;

      texture();
      ~texture() override;


      //virtual int render_target_view_index(int iFace, int iMip) const;
      void create_texture(const pointer_array<image::image>* pimagea) override;

      // void initialize_texture(::gpu::renderer* prenderer,
      //    const ::int_rectangle& rectangleTarget, bool bWithDepth,
      //    const ::pointer_array < ::image::image > * imagea = nullptr, enum_type type = e_type_image) override;
      virtual void _initialize_gpu_texture(::gpu::renderer* prenderer, IDXGISwapChain1 * pdxgiswapchain);


      void create_render_target() override;
      void create_shader_resource_view() override;


      void create_depth_resources() override;

      //void _new_state(ID3D12GraphicsCommandList* pcommandlist, D3D12_RESOURCE_STATES estate);

      //class d3d11* d3d11();

      void blend(::gpu::texture* ptexture) override;


      virtual IDXGISurface* __get_dxgi_surface();

      void set_pixels(const ::int_rectangle& rectangle, const void* data) override;
      void initialize_with_image_data(::gpu::renderer *pgpurenderer, const ::int_rectangle &rectangleTarget,
                                      int numChannels, bool bSrgb, const void *pdata, enum_type etype) override;


      void initialize_hdr_texture_on_memory(::gpu::renderer *pgpurenderer, const ::block &block) override;
      virtual ID3D11RenderTargetView * render_target_view(int iFace, int iMip);

   };


} // namespace gpu_directx11



