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


   void bitmap::update_bitmap_as_image_render_target(
      ::image::image * pimage,
      ::acme::user::interaction * pacmeuserinteractionAffinity,
      ::draw2d::graphics * pgraphics)
   {

      ::draw2d::bitmap::update_bitmap_as_image_render_target(
         pimage,
         pacmeuserinteractionAffinity,
         pgraphics);

   }

   
   ::comptr < IDXGISurface> bitmap::_get_dxgi_surface(::gpu::texture * pgputexture)
   {

      ::cast < ::gpu_directx11::texture > ptexture = pgputexture;

      ID3D11Texture2D * offscreenTexture = ptexture->m_ptextureOffscreen;

      ::comptr<IDXGISurface> pdxgisurface;
      ptexture->m_ptextureOffscreen.as(pdxgisurface);

      return pdxgisurface;

   }



   void bitmap::update_bitmap_as_backed_by_gpu_texture(::gpu::texture * pgputexture, ::draw2d::graphics * pdraw2dgraphics)
   {

      ::gpu::bitmap::update_bitmap_as_backed_by_gpu_texture(pgputexture, pdraw2dgraphics);

         ::cast < ::draw2d_direct2d_for_directx11::graphics > pgraphics = pdraw2dgraphics;
         //::cast < ::gpu_directx11::approach > papproach = m_papplication->gpu_approach();
         //::cast < ::gpu_directx11::device > pgpudevice = papproach->get_gpu_device(pgraphics->m_pacmeuserinteractionAffinity->acme_windowing_window());
         //auto pgpuwindowattachment = ::gpu::window_attachment::get(pgraphics->m_pacmeuserinteractionAffinity);
         //::cast < ::gpu_directx11::context > pgpucontextWindow = pgpuwindowattachment->m_pgpucontextWindow;
         //::cast< ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;
         //::cast< ::gpu_directx11::renderer > prenderer = pgraphics->m_pgpucontextOwned->get_gpu_renderer();
         //auto prendertarget = prenderer->render_target();
         //::cast < ::gpu_directx11::texture > ptexture = pgputexture;
         //::cast < ::gpu_directx11::render_target_view > prendertargetview = prenderer->render_target();
         //::cast < ::gpu_directx11::offscreen_render_target_view > poffscreenrendertargetview = prendertargetview;
         //::cast< ::gpu_directx11::device > pgpudevice = pgraphics->m_pgpucontextLease.m_p->m_pgpudevice;
         //ID3D11Device * device = pgpudevice->m_pd3d11device;
         //ID3D11DeviceContext * context = pgpucontextWindow->m_pd3d11devicecontext;
         //::cast < ::gpu_directx11::texture > ptexture = prendertarget->current_texture(::gpu::current_layer());
         //ID3D11Texture2D * offscreenTexture = ptexture->m_ptextureOffscreen;

         auto pdxgisurface = _get_dxgi_surface(pgputexture);
         //::comptr<IDXGISurface> pdxgisurface;
         //ptexture->m_ptextureOffscreen.as(pdxgisurface);
         //if (!device || !context || !pdxgisurface)
         if (!pdxgisurface)
         {
            throw ::exception(error_wrong_state);
         }


         //auto size = pgputexture->size();
         //D3D11_TEXTURE2D_DESC desc{};

         //offscreenTexture->GetDesc(&desc);

         //D3D11_TEXTURE2D_DESC texDesc = {};

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

         auto hrCreateBitmapFromDxgiSurface = pgraphics->m_pd2d1devicecontext->CreateBitmapFromDxgiSurface(
            pdxgisurface,
            &bitmapProps,
            &m_pd2d1bitmap1
         );

         if (FAILED(hrCreateBitmapFromDxgiSurface))
         {

            throw ::hresult_exception(hrCreateBitmapFromDxgiSurface);

         }

         m_pd2d1bitmap1.as(m_pd2d1bitmap);

         auto size = m_pd2d1bitmap->GetSize();

         informationf("m_pd2d1bitmap %p %d,%d", m_pd2d1bitmap.m_p, size.width, size.height);



      }

   //void bitmap::_create_from_dxgi_surface(::i32 iIndex, ::i32 iLayerIndex, ::gpu::texture * pgputexture, ::draw2d_direct2d_for_directx11::graphics * pdraw2dgraphics)
   //{

   //   ::cast < ::draw2d_direct2d_for_directx11::graphics > pgraphics = pdraw2dgraphics;
   //   ::cast < ::gpu_directx11::approach > papproach = m_papplication->gpu_approach();
   //   ::cast < ::gpu_directx11::device > pgpudevice = papproach->get_gpu_device(pgraphics->m_pacmeuserinteractionAffinity->acme_windowing_window());
   //   auto pgpuwindowattachment = ::gpu::window_attachment::get(pgraphics->m_pacmeuserinteractionAffinity);
   //   ::cast < ::gpu_directx11::context > pgpucontextWindow = pgpuwindowattachment->m_pgpucontextWindow;
   //   //::cast< ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;
   //   //::cast< ::gpu_directx11::renderer > prenderer = pgraphics->m_pgpucontextOwned->get_gpu_renderer();
   //   //auto prendertarget = prenderer->render_target();
   //   ::cast < ::gpu_directx11::texture > ptexture = pgputexture;
   //   //::cast < ::gpu_directx11::render_target_view > prendertargetview = prenderer->render_target();
   //   //::cast < ::gpu_directx11::offscreen_render_target_view > poffscreenrendertargetview = prendertargetview;
   //   //::cast< ::gpu_directx11::device > pgpudevice = pgraphics->m_pgpucontextLease.m_p->m_pgpudevice;
   //   ID3D11Device * device = pgpudevice->m_pd3d11device;
   //   ID3D11DeviceContext * context = pgpucontextWindow->m_pd3d11devicecontext;
   //   //::cast < ::gpu_directx11::texture > ptexture = prendertarget->current_texture(::gpu::current_layer());
   //   ID3D11Texture2D * offscreenTexture = ptexture->m_ptextureOffscreen;
   //   ::comptr<IDXGISurface> pdxgisurface;
   //   ptexture->m_ptextureOffscreen.as(pdxgisurface);
   //   if (!device || !context || !pdxgisurface)
   //   {
   //      throw ::exception(error_wrong_state);
   //   }


   //   D3D11_TEXTURE2D_DESC desc{};

   //   offscreenTexture->GetDesc(&desc);

   //   //D3D11_TEXTURE2D_DESC texDesc = {};

   //   //D3D11_TEXTURE2D_DESC texDesc = {};
   //   //texDesc.Width = width;
   //   //texDesc.Height = height;
   //   //texDesc.MipLevels = 1;
   //   //texDesc.ArraySize = 1;
   //   //texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
   //   //texDesc.SampleDesc.Count = 1;
   //   //texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
   //   //texDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

   //   // ... Create texture using device->CreateTexture2D

   //   // 2. Wrap the texture in a DXGI surface
   //   //comptr<IDXGISurface> dxgiSurface;
   //   //offscreenTexture->QueryInterface(IID_PPV_ARGS(&dxgiSurface));

   //   // 3. Create the Direct2D bitmap
   //   D2D1_BITMAP_PROPERTIES1 bitmapProps =
   //      D2D1::BitmapProperties1(
   //         D2D1_BITMAP_OPTIONS_TARGET,
   //         D2D1::PixelFormat(desc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED)
   //      );

   //   auto hrCreateBitmapFromDxgiSurface = pgraphics->m_pd2d1devicecontext->CreateBitmapFromDxgiSurface(
   //      pdxgisurface,
   //      &bitmapProps,
   //      &m_pd2d1bitmap1
   //   );

   //   if (FAILED(hrCreateBitmapFromDxgiSurface))
   //   {

   //      throw ::hresult_exception(hrCreateBitmapFromDxgiSurface);

   //   }

   //   m_pd2d1bitmap1.as(m_pd2d1bitmap);

   //   auto size = m_pd2d1bitmap->GetSize();

   //   informationf("m_pd2d1bitmap %p %d,%d", m_pd2d1bitmap.m_p, size.width, size.height);



   //}


   void bitmap::CreateBitmap(::draw2d::graphics * pgraphics, const ::i32_size & size, ::u32 nPlanes, ::u32 nBitcount, const void * lpBits, ::i32 stride)
   {

      draw2d_direct2d::bitmap::CreateBitmap(pgraphics, size, nPlanes, nBitcount, lpBits, stride);

   }


   void bitmap::create_gpu_bitmap(::draw2d::graphics* pdraw2dgraphics, const ::i32_size& sizeParam, ::pixmap * ppixmap)
   {

      //::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      if (m_pd2d1bitmap != nullptr)
      {

         destroy();

      }

      //if (::is_set(ppixmap))
      //{

      //   //pixmap_t pixmap;

      //   //pixmap.m_size = sizeParam;
      //   //pixmap.m_sizeRaw = sizeParam;
      //   //pixmap.m_iScan = stride;
      //   //pixmap.m_pimage32 = (::image32_t *)lpBits;
      //   //pixmap.m_pimage32Raw = (::image32_t *)lpBits;

      //   //_create_gpu_bitmap(sizeParam, &pixmap);

      //   _create_gpu_bitmap(sizeParam, ppixmap);

      //}
      //else
      //{

         _create_gpu_bitmap(sizeParam, pdraw2dgraphics, ppixmap);

      //}

      //::cast < ::gpu_directx11::texture > ptexture = m_pgputexture;

      //auto & pdxgisurface = ptexture->m_pdxgisurface;

      //if (!pdxgisurface)
      //{

      //   ptexture->m_ptextureOffscreen.as(pdxgisurface);

      //}

      ::cast < ::draw2d_direct2d_for_directx11::graphics > pgraphics = pdraw2dgraphics;

      update_bitmap_as_backed_by_gpu_texture(m_pgputexture, pgraphics);

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


   void bitmap::create_bitmap(::draw2d::graphics* pgraphics, const ::i32_size & size, ::pixmap * ppixmap)
   {

      //::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      if (m_pd2d1bitmap != nullptr)
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

      HRESULT hr = ((ID2D1DeviceContext *)pgraphics->get_os_data())->CreateBitmap(usize, nullptr, 0, props, &m_pd2d1bitmap1);

      if (FAILED(hr) || m_pd2d1bitmap1 == nullptr)
      {

         throw ::exception(error_failed);

      }

      if (FAILED(m_pd2d1bitmap1.as(m_pd2d1bitmap)))
      {

         throw ::exception(error_failed);

      }

      //auto pimage32Target = (color32_t *)*ppimage32;

      int iScan = size.cx * 4;

      if (ppixmap && ppixmap->m_iScan > iScan)
      {

         iScan = ppixmap->m_iScan;

      }

      HRESULT hrResultCopyBitmap = S_OK;

      if (ppixmap && ppixmap->m_memoryPixmap.data_if_at_least(size.cy * iScan))
      {

         D2D1_RECT_U rectangleDst = {};

         rectangleDst.right = size.cx;

         rectangleDst.bottom = size.cy;

         hrResultCopyBitmap = m_pd2d1bitmap->CopyFromMemory(&rectangleDst, ppixmap->image32(), iScan);

      }

      m_size = size;

      m_osdata[0] = m_pd2d1bitmap;

      m_osdata[1] = m_pd2d1bitmap1;

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

   //i32_size bitmap::GetBitmapDimension() const
   //{

   //   if (!m_pd2d1bitmap)
   //   {

   //      return ::i32_size(0, 0);

   //   }

   //   D2D1_SIZE_U size = m_pd2d1bitmap->GetPixelSize();

   //   return ::i32_size(size.width, size.height);

   //}


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

      if (m_pd2d1bitmap != nullptr)
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
         ((ID2D1DeviceContext *)pgraphics->get_os_data())->CreateBitmap(size, nullptr, size.width * sizeof(::color32_t), props, &m_pd2d1bitmap1);
      }
      m_pd2d1bitmap = m_pd2d1bitmap1;

      m_osdata[0] = m_pd2d1bitmap;

      m_osdata[1] = m_pd2d1bitmap1;

      //return true;

   }


   void bitmap::CreateDiscardableBitmap(::draw2d::graphics* pgraphics, ::i32 nWidth, ::i32 nHeight)
   {

      //::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      if (m_pd2d1bitmap != nullptr)
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
         ((ID2D1DeviceContext *)pgraphics->get_os_data())->CreateBitmap(size, nullptr, size.width * sizeof(::color32_t), &props, &m_pd2d1bitmap1);
      }
      m_pd2d1bitmap = m_pd2d1bitmap1;

      m_osdata[0] = m_pd2d1bitmap;

      m_osdata[1] = m_pd2d1bitmap1;

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

      if (m_pd2d1bitmap != nullptr)
      {

         destroy();

      }

      m_pd2d1bitmap = (ID2D1Bitmap *)hbitmap;

      m_pd2d1bitmap.as(m_pd2d1bitmap1);

      m_osdata[0] = m_pd2d1bitmap;

      m_osdata[1] = m_pd2d1bitmap1;

      //return true;

   }


   void * bitmap::detach()
   {

      clear_os_data();

      m_pd2d1bitmap1 = nullptr;

      return m_pd2d1bitmap.detach();

   }


   void bitmap::destroy()
   {

      destroy_os_data();

      ::draw2d::bitmap::destroy();

   }


   void bitmap::destroy_os_data()
   {

      m_pd2d1bitmap = nullptr;

      m_pd2d1bitmap1 = nullptr;

      object::destroy_os_data();

   }


} // namespace draw2d_direct2d_for_directx11
