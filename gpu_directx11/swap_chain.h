// Created by camilo on 2025-06-10 18:23 <3ThomasBorregaardSørensen!!
#pragma once


#include "directx11/swap_chain.h"
#include "gpu_directx11/render_target_view.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 swap_chain :
      virtual public ::directx11::swap_chain,
      virtual public ::gpu_directx11::render_target_view
   {
   public:

      //D3D11_TEXTURE2D_DESC texDesc = {};
      ::pointer < ::gpu_directx11::shader > m_pshaderPresent;
      //::comptr<ID3D11Texture2D> m_textureaSwapChain;
      //::comptr<ID3D11RenderTargetView> m_prendertargetviewSwapChain;
      ::pointer_array<::gpu_directx11::texture> m_textureaSwapChain;
      ::comptr <ID3D11BlendState > m_pblendstateDisabled;
      ::comptr <IDXGISwapChain3> m_pdxgiswapchain3;


      swap_chain();
      ~swap_chain() override;

      void initialize_swap_chain_window(::gpu::context * pgpucontext, ::acme::windowing::window* pwindow) override;

      void initialize_gpu_swap_chain(::gpu::renderer* pgpurenderer) override;
      //void _update_swap_chain() override;

      void present(::gpu::texture * pgputexture, ::gpu::command_buffer * pgpucommandbuffer) override;

      void swap_buffers() override;
      //::gpu::texture* current_texture() override;


   };


} // namespace gpu_directx11



