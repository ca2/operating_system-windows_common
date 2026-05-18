// Created by camilo on 2025-07-05 09:20 <3ThomasBorregaardSorensen!!
#pragma once

//
//#include "bred/gpu/pixmap.h"
//
//
//namespace gpu_directx11
//{
//
//
//   class CLASS_DECL_GPU_DIRECTX11 pixmap :
//      virtual public ::gpu::pixmap
//   {
//   public:
//
//      
//      ::gpu::renderer* m_pgpurenderer;
//      comptr<ID3D11Texture2D> m_ptexture;
//      comptr< ID3D11SamplerState> m_psamplerstate;
//      comptr<ID3D11ShaderResourceView>m_pshaderresourceview;
//
//
//      pixmap();
//      ~pixmap() override;
//
//
//      // virtual ::i32_size size();
//      //virtual bool is_initialized() const override;
//
//      void initialize_gpu_pixmap(::gpu::renderer* pgpurenderer, const ::i32_size& size) override;
//
//
//      void set_pixels(const void* data, int w, int h) override;
//      //virtual void merge_layers(::pointer_array < ::gpu::layer >* playera);
//      //virtual void blend(::gpu::layer* player);
//      //virtual void blend(::gpu::texture* ptexture);
//
//
//      void bind_texture(::gpu::shader* pgpushader) override;
//      void unbind_texture(::gpu::shader* pgpushader) override;
//
//
//   };
//
//
//} // namespace gpu_directx11
//
//
