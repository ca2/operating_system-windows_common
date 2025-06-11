// Created by camilo on 2025-06-10 18:23 <3ThomasBorregaardSørensen!!
#pragma once


#include "aura/graphics/gpu/swap_chain.h"
#include <dcomp.h>

namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 swap_chain :
      virtual public ::gpu::swap_chain
   {
   public:

      ::comptr<ID3D11Texture2D> m_ptextureShared;

      comptr<IDCompositionDevice> m_pdcompositiondevice;
      comptr<IDCompositionTarget> m_pdcompositiontarget;
      comptr<IDCompositionVisual> m_pdcompositionvisual;

      ::comptr<ID3D11Texture2D>                       m_ptextureBackBuffer;
      ::comptr<ID3D11RenderTargetView>                m_prendertargetviewBackBuffer;
      
      
      
      ::comptr<ID3D11VertexShader> m_pvertexshaderFullscreen;
      ::comptr<ID3D11PixelShader> m_ppixelshaderFullscreen;

      ::comptr<ID3D11ShaderResourceView> m_pshaderresourceviewShader;
      ::comptr<ID3D11SamplerState> m_psamplerstateLinear;

      swap_chain();
      ~swap_chain() override;


      void _update_swap_chain(ID3D11Device* pdevice);

   };


} // namespace gpu_directx11



