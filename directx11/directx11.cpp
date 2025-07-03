#include "framework.h"
#include "directx11.h"
#include "swap_chain.h"
#include "acme/graphics/write_text/font_weight.h"
#include "acme/prototype/geometry2d/rectangle.h"
#include "acme_windows_common/hresult_exception.h"
#include "bred/gpu/bred_approach.h"
#include "bred/gpu/device.h"
#include "aura/windowing/window.h"


namespace directx11
{


   //::pointer < class directx11 > directx11::s_pdirectx11;


   directx11::directx11()
   {

  /*    if (s_pdirectx11)
      {

         throw ::exception(error_wrong_state);

      }

      s_pdirectx11 = this;*/

   }


   directx11::~directx11()
   {


   }


   void directx11::initialize(::particle* pparticle)
   {

      ::app_consumer<::aura::application>::initialize(pparticle);

   }


   //CLASS_DECL_DIRECTX11 void defer_initialize(::windowing::window* pwindow, const ::int_rectangle& rectanglePlacement)
   //{

   //   //if (::is_set(directx11::s_pdirectx11))
   //   //{

   //   //   return;

   //   //}

   //   pwindow->__defer_construct(directx11::s_pdirectx11);

   //}


   //CLASS_DECL_DIRECTX11 void finalize()
   //{

   //   directx11::s_pdirectx11.release();

   //}



   //::comptr < IDXGISurface> directx11::_create_dxgi_surface(const ::int_size& size)
   //{
   //   ::comptr < IDXGISurface> pdxgisurface;
   //   //auto r = m_pwindow->get_window_rectangle();

   //   //if (texDesc.Width != r.width()
   //   //   || texDesc.Height != r.height())
   //   //{

   //   D3D11_TEXTURE2D_DESC texDesc = {};
   //   texDesc.Width = size.width();
   //   texDesc.Height = size.height();
   //   texDesc.MipLevels = 1;
   //   texDesc.ArraySize = 1;
   //   texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
   //   texDesc.SampleDesc.Count = 1;
   //   texDesc.Usage = D3D11_USAGE_DEFAULT;
   //   texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
   //   texDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

   //   auto pd3d11device = _get_d3d11_device();

   //   ::comptr <ID3D11Texture2D> ptextureShared;

   //   pd3d11device->CreateTexture2D(&texDesc, nullptr, &ptextureShared);

   //   ptextureShared.as(pdxgisurface);

   //   //}

   //   return pdxgisurface;

   //}


   IDXGIDevice* directx11::_get_dxgi_device()
   {

      return m_pdxgidevice_2;

   }


   CLASS_DECL_DIRECTX11 directx11* from_gpu_device(::gpu::device* pgpudevice)
   {

      ::cast < directx11 > pdirectx11 = pgpudevice->payload("directx11").as_subparticle();

      if (!pdirectx11)
      {

         pdirectx11 = pgpudevice->__create_new < directx11>();

         pgpudevice->payload("directx11") = pdirectx11;

         pdirectx11.m_p = dynamic_cast < directx11 * > (pgpudevice->payload("directx11").as_subparticle());

      }

      return pdirectx11;

   }


} // namespace draw2d_directx11



