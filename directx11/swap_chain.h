// Created by camilo on 2025-06-04 23:08 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/swap_chain.h"
#include <dcomp.h>


namespace directx11
{


   class CLASS_DECL_DIRECTX11 swap_chain :
      virtual public ::gpu::swap_chain
   {
   public:


      //D3D11_TEXTURE2D_DESC texDesc = {};

      ::comptr<IDXGIDevice>               m_pdxgidevice_2;
      //::comptr<IDXGISurface>              m_pdxgisurface_2;

      ::comptr<IDXGISwapChain1> m_pdxgiswapchain1;



      //::comptr<ID3D11Texture2D> m_ptextureShared;

      comptr<IDCompositionDevice> m_pdcompositiondevice;
      comptr<IDCompositionTarget> m_pdcompositiontarget;
      comptr<IDCompositionVisual> m_pdcompositionvisual;

      //::comptr<ID3D11Texture2D>                       m_ptextureBackBuffer;
      //::comptr<ID3D11RenderTargetView>                m_prendertargetviewBackBuffer;

      //::comptr<ID3D11VertexShader> m_pvertexshaderFullscreen;
      //::comptr<ID3D11PixelShader> m_ppixelshaderFullscreen;

      //::comptr<ID3D11ShaderResourceView> m_pshaderresourceviewShader;
      //::comptr<ID3D11SamplerState> m_psamplerstateLinear;


      swap_chain();
      ~swap_chain() override;


      void initialize_swap_chain_window(::gpu::context* pgpucontext, ::windowing::window* pwindow) override;


      void initialize_gpu_swap_chain(::gpu::renderer * pgpurenderer) override;


      //virtual void _update_swap_chain();


      virtual ::string _fullscreen_vertex_shader_hlsl();
      virtual ::string _fullscreen_pixel_shader_hlsl();


      
      virtual ID3D11Device* _get_d3d11_device();

      virtual IDXGIDevice* _get_dxgi_device();

      virtual comptr < IDXGISurface> _create_dxgi_surface(const ::int_size & size);





   };


} // namespace directx11



