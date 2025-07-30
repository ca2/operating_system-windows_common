// From bred/gpu/pixmap.cpp by
//  camilo on 2025-07-05 09:26 <3ThomasBorregaardSørensen!!
#include "framework.h"
//#include "device.h"
//#include "pixmap.h"
//#include "renderer.h"
//
//
//namespace gpu_directx11
//{
//
//
//   pixmap::pixmap()
//   {
//      m_pgpurenderer = nullptr;
//   }
//
//
//   pixmap::~pixmap()
//   {
//
//   }
//
//
//   void pixmap::initialize_gpu_pixmap(::gpu::renderer* pgpurenderer, const ::int_size& size)
//   {
//
//      m_pgpurenderer = pgpurenderer;
//
//      ::gpu::pixmap::initialize_gpu_pixmap(pgpurenderer, size);
//
//      D3D11_TEXTURE2D_DESC texture2ddesc{};
//      // 1. Create offscreen render target texture
//      texture2ddesc.Width = size.width();
//      texture2ddesc.Height = size.height();
//      texture2ddesc.MipLevels = 1;
//      texture2ddesc.ArraySize = 1;
//      texture2ddesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
//      texture2ddesc.SampleDesc.Count = 1;
//      texture2ddesc.Usage = D3D11_USAGE_DYNAMIC;
//      texture2ddesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//      texture2ddesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//
//      ::cast < ::gpu_directx11::device > pgpudevice = pgpurenderer->m_pgpucontext->m_pgpudevice;
//
//      auto pdevice = pgpudevice->m_pdevice;
//
//      HRESULT hrCreateTexture = pdevice->CreateTexture2D(&texture2ddesc, nullptr, &m_ptexture);
//
//      if (FAILED(hrCreateTexture))
//      {
//
//         throw ::hresult_exception(hrCreateTexture, "Failed to create offscreen texture");
//
//      }
//
//  
//      D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//      srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // Must match or be compatible
//      srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
//      srvDesc.Texture2D.MostDetailedMip = 0;
//      srvDesc.Texture2D.MipLevels = 1;
//
//      HRESULT hrCreateShaderResourceView = pgpudevice->m_pdevice->CreateShaderResourceView(
//         m_ptexture,
//         &srvDesc, &m_pshaderresourceview);
//
//      if (FAILED(hrCreateShaderResourceView))
//      {
//
//         throw ::hresult_exception(hrCreateShaderResourceView, "Failed to create offscreen shader resource view");
//
//      }
//
//
//      D3D11_SAMPLER_DESC samp = {};
//      samp.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//      samp.AddressU = samp.AddressV = samp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
//      pdevice->CreateSamplerState(&samp, &m_psamplerstate);
//
//      
//   }
//
//
//   void pixmap::set_pixels(const void* data, int w, int h)
//   {
//
//      ::cast < ::gpu_directx11::context > pcontext = m_pgpurenderer->m_pgpucontext;
//
//      D3D11_MAPPED_SUBRESOURCE mapped;
//      pcontext->m_pcontext->Map(m_ptexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
//
//      image32_t* pimage32 = (image32_t*) mapped.pData;
//
//      pimage32->copy(::int_size(w, h), mapped.RowPitch,(const ::image32_t*) data, w * 4);
//      //// Fill the buffer
//      //for (int y = 0; y < height; ++y)
//      //{
//      //   UINT32* row = (UINT32*)((uint8_t*)mapped.pData + y * mapped.RowPitch);
//      //   for (int x = 0; x < width; ++x)
//      //   {
//      //      row[x] = 0xFF00FF00; // Green with full alpha
//      //   }
//      //}
//
//      pcontext->m_pcontext->Unmap(m_ptexture, 0);
//
//   }
//
//
//   void pixmap::bind_texture(::gpu::shader* pgpushader)
//   {
//      
//      ::cast < ::gpu_directx11::context > pcontext = m_pgpurenderer->m_pgpucontext;
//
//      pcontext->m_pcontext->PSSetShaderResources(0, 1, m_pshaderresourceview.pp());
//
//      pcontext->m_pcontext->PSSetSamplers(0, 1, m_psamplerstate.pp());
//
//      //glBindTexture(GL_TEXTURE_2D, m_gluTexture);
//      //GLCheckError("");
//
//   }
//
//
//   void pixmap::unbind_texture(::gpu::shader* pgpushader)
//   {
//
//      //glBindTexture(GL_TEXTURE_2D, 0);
//      //GLCheckError("");
//
//   }
//
//
//} // namespace gpu_directx11
//
//
