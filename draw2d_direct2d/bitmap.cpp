#include "framework.h"
#include "bitmap.h"
#include "graphics.h"
#include "aura/graphics/draw2d/lock.h"
#include "aura/graphics/draw2d/device_lock.h"


namespace draw2d_direct2d
{


   bitmap::bitmap()
   {


   }


   bitmap::~bitmap()
   {

      destroy();

   }


   void bitmap::CreateBitmap(::draw2d::graphics* pgraphics, const ::size_i32& sizeParam, ::u32 nPlanes, ::u32 nBitcount, const void * lpBits, int stride)
   {

      //::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      if (m_pbitmap != nullptr)
      {

         destroy();

      }

      D2D1_SIZE_U size;

      size.width = sizeParam.cx();
      size.height = sizeParam.cy();

      D2D1_BITMAP_PROPERTIES props;

      props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;

      draw2d_direct2d::graphics * pgraphics2d = dynamic_cast < ::draw2d_direct2d::graphics * > (pgraphics);

      pgraphics2d->m_prendertarget->GetDpi(&props.dpiX, &props.dpiY); // Thank you https://repo.anl-external.org/repos/BlueTBB/tbb41_20130314oss/examples/common/gui/d2dvideo.cpp

      //props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
      //props.colorContext = nullptr;

      //if(ppdata != nullptr)
      {
         // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(::color32_t), props, &m_pbitmap);
      }
      //else
      //{
      HRESULT hr = ((ID2D1DeviceContext *)pgraphics->get_os_data())->CreateBitmap(size, lpBits, stride, props, &m_pbitmap);

      //}

      if (FAILED(hr))
      {

         throw ::exception(error_failed);

      }

      //zero(m_map);
      //    m_pbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);
      //
      //if(ppdata != nullptr)
      // *ppdata = (::color::color *) m_map.bits;
      m_osdata[0] = m_pbitmap;

      //return true;

   }

   
   bool bitmap::CreateBitmapIndirect(::draw2d::graphics* pgraphics, LPBITMAP lpBitmap)
   {
      
      return false;

   }


   void bitmap::create_bitmap(::draw2d::graphics* pgraphics, const ::size_i32 & size, void **ppdata, int * pstride)
   {

      //::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      if (m_pbitmap != nullptr)
      {

         destroy();

      }

      D2D1_SIZE_U usize;

      usize.width = abs(size.cx());

      usize.height = abs(size.cy());

      D2D1_BITMAP_PROPERTIES1 props = {};

      props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

      props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;

      draw2d_direct2d::graphics * pgraphics2d = dynamic_cast < ::draw2d_direct2d::graphics * > (pgraphics);

      if (!::is_set(pgraphics2d))
      {

         throw ::exception(error_failed);

      }

      auto prendertarget = pgraphics2d->m_prendertarget;

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

      auto pcolorref = (color32_t *)*ppdata;

      auto iScan = *pstride;

      HRESULT hrResultCopyBitmap = S_OK;

      if (pcolorref && iScan > 0)
      {

         D2D1_RECT_U rectangleDst = {};

         rectangleDst.right = size.cx();

         rectangleDst.bottom = size.cy();

         hrResultCopyBitmap = m_pbitmap->CopyFromMemory(&rectangleDst, pcolorref, iScan);

      }

      m_size = size;

      m_osdata[0] = m_pbitmap;

      m_osdata[1] = m_pbitmap1;

      //return true;

   }


   void bitmap::CreateDIBitmap(::draw2d::graphics* pgraphics, int cx, int cy, u32 flInit, const void *pjBits, ::u32 iUsage)
   {

      //return false;

   }


   u32 bitmap::SetBitmapBits(u32 dwCount, const void * lpBits)
   {

      //return ::SetBitmapBits((HBITMAP)get_handle(), dwCount, lpBits);
      return 0;

   }
   u32 bitmap::GetBitmapBits(u32 dwCount, LPVOID lpBits) const
   {
      //return ::GetBitmapBits((HBITMAP)get_handle(), dwCount, lpBits);
      return 0;
   }
   bool bitmap::LoadBitmap(const ::string & lpszResourceName)
   {
      //   return Attach(::LoadBitmap(::aura::FindResourceHandle(
      //lpszResourceName, RT_BITMAP), lpszResourceName));
      return false;

   }

   //size_i32 bitmap::SetBitmapDimension(int nWidth, int nHeight)
   //{


   //   throw ::interface_only();

   //   return ::size_i32(0, 0);

   //}

   size_i32 bitmap::GetBitmapDimension() const
   {

      if (!m_pbitmap)
      {

         return ::size_i32(0, 0);

      }

      D2D1_SIZE_U size = m_pbitmap->GetPixelSize();

      return ::size_i32(size.width, size.height);

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


   void bitmap::CreateCompatibleBitmap(::draw2d::graphics* pgraphics, int nWidth, int nHeight)
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

      pgraphics2d->m_prendertarget->GetDpi(&props.dpiX, &props.dpiY); // Thanks again and a third time https://repo.anl-external.org/repos/BlueTBB/tbb41_20130314oss/examples/common/gui/d2dvideo.cpp      props.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

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


   void bitmap::CreateDiscardableBitmap(::draw2d::graphics* pgraphics, int nWidth, int nHeight)
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

      pgraphics2d->m_prendertarget->GetDpi(&props.dpiX, &props.dpiY); // One more time, Thank you very much https://repo.anl-external.org/repos/BlueTBB/tbb41_20130314oss/examples/common/gui/d2dvideo.cpp      props.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

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
