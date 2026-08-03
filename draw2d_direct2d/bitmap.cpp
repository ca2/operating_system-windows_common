#include "framework.h"
#include "bitmap.h"
#include "graphics.h"
#include "window_attachment.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/draw2d/lock.h"
#include "aura/graphics/draw2d/device_lock.h"
#include "aura/windowing/window.h"
#include "aura/user/user/interaction_thread.h"
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
      m_pbitmap(::transfer(bitmap.m_pbitmap)),
      m_pbitmap1(::transfer(bitmap.m_pbitmap1)),
      m_memory(::transfer(bitmap.m_memory)),
      m_pbitmap1Map(::transfer(bitmap.m_pbitmap1Map)),
      m_pbitmaprendertarget(::transfer(bitmap.m_pbitmaprendertarget)),
      m_pdevicecontext(::transfer(bitmap.m_pdevicecontext))
   {


   }


   bitmap::~bitmap()
   {

      destroy();

   }


   void bitmap::create_bitmap_for_image(
      ::image::image * pimage,
      ::acme::user::interaction * pacmeuserinteractionAffinity)
   {

      _create_bitmap(
         nullptr,
         pimage->m_sizeRaw,
         pimage->image32(),
         pimage->m_iScan,
         pacmeuserinteractionAffinity);

   }


   void bitmap::_create_bitmap(
      ::draw2d::graphics * pgraphics,
      const ::i32_size & sizeParam,
      const void * pbits,
      ::i32 stride,
      ::acme::user::interaction * pacmeuserinteractionAffinity)
   {

      //::draw2d::lock draw2dlock;

      __UNREFERENCED_PARAMETER(pgraphics);
      //__UNREFERENCED_PARAMETER(pbits);
      //__UNREFERENCED_PARAMETER(stride);

      ::draw2d::device_lock devicelock(this);

      if (m_pbitmap != nullptr)
      {

         destroy();

      }



      D2D1_SIZE_F size;

      size.width = sizeParam.cx;
      size.height = sizeParam.cy;

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

      //draw2d_direct2d::graphics * pgraphics2d = dynamic_cast < ::draw2d_direct2d::graphics * > (pgraphics);

      //pgraphics2d->m_pd2d1rendertarget->GetDpi(&props.dpiX, &props.dpiY); // Thank you https://repo.anl-external.org/repos/BlueTBB/tbb41_20130314oss/examples/common/gui/d2dvideo.cpp

      auto pd2d1devicecontext = pwindowattachment->_d2d1_device_context();

      auto hrCreateCompatibleRenderTarget = pd2d1devicecontext->CreateCompatibleRenderTarget(
         size,
         sizeu,
         pixelformat,
         options,
         &m_pbitmaprendertarget);

      if (FAILED(hrCreateCompatibleRenderTarget))
      {

         throw hresult_exception(hrCreateCompatibleRenderTarget, "Failed to create compatible bitmap render target");

      }

      D2D1_BITMAP_PROPERTIES1 bitmapproperties1 = {};

      bitmapproperties1.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;
      bitmapproperties1.pixelFormat = pixelformat;
      //props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
      bitmapproperties1.dpiX = 96.0f;
      bitmapproperties1.dpiY = 96.0f;
      bitmapproperties1.colorContext = nullptr;

      auto hrQueryDeviceContext = m_pbitmaprendertarget.as(m_pdevicecontext);
      //if(ppdata != nullptr)
      {
         // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(::color32_t), props, &m_pbitmap);
      }
      //else
      //{
      //HRESULT hr = ((ID2D1DeviceContext *)pgraphics->get_os_data())->CreateBitmap(size, lpBits, stride, props, &m_pbitmap1);
      HRESULT hrCreateBitmap = m_pbitmaprendertarget->GetBitmap(&m_pbitmap);

      //}

      if (FAILED(hrCreateBitmap))
      {

         throw ::exception(error_failed);

      }


      if (pbits && stride >= size.width * 4)
      {

         D2D1_RECT_U r;

         r.left = 0;
         r.top = 0;
         r.right = size.width;
         r.bottom = size.height;

         m_pbitmap->CopyFromMemory(&r, pbits, stride);

      }


      auto hrQueryBitmap1 = m_pbitmap.as(m_pbitmap1);

      static ::std::atomic<unsigned int> s_uBitmapCreationDiagnosticCount{ 0 };
      auto uBitmapCreationDiagnosticCount = s_uBitmapCreationDiagnosticCount.fetch_add(1, ::std::memory_order_relaxed);

      if (uBitmapCreationDiagnosticCount < 32
         || FAILED(hrQueryDeviceContext)
         || FAILED(hrQueryBitmap1))
      {

         informationf(
            "Direct2DBitmapCreationDiagnostic compatible=0x%08lx queryContext=0x%08lx getBitmap=0x%08lx "
            "queryBitmap1=0x%08lx thread=%lu requested=(%d,%d) sourceContext=%p bitmapTarget=%p "
            "bitmap=%p bitmap1=%p bitmapContext=%p",
            (unsigned long)hrCreateCompatibleRenderTarget,
            (unsigned long)hrQueryDeviceContext,
            (unsigned long)hrCreateBitmap,
            (unsigned long)hrQueryBitmap1,
            (unsigned long)::GetCurrentThreadId(),
            sizeParam.cx,
            sizeParam.cy,
            pd2d1devicecontext,
            (ID2D1BitmapRenderTarget *)m_pbitmaprendertarget,
            (ID2D1Bitmap *)m_pbitmap,
            (ID2D1Bitmap1 *)m_pbitmap1,
            (ID2D1DeviceContext *)m_pdevicecontext);

      }

      if (FAILED(hrQueryDeviceContext))
      {

         throw hresult_exception(hrQueryDeviceContext, "Failed to query bitmap device context");

      }

      if (FAILED(hrQueryBitmap1))
      {

         throw hresult_exception(hrQueryBitmap1, "Failed to query ID2D1Bitmap1");

      }


      //m_pdevicecontext->SetTarget(m_pbitmap1);

      //zero(m_map);
      //    m_pbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);
      //
      //if(ppdata != nullptr)
      // *ppdata = (::color::color *) m_map.bits;
      m_osdata[0] = m_pbitmap;

      //return true;

      set_ok_flag();

      m_estatus = success;

   }



   void bitmap::preserve_image(const ::i32_size & size, ::image::image * pimage)
   {

      auto pbitmapThis = ::as_pointer(this);

      auto pbitmap = ::transfer(m_pbitmap);
      auto pbitmap1 = ::transfer(m_pbitmap1);
      auto memory = ::transfer(m_memory);
      auto pbitmap1Map = ::transfer(m_pbitmap1Map);
      auto pbitmaprendertarget = ::transfer(m_pbitmaprendertarget);
      auto pdevicecontext = ::transfer(m_pdevicecontext);


      pimage->create_as_descriptor(size);

      _create_bitmap(
         nullptr,
         size,
         nullptr,
         0,
         pimage->m_pacmeuserinteractionAffinity);

      if (pbitmap)
      {

         D2D1_RECT_U rectSource;

         auto sizeMinimum = pbitmap->GetSize();

         rectSource.left = 0;
         rectSource.top = 0;
         rectSource.right = minimum(sizeMinimum.width, size.cx);
         rectSource.bottom = minimum(sizeMinimum.height, size.cy);

         D2D1_POINT_2U pointTarget;

         pointTarget.x = 0;
         pointTarget.y = 0;

         auto hrCopyFromBitmap = m_pbitmap->CopyFromBitmap(&pointTarget, pbitmap, &rectSource);

      }

      pimage->m_pbitmap = this;

   }


   void bitmap::CreateBitmap(
      ::draw2d::graphics * pgraphics,
      const ::i32_size & size,
      ::u32 nPlanes,
      ::u32 nBitcount,
      const void * pbits,
      ::i32 stride)
   {

      __UNREFERENCED_PARAMETER(nPlanes);
      __UNREFERENCED_PARAMETER(nBitcount);

      auto pacmeuserinteractionAffinity = pgraphics
         ? pgraphics->m_pacmeuserinteractionAffinity.m_p
         : nullptr;

      _create_bitmap(
         pgraphics,
         size,
         pbits,
         stride,
         pacmeuserinteractionAffinity);

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

      draw2d_direct2d::graphics * pgraphics2d = dynamic_cast < ::draw2d_direct2d::graphics * > (pgraphics);

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

   ID2D1Bitmap1 * bitmap::_map_bitmap1()
   {

      D2D1_SIZE_U sizeu1{};
      
      if (m_pbitmap1Map)
      {

         sizeu1 = m_pbitmap1Map->GetPixelSize();

      }

      auto sizeu2 = m_pbitmap1->GetPixelSize();

      ::i32_size size1;

      size1.cx = sizeu1.width;

      size1.cy = sizeu1.height;

      ::i32_size size2;

      size2.cx = sizeu2.width;

      size2.cy = sizeu2.height;


      if (!m_pbitmap1Map || size1 != size2)
      {

         if (m_pbitmap1Map)
         {

            m_pbitmap1Map.release();

         }

         D2D1_BITMAP_PROPERTIES1 stagingProperties = D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_CANNOT_DRAW | D2D1_BITMAP_OPTIONS_CPU_READ,
            { DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED }
         );

         auto hrCreateMapBitmap = m_pdevicecontext->CreateBitmap(
             sizeu2,
             nullptr,
             0,
             &stagingProperties,
             &m_pbitmap1Map
         ); 

         if (FAILED(hrCreateMapBitmap))
         {

            throw hresult_exception(hrCreateMapBitmap, "Failed to create map bitmap");

         }

         auto stagingSize = m_pbitmap1Map->GetPixelSize();
         auto stagingPixelFormat = m_pbitmap1Map->GetPixelFormat();
         auto stagingOptions = m_pbitmap1Map->GetOptions();

         static ::std::atomic<unsigned int> s_uStagingCreationDiagnosticCount{ 0 };
         auto uStagingCreationDiagnosticCount = s_uStagingCreationDiagnosticCount.fetch_add(1, ::std::memory_order_relaxed);

         if (uStagingCreationDiagnosticCount < 32)
         {

            informationf(
               "Direct2DStagingCreationDiagnostic create=0x%08lx thread=%lu context=%p source=%p staging=%p "
               "requested=(%u,%u) actual=(%u,%u) options=0x%08x format=%u alpha=%u",
               (unsigned long)hrCreateMapBitmap,
               (unsigned long)::GetCurrentThreadId(),
               (ID2D1DeviceContext *)m_pdevicecontext,
               (ID2D1Bitmap1 *)m_pbitmap1,
               (ID2D1Bitmap1 *)m_pbitmap1Map,
               (unsigned int)sizeu2.width,
               (unsigned int)sizeu2.height,
               (unsigned int)stagingSize.width,
               (unsigned int)stagingSize.height,
               (unsigned int)stagingOptions,
               (unsigned int)stagingPixelFormat.format,
               (unsigned int)stagingPixelFormat.alphaMode);

         }

      }

      return m_pbitmap1Map;

   }


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

      draw2d_direct2d::graphics * pgraphics2d = dynamic_cast < ::draw2d_direct2d::graphics * > (pgraphics);

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

      draw2d_direct2d::graphics * pgraphics2d = dynamic_cast < ::draw2d_direct2d::graphics * > (pgraphics);

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


} // namespace draw2d_direct2d
