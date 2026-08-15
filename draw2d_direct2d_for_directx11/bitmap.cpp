#include "platform.h"
#include "bitmap.h"
#include "graphics.h"
#include "aura/graphics/draw2d/lock.h"
#include "aura/graphics/draw2d/device_lock.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/window_attachment.h"
#include "gpu_directx11/approach.h"
#include "gpu_directx11/context.h"
#include "gpu_directx11/renderer.h"
#include "gpu_directx11/texture.h"


namespace draw2d_direct2d_for_directx11
{


   bitmap::bitmap()
   {


   }


   bitmap::~bitmap()
   {

      destroy();

   }


   void bitmap::create_bitmap_for_image(
      ::image::image * pimage,
      ::acme::user::interaction * pacmeuserinteractionAffinity,
      ::draw2d::graphics * pgraphics)
   {

      ::draw2d::bitmap::create_bitmap_for_image(
         pimage,
         pacmeuserinteractionAffinity,
         pgraphics);

   }


   void bitmap::_create_from_dxgi_surface(::i32 iIndex, ::i32 iLayerIndex, IDXGISurface * pdxgisurface, ::draw2d_direct2d_for_directx11::graphics * pdraw2dgraphics)
   {

      ::cast < ::draw2d_direct2d_for_directx11::graphics > pgraphics = pdraw2dgraphics;
      ::cast < ::gpu_directx11::approach > papproach = m_papplication->gpu_approach();
      ::cast < ::gpu_directx11::device > pgpudevice = papproach->get_gpu_device(pgraphics->m_pacmeuserinteractionAffinity->acme_windowing_window());
      auto pgpuwindowattachment = ::gpu::window_attachment::get(pgraphics->m_pacmeuserinteractionAffinity);
      ::cast < ::gpu_directx11::context > pgpucontextWindow = pgpuwindowattachment->m_pgpucontextWindow;
      //::cast< ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;
      //::cast< ::gpu_directx11::renderer > prenderer = pgraphics->m_pgpucontextOwned->get_gpu_renderer();
      //auto prendertarget = prenderer->render_target();
      //::cast < ::gpu_directx11::render_target_view > prendertargetview = prenderer->render_target();
      //::cast < ::gpu_directx11::offscreen_render_target_view > poffscreenrendertargetview = prendertargetview;
      //::cast< ::gpu_directx11::device > pgpudevice = pgraphics->m_pgpucontextLease.m_p->m_pgpudevice;
      ID3D11Device * device = pgpudevice->m_pd3d11device;
      ID3D11DeviceContext * context = pgpucontextWindow->m_pcontext;
      //::cast < ::gpu_directx11::texture > ptexture = prendertarget->current_texture(::gpu::current_layer());
      //ID3D11Texture2D * offscreenTexture = ptexture->m_ptextureOffscreen;
      if (!device || !context || !pdxgisurface)
      {
         throw ::exception(error_wrong_state);
      }



      //D3D11_TEXTURE2D_DESC texDesc = {};
      //texDesc.Width = width;
      //texDesc.Height = height;
      //texDesc.MipLevels = 1;
      //texDesc.ArraySize = 1;
      //texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      //texDesc.SampleDesc.Count = 1;
      //texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
      //texDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

      // ... Create texture using device->CreateTexture2D

      // 2. Wrap the texture in a DXGI surface
      //comptr<IDXGISurface> dxgiSurface;
      //offscreenTexture->QueryInterface(IID_PPV_ARGS(&dxgiSurface));

      // 3. Create the Direct2D bitmap
      D2D1_BITMAP_PROPERTIES1 bitmapProps =
         D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
         );

      auto hrCreateBitmapFromDxgiSurface = pgraphics->m_pdevicecontext->CreateBitmapFromDxgiSurface(
         pdxgisurface,
         &bitmapProps,
         &m_pbitmap1
      );

      if (FAILED(hrCreateBitmapFromDxgiSurface))
      {

         throw ::hresult_exception(hrCreateBitmapFromDxgiSurface);

      }

      m_pbitmap1.as(m_pbitmap);

   }


   void bitmap::CreateBitmap(::draw2d::graphics* pdraw2dgraphics, const ::i32_size& sizeParam, ::u32 nPlanes, ::u32 nBitcount, const void * lpBits, ::i32 stride)
   {

      //::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      if (m_pbitmap != nullptr)
      {

         destroy();

      }

      if (::is_set(lpBits))
      {

         pixmap_t pixmap;

         pixmap.m_size = sizeParam;
         pixmap.m_sizeRaw = sizeParam;
         pixmap.m_iScan = stride;
         pixmap.m_pimage32 = (::image32_t *)lpBits;
         pixmap.m_pimage32Raw = (::image32_t *)lpBits;

         _create_gpu_bitmap(sizeParam, &pixmap);

      }
      else
      {

         _create_gpu_bitmap(sizeParam);

      }

      ::cast < ::gpu_directx11::texture > ptexture = m_pgputexture;

      auto & pdxgisurface = ptexture->m_pdxgisurface;

      if (!pdxgisurface)
      {

         ptexture->m_ptextureOffscreen.as(pdxgisurface);

      }

      ::cast < ::draw2d_direct2d_for_directx11::graphics > pgraphics = pdraw2dgraphics;

      _create_from_dxgi_surface(0, 0, pdxgisurface, pgraphics);

      //D2D1_SIZE_U size;

      //size.width = sizeParam.cx;
      //size.height = sizeParam.cy;

      //D2D1_BITMAP_PROPERTIES1 props;

      //props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      //props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;

      //draw2d_direct2d_for_directx11::graphics * pgraphics2d = dynamic_cast < ::draw2d_direct2d_for_directx11::graphics * > (pgraphics);

      //pgraphics2d->m_pd2d1rendertarget->GetDpi(&props.dpiX, &props.dpiY); // Thank you https://repo.anl-external.org/repos/BlueTBB/tbb41_20130314oss/examples/common/gui/d2dvideo.cpp

      ////props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
      //props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;
      ////props.colorContext = nullptr;

      ////if(ppdata != nullptr)
      //{
      //   // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(::color32_t), props, &m_pbitmap);
      //}
      ////else
      ////{
      //HRESULT hr = ((ID2D1DeviceContext *)pgraphics->get_os_data())->CreateBitmap(size, lpBits, stride, props, &m_pbitmap1);

      ////}

      //if (FAILED(hr))
      //{

      //   throw ::exception(error_failed);

      //}

      //m_pbitmap1.as(m_pbitmap);

      ////zero(m_map);
      ////    m_pbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);
      ////
      ////if(ppdata != nullptr)
      //// *ppdata = (::color::color *) m_map.bits;
      //m_osdata[0] = m_pbitmap;

      ////return true;

   }

   
   bool bitmap::CreateBitmapIndirect(::draw2d::graphics* pgraphics, LPBITMAP lpBitmap)
   {
      
      return false;

   }


   void bitmap::create_bitmap(::draw2d::graphics* pgraphics, const ::i32_size & size, ::memory & memory,  ::i32 * piScan)
   {

      //::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      if (m_pbitmap != nullptr)
      {

         destroy();

      }

      D2D1_SIZE_U usize;

      usize.width = abs(size.cx);

      usize.height = abs(size.cy);

      D2D1_BITMAP_PROPERTIES1 props = {};

      props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

      props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;

      draw2d_direct2d_for_directx11::graphics * pgraphics2d = dynamic_cast < ::draw2d_direct2d_for_directx11::graphics * > (pgraphics);

      if (!::is_set(pgraphics2d))
      {

         throw ::exception(error_failed);

      }

      auto prendertarget = pgraphics2d->m_pd2d1rendertarget;

      if (!prendertarget)
      {

         throw ::exception(error_failed);

      }

      prendertarget->GetDpi(&props.dpiX, &props.dpiY);

      props.colorContext = nullptr;

      props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;

      HRESULT hr = ((ID2D1DeviceContext *)pgraphics->get_os_data())->CreateBitmap(usize, nullptr, 0, props, &m_pbitmap1);

      if (FAILED(hr) || m_pbitmap1 == nullptr)
      {

         throw ::exception(error_failed);

      }

      if (FAILED(m_pbitmap1.as(m_pbitmap)))
      {

         throw ::exception(error_failed);

      }

      //auto pimage32Target = (color32_t *)*ppimage32;

      int iScan = size.cx * 4;

      if (piScan && *piScan > iScan)
      {

         iScan = *piScan;

      }

      HRESULT hrResultCopyBitmap = S_OK;

      if (memory.data_if_at_least(size.cy * iScan))
      {

         D2D1_RECT_U rectangleDst = {};

         rectangleDst.right = size.cx;

         rectangleDst.bottom = size.cy;

         hrResultCopyBitmap = m_pbitmap->CopyFromMemory(&rectangleDst, memory.data(), iScan);

      }

      m_size = size;

      m_osdata[0] = m_pbitmap;

      m_osdata[1] = m_pbitmap1;

      //return true;

   }


   void bitmap::CreateDIBitmap(::draw2d::graphics* pgraphics, ::i32 cx, ::i32 cy, ::u32 flInit, const void *pjBits, ::u32 iUsage)
   {

      //return false;

   }


   ::u32 bitmap::SetBitmapBits(::u32 dwCount, const void * lpBits)
   {

      //return ::SetBitmapBits((HBITMAP)get_handle(), dwCount, lpBits);
      return 0;

   }
   ::u32 bitmap::GetBitmapBits(::u32 dwCount, LPVOID lpBits) const
   {
      //return ::GetBitmapBits((HBITMAP)get_handle(), dwCount, lpBits);
      return 0;
   }
   bool bitmap::LoadBitmap(const ::scoped_string & scopedstrResourceName)
   {
      //   return Attach(::LoadBitmap(::aura::FindResourceHandle(
      //lpszResourceName, RT_BITMAP), lpszResourceName));
      return false;

   }

   //i32_size bitmap::SetBitmapDimension(::i32 nWidth, ::i32 nHeight)
   //{


   //   throw ::interface_only();

   //   return ::i32_size(0, 0);

   //}

   i32_size bitmap::GetBitmapDimension() const
   {

      if (!m_pbitmap)
      {

         return ::i32_size(0, 0);

      }

      D2D1_SIZE_U size = m_pbitmap->GetPixelSize();

      return ::i32_size(size.width, size.height);

   }


   bool bitmap::LoadBitmap(::u32 nIDResource)
   {

      //return Attach(::LoadBitmap(::aura::FindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_BITMAP), MAKEINTRESOURCE(nIDResource)));
      return false;

   }


   bool bitmap::LoadOEMBitmap(::u32 nIDBitmap)
   {

      //return Attach(::LoadBitmap(nullptr, MAKEINTRESOURCE(nIDBitmap)));
      return false;

   }


   void bitmap::CreateCompatibleBitmap(::draw2d::graphics* pgraphics, ::i32 nWidth, ::i32 nHeight)
   {

      //::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      if (m_pbitmap != nullptr)
      {

         destroy();

      }

      D2D1_SIZE_U size;

      size.width = nWidth;
      size.height = nHeight;

      D2D1_BITMAP_PROPERTIES1 props;

      props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

      draw2d_direct2d_for_directx11::graphics * pgraphics2d = dynamic_cast < ::draw2d_direct2d_for_directx11::graphics * > (pgraphics);

      pgraphics2d->m_pd2d1rendertarget->GetDpi(&props.dpiX, &props.dpiY); // Thanks again and a third time https://repo.anl-external.org/repos/BlueTBB/tbb41_20130314oss/examples/common/gui/d2dvideo.cpp      props.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

      props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CPU_READ;

      //if(ppdata != nullptr)
      {
         // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(::color32_t), props, &m_pbitmap);
      }
      //else
      //      ID2D1Bitmap1 * pbitmap1;
      {
         ((ID2D1DeviceContext *)pgraphics->get_os_data())->CreateBitmap(size, nullptr, size.width * sizeof(::color32_t), props, &m_pbitmap1);
      }
      m_pbitmap = m_pbitmap1;

      m_osdata[0] = m_pbitmap;

      m_osdata[1] = m_pbitmap1;

      //return true;

   }


   void bitmap::CreateDiscardableBitmap(::draw2d::graphics* pgraphics, ::i32 nWidth, ::i32 nHeight)
   {

      //::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      if (m_pbitmap != nullptr)
      {

         destroy();

      }

      D2D1_SIZE_U size;

      size.width = nWidth;
      size.height = nHeight;

      D2D1_BITMAP_PROPERTIES1 props;

      props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

      draw2d_direct2d_for_directx11::graphics * pgraphics2d = dynamic_cast < ::draw2d_direct2d_for_directx11::graphics * > (pgraphics);

      pgraphics2d->m_pd2d1rendertarget->GetDpi(&props.dpiX, &props.dpiY); // One more time, Thank you very much https://repo.anl-external.org/repos/BlueTBB/tbb41_20130314oss/examples/common/gui/d2dvideo.cpp      props.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

      props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CPU_READ;

      //if(ppdata != nullptr)
      {
         // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(::color32_t), props, &m_pbitmap);
      }
      //else
      {
         ((ID2D1DeviceContext *)pgraphics->get_os_data())->CreateBitmap(size, nullptr, size.width * sizeof(::color32_t), &props, &m_pbitmap1);
      }
      m_pbitmap = m_pbitmap1;

      m_osdata[0] = m_pbitmap;

      m_osdata[1] = m_pbitmap1;

      //m_pbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);

      //if(ppdata != nullptr)
      // *ppdata = (::color::color *) m_map.bits;


      //return true;

   }


   //void bitmap::dump(dump_context & dumpcontext) const
   //{

   //   ::draw2d::object::dump(dumpcontext);

   //   //dumpcontext << "\n";

   //}




   void bitmap::defer_update(::draw2d::graphics* pgraphics) const
   {

   }


   void bitmap::attach(void * hbitmap)
   {

      if (m_pbitmap != nullptr)
      {

         destroy();

      }

      m_pbitmap = (ID2D1Bitmap *)hbitmap;

      m_pbitmap.as(m_pbitmap1);

      m_osdata[0] = m_pbitmap;

      m_osdata[1] = m_pbitmap1;

      //return true;

   }


   void * bitmap::detach()
   {

      clear_os_data();

      m_pbitmap1 = nullptr;

      return m_pbitmap.detach();

   }


   void bitmap::destroy()
   {

      destroy_os_data();

      ::draw2d::bitmap::destroy();

   }


   void bitmap::destroy_os_data()
   {

      m_pbitmap = nullptr;

      m_pbitmap1 = nullptr;

      object::destroy_os_data();

   }


} // namespace draw2d_direct2d_for_directx11
