#include "framework.h"
#include "bitmap.h"
#include "graphics.h"
#include "aura/graphics/draw2d/lock.h"
#include "aura/graphics/draw2d/device_lock.h"


namespace draw2d_directx11
{


   bitmap::bitmap()
   {


   }


   bitmap::~bitmap()
   {

      destroy();

   }


   void bitmap::CreateBitmap(::draw2d::graphics* pgraphics, const ::i32_size& sizeParam, ::u32 nPlanes, ::u32 nBitcount, const void * lpBits, ::i32 stride)
   {

      //::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      //if (m_pbitmap != nullptr)
      //{

      //   destroy();

      //}

      //D2D1_SIZE_U size;

      //size.width = sizeParam.cx;
      //size.height = sizeParam.cy;

      //D2D1_BITMAP_PROPERTIES props;

      //props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      //props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;

      //draw2d_directx11::graphics * pgraphics2d = dynamic_cast < ::draw2d_directx11::graphics * > (pgraphics);

      //pgraphics2d->m_pd2d1rendertarget->GetDpi(&props.dpiX, &props.dpiY); // Thank you https://repo.anl-external.org/repos/BlueTBB/tbb41_20130314oss/examples/common/gui/d2dvideo.cpp

      ////props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
      ////props.colorContext = nullptr;

      ////if(ppdata != nullptr)
      //{
      //   // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(::color32_t), props, &m_pbitmap);
      //}
      ////else
      ////{
      //HRESULT hr = ((ID2D1DeviceContext *)pgraphics->get_os_data())->CreateBitmap(size, lpBits, stride, props, &m_pbitmap);

      ////}

      //if (FAILED(hr))
      //{

      //   throw ::exception(error_failed);

      //}

      //zero(m_map);
      //    m_pbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);
      //
      //if(ppdata != nullptr)
      // *ppdata = (::color::color *) m_map.bits;
      //m_osdata[0] = m_pbitmap;

      //return true;

   }

   
   bool bitmap::CreateBitmapIndirect(::draw2d::graphics* pgraphics, LPBITMAP lpBitmap)
   {
      
      return false;

   }


   void bitmap::create_bitmap(::draw2d::graphics* pgraphics, const ::i32_size & size, ::image32_t **ppimage32, const ::image32_t * pimage32,  ::i32 * piScan)
   {

      //::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);
      m_memory.set_size(size.area() * 4);
      m_size = size;
      m_iStride = size.cx * 4;
      //if (m_pbitmap != nullptr)
      //{

      //   destroy();

      //}

      //D2D1_SIZE_U usize;

      //usize.width = abs(size.cx);

      //usize.height = abs(size.cy);

      //D2D1_BITMAP_PROPERTIES1 props = {};

      //props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

      //props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;

      //draw2d_directx11::graphics * pgraphics2d = dynamic_cast < ::draw2d_directx11::graphics * > (pgraphics);

      //if (!::is_set(pgraphics2d))
      //{

      //   throw ::exception(error_failed);

      //}

      //auto prendertarget = pgraphics2d->m_pd2d1rendertarget;

      //if (!prendertarget)
      //{

      //   throw ::exception(error_failed);

      //}

      //prendertarget->GetDpi(&props.dpiX, &props.dpiY);

      //props.colorContext = nullptr;

      //props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;

      //HRESULT hr = ((ID2D1DeviceContext *)pgraphics->get_os_data())->CreateBitmap(usize, nullptr, 0, props, &m_pbitmap1);

      //if (FAILED(hr) || m_pbitmap1 == nullptr)
      //{

      //   throw ::exception(error_failed);

      //}

      //if (FAILED(m_pbitmap1.as(m_pbitmap)))
      //{

      //   throw ::exception(error_failed);

      //}

      //auto pimage32 = (color32_t *)*ppdata;

      //auto iScan = *pstride;

      //HRESULT hrResultCopyBitmap = S_OK;

      //if (pimage32 && iScan > 0)
      //{

      //   D2D1_RECT_U rectangleDst = {};

      //   rectangleDst.right = size.cx;

      //   rectangleDst.bottom = size.cy;

      //   hrResultCopyBitmap = m_pbitmap->CopyFromMemory(&rectangleDst, pimage32, iScan);

      //}

      //m_size = size;

      //m_osdata[0] = m_pbitmap;

      //m_osdata[1] = m_pbitmap1;

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

      //if (!m_pbitmap)
      //{

      //   return ::i32_size(0, 0);

      //}

      //D2D1_SIZE_U size = m_pbitmap->GetPixelSize();

      //return ::i32_size(size.width, size.height);
      return {};

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

      //if (m_pbitmap != nullptr)
      //{

      //   destroy();

      //}

      //D2D1_SIZE_U size;

      //size.width = nWidth;
      //size.height = nHeight;

      //D2D1_BITMAP_PROPERTIES1 props;

      //props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      //props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

      //draw2d_directx11::graphics * pgraphics2d = dynamic_cast < ::draw2d_directx11::graphics * > (pgraphics);

      //pgraphics2d->m_pd2d1rendertarget->GetDpi(&props.dpiX, &props.dpiY); // Thanks again and a third time https://repo.anl-external.org/repos/BlueTBB/tbb41_20130314oss/examples/common/gui/d2dvideo.cpp      props.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

      //props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CPU_READ;

      ////if(ppdata != nullptr)
      //{
      //   // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(::color32_t), props, &m_pbitmap);
      //}
      ////else
      ////      ID2D1Bitmap1 * pbitmap1;
      //{
      //   ((ID2D1DeviceContext *)pgraphics->get_os_data())->CreateBitmap(size, nullptr, size.width * sizeof(::color32_t), props, &m_pbitmap1);
      //}
      //m_pbitmap = m_pbitmap1;

      //m_osdata[0] = m_pbitmap;

      //m_osdata[1] = m_pbitmap1;

      //return true;

   }


   void bitmap::CreateDiscardableBitmap(::draw2d::graphics* pgraphics, ::i32 nWidth, ::i32 nHeight)
   {

      //::draw2d::lock draw2dlock;

      ::draw2d::device_lock devicelock(this);

      //if (m_pbitmap != nullptr)
      //{

      //   destroy();

      //}

      //D2D1_SIZE_U size;

      //size.width = nWidth;
      //size.height = nHeight;

      //D2D1_BITMAP_PROPERTIES1 props;

      //props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      //props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

      //draw2d_directx11::graphics * pgraphics2d = dynamic_cast < ::draw2d_directx11::graphics * > (pgraphics);

      //pgraphics2d->m_pd2d1rendertarget->GetDpi(&props.dpiX, &props.dpiY); // One more time, Thank you very much https://repo.anl-external.org/repos/BlueTBB/tbb41_20130314oss/examples/common/gui/d2dvideo.cpp      props.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

      //props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CPU_READ;

      ////if(ppdata != nullptr)
      //{
      //   // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(::color32_t), props, &m_pbitmap);
      //}
      ////else
      //{
      //   ((ID2D1DeviceContext *)pgraphics->get_os_data())->CreateBitmap(size, nullptr, size.width * sizeof(::color32_t), &props, &m_pbitmap1);
      //}
      //m_pbitmap = m_pbitmap1;

      //m_osdata[0] = m_pbitmap;

      //m_osdata[1] = m_pbitmap1;

      ////m_pbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);

      ////if(ppdata != nullptr)
      //// *ppdata = (::color::color *) m_map.bits;


      ////return true;

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

      //if (m_pbitmap != nullptr)
      //{

      //   destroy();

      //}

      //m_pbitmap = (ID2D1Bitmap *)hbitmap;

      //m_pbitmap.as(m_pbitmap1);

      //m_osdata[0] = m_pbitmap;

      //m_osdata[1] = m_pbitmap1;

      ////return true;

   }


   void * bitmap::detach()
   {

      clear_os_data();

      //m_pbitmap1 = nullptr;

      //return m_pbitmap.detach();

      return nullptr;

   }


   void bitmap::destroy()
   {

      destroy_os_data();

      ::draw2d::bitmap::destroy();

   }


   void bitmap::destroy_os_data()
   {

      //m_pbitmap = nullptr;

      //m_pbitmap1 = nullptr;

      object::destroy_os_data();

   }


} // namespace draw2d_directx11
