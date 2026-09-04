#include "platform.h"
#include "bitmap.h"
#include "draw2d.h"
#include "graphics.h"
#include "window_attachment.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/draw2d/lock.h"
#include "aura/graphics/draw2d/device_lock.h"
#include "aura/windowing/window.h"
#include "aura/user/user/interaction_thread.h"
#include "operating_system-windows_common/direct2d/direct2d.h"
#include <atomic>


namespace draw2d_direct2d
{


   bitmap::bitmap()
   {


   }


   bitmap::bitmap(bitmap && bitmap) :
      DRAW2D_BITMAP_TRANSFER(bitmap),
      draw2d_direct2d::object(::transfer(bitmap)),
      direct2d::object(::transfer(bitmap)),
      m_pd2d1bitmap(::transfer(bitmap.m_pd2d1bitmap)),
      m_pd2d1bitmap1(::transfer(bitmap.m_pd2d1bitmap1)),
      m_memory(::transfer(bitmap.m_memory)),
      m_pd2d1bitmap1Map(::transfer(bitmap.m_pd2d1bitmap1Map)) //,
      //m_pd2d1bitmaprendertarget(::transfer(bitmap.m_pd2d1bitmaprendertarget)),
      //m_pd2d1devicecontext(::transfer(bitmap.m_pd2d1devicecontext))
   {


   }


   bitmap::~bitmap()
   {

      destroy();

   }


   void bitmap::update_bitmap_as_image_render_target(
      ::image::image * pimage,
      ::acme::user::interaction * pacmeuserinteractionAffinity, 
      ::draw2d::graphics * pdraw2dgraphics)
   {

      if (pimage->m_ppixmapOwned)
      {

         _create_d2d1_bitmap(
            pdraw2dgraphics,
            pimage->raw_size(),
            pimage->m_ppixmapOwned->image32(),
            pimage->origin(),
            pimage->size(),
            pimage->m_ppixmapOwned->m_iScan,
            pacmeuserinteractionAffinity);

      }
      else
      {

         if (m_pd2d1bitmap)
         {

            set_size(pimage->raw_size(), true);

         }
         else
         {

            _create_d2d1_bitmap(
              pdraw2dgraphics,
              pimage->raw_size(),
              nullptr,
              pimage->origin(),
              pimage->size(),
              pimage->m_iScan,
              pacmeuserinteractionAffinity);

         }

      }

   }


   void bitmap::_create_d2d1_bitmap(
      ::draw2d::graphics * pdraw2dgraphics,
      const ::i32_size & sizeParam,
      const void * pbits,
      const ::i32_point & pointBits,
      const ::i32_size & sizeBits,
      ::i32 stride,
      ::acme::user::interaction * pacmeuserinteractionAffinity)
   {

      //::draw2d::lock draw2dlock;

      __UNREFERENCED_PARAMETER(pdraw2dgraphics);
      //__UNREFERENCED_PARAMETER(pbits);
      //__UNREFERENCED_PARAMETER(stride);

      ::draw2d::device_lock devicelock(this);

      if (m_pd2d1bitmap != nullptr)
      {

         destroy();

      }



      D2D1_SIZE_F size;

      size.width = (FLOAT) sizeParam.cx;
      size.height = (FLOAT) sizeParam.cy;

      D2D1_SIZE_U sizeu;

      sizeu.width = sizeParam.cx;
      sizeu.height = sizeParam.cy;

      D2D1_PIXEL_FORMAT pixelformat{};

      pixelformat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      pixelformat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
      //properties.dpiX = 96.0;
      //properties.dpiY = 96.0;

      D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS options;

      options = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE;

      if (!pacmeuserinteractionAffinity)
      {

         throw ::exception(
            error_wrong_state,
            "Direct2D bitmap creation requires an interaction affinity");

      }

      ::cast < ::windowing::window > pwindow =
         pacmeuserinteractionAffinity->acme_windowing_window();

      if (!pwindow)
      {

         throw ::exception(
            error_wrong_state,
            "Direct2D bitmap interaction affinity has no window");

      }

      ::cast < ::draw2d_direct2d::window_attachment > pwindowattachment = pwindow->m_pdraw2dwindowattachment;

      if (!pwindowattachment)
      {

         throw ::exception(
            error_wrong_state,
            "Direct2D bitmap window has no Direct2D attachment");

      }

      _synchronous_lock synchronouslock(pwindowattachment->_d2d1_device_context_mutex());

      //draw2d_direct2d::graphics * pgraphics2d = dynamic_cast < ::draw2d_direct2d::graphics * > (pdraw2dgraphics);

      //pgraphics2d->m_pd2d1devicecontext->GetDpi(&props.dpiX, &props.dpiY); // Thank you https://repo.anl-external.org/repos/BlueTBB/tbb41_20130314oss/examples/common/gui/d2dvideo.cpp

      auto pd2d1devicecontext = pwindowattachment->_d2d1_device_context();

      if (!pd2d1devicecontext)
      {

         ::cast < ::draw2d_direct2d::graphics > pdirect2dgraphics = pdraw2dgraphics;

         pd2d1devicecontext = pdirect2dgraphics->m_pd2d1devicecontext.m_p;

      }

      //auto hrCreateCompatibleRenderTarget = pd2d1devicecontext->CreateCompatibleRenderTarget(
      //   size,
      //   sizeu,
      //   pixelformat,
      //   options,
      //   &m_pd2d1bitmaprendertarget);

      //if (FAILED(hrCreateCompatibleRenderTarget))
      //{

      //   throw hresult_exception(hrCreateCompatibleRenderTarget, "Failed to create compatible bitmap render target");

      //}

      D2D1_BITMAP_PROPERTIES1 bitmapproperties1 = {};

      bitmapproperties1.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;
      bitmapproperties1.pixelFormat = pixelformat;
      //props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
      bitmapproperties1.dpiX = 96.0f;
      bitmapproperties1.dpiY = 96.0f;
      bitmapproperties1.colorContext = nullptr;

      //auto hrQueryDeviceContext = m_pd2d1bitmaprendertarget.as(m_pd2d1devicecontext);
      //if(ppdata != nullptr)
      {
         // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(::color32_t), props, &m_pdraw2dbitmap);
      }

      D2D1_BITMAP_PROPERTIES1 properties{};

      properties.pixelFormat =
         D2D1::PixelFormat(
            DXGI_FORMAT_B8G8R8A8_UNORM,
            D2D1_ALPHA_MODE_PREMULTIPLIED);

      properties.dpiX = 96.0f;
      properties.dpiY = 96.0f;

      properties.bitmapOptions =
         D2D1_BITMAP_OPTIONS_TARGET;

      comptr<ID2D1Bitmap1> pdraw2dbitmap;

      HRESULT hrCreateBitmap =
         pd2d1devicecontext->CreateBitmap(
            sizeu,
            nullptr,
            0,
            &properties,
            &pdraw2dbitmap);

      //::defer_throw_hresult(hr);

      //else
      //{
      //HRESULT hr = ((ID2D1DeviceContext *)pdraw2dgraphics->get_os_data())->CreateBitmap(size, lpBits, stride, props, &m_pbitmap1);
      //HRESULT hrCreateBitmap = m_pd2d1bitmaprendertarget->GetBitmap(&m_pd2d1bitmap);

      //}

      if (FAILED(hrCreateBitmap))
      {

         throw ::exception(error_failed);

      }

      m_pd2d1bitmap = pdraw2dbitmap;


      if (pbits && stride >= sizeBits.cx * 4)
      {

         D2D1_RECT_U r;

         r.left = pointBits.x;
         r.top = pointBits.y;
         r.right = sizeBits.cx;
         r.bottom = sizeBits.cy;

         m_pd2d1bitmap->CopyFromMemory(&r, pbits, stride);

      }


      auto hrQueryBitmap1 = m_pd2d1bitmap.as(m_pd2d1bitmap1);

      static ::std::atomic<unsigned int> s_uBitmapCreationDiagnosticCount{ 0 };
      auto uBitmapCreationDiagnosticCount = s_uBitmapCreationDiagnosticCount.fetch_add(1, ::std::memory_order_relaxed);

      if (uBitmapCreationDiagnosticCount < 32
         //|| FAILED(hrQueryDeviceContext)
         || FAILED(hrQueryBitmap1))
      {

         informationf(
            "Direct2DBitmapCreationDiagnostic create=0x%08lx queryBitmap1=0x%08lx "
            "thread=%lu requested=(%d,%d) sourceContext=%p "
            "bitmap=%p bitmap1=%p ",
            //"bitmap=%p bitmap1=%p bitmapContext=%p",
            //(unsigned long)hrCreateCompatibleRenderTarget,
            //(unsigned long)hrQueryDeviceContext,
            (unsigned long)hrCreateBitmap,
            (unsigned long)hrQueryBitmap1,
            (unsigned long)::GetCurrentThreadId(),
            sizeParam.cx,
            sizeParam.cy,
            pd2d1devicecontext,
            (ID2D1Bitmap *)m_pd2d1bitmap,
            (ID2D1Bitmap1 *)m_pd2d1bitmap1
            //,           (ID2D1DeviceContext *)m_pd2d1devicecontext
         );

      }

      //if (FAILED(hrQueryDeviceContext))
      //{

      //   throw hresult_exception(hrQueryDeviceContext, "Failed to query bitmap device context");

      //}

      if (FAILED(hrQueryBitmap1))
      {

         throw hresult_exception(hrQueryBitmap1, "Failed to query ID2D1Bitmap1");

      }


      //m_pdevicecontext->SetTarget(m_pbitmap1);

      //zero(m_map);
      //    m_pdraw2dbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);
      //
      //if(ppdata != nullptr)
      // *ppdata = (::color::color *) m_map.bits;
      //m_osdata[0] = m_pd2d1bitmap;

      //return true;

      set_ok_flag();

      m_estatus = success;

   }



   void bitmap::preserve_image(const ::i32_size & size, ::image::image * pimage)
   {

      if (size.cx <= 0 || size.cy <= 0)
      {

         throw ::exception(error_bad_argument);

      }

      if (!pimage)
      {

         throw ::exception(error_null_pointer);

      }

      if (!m_pd2d1bitmap)
      {

         pimage->create_as_descriptor(size);

         _create_d2d1_bitmap(
            nullptr, size, nullptr, {}, {}, 0,
            pimage->m_pacmeuserinteractionAffinity);

         pimage->m_pdraw2dbitmap = this;

         return;

      }

      auto sizeOld = m_pd2d1bitmap->GetPixelSize();

      if (sizeOld.width == (UINT32)size.cx
         && sizeOld.height == (UINT32)size.cy)
      {

         pimage->create_as_descriptor(size);

         return;

      }

      auto pacmeuserinteractionAffinity = pimage->m_pacmeuserinteractionAffinity;

      if (!pacmeuserinteractionAffinity)
      {

         throw ::exception(
            error_wrong_state,
            "Direct2D bitmap preservation requires an interaction affinity");

      }

      ::cast < ::windowing::window > pwindow =
         pacmeuserinteractionAffinity->acme_windowing_window();

      if (!pwindow)
      {

         throw ::exception(
            error_wrong_state,
            "Direct2D bitmap preservation requires a window");

      }

      ::cast < ::draw2d_direct2d::window_attachment > pwindowattachment =
         pwindow->m_pdraw2dwindowattachment;

      if (!pwindowattachment)
      {

         throw ::exception(
            error_wrong_state,
            "Direct2D bitmap preservation requires a Direct2D window attachment");

      }

      _synchronous_lock synchronouslock(
         pwindowattachment->_d2d1_device_context_mutex());

      auto pd2d1devicecontext = pwindowattachment->_d2d1_device_context();

      if (!pd2d1devicecontext)
      {

         throw ::exception(
            error_wrong_state,
            "Direct2D bitmap preservation requires a device context");

      }

      auto pd2d1bitmapOld = m_pd2d1bitmap;

      comptr < ID2D1Image > pd2d1imageTarget;

      pd2d1devicecontext->GetTarget(&pd2d1imageTarget);

      bool bOldBitmapWasTarget = false;

      if (pd2d1imageTarget)
      {

         comptr < IUnknown > punknownTarget;
         comptr < IUnknown > punknownOldBitmap;

         pd2d1imageTarget.as(punknownTarget);
         pd2d1bitmapOld.as(punknownOldBitmap);

         bOldBitmapWasTarget = punknownTarget.m_p == punknownOldBitmap.m_p;

         if (bOldBitmapWasTarget)
         {

            pd2d1devicecontext->SetTarget(nullptr);

         }

      }

      D2D1_BITMAP_PROPERTIES1 properties{};

      properties.pixelFormat = pd2d1bitmapOld->GetPixelFormat();
      pd2d1bitmapOld->GetDpi(&properties.dpiX, &properties.dpiY);
      properties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;

      D2D1_SIZE_U sizeNew{};

      sizeNew.width = (UINT32)size.cx;
      sizeNew.height = (UINT32)size.cy;

      auto iStride = size.cx * (::i32)sizeof(::image32_t);

      ::memory memoryTransparent((memsize)iStride * size.cy);

      memoryTransparent.set(0);

      comptr < ID2D1Bitmap1 > pd2d1bitmapNew;

      auto hr = pd2d1devicecontext->CreateBitmap(
         sizeNew,
         memoryTransparent.data(),
         (UINT32)iStride,
         &properties,
         &pd2d1bitmapNew);

      if (FAILED(hr))
      {

         if (bOldBitmapWasTarget)
         {

            pd2d1devicecontext->SetTarget(pd2d1bitmapOld);

         }

         throw hresult_exception(
            hr,
            "Failed to create a preserving Direct2D bitmap");

      }

      D2D1_RECT_U rectangleSource{};

      rectangleSource.right = minimum(sizeOld.width, sizeNew.width);
      rectangleSource.bottom = minimum(sizeOld.height, sizeNew.height);

      if (rectangleSource.right > 0 && rectangleSource.bottom > 0)
      {

         hr = pd2d1bitmapNew->CopyFromBitmap(
            nullptr,
            pd2d1bitmapOld,
            &rectangleSource);

         if (FAILED(hr))
         {

            if (bOldBitmapWasTarget)
            {

               pd2d1devicecontext->SetTarget(pd2d1bitmapOld);

            }

            throw hresult_exception(
               hr,
               "Failed to preserve resized Direct2D bitmap contents");

         }

      }

      m_pd2d1bitmap = pd2d1bitmapNew;
      m_pd2d1bitmap1 = pd2d1bitmapNew;
      m_pd2d1bitmap1Map = nullptr;
      m_memory.set_size(0);
      m_size = size;

      pimage->create_as_descriptor(size);
      pimage->m_pdraw2dbitmap = this;

   }


   void bitmap::CreateBitmap(
      ::draw2d::graphics * pdraw2dgraphics,
      const ::i32_size & size,
      ::u32 nPlanes,
      ::u32 nBitcount,
      const void * pbits,
      ::i32 stride)
   {

      __UNREFERENCED_PARAMETER(nPlanes);
      __UNREFERENCED_PARAMETER(nBitcount);

      auto pacmeuserinteractionAffinity = pdraw2dgraphics
         ? pdraw2dgraphics->m_pacmeuserinteractionAffinity.m_p
         : nullptr;

      _create_d2d1_bitmap(
         pdraw2dgraphics,
         size,
         pbits,
         {},
         size,
         stride,
         pacmeuserinteractionAffinity);

   }

   
   bool bitmap::CreateBitmapIndirect(::draw2d::graphics * pdraw2dgraphics, LPBITMAP lpBitmap)
   {
      
      return false;

   }


   void bitmap::create_bitmap(::draw2d::graphics * pdraw2dgraphics, const ::i32_size & size, ::pixmap * ppixmap)
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

      draw2d_direct2d::graphics * pgraphics2d = dynamic_cast < ::draw2d_direct2d::graphics * > (pdraw2dgraphics);

      if (!::is_set(pgraphics2d))
      {

         throw ::exception(error_failed);

      }

      auto prendertarget = pgraphics2d->m_pd2d1devicecontext;

      if (!prendertarget)
      {

         throw ::exception(error_failed);

      }

      prendertarget->GetDpi(&props.dpiX, &props.dpiY);

      props.colorContext = nullptr;

      props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;

      ::cast < ::draw2d_direct2d::graphics > pdraw2ddirect2dgraphics = pdraw2dgraphics;

      auto pd2d1devicecontext = pdraw2ddirect2dgraphics->m_pd2d1devicecontext;

      if (!pd2d1devicecontext)
      {

         throw ::exception(error_wrong_state);

      }

      //m_pd2d1devicecontext = pd2d1devicecontext;

      HRESULT hr = pd2d1devicecontext->CreateBitmap(usize, nullptr, 0, props, &m_pd2d1bitmap1);

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

      //defer_construct_newø(m_ppixmapOwned);

      HRESULT hrResultCopyBitmap = S_OK;

      if (ppixmap && ppixmap->m_memoryPixmap.data_if_at_least(size.cy * iScan))
      {

         D2D1_RECT_U rectangleDst = {};

         rectangleDst.right = size.cx;

         rectangleDst.bottom = size.cy;

         hrResultCopyBitmap = m_pd2d1bitmap->CopyFromMemory(&rectangleDst, ppixmap->m_memoryPixmap.data(), iScan);

      }

      m_size = size;

      //m_osdata[0] = m_pd2d1bitmap;

      //m_osdata[1] = m_pd2d1bitmap1;

      //return true;

   }


   void bitmap::read_pixels(
      const ::i32_size & size,
      const ::i32_point & point,
      ::image32_t * pimage32,
      ::i32 iScan)
   {

      if (size.cx <= 0 || size.cy <= 0)
      {

         return;

      }

      if (!pimage32)
      {

         throw ::exception(error_null_pointer);

      }

      if (iScan < size.cx * (::i32) sizeof(::image32_t))
      {

         throw ::exception(error_bad_argument);

      }

      ::draw2d::device_lock devicelock(this);

      //if (!m_pd2d1bitmap1 || !m_pd2d1devicecontext)
      if (!m_pd2d1bitmap1)
      {

         throw ::exception(error_wrong_state);

      }

      auto sizeBitmap = m_pd2d1bitmap1->GetPixelSize();

      if (point.x < 0 || point.y < 0
         || point.x > (::i32) sizeBitmap.width - size.cx
         || point.y > (::i32) sizeBitmap.height - size.cy)
      {

         throw ::exception(error_bad_argument);

      }

      auto pbitmap1Map = _map_bitmap1();

      if (!pbitmap1Map)
      {

         throw ::exception(error_wrong_state);

      }

      auto hrCopy = pbitmap1Map->CopyFromBitmap(nullptr, m_pd2d1bitmap1, nullptr);

      if (FAILED(hrCopy))
      {

         throw hresult_exception(hrCopy, "Failed to copy Direct2D bitmap for pixel readback");

      }

      D2D1_MAPPED_RECT mappedrect{};

      auto hrMap = pbitmap1Map->Map(D2D1_MAP_OPTIONS_READ, &mappedrect);

      if (FAILED(hrMap))
      {

         throw hresult_exception(hrMap, "Failed to map Direct2D bitmap for pixel readback");

      }

      try
      {

         if (!mappedrect.bits
            || mappedrect.pitch < sizeBitmap.width * sizeof(::image32_t))
         {

            throw ::exception(error_failed);

         }

         auto pimage32Source = (const ::image32_t *)
            (mappedrect.bits
               + point.y * mappedrect.pitch
               + point.x * sizeof(::image32_t));

         auto pimage32Target = pimage32;

         pimage32Target->copy(size, iScan, pimage32Source, (::i32) mappedrect.pitch);

      }
      catch (...)
      {

         pbitmap1Map->Unmap();

         throw;

      }

      auto hrUnmap = pbitmap1Map->Unmap();

      if (FAILED(hrUnmap))
      {

         throw hresult_exception(hrUnmap, "Failed to unmap Direct2D bitmap after pixel readback");

      }

   }


   void bitmap::write_pixels(
      const ::i32_size & size,
      const ::i32_point & point,
      const ::image32_t * pimage32,
      ::i32 iScan)
   {

      if (size.cx <= 0 || size.cy <= 0)
      {

         return;

      }

      if (!pimage32)
      {

         throw ::exception(error_null_pointer);

      }

      if (iScan < size.cx * (::i32) sizeof(::image32_t))
      {

         throw ::exception(error_bad_argument);

      }

      ::draw2d::device_lock devicelock(this);

      if (!m_pd2d1bitmap1)
      {

         throw ::exception(error_wrong_state);

      }

      auto sizeBitmap = m_pd2d1bitmap1->GetPixelSize();

      if (point.x < 0 || point.y < 0
         || point.x > (::i32) sizeBitmap.width - size.cx
         || point.y > (::i32) sizeBitmap.height - size.cy)
      {

         throw ::exception(error_bad_argument);

      }

      D2D1_RECT_U rectangleDst{};

      rectangleDst.left = (UINT32) point.x;
      rectangleDst.top = (UINT32) point.y;
      rectangleDst.right = (UINT32) (point.x + size.cx);
      rectangleDst.bottom = (UINT32) (point.y + size.cy);

      auto hrCopy = m_pd2d1bitmap1->CopyFromMemory(&rectangleDst, pimage32, (UINT32) iScan);

      if (FAILED(hrCopy))
      {

         throw hresult_exception(hrCopy, "Failed to write pixels to Direct2D bitmap");

      }

   }


   void bitmap::CreateDIBitmap(::draw2d::graphics * pdraw2dgraphics, ::i32 cx, ::i32 cy, ::u32 flInit, const void *pjBits, ::u32 iUsage)
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


   ID2D1Bitmap1 * bitmap::_map_bitmap1()
   {

      D2D1_SIZE_U sizeuMap{};
      
      if (m_pd2d1bitmap1Map)
      {

         sizeuMap = m_pd2d1bitmap1Map->GetPixelSize();

      }

      ::i32_size sizeMap(sizeuMap.width, sizeuMap.height);

      auto sizeuThis = m_pd2d1bitmap1->GetPixelSize();

      ::i32_size sizeThis(sizeuThis.width, sizeuThis.height);

      if (!m_pd2d1bitmap1Map || sizeMap != sizeThis)
      {

         if (m_pd2d1bitmap1Map)
         {

            m_pd2d1bitmap1Map.release();

         }

         D2D1_BITMAP_PROPERTIES1 stagingProperties = D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_CANNOT_DRAW | D2D1_BITMAP_OPTIONS_CPU_READ,
            m_pd2d1bitmap1->GetPixelFormat()
         );

         {

            auto pdraw2ddirect2d = ::draw2d_direct2d::draw2d::get();

            synchronous_lock synchronouslock(pdraw2ddirect2d->default_device_context_mutex());

            auto pd2d1devicecontextDefault = pdraw2ddirect2d->default_d2d1_device_context();

            auto hrCreateMapBitmap = pd2d1devicecontextDefault->CreateBitmap(
                sizeuThis,
                nullptr,
                0,
                &stagingProperties,
                &m_pd2d1bitmap1Map
            );

            if (FAILED(hrCreateMapBitmap))
            {

               throw hresult_exception(hrCreateMapBitmap, "Failed to create map bitmap");

            }

         }

         auto stagingSize = m_pd2d1bitmap1Map->GetPixelSize();
         auto stagingPixelFormat = m_pd2d1bitmap1Map->GetPixelFormat();
         auto stagingOptions = m_pd2d1bitmap1Map->GetOptions();

         static ::std::atomic<unsigned int> s_uStagingCreationDiagnosticCount{ 0 };
         auto uStagingCreationDiagnosticCount = s_uStagingCreationDiagnosticCount.fetch_add(1, ::std::memory_order_relaxed);

         if (uStagingCreationDiagnosticCount < 32)
         {

            informationf(
               "Direct2DStagingCreationDiagnostic thread=%lu source=%p staging=%p "
               "requested=(%u,%u) actual=(%u,%u) options=0x%08x format=%u alpha=%u",
               //(unsigned long)hrCreateMapBitmap,
               (unsigned long)::GetCurrentThreadId(),
               //(ID2D1DeviceContext *)m_pd2d1devicecontext,
               (ID2D1Bitmap1 *)m_pd2d1bitmap1,
               (ID2D1Bitmap1 *)m_pd2d1bitmap1Map,
               (unsigned int)sizeuThis.width,
               (unsigned int)sizeuThis.height,
               (unsigned int)stagingSize.width,
               (unsigned int)stagingSize.height,
               (unsigned int)stagingOptions,
               (unsigned int)stagingPixelFormat.format,
               (unsigned int)stagingPixelFormat.alphaMode);

         }

      }

      return m_pd2d1bitmap1Map;

   }


   //i32_size bitmap::GetBitmapDimension() const
   //{

   //   if (!m_pd2d1bitmap)
   //   {

   //      return ::i32_size(0, 0);

   //   }

   //   D2D1_SIZE_U size = m_pd2d1bitmap->GetPixelSize();

   //   return ::i32_size(size.width, size.height);

   //}


   i32_size bitmap::size() const
   {

      if (!m_pd2d1bitmap)
      {

         return ::i32_size(0, 0);

      }

      D2D1_SIZE_U size = m_pd2d1bitmap->GetPixelSize();

      return ::i32_size(size.width, size.height);

   }


   void bitmap::set_size(const ::i32_size & size, bool bPreserve)
   {

      if (size.cx <= 0 || size.cy <= 0)
      {

         throw ::exception(error_bad_argument);

      }

      auto sizeOld = this->size();

      if (size == sizeOld)
      {

         return;

      }

      ::draw2d::device_lock devicelock(this);

      //if (!m_pd2d1bitmap || !m_pd2d1devicecontext)
      if (!m_pd2d1bitmap)
      {

         throw ::exception(error_wrong_state);

      }

      D2D1_SIZE_U sizeNew{};

      sizeNew.width = (UINT32) size.cx;
      sizeNew.height = (UINT32) size.cy;

      comptr<ID2D1Bitmap> pbitmapNew;
      comptr<ID2D1Bitmap1> pbitmap1New;
      comptr<ID2D1BitmapRenderTarget> pbitmaprendertargetNew;
      //comptr<ID2D1DeviceContext> pdevicecontextNew;

      //if (m_pd2d1bitmaprendertarget)
      //{

      //   D2D1_SIZE_F sizeNewDips{};

      //   sizeNewDips.width = (FLOAT) size.cx;
      //   sizeNewDips.height = (FLOAT) size.cy;

      //   auto pixelformat = m_pd2d1bitmap->GetPixelFormat();

      //   auto hr = m_pd2d1bitmaprendertarget->CreateCompatibleRenderTarget(
      //      &sizeNewDips,
      //      &sizeNew,
      //      &pixelformat,
      //      D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE,
      //      &pbitmaprendertargetNew);

      //   if (FAILED(hr))
      //   {

      //      throw hresult_exception(hr, "Failed to resize Direct2D bitmap render target");

      //   }

      //   hr = pbitmaprendertargetNew.as(pdevicecontextNew);

      //   if (FAILED(hr))
      //   {

      //      throw hresult_exception(hr, "Failed to query resized Direct2D bitmap device context");

      //   }

      //   D2D1_COLOR_F colorTransparent{};

      //   pbitmaprendertargetNew->BeginDraw();
      //   pbitmaprendertargetNew->Clear(&colorTransparent);

      //   hr = pbitmaprendertargetNew->EndDraw();

      //   if (FAILED(hr))
      //   {

      //      throw hresult_exception(hr, "Failed to clear resized Direct2D bitmap");

      //   }

      //   hr = pbitmaprendertargetNew->GetBitmap(&pbitmapNew);

      //   if (FAILED(hr))
      //   {

      //      throw hresult_exception(hr, "Failed to get resized Direct2D bitmap");

      //   }

      //   hr = pbitmapNew.as(pbitmap1New);

      //   if (FAILED(hr))
      //   {

      //      throw hresult_exception(hr, "Failed to query resized ID2D1Bitmap1");

      //   }

      //}
      //else
      {

         auto pdirect2d = ::direct2d::get();

         synchronous_lock synchronouslock(pdirect2d->m_pmutexDeviceContextDefault);

         D2D1_BITMAP_PROPERTIES1 properties{};

         properties.pixelFormat = m_pd2d1bitmap->GetPixelFormat();
         m_pd2d1bitmap->GetDpi(&properties.dpiX, &properties.dpiY);
         properties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;

         auto iStride = size.cx * (::i32) sizeof(::image32_t);
         ::memory memoryTransparent((memsize) iStride * size.cy);
         memoryTransparent.set(0);

         auto hr = pdirect2d->m_pd2d1devicecontextDefault->CreateBitmap(
            sizeNew,
            memoryTransparent.data(),
            (UINT32) iStride,
            &properties,
            &pbitmap1New);

         if (FAILED(hr))
         {

            throw hresult_exception(hr, "Failed to create resized Direct2D bitmap");

         }

         hr = pbitmap1New.as(pbitmapNew);

         if (FAILED(hr))
         {

            throw hresult_exception(hr, "Failed to query resized ID2D1Bitmap");

         }

         //pdevicecontextNew = pdirect2d->m_pd2d1devicecontext;

      }

      if (bPreserve)
      {

         D2D1_RECT_U rectangleSource{};

         rectangleSource.right = (UINT32)minimum(sizeOld.cx, size.cx);
         rectangleSource.bottom = (UINT32)minimum(sizeOld.cy, size.cy);

         auto hrCopy = pbitmapNew->CopyFromBitmap(nullptr, m_pd2d1bitmap, &rectangleSource);

         if (FAILED(hrCopy))
         {

            throw hresult_exception(hrCopy, "Failed to preserve resized Direct2D bitmap contents");

         }

      }

      m_pd2d1bitmap = ::transfer(pbitmapNew);
      m_pd2d1bitmap1 = ::transfer(pbitmap1New);
      m_pd2d1bitmap1Map = nullptr;

      //if (pbitmaprendertargetNew)
      //{

      //   m_pd2d1bitmaprendertarget = ::transfer(pbitmaprendertargetNew);

      //}

      //m_pd2d1devicecontext = ::transfer(pdevicecontextNew);
      //m_osdata[0] = m_pd2d1bitmap;
      //m_osdata[1] = m_pd2d1bitmap1;
      m_size = size;

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


   void bitmap::CreateCompatibleBitmap(::draw2d::graphics * pdraw2dgraphics, ::i32 nWidth, ::i32 nHeight)
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

      D2D1_BITMAP_PROPERTIES1 props{};

      props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

      ::cast < draw2d_direct2d::graphics >pdraw2ddirect2dgraphics = pdraw2dgraphics;

      pdraw2ddirect2dgraphics->m_pd2d1devicecontext->GetDpi(&props.dpiX, &props.dpiY); // Thanks again and a third time https://repo.anl-external.org/repos/BlueTBB/tbb41_20130314oss/examples/common/gui/d2dvideo.cpp      props.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

      props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;

      //if(ppdata != nullptr)
      {
         // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(::color32_t), props, &m_pdraw2dbitmap);
      }
      //else
      //      ID2D1Bitmap1 * pbitmap1;
      {

         auto pd2d1devicecontext = pdraw2ddirect2dgraphics->m_pd2d1devicecontext;

         if (!pd2d1devicecontext)
         {

            throw ::exception(error_wrong_state);

         }

         //m_pd2d1devicecontext = pd2d1devicecontext;

         auto hrCreateBitmap = pd2d1devicecontext->CreateBitmap(
            size,
            nullptr,
            0,
            props,
            &m_pd2d1bitmap1);

         if (FAILED(hrCreateBitmap) || !m_pd2d1bitmap1)
         {

            throw hresult_exception(hrCreateBitmap, "Failed to create compatible Direct2D bitmap");

         }

      }

      m_pd2d1bitmap = m_pd2d1bitmap1;

      //m_osdata[0] = m_pd2d1bitmap;

      //m_osdata[1] = m_pd2d1bitmap1;

      //return true;

   }


   void bitmap::CreateDiscardableBitmap(::draw2d::graphics * pdraw2dgraphics, ::i32 nWidth, ::i32 nHeight)
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

      D2D1_BITMAP_PROPERTIES1 props{};

      props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

      ::cast<draw2d_direct2d::graphics > pdraw2ddirect2dgraphics = pdraw2dgraphics;

      auto pd2d1devicecontext = pdraw2ddirect2dgraphics->m_pd2d1devicecontext;

      if (!pd2d1devicecontext)
      {

         throw ::exception(error_wrong_state);

      }

      pd2d1devicecontext->GetDpi(&props.dpiX, &props.dpiY); // One more time, Thank you very much https://repo.anl-external.org/repos/BlueTBB/tbb41_20130314oss/examples/common/gui/d2dvideo.cpp      props.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

      props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;

      ////if(ppdata != nullptr)
      //{
      //   // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(::color32_t), props, &m_pdraw2dbitmap);
      //}
      ////else
      //{

      //   

      // auto pdevicecontext = (ID2D1DeviceContext *)pdraw2dgraphics->get_os_data();


         //m_pd2d1devicecontext = pdevicecontext;

         auto hrCreateBitmap = pd2d1devicecontext->CreateBitmap(
            size,
            nullptr,
            0,
            &props,
            &m_pd2d1bitmap1);

         if (FAILED(hrCreateBitmap) || !m_pd2d1bitmap1)
         {

            throw hresult_exception(hrCreateBitmap, "Failed to create discardable Direct2D bitmap");

         }

      //}
      m_pd2d1bitmap1.as(m_pd2d1bitmap);

      //m_osdata[0] = m_pd2d1bitmap;

      //m_osdata[1] = m_pd2d1bitmap1;

      //m_pdraw2dbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);

      //if(ppdata != nullptr)
      // *ppdata = (::color::color *) m_map.bits;


      //return true;

   }


   //void bitmap::dump(dump_context & dumpcontext) const
   //{

   //   ::draw2d::object::dump(dumpcontext);

   //   //dumpcontext << "\n";

   //}




   void bitmap::defer_update(::draw2d::graphics * pdraw2dgraphics) const
   {

   }


   //void bitmap::attach(void * hbitmap)
   //{

   //   if (m_pd2d1bitmap != nullptr)
   //   {

   //      destroy();

   //   }

   //   m_pd2d1bitmap = (ID2D1Bitmap *)hbitmap;

   //   m_pd2d1bitmap.as(m_pd2d1bitmap1);

   //   //m_osdata[0] = m_pd2d1bitmap;

   //   //m_osdata[1] = m_pd2d1bitmap1;

   //   //return true;

   //}


   //void * bitmap::detach()
   //{

   //   m_pd2d1bitmap1 = nullptr;

   //   return m_pd2d1bitmap.detach();

   //}


   //void bitmap::destroy()
   //{

   //   destroy_os_data();

   //   ::draw2d::bitmap::destroy();

   //}


   void bitmap::clear_node_data()
   {

      m_pd2d1bitmap1Map = nullptr;

      m_pd2d1bitmap = nullptr;

      m_pd2d1bitmap1 = nullptr;

      //m_pd2d1bitmaprendertarget = nullptr;

      //m_pd2d1devicecontext = nullptr;

      //object::destroy();

   }


} // namespace draw2d_direct2d
