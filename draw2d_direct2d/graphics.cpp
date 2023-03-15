#include "framework.h"
#include "graphics.h"
#include "bitmap.h"
#include "path.h"
#include "pen.h"
#include "brush.h"
#include "region.h"
#include "direct2d/direct2d.h"
#include "CustomRenderer.h"
#include "acme/platform/scoped_restore.h"
#include "aura/windowing/windowing.h"
#include "aura/windowing/display.h"
#include "aura/user/user/user.h"
#include "aura/graphics/draw2d/lock.h"
#include "aura/graphics/draw2d/region.h"
#include "aura/graphics/draw2d/device_lock.h"
#include "aura/graphics/image/context_image.h"
#include "aura/graphics/image/drawing.h"
#include "aura/graphics/image/frame_array.h"
#include "acme/primitive/geometry2d/_enhanced.h"
#include "acme/primitive/geometry2d/_collection_enhanced.h"
#include "acme/primitive/geometry2d/_defer_shape.h"
#include "aura/platform/session.h"
#include <math.h>


void dpi_initialize(ID2D1Factory* pFactory);


#define d2d1_fax_options D2D1_FACTORY_OPTIONS // fax of merde
#define multi_threaded D2D1_FACTORY_TYPE_MULTI_THREADED // ???? muliple performance multi thread hidden option there exists cost uses?


//#if defined(WINDOWS_DESKTOP
//
//CLASS_DECL_AURA void directx_debug();
//
//#endif


namespace draw2d_direct2d
{


   //graphics::state::state()
   //{

   //}


   //graphics::state::~state()
   //{

   //}


   graphics::graphics()
   {

      m_iLayerCount = 0;

      m_ealphamodeDevice = ::draw2d::e_alpha_mode_none;

      clear_os_data();

      m_pthis = this;

      defer_create_synchronization();

      m_bSaveClip = false;

      m_hdcAttach = nullptr;

      m_iType     = 0;

      m_interpolationmode = D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;

      m_bitmapinterpolationmode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;

   }


   graphics::~graphics()
   {

      destroy();

   }


   //void graphics::assert_ok() const
   //{

   //   object::assert_ok();

   //}


   //void graphics::dump(dump_context & dumpcontext) const
   //{

   //   object::dump(dumpcontext);

   //   ////dumpcontext << "get_handle1() = " << (::iptr) get_handle1();
   //   ////dumpcontext << "\nm_hAttribDC = " << (::iptr) get_handle2();
   //   //dumpcontext << "\nm_bPrinting = " << m_bPrinting;

   //   //dumpcontext << "\n";

   //}


   //void graphics::IsPrinting()
   //{

   //   return m_bPrinting;

   //}


   //void graphics::CreateDC(const ::string & lpszDriverName, const ::string & lpszDeviceName, const ::string & lpszOutput, const void * lpInitData)
   //{
   //   throw ::exception(todo);
   //   //return Attach(::CreateDC(lpszDriverName, lpszDeviceName, lpszOutput, (const DEVMODE*)lpInitData));
   //}


   //void graphics::CreateIC(const ::string & lpszDriverName, const ::string & lpszDeviceName, const ::string & lpszOutput, const void * lpInitData)
   //{

   //   throw ::exception(todo);

   //}


   void graphics::CreateCompatibleDC(::draw2d::graphics * pgraphics)
   {

      //::draw2d::lock draw2dlock;

      // ::draw2d::device_lock devicelock(this);

      if (m_iType != 0)
      {

         destroy();

      }

      /*::direct2d::direct2d() = __new(::draw2d_direct2d::plugin);

      ::direct2d::direct2d()->initialize();*/

      HRESULT hr;

      comptr<ID2D1DeviceContext> pdevicecontextTemplate;

      if (FAILED(hr = ::direct2d::direct2d()->m_pd2device->CreateDeviceContext(
                      //D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
                      D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
                      &pdevicecontextTemplate)))
      {

         WARNING("graphics::CreateCompatibleDC, CreateDeviceContext (1) " << hresult_text(hr));

         //return false;

         throw ::exception(error_failed);

      }

      comptr<ID2D1RenderTarget> prendertargetTemplate;

      auto psession = acmesession();

      auto paurasession = psession->m_paurasession;

      auto puser = paurasession->user();

      auto pwindowing = puser->windowing();

      auto pdisplay = pwindowing->display();

      auto dpi = pdisplay->get_dpi();

      if (dpi <= 0.0)
      {

         ASSERT(false);

         throw ::exception(error_failed);

      }

      pdevicecontextTemplate->SetDpi(dpi, dpi);

      if (FAILED(hr = pdevicecontextTemplate->QueryInterface(IID_ID2D1RenderTarget,(void **)&prendertargetTemplate)))
      {

         WARNING("graphics::CreateCompatibleDC, QueryInterface (2) " << hresult_text(hr));

         throw ::exception(error_failed);

      }

      D2D1_SIZE_U sizeu = D2D1::SizeU(256, 256);

      D2D1_PIXEL_FORMAT pixelformat;

      pixelformat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

      pixelformat.format = DXGI_FORMAT_B8G8R8A8_UNORM;

      if (FAILED(hr = prendertargetTemplate->CreateCompatibleRenderTarget(
                      nullptr,
                      &sizeu,
                      &pixelformat,
         D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE,
                      &m_pbitmaprendertarget)))
      {

         WARNING("graphics::CreateCompatibleDC, CreateCompatibleRenderTarget (3) " << hresult_text(hr));

         throw ::exception(error_failed);

      }

      hr = m_pbitmaprendertarget.as(m_prendertarget);

      if(FAILED(hr))
      {

         m_pbitmaprendertarget = nullptr;

         throw ::exception(error_failed);

      }

      hr = m_pbitmaprendertarget.as(m_pdevicecontext);

      m_pdevicecontext.as(m_pdevicecontext1);

      if(FAILED(hr))
      {

         m_prendertarget = nullptr;

         m_pbitmaprendertarget = nullptr;

         throw ::exception(error_failed);

      }

      __defer_construct(m_pbitmap);

      ID2D1Bitmap * pbitmap;

      hr = m_pbitmaprendertarget->GetBitmap(&pbitmap);

      if(FAILED(hr))
      {

         m_pbitmaprendertarget = nullptr;

         throw ::exception(error_failed);

      }

      m_pbitmap->attach(pbitmap);

      m_iType = 3;

      m_osdata[data_device_context] = m_pdevicecontext;

      m_osdata[data_render_target] = m_prendertarget;

      //return true;

   }


   //int graphics::GetDeviceCaps(double dIndex)
   //{

   //   throw ::exception(todo);

   //}


   point_f64 graphics::GetBrushOrg()
   {
      //throw ::exception(todo);
      //ASSERT(get_handle1() != nullptr);
      //::point_f64 point;
      //VERIFY(::GetBrushOrgEx(get_handle1(), &point));
      //return point;

      return ::point_f64();

   }

   
   point_f64 graphics::SetBrushOrg(double x, double y)
   {
      //throw ::exception(todo);
      //ASSERT(get_handle1() != nullptr);
      //::point_f64 point;
      //VERIFY(::SetBrushOrgEx(get_handle1(), x, y, &point));
      //return point;

      return ::point_f64();

   }

   
   point_f64 graphics::SetBrushOrg(const ::point_f64 & point)
   {

      //throw ::exception(todo);
      //ASSERT(get_handle1() != nullptr);
      //VERIFY(::SetBrushOrgEx(get_handle1(), point.x, point.y, &point));
      //return point;

      return ::point_f64();

   }


   //int graphics::EnumObjects(double dObjectType, int (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData)
   //{
   //   throw ::exception(todo);
   //   //ASSERT(get_handle2() != nullptr);
   //   //return ::EnumObjects(get_handle2(), nObjectType, (GOBJENUMPROC)lpfn, lpData);
   //}

   
   bool graphics::_draw_blend(const ::image_drawing & imagedrawing)
   {

      //return ::draw2d::graphics::BitBltAlphaBlend(x, y, nWidth, nHeight, pgraphicsSrc, xSrc, ySrc);

      //if (m_pimageAlphaBlend->is_set())
      //{
      //   //return true;

      //   // Reference implementation

      //   ::rectangle_i32 rectangleAlphaBlend(m_pointAlphaBlend, m_pimageAlphaBlend->size());

      //   if (x < 0)
      //   {

      //      xSrc -= x;

      //      nWidth += x;

      //      x = 0;

      //   }

      //   if (y < 0)
      //   {

      //      ySrc -= y;

      //      nHeight += y;

      //      y = 0;

      //   }

      //   point_i32 ::point_f64(x, y);

      //   ::size_f64 size(nWidth, nHeight);

      //   ::rectangle_i32 rectangleBlend(point, ::size_f64);

      //   ::rectangle_i32 rectangleIntersect;

      //   if (rectangleIntersect.intersect(rectangleAlphaBlend, rectangleBlend))
      //   {

      //      ::image_pointer pimage1;

      //      pimage1 = create_image(::size_f64);

      //      ::rectangle_i32 rectangleDib1(::point_f64(), pimage1->m_size);

      //      pimage1->get_graphics()->set_alpha_mode(::draw2d::e_alpha_mode_set);

      //      pimage1->get_graphics()->fill_rectangle(rectangleDib1, argb(0, 0, 0, 0));

      //      if (!pimage1->from(::point_f64(), pgraphicsSrc, ::point_f64(xSrc, ySrc), ::size_f64))
      //      {

      //         return false;

      //      }

      //      ::image_pointer pimage2;

      //      pimage2 = create_image(::size_f64);

      //      pimage2->get_graphics()->set_alpha_mode(::draw2d::e_alpha_mode_set);

      //      pimage2->get_graphics()->fill_rectangle(rectangleDib1, argb(255, 0, 0, 0));

      //      if (!pimage2->from(::point_f64(), m_pimageAlphaBlend, point_i32 - m_pointAlphaBlend, rectangleIntersect.size()))
      //      {

      //         return false;

      //      }

      //      ::pointer<::draw2d_direct2d::graphics>pgraphicsDib1 = pimage1->get_graphics();

      //      ::pointer<::draw2d_direct2d::graphics>pgraphicsDib2 = pimage2->get_graphics();

      //      HRESULT hr = ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->EndDraw();

      //      pgraphicsDib2->m_pdevicecontext->DrawImage(
      //      (ID2D1Bitmap *)pgraphicsDib1->get_current_bitmap()->get_os_data(),
      //      D2D1::Point2F(0.f, 0.f),
      //      d2d1::rectangle_f32(rectangleDib1),
      //      D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
      //      D2D1_COMPOSITE_MODE_SOURCE_OVER);

      //      if (SUCCEEDED(hr))
      //      {

      //         ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->BeginDraw();

      //      }

      //      set_alpha_mode(::draw2d::e_alpha_mode_blend);

      //      BitBltRaw(x, y, nWidth, nHeight, pimage2->get_graphics(), 0, 0);

      //      return true;

      //   }

      //}

      //return false;

      if (m_pimageAlphaBlend->is_set())
      {

         auto rectangleTarget = imagedrawing.m_rectangleTarget;
         auto rectangleSource = imagedrawing.source_rectangle();


         auto x = rectangleTarget.left;
         auto y = rectangleTarget.top;
         auto xSrc = rectangleSource.left;
         auto ySrc = rectangleSource.top;
         auto nWidth = rectangleTarget.width();
         auto nHeight = rectangleTarget.height();

         if (x < 0)
         {

            xSrc -= x;

            nWidth += x;

            x = 0;

         }

         if (y < 0)
         {

            ySrc -= y;

            nHeight += y;

            y = 0;

         }


         ::rectangle_f64 rectangleIntersect(m_pointAlphaBlend, m_pimageAlphaBlend->size());

         ::rectangle_f64 rectangleBlt(::point_f64(x, y), ::size_f64(nWidth, nHeight));

         if (rectangleIntersect.intersect(rectangleIntersect, rectangleBlt))
         {

            //if (m_pointAlphaBlend.x < 0)
            //{

            //   xSrc += -m_pointAlphaBlend.x;

            //}
            //if (m_pointAlphaBlend.y < 0)
            //{

            //   ySrc += -m_pointAlphaBlend.y;

            //}

            // The following commented out code does not work well when there is clipping
            // and some calculations are not precise
            //if (m_pimage->is_set() && pgraphicsSrc->m_pimage->is_set())
            //{

            //   ::point_f64 pointOff = get_origin();

            //   x += pointOff.x;

            //   y += pointOff.y;

            //   return m_pimage->blend(::point_f64(x, y), pgraphicsSrc->m_pimage, ::point_f64(xSrc, ySrc), m_pimageAlphaBlend, point_i32(m_pointAlphaBlend.x - x, m_pointAlphaBlend.y - y), rectangleBlt.size());

            //}
            //else
            //{

               auto pimage1 = m_pcontext->m_pauracontext->create_image(rectangleBlt.size());

               pimage1->get_graphics()->set_alpha_mode(::draw2d::e_alpha_mode_set);

               auto pimage = imagedrawing.image();

               pimage1->_draw_raw(::rectangle_f64(rectangleBlt.size()), pimage, ::point_f64(xSrc, ySrc));

               //if (!pimage1->_draw_raw(::rectangle_f64(rectangleBlt.size()), pimage , ::point_f64(xSrc, ySrc)))
               //{

               //   return false;

               //}

               pimage1->blend2(::point_f64(), m_pimageAlphaBlend, point_f64(x - m_pointAlphaBlend.x, y - m_pointAlphaBlend.y), rectangleBlt.size(), 255);

               image_drawing_options imagedrawingoptions;

               _draw_raw(rectangleTarget, pimage1, imagedrawingoptions, ::point_f64());

            //}

            return true;

         }

      }

      return false;

   }


   bool graphics::TextOutAlphaBlend(double x, double y, const ::scoped_string & scopedstr)
   {

      if (scopedstr.is_empty())
      {

         throw ::exception(error_invalid_empty_argument);

      }

      ASSERT(m_pimageAlphaBlend->is_ok());
      
      // "Reference" implementation for TextOutAlphaBlend

      auto size = get_text_extent(scopedstr);

      ::rectangle_f64 rectangleAlphaBlend(m_pointAlphaBlend, m_pimageAlphaBlend->size());

      ::point_f64 point(x, y);

      ::rectangle_f64 rectangleText(point, size);

      ::rectangle_f64 rectangleIntersect;

      if (rectangleIntersect.intersect(rectangleAlphaBlend, rectangleText))
      {

         //::draw2d::lock draw2dlock;

         // ::draw2d::device_lock devicelock(this);

         auto pimage1 = m_pcontext->m_pauracontext->create_image(size);

         ::rectangle_f64 rectangleDib1(::point_f64(), size);

         pimage1->get_graphics()->set(get_current_font());

         pimage1->get_graphics()->set(get_current_brush());

         pimage1->get_graphics()->set_alpha_mode(::draw2d::e_alpha_mode_set);

         pimage1->get_graphics()->fill_rectangle(rectangleDib1, argb(0, 0, 0, 0));

         pimage1->get_graphics()->text_out(0, 0, scopedstr);

         ::image_pointer pimage2;

         pimage2->create(size);

         pimage2->get_graphics()->set_alpha_mode(::draw2d::e_alpha_mode_set);

         pimage2->get_graphics()->fill_rectangle(rectangleDib1, argb(255, 0, 0, 0));

         pimage2->_draw_raw(rectangleIntersect.size(), m_pimageAlphaBlend, ::point_f64(point - m_pointAlphaBlend));

         ::pointer<::draw2d_direct2d::graphics>pgraphicsDib1 = pimage1->get_graphics();

         ::pointer<::draw2d_direct2d::graphics>pgraphicsDib2 = pimage2->get_graphics();

         pimage2->unmap();

         HRESULT hrFlush = ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->Flush();

         HRESULT hrEndDraw = ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->EndDraw();

         pgraphicsDib1->m_pdevicecontext->DrawImage(
         (ID2D1Bitmap *)pgraphicsDib2->get_current_bitmap()->m_osdata[0],
         D2D1::Point2F(0.f, 0.f),
         d2d1::rectangle_f32(rectangleDib1),
         D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
         D2D1_COMPOSITE_MODE_DESTINATION_IN);

         if (SUCCEEDED(hrEndDraw))
         {

            ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->BeginDraw();

         }

         set_alpha_mode(::draw2d::e_alpha_mode_blend);

         image_source imagesource(pimage1);

         ::rectangle_f64 rectangleTarget(::point_f64(x, y), pimage1->size());

         image_drawing_options imagedrawingoptions(rectangleTarget);

         image_drawing imagedrawing(imagedrawingoptions, imagesource);

         draw(imagedrawing);

         return true;

      }

      return false;

   }


   void graphics::set(::draw2d::bitmap* pbitmapParam)
   {

      //::draw2d::lock draw2dlock;

      // ::draw2d::device_lock devicelock(this);

      ::pointer<::draw2d_direct2d::bitmap>pbitmap = pbitmapParam;

      if (::is_null(pbitmapParam))
      {

         throw ::exception(error_invalid_empty_argument);

      }


      //if(m_prendertarget == nullptr)
      //{

        // CreateCompatibleDC(nullptr);

      //}

      //bool image::realize(::draw2d::graphics *) const
      //{

         //if (is_realized())
         //{

         //   unrealize();

         //}

         //if (is_realized())
         //{

         //   return false;

         //}

         //if (m_pbitmap.is_null()
         //   || m_pbitmapMap.is_null()
         //   || m_pgraphics.is_null()
         //   || m_pgraphicsMap.is_null())
         //{

         //   return false;

         //}

         //::pointer<::draw2d_direct2d::graphics>pgraphicsMap = m_pgraphicsMap;

         //::pointer<::draw2d_direct2d::graphics>pgraphics = m_pgraphics;

         //::pointer<::draw2d_direct2d::bitmap>pbitmap = m_pbitmap;

      //   m_pbitmaprendertarget = nullptr;

      //   m_iType = 11;

      //   ::size_i32 size = pbitmap->GetBitmapDimension();

      //   D2D1_SIZE_U sizeu = D2D1::SizeU(size.cx, size.cy);

      //   D2D1_PIXEL_FORMAT pixelformat;

      //   pixelformat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

      //   pixelformat.format = DXGI_FORMAT_B8G8R8A8_UNORM;

      //   auto & prendertarget = m_prendertarget;

      //   HRESULT hr = prendertarget->CreateCompatibleRenderTarget(nullptr, &sizeu, &pixelformat, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE, &m_pbitmaprendertarget);

      //   if (m_pbitmaprendertarget == nullptr)
      //   {

      //      return false;

      //   }

      //   if (FAILED(m_pbitmaprendertarget.as(m_prendertarget)))
      //   {

      //      m_pbitmaprendertarget = nullptr;

      //      return false;

      //   }

      //   if (FAILED(m_pbitmaprendertarget.as(m_pdevicecontext)))
      //   {

      //      m_pbitmaprendertarget = nullptr;

      //      m_prendertarget = nullptr;

      //      return false;

      //   }

      //   comptr<ID2D1Bitmap> pd2d1bitmap;

      //   m_pbitmaprendertarget->GetBitmap(&pd2d1bitmap);

      //   if (pbitmap->m_pbitmap == nullptr)
      //   {

      //      return false;

      //   }

      //   //pgraphics->m_pplugin = pgraphicsMap->m_pplugin;

      //   pbitmap->m_pbitmap = pd2d1bitmap;

      //   pbitmap->m_pbitmap.as(pbitmap->m_pbitmap1);

      //   pbitmap->m_osdata[0] = pbitmap->m_pbitmap;

      //   pbitmap->m_osdata[1] = pbitmap->m_pbitmap1;

      //   m_osdata[0] = m_pdevicecontext;

      //   m_osdata[1] = m_prendertarget;

      //   //D2D1_POINT_2U p;

      //   //p.x = 0;
      //   //p.y = 0;

      //   //D2D1_RECT_U srcRect;

      //   //srcRect.left = 0;
      //   //srcRect.right = this->width();
      //   //srcRect.top = 0;
      //   //srcRect.bottom = this->height();

      ////   return true;

      ////}

         m_pdevicecontext->SetTarget(pbitmap->m_pbitmap);

      m_pbitmap = pbitmap;

      m_iType = 3;

      //return ::success;

   }


   //::color::color graphics::GetNearestColor(const ::color::color & color)
   //{

   //   throw ::exception(todo);

   //}


   //::u32 graphics::RealizePalette()
   //{

   //   throw ::exception(todo);
   //   
   //}


   //void graphics::UpdateColors()
   //{

   //   throw ::exception(todo);
   //   //::UpdateColors(get_handle1());

   //}


   //int graphics::GetPolyFillMode()
   //{
   //   throw ::exception(todo);
   //   //return ::GetPolyFillMode(get_handle2());
   //}

   //int graphics::GetROP2()
   //{
   //   throw ::exception(todo);
   //   //return ::GetROP2(get_handle2());
   //}

   //int graphics::GetStretchBltMode()
   //{
   //   throw ::exception(todo);
   //   //return ::GetStretchBltMode(get_handle2());
   //}

   //int graphics::GetMapMode()
   //{
   //   throw ::exception(todo);
   //   //return ::GetMapMode(get_handle2());
   //}

   //int graphics::GetGraphicsMode()
   //{
   //   throw ::exception(todo);
   //   //return ::GetGraphicsMode(get_handle2());
   //}


   //void graphics::GetWorldTransform(XFORM* pXform)
   //{

   //   throw ::exception(todo);

   //}


   void graphics::_get(::draw2d::matrix & matrix)
   {

      D2D1::Matrix3x2F m;

      m_prendertarget->GetTransform(&m);

      matrix = ::draw2d::matrix();

      matrix.a1 = m._11;
      matrix.a2 = m._12;
      matrix.b1 = m._21;
      matrix.b2 = m._22;
      matrix.c1 = m._31;
      matrix.c2 = m._32;

      //return true;

   }


   void graphics::_set(const ::draw2d::matrix & matrix)
   {
      
      if (!m_prendertarget)
      {

         throw ::exception(error_null_pointer);

      }

      D2D1::Matrix3x2F m;

      m._11 = (FLOAT) matrix.a1;
      m._12 = (FLOAT) matrix.a2;
      m._21 = (FLOAT) matrix.b1;
      m._22 = (FLOAT) matrix.b2;
      m._31 = (FLOAT) matrix.c1;
      m._32 = (FLOAT) matrix.c2;

      m_prendertarget->SetTransform(&m);

      //return true;

   }


   point_f64 graphics::get_origin()
   {

      return ::draw2d::graphics::get_origin();

   }


   size_f64 graphics::get_extents()
   {

      throw ::exception(todo);

      return nullptr;

   }

   
   point_f64 graphics::GetWindowOrg()
   {

      throw ::exception(todo);

      return nullptr;

   }


   size_f64 graphics::GetWindowExt()
   {
      throw ::exception(todo);
      //::size_f64::size_f64;
      //::GetWindowExtEx(get_handle2(), &::size_f64);
      //return ::size_f64;
      return nullptr;
   }


   point_f64 graphics::set_origin(const ::point_f64 & point)
   {

      return ::draw2d::graphics::set_origin(point.x, point.y);

   }


   ::size_f64 graphics::set_extents(const ::size_f64 & size)
   {

      return ::draw2d::graphics::set_extents(size.cx, size.cy);

   }


   point_f64 graphics::SetWindowOrg(const ::point_f64 & point)
   {

      return SetWindowOrg(point.x, point.y);

   }


   ::size_f64 graphics::set_window_ext(const ::size_f64 & size)
   {
      
      return set_window_ext(size.cx, size.cy);

   }


   void graphics::DPtoLP(::point_f64 * ppoints, count nCount)
   {

      throw ::exception(todo);
      //::DPtoLP(get_handle2(), ppoints, nCount);

   }


   void graphics::DPtoLP(::rectangle_f64 * prectangle)
   {

      throw ::exception(todo);
      //::DPtoLP(get_handle2(), (::point_f64 *)rectangle, 2);

   }


   void graphics::LPtoDP(::point_f64 * ppoints, count nCount)
   {

      throw ::exception(todo);
      //::LPtoDP(get_handle2(), ppoints, nCount);

   }


   void graphics::LPtoDP(::rectangle_f64 * prectangle)
   {

      throw ::exception(todo);
      //::LPtoDP(get_handle2(), (::point_f64 *)rectangle, 2);

   }


   void graphics::fill_region(::draw2d::region* pRgn, ::draw2d::brush* pBrush)
   {

      throw ::exception(todo);
      //return ::FillRgn(get_handle1(), (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data()) != false;

      //return false;

   }


   void graphics::frame_region(::draw2d::region* pRgn, ::draw2d::brush* pBrush, double nWidth, double nHeight)
   {

      throw ::exception(todo);
      //return ::FrameRgn(get_handle1(), (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data(), nWidth, nHeight) != false;

      //return false;

   }


   void graphics::invert_region(::draw2d::region* pregion)
   {
      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::InvertRgn(get_handle1(), (HRGN)pRgn->get_os_data()) != false;

      //return false;

   }


   void graphics::paint_region(::draw2d::region* pregion)
   {

      throw ::exception(todo);
      //ASSERT(get_handle1() != nullptr);

      //return ::PaintRgn(get_handle1(), (HRGN)pRgn->get_os_data())  != false;

      //return false;

   }

   //void graphics::PtVisible(double x, double y)
   //{

   //   throw ::exception(todo);
   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::PtVisible(get_handle1(), x, y) != false;

   //}

   //void graphics::PtVisible(const ::point_f64 & point)
   //{

   //   ASSERT(get_handle1() != nullptr);

   //   return PtVisible(point.x, point.y);

   //} // call virtual


   //void graphics::rectVisible(const ::rectangle_i32 & rectangle)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::rectVisible(get_handle1(), rectangle) != false;

   //}


   point_f64 graphics::current_position()
   {

      throw ::exception(todo);
      //ASSERT(get_handle2() != nullptr);
      //::point_f64 point;
      //VERIFY(::GetCurrentPositionEx(get_handle2(), &point));
      //return point;

      return nullptr;

   }


   //void graphics::Arc(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
   //{

   //   auto ppath = __create < ::draw2d::path > ();

   //   double pi = 3.1415927f;

   //   ::rectangle_f64 rectangle(x1, y1, x2, y2);

   //   double centerx    = (x2 + x1) / 2.0;
   //   double centery    = (y2 + y1) / 2.0;

   //   double start      = atan2(y3 - centery, x3 - centerx) * 180.0 / pi;
   //   double end        = atan2(y4 - centery, x4 - centerx) * 180.0 / pi;

   //   //ppath->begin_figure(false, ::draw2d::e_fill_mode_winding);
   //   ppath->begin_figure();
   //   ppath->add_arc(rectangle, (int) start, (int) fmod(end + 360.0 - start, 360.0));
   //   //ppath->end_figure(false);
   //   //ppath->close_figure();

   //   return this->path(ppath);

   //}

   void graphics::arc(double x1, double y1, double w, double h, angle start, angle extends)
   {

      auto ppath = __create < ::draw2d::path > ();

      ::rectangle_i32 rectangle((::i32) x1, (::i32)y1, (::i32)(x1 + w), (::i32)(y1 + h));

      //ppath->begin_figure(false, ::draw2d::e_fill_mode_winding);
      ppath->begin_figure();
      ppath->add_arc(rectangle, start, extends);
      //ppath->end_figure(false);
      //ppath->close(false);

      return this->path(ppath);

   }


   void graphics::arc(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
   {

      auto ppath = __create < ::draw2d::path > ();

      double pi = 3.1415927f;

      ::rectangle_i32 rectangle((::i32)x1, (::i32)y1, (::i32)x2, (::i32)y2);

      double centerx = (x2 + x1) / 2.0;
      double centery = (y2 + y1) / 2.0;

      double start = atan2(y3 - centery, x3 - centerx) * 180.0 / pi;
      double end = atan2(y4 - centery, x4 - centerx) * 180.0 / pi;

      //ppath->begin_figure(false, ::draw2d::e_fill_mode_winding);
      ppath->begin_figure();
      ppath->add_arc(rectangle, start, end - start);
      //ppath->end_figure(false);

      return this->path(ppath);

   }


   void graphics::arc(const ::rectangle_f64 & rectangle, const ::point_f64 & pointStart, const ::point_f64 & pointEnd)
   {

      //ASSERT(get_handle1() != nullptr);

      arc(rectangle.left, rectangle.top, rectangle.right, rectangle.bottom, pointStart.x, pointStart.y, pointEnd.x, pointEnd.y);


   }


   void graphics::polyline(const ::point_f64* ppoints, count nCount)
   {

      auto ppath = __create < ::draw2d::path > ();

      //ppath->begin_figure(false, ::draw2d::e_fill_mode_winding);

      ppath->begin_figure();

      ppath->add_lines(ppoints, nCount);

      //ppath->end_figure(false);

      //ppath->end_figure();

      this->draw(ppath);

   }


   void graphics::frame_rectangle(const ::rectangle_f64 & rectangleParam, ::draw2d::brush* pbrush)
   {

      if (m_prendertarget == nullptr)
      {

         throw ::exception(error_null_pointer);

      }

      D2D1_RECT_F rectangle;

      copy(rectangle, rectangleParam);

      m_prendertarget->DrawRectangle(rectangle, pbrush->get_os_data < ID2D1Brush * > (this));

   }


   void graphics::invert_rectangle(const ::rectangle_f64 & rectangle)
   {

      throw ::exception(todo);

      //return false;

   }


   //void graphics::DrawIcon(double x, double y, ::draw2d::icon * picon)
   //{
   //   throw ::exception(todo);
   //   //ASSERT(get_handle1() != nullptr);

   //   //if(picon == nullptr)
   //   //   return false;

   //   //return ::DrawIcon(get_handle1(), x, y, picon->m_hicon) != false;

   //}

   //void graphics::DrawIcon(const ::point_f64 & point, ::draw2d::icon * picon)
   //{
   //   throw ::exception(todo);
   //   //ASSERT(get_handle1() != nullptr);

   //   //if(picon == nullptr)
   //   //   return false;

   //   //return ::DrawIcon(get_handle1(), point.x, point.y, picon->m_hicon) != false;

   //}

//   void graphics::draw(const ::rectangle_f64 & rectangleTarget, ::draw2d::icon * picon)
//   {
//
//#ifdef _UWP
//
//      //throw ::interface_only();
//      return false;
//
//#else
//
//      try
//      {
//
//         if(picon == nullptr)
//            return false;
//
//         if(m_prendertarget == nullptr)
//            return false;
//
//         bool bOk = false;
//
//         BITMAPINFO info;
//         ::color::color * pcolorref;
//
//         ZeroMemory(&info, sizeof (BITMAPINFO));
//
//         info.bmiHeader.biSize          = sizeof (BITMAPINFOHEADER);
//         info.bmiHeader.biWidth         = rectangleTarget.width();
//         info.bmiHeader.biHeight        = - rectangleTarget.height();
//         info.bmiHeader.biPlanes        = 1;
//         info.bmiHeader.biBitCount      = 32;
//         info.bmiHeader.biCompression   = BI_RGB;
//         info.bmiHeader.biSizeImage     = rectangleTarget.area() * 4;
//
//         HBITMAP hbitmap = ::CreateDIBSection(nullptr, &info, DIB_RGB_COLORS, (void **) &pcolorref, nullptr, 0);
//
//         HDC hdc = ::CreateCompatibleDC(nullptr);
//
//         HBITMAP hbitmapOld = (HBITMAP) ::SelectObject(hdc, hbitmap);
//
//         int istepIfAniCur = 0;
//         if(::DrawIconEx(hdc, 0, 0, (HICON) picon->m_picon, rectangleTarget.width(), rectangleTarget.height(), istepIfAniCur, nullptr, DI_IMAGE | DI_MASK))
//         {
//
//            ::SelectObject(hdc, hbitmapOld);
//
//            try
//            {
//
//               //Gdiplus::Bitmap b(cx, cy, cx * 4 , PixelFormat32bppARGB, (byte *) pcolorref);
//
//               ::draw2d::bitmap_pointer b(e_create);
//
//               b->CreateBitmap(this, rectangleTarget.size(), 1, 32, pcolorref, rectangleTarget.width() * sizeof(::color32_t));
//
//               D2D1_RECT_F rectangle_i32;
//
//               __copy(rectangle, rectangleTarget);
//               //rectangle.left   = (FLOAT) rectangle_i32;
//               //rectangle.top    = (FLOAT) y;
//               //rectangle.right  = rectangle.left    + cx;
//               //rectangle.bottom = rectangle.top     + cy;
//
//               m_prendertarget->DrawBitmap((ID2D1Bitmap *) b->get_os_data(), rectangle);
//
//            }
//            catch(...)
//            {
//            }
//
//         }
//
//         ::DeleteDC(hdc);
//
//         ::DeleteObject(hbitmap);
//
//         return bOk;
//
//      }
//      catch(...)
//      {
//      }
//
//      return false;
//
//#endif
//
//   }
//
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, HBITMAP hBitmap, ::u32 nFlags, HBRUSH hBrush)
//   {
//
//      throw ::exception(todo);
//      //ASSERT(get_handle1() != nullptr);
//      //return ::DrawState(get_handle1(), hBrush, nullptr, (LPARAM)hBitmap, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_BITMAP) != false;
//
//   }
//
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, ::draw2d::bitmap* pBitmap, ::u32 nFlags, ::draw2d::brush* pBrush)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//      //return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), nullptr, (LPARAM)pBitmap->get_os_data(), 0, point.x, point.y, size.cx, size.cy, nFlags|DST_BITMAP) != false;
//
//   }
//
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, HICON hIcon, ::u32 nFlags, HBRUSH hBrush)
//   {
//
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//      //return ::DrawState(get_handle1(), hBrush, nullptr, (LPARAM)hIcon, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_ICON) != false;
//
//   }
//
//
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, HICON hIcon, ::u32 nFlags, ::draw2d::brush* pBrush)
//   {
//
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//      //return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), nullptr, (LPARAM)hIcon, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_ICON) != false;
//
//   }
//
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, const ::string & lpszText, ::u32 nFlags, bool bPrefixText, double dTextLen, HBRUSH hBrush)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//      //return ::DrawState(get_handle1(), hBrush,  nullptr, (LPARAM)lpszText, (WPARAM)nTextLen, point.x, point.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)) != false;
//
//   }
//
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, const ::string & lpszText, ::u32 nFlags, bool bPrefixText, double dTextLen, ::draw2d::brush* pBrush)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//      //return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), nullptr, (LPARAM)lpszText, (WPARAM)nTextLen, point.x, point.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)) != false;
//
//   }
//
//#ifdef WINDOWS_DESKTOP
//
//   /*
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, DRAWSTATEPROC lpDrawProc, LPARAM lData, ::u32 nFlags, HBRUSH hBrush)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr); return ::DrawState(get_handle1(), hBrush, lpDrawProc, lData, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_COMPLEX) != false;
//
//   }
//
//   void graphics::DrawState(const ::point_f64 & point, const ::size_f64& ::size_f64, DRAWSTATEPROC lpDrawProc, LPARAM lData, ::u32 nFlags, ::draw2d::brush* pBrush)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr); return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), lpDrawProc, lData, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_COMPLEX) != false;
//
//   }
//   */
//
//#endif
//
//
   //void graphics::DrawEdge(::rectangle_f64 * prectangle, ::u32 nEdge, ::u32 nFlags)
   //{

   //   throw ::exception(todo);

   //   ASSERT(get_handle1() != nullptr);

   //   //return ::DrawEdge(get_handle1(), rectangle, nEdge, nFlags) != false;

   //}


   //void graphics::DrawFrameControl(::rectangle_f64 * prectangle, ::u32 nType, ::u32 nState)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr); return ::DrawFrameControl(get_handle1(), rectangle, nType, nState) != false;

   //}

   void graphics::Chord(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::Chord(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4) != false;

      //return false;

   }

   void graphics::Chord(const ::rectangle_f64 & rectangle, const ::point_f64 & pointStart, const ::point_f64 & pointEnd)
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::Chord(get_handle1(), rectangle.left, rectangle.top, rectangle.right, rectangle.bottom, pointStart.x, pointStart.y, pointEnd.x, pointEnd.y) != false;

      //return false;

   }


   void graphics::DrawFocusRect(const ::rectangle_f64 & rectangle)
   {

      throw ::exception(todo);

//      ASSERT(get_handle1() != nullptr);

   }


   void graphics::fill_ellipse(const ::rectangle_f64 & rectangle)
   {

      if (m_pbrush.is_null())
      {

         throw ::exception(error_null_pointer);

      }

      if (m_pbrush->m_ebrush == ::draw2d::e_brush_box_gradient)
      {

         //comptr < ID2D1PathGeometry1 > pgeometry;

         //HRESULT hr = ::direct2d::direct2d()->d2d1_factory1()->CreatePathGeometry(&pgeometry);

         //if (FAILED(hr))
         //{

         //   throw ::hresult_exception(hr);

         //}

         //comptr < ID2D1GeometrySink > psink;

         //pgeometry->Open(&psink)

         //if (FAILED(hr))
         //{

         //   throw ::hresult_exception(hr);

         //}

         //psink->SetFillMode(D2D1_FILL_MODE_WINDING);
         //psink->BeginFigure(D2D1::Point2F(20, 50), D2D1_FIGURE_BEGIN_FILLED);
         //psink->AddLine(D2D1::Point2F(130, 50));
         //psink->AddLine(D2D1::Point2F(20, 130));
         //psink->AddLine(D2D1::Point2F(80, 0));
         //psink->AddLine(D2D1::Point2F(130, 130));
         //psink->EndFigure(D2D1_FIGURE_END_CLOSED);

         //   hr = pSink->Close();
         //}

         //SafeRelease(&pSink);

      }

      auto pbrush = m_pbrush->get_os_data < ID2D1Brush * >(this);

      if (!pbrush)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      D2D1_ELLIPSE ellipse;

      ellipse.point.x = (float)(rectangle.right + rectangle.left) / 2.f;
      ellipse.point.y = (float)(rectangle.bottom + rectangle.top) / 2.f;
      ellipse.radiusX = (float)(rectangle.right - rectangle.left) / 2.f;
      ellipse.radiusY = (float)(rectangle.bottom - rectangle.top) / 2.f;

      defer_primitive_blend();

      m_pdevicecontext->FillEllipse(&ellipse, pbrush);

      //return true;

   }


   void graphics::draw_ellipse(const ::rectangle_f64 & rectangle)
   {

      if (m_ppen.is_null())
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      auto pbrush = m_ppen->get_os_data < ID2D1Brush * >(this);

      if (!pbrush)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      D2D1_ELLIPSE ellipse;

      ellipse.point.x = (float)(rectangle.right + rectangle.left) / 2.f;
      ellipse.point.y = (float)(rectangle.bottom + rectangle.top) / 2.f;
      ellipse.radiusX = (float)(rectangle.right - rectangle.left) / 2.f;
      ellipse.radiusY = (float)(rectangle.bottom - rectangle.top) / 2.f;

      defer_primitive_blend();

      m_pdevicecontext->DrawEllipse(&ellipse, pbrush,(FLOAT) m_ppen->m_dWidth);

      //return true;

   }


   void graphics::Pie(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
   {

      throw ::exception(todo);

      //return false;

   }


   void graphics::Pie(const ::rectangle_f64 & rectangle, const ::point_f64 & pointStart, const ::point_f64 & pointEnd)
   {

      throw ::exception(todo);

      //return false;

   }


   void graphics::polygon(const POINT_F64 * ppoints, count count)
   {

      if (count < 3)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      comptr<ID2D1PathGeometry> pgeometry;

      HRESULT hr = ::direct2d::direct2d()->d2d1_factory1()->CreatePathGeometry(&pgeometry);

      {

         comptr<ID2D1GeometrySink> psink;

         pgeometry->Open(&psink);

         psink->SetFillMode(D2D1_FILL_MODE_WINDING);

         psink->BeginFigure(D2D1::Point2F((FLOAT) ppoints->x, (FLOAT)ppoints->y), D2D1_FIGURE_BEGIN_FILLED);

         for (index i = 1; i < count; i++)
         {

            psink->AddLine({ (FLOAT)ppoints[i].x, (FLOAT)ppoints[i].y });

         }

         psink->EndFigure(D2D1_FIGURE_END_CLOSED);

         psink->Close();

      }

      //bool bOk = 
      
      fill(pgeometry, m_pbrush);

      //bOk = bOk && 
      
      draw(pgeometry, m_ppen);

      //return bOk;

   }


   void graphics::draw_polygon(const POINT_F64 * ppoints, count count)
   {

      if (count < 3)
      {

         //return false;

         throw ::exception(error_invalid_empty_argument);

      }

      comptr<ID2D1PathGeometry> pgeometry;

      HRESULT hr = ::direct2d::direct2d()->d2d1_factory1()->CreatePathGeometry(&pgeometry);

      {

         comptr<ID2D1GeometrySink> psink;

         pgeometry->Open(&psink);

         psink->SetFillMode(D2D1_FILL_MODE_WINDING);

         psink->BeginFigure(D2D1::Point2F((FLOAT)ppoints->x, (FLOAT)ppoints->y), D2D1_FIGURE_BEGIN_FILLED);

         for (index i = 1; i < count; i++)
         {

            psink->AddLine({ (FLOAT)ppoints[i].x, (FLOAT)ppoints[i].y });

         }

         psink->EndFigure(D2D1_FIGURE_END_CLOSED);

         psink->Close();

      }

      //bool bOk = 
      
      draw(pgeometry, m_ppen);

      //return bOk;

   }


   void graphics::fill_polygon(const POINT_F64 * ppoints, count count)
   {

      if (count < 3)
      {

         //return false;

         throw ::exception(error_invalid_empty_argument);

      }

      comptr<ID2D1PathGeometry> pgeometry;

      HRESULT hr = ::direct2d::direct2d()->d2d1_factory1()->CreatePathGeometry(&pgeometry);

      {

         comptr<ID2D1GeometrySink> psink;

         pgeometry->Open(&psink);

         psink->SetFillMode(D2D1_FILL_MODE_WINDING);

         psink->BeginFigure(D2D1::Point2F((FLOAT)ppoints->x, (FLOAT)ppoints->y), D2D1_FIGURE_BEGIN_FILLED);

         for (index i = 1; i < count; i++)
         {

            psink->AddLine({ (FLOAT)ppoints[i].x, (FLOAT)ppoints[i].y });

         }

         psink->EndFigure(D2D1_FIGURE_END_CLOSED);

         psink->Close();

      }

      //bool bOk = 
      
      fill(pgeometry, m_pbrush);

      //return bOk;

   }


   void graphics::rectangle(const ::rectangle_f64 & rectangle)
   {

      fill_rectangle(rectangle);

      draw_rectangle(rectangle);

   }


   void graphics::draw_rectangle(const ::rectangle_f64 & rectangleParam, ::draw2d::pen * ppen)
   {

      if (ppen == nullptr)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      if (ppen->m_epen == ::draw2d::e_pen_null)
      {

         //return true;

         return;

      }

      D2D1_RECT_F rectangle;

      copy(rectangle, rectangleParam);

      ::draw2d_direct2d::pen * ppen2 = dynamic_cast <::draw2d_direct2d::pen *> (ppen);

      defer_primitive_blend();

      m_prendertarget->DrawRectangle(&rectangle, ppen2->get_os_data < ID2D1Brush * >(this), (FLOAT)ppen->m_dWidth);

      //return true;

   }


   void graphics::fill_rectangle(const ::rectangle_f64 & rectangleParam, ::draw2d::brush * pbrush)
   {

      if (pbrush == nullptr)
      {

         throw ::exception(error_null_pointer);

      }

      if (pbrush->m_ebrush == ::draw2d::e_brush_null)
      {

         return;

      }

      if (pbrush->m_ebrush == ::draw2d::e_brush_box_gradient)
      {



         return;

      }

      D2D1_RECT_F rectangle;

      copy(rectangle, rectangleParam);

      ::draw2d_direct2d::brush * pbrush2 = dynamic_cast <::draw2d_direct2d::brush *> (pbrush);

      defer_primitive_blend();

      m_prendertarget->FillRectangle(rectangle, pbrush2->get_os_data < ID2D1Brush * >(this));

      //return true;

   }


   void graphics::fill_round_rectangle(const ::rectangle_f64 & rectangleParam, ::draw2d::brush * pbrush, double dRadius)
   {

      if (pbrush->m_ebrush == ::draw2d::e_brush_box_gradient)
      {

         ::draw2d::graphics::fill_round_rectangle(rectangleParam, pbrush, dRadius);

         return;

      }

      if (m_pbrush.is_set() && m_pbrush->m_ebrush != ::draw2d::e_brush_null)
      {

         D2D1_ROUNDED_RECT rectangle;

         copy(rectangle.rect, rectangleParam);

         rectangle.radiusX = (FLOAT)dRadius;

         rectangle.radiusY = (FLOAT)dRadius;

         auto pbrush = m_pbrush.cast <::draw2d_direct2d::brush >();

         ID2D1Brush * pd2d1brush = pbrush->get_os_data < ID2D1Brush * >(this);

         defer_primitive_blend();

         m_prendertarget->FillRoundedRectangle(rectangle, pd2d1brush);

      }

   }

   
   void graphics::draw_round_rectangle(const ::rectangle_f64 & rectangleParam, ::draw2d::pen * ppen, double dRadius)
   {

      if (m_ppen.is_set() && m_ppen->m_epen != ::draw2d::e_pen_null)
      {

         D2D1_ROUNDED_RECT rectangle;

         copy(rectangle.rect, rectangleParam);

         rectangle.radiusX = (FLOAT)dRadius;

         rectangle.radiusY = (FLOAT)dRadius;

         auto ppen = m_ppen.cast <::draw2d_direct2d::pen >();

         ID2D1Brush * pd2d1brush = ppen->get_os_data < ID2D1Brush * >(this);
         
         defer_primitive_blend();

         m_prendertarget->DrawRoundedRectangle(rectangle, pd2d1brush, (FLOAT) ppen->m_dWidth);

      }

      //return true;

   }


   //void graphics::PatBlt(double x, double y, double dWidth, double dHeight)
   //{

   //   throw ::exception(todo);

   //}


   void graphics::_draw_raw(const ::rectangle_f64 & rectangleTarget, ::image * pimage, const image_drawing_options & imagedrawingoptions, const ::point_f64 & pointSrc)
   {

      //::draw2d::lock draw2dlock;

      // ::draw2d::device_lock devicelock(this);

      pimage->defer_update_image();

      //try
      //{

         if (pimage == nullptr || pimage->get_bitmap() == nullptr)
         {

            //return false;

            throw ::exception(error_null_pointer);

         }

         //if (pgraphicsSrc->get_current_bitmap() == nullptr)
         //{

         //   return false;

         //}

         //if (pgraphicsSrc->get_current_bitmap()->get_os_data() == nullptr)
         //{

         //   return false;

         //}

         double x = rectangleTarget.left;
         double y = rectangleTarget.top;
         double nWidth = rectangleTarget.width();
         double nHeight = rectangleTarget.height();
         double xSrc = pointSrc.x;
         double ySrc = pointSrc.y;

         if (x < 0)
         {

            xSrc -= x;

            nWidth += x;

            x = 0;

         }

         if (y < 0)
         {

            ySrc -= y;

            nHeight += y;

            y = 0;

         }

         if (get_current_bitmap())
         {

            auto pd2d1bitmap = get_current_bitmap()->get_os_data < ID2D1Bitmap * >();

            if (::is_set(pd2d1bitmap))
            {

               D2D1_SIZE_U sz = pd2d1bitmap->GetPixelSize();

               if (nWidth + x + get_origin().x > sz.width)
               {

                  nWidth = sz.width - x - get_origin().x;

               }

               if (nHeight + y + get_origin().y > sz.height)
               {

                  nHeight = sz.height - y - get_origin().y;

               }

            }

         }

         {

            D2D1_SIZE_U sz = ((ID2D1Bitmap *)pimage->get_bitmap()->get_os_data())->GetPixelSize();

            if (nWidth + xSrc > sz.width)
            {

               nWidth = sz.width - xSrc;

            }

            if (nHeight + ySrc > sz.height)
            {

               nHeight = sz.height - ySrc;

            }

         }

         {

            D2D1_RECT_F rectangleTarget = D2D1::RectF((float)x, (float)y, (float)(x + nWidth), (float)(y + nHeight));

            D2D1_RECT_F rectangleSource = D2D1::RectF((float)xSrc, (float)ySrc, (float)(xSrc + nWidth), (float)(ySrc + nHeight));

            auto pd2d1bitmap = ((ID2D1Bitmap*)pimage->get_bitmap()->get_os_data());

            int cx = pd2d1bitmap->GetPixelSize().width;

            int cy = pd2d1bitmap->GetPixelSize().height;

            pimage->unmap();

            HRESULT hrFlush = ((ID2D1DeviceContext*)pimage->g()->get_os_data())->Flush();

            HRESULT hrEndDraw = ((ID2D1DeviceContext*)pimage->g()->get_os_data())->EndDraw();

            defer_primitive_blend();

            if (imagedrawingoptions.is_identity())
            {

               m_pdevicecontext->DrawBitmap(pd2d1bitmap, rectangleTarget, (FLOAT) imagedrawingoptions.opacity().get_opacity_rate(), D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR, rectangleSource);

            }
            else
            {

               comptr<ID2D1Effect> colorMatrixEffect;
               HRESULT hr = m_pdevicecontext->CreateEffect(CLSID_D2D1ColorMatrix, &colorMatrixEffect);

               defer_throw_hresult(hr);

               colorMatrixEffect->SetInput(0, pd2d1bitmap);
               D2D1_MATRIX_5X4_F matrix =
                  D2D1::Matrix5x4F(
                     (FLOAT) imagedrawingoptions.matrix().a1,
                     (FLOAT) imagedrawingoptions.matrix().a2,
                     (FLOAT) imagedrawingoptions.matrix().a3,
                     (FLOAT) imagedrawingoptions.matrix().a4,
                     (FLOAT) imagedrawingoptions.matrix().b1,
                     (FLOAT) imagedrawingoptions.matrix().b2,
                     (FLOAT) imagedrawingoptions.matrix().b3,
                     (FLOAT) imagedrawingoptions.matrix().b4,
                     (FLOAT) imagedrawingoptions.matrix().c1,
                     (FLOAT) imagedrawingoptions.matrix().c2,
                     (FLOAT) imagedrawingoptions.matrix().c3,
                     (FLOAT) imagedrawingoptions.matrix().c4,
                     (FLOAT) imagedrawingoptions.matrix().d1,
                     (FLOAT) imagedrawingoptions.matrix().d2,
                     (FLOAT) imagedrawingoptions.matrix().d3,
                     (FLOAT) imagedrawingoptions.matrix().d4,
                     (FLOAT) imagedrawingoptions.matrix().e1,
                     (FLOAT) imagedrawingoptions.matrix().e2,
                     (FLOAT) imagedrawingoptions.matrix().e3,
                     (FLOAT) imagedrawingoptions.matrix().e4);
               colorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, matrix);
               D2D1_POINT_2F pointTarget;
               pointTarget.x = rectangleTarget.left;
               pointTarget.y = rectangleTarget.top;
               //m_pdevicecontext->BeginDraw();
               m_pdevicecontext->DrawImage(colorMatrixEffect, &pointTarget, &rectangleSource, D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
               //m_pdevicecontext->EndDraw();

            }


            if (SUCCEEDED(hrEndDraw))
            {

               ((ID2D1DeviceContext*)pimage->g()->get_os_data())->BeginDraw();

            }

         }

         //return true;

      //}
      //catch (...)
      //{

      //   return false;

      //}

   }


   void graphics::_stretch_raw(const ::rectangle_f64 & rectangleTarget, ::image * pimage, const image_drawing_options & imagedrawingoptions, const ::rectangle_f64 & rectangleSource)
   {

      //try
      //{

         double xDst = rectangleTarget.left;
         double yDst = rectangleTarget.top;
         double nDstWidth = rectangleTarget.width();
         double nDstHeight = rectangleTarget.height();
         double xSrc = rectangleSource.left;
         double ySrc = rectangleSource.top;
         double nSrcWidth = rectangleSource.width();
         double nSrcHeight = rectangleSource.height();

         if (::is_null(pimage) || pimage->nok())
         {

            //return false;

            throw ::exception(error_failed);

         }

         if (pimage->m_pextension && pimage->m_pextension->m_pframea)
         {

            if (m_pimage)
            {

               auto & pframeaSource = pimage->m_pextension->m_pframea;

               auto & pframeaTarget = m_pimage->get_extension()->m_pframea;

               __defer_construct_new(pframeaTarget);

               if (pframeaTarget->get_size() != pframeaSource->get_size())
               {

                  pframeaTarget->set_size(pframeaSource->get_size());

                  for (::index i = 0; i < pframeaTarget->get_size(); i++)
                  {

                     auto & pframeSource = pframeaSource->element_at(i);

                     auto & pframeTarget = pframeaTarget->element_at(i);

                     __defer_construct_new(pframeTarget);

                     pframeTarget->m_time = pframeSource->m_time;

                     pframeTarget->m_iFrame = pframeSource->m_iFrame;

                     auto & pimageSource = pframeSource->m_pimage;

                     pimageSource->set_ok_flag();

                     auto & pimageTarget = pframeTarget->m_pimage;

                     __defer_construct(pimageTarget);

                     pimageTarget->create(m_pimage->size());

                     pimageTarget->g()->_stretch_raw(rectangleTarget, pimageSource, imagedrawingoptions, rectangleSource);

                  }

                  pframeaTarget->m_timeTotal = pframeaSource->m_timeTotal;

               }

               return;

            }

         }

         if (pimage->get_bitmap() == nullptr)
         {

            //return false;

            throw ::exception(error_failed);

         }

         if (pimage->get_graphics() == nullptr)
         {

            //return false;

            throw ::exception(error_failed);

         }

         {

            D2D1_RECT_F rectangleTarget = D2D1::RectF((float)xDst, (float)yDst, (float)(xDst + nDstWidth), (float)(yDst + nDstHeight));

            D2D1_RECT_F rectangleSource = D2D1::RectF((float)xSrc, (float)ySrc, (float)(xSrc + nSrcWidth), (float)(ySrc + nSrcHeight));

            pimage->unmap();

            HRESULT hrFlush = ((ID2D1DeviceContext*)pimage->g()->get_os_data())->Flush();

            HRESULT hrEndDraw = ((ID2D1DeviceContext*)pimage->g()->get_os_data())->EndDraw();

            defer_primitive_blend();

            if (m_prendertarget != nullptr)
            {

               m_prendertarget->DrawBitmap((ID2D1Bitmap*)pimage->get_bitmap()->get_os_data(), &rectangleTarget, 1.0, m_bitmapinterpolationmode, &rectangleSource);

            }
            else
            {

               m_pdevicecontext->DrawBitmap((ID2D1Bitmap*)pimage->g()->get_current_bitmap()->get_os_data(), rectangleTarget, 1.0, m_interpolationmode, rectangleSource);

            }

            if (SUCCEEDED(hrEndDraw))
            {

               ((ID2D1DeviceContext*)pimage->g()->get_os_data())->BeginDraw();

            }

         }


         //else
         //{

         //   output_debug_string("direct2d graphics::StretchBltRaw hr failed");

         //}

         //return true;

      //}
      //catch (...)
      //{

      //}

      //return false;

   }


   //void graphics::_draw_raw(const ::rectangle_f64 & rectangleTarget, ::draw2d::graphics * pgraphicsSrc, const ::point_f64 & pointSrc)
   //{


   //}


   //void graphics::_stretch_raw(const ::rectangle_f64 & rectangleTarget, ::draw2d::graphics * pgraphicsSrc, const ::rectangle_f64 & rectangleSource)
   //{


   //}


   ::color::color graphics::GetPixel(double x, double y)
   {

      throw ::exception(todo);


      return false;

   }


   ::color::color graphics::GetPixel(const ::point_f64 & point)
   {

      throw ::exception(todo);

      return false;

   }


   ::color::color graphics::SetPixel(double x, double y, const ::color::color & color)
   {

      throw ::exception(todo);

      return false;

   }


   ::color::color graphics::SetPixel(const ::point_f64 & point, const ::color::color & color)
   {

      throw ::exception(todo);

      return false;

   }


//   void graphics::FloodFill(double x, double y, const ::color::color & color)
//   {
//
//      throw ::exception(todo);
//
//   }
//
//
//   void graphics::ExtFloodFill(double x, double y, const ::color::color & color, ::u32 nFillType)
//   {
//
//      throw ::exception(todo);
//
//   }


   //void graphics::text_out(double x, double y, const block & block)
   //{

   //   return text_out(x, y, (const ::string &) block.get_data(), block.get_size());

   //}


   //void graphics::text_out(double x, double y, const ::string & str)
   //{

   //   return text_out((int) x, (int) y, str, (int) str.length());

   //}


   //void graphics::ExtTextOut(double x, double y, ::u32 nOptions, const ::rectangle_f64 & rectangle, const ::string & lpszString, strsize nCount, LPINT lpDxWidths)
   //{

   //   throw ::exception(todo);

   //}


   //void graphics::ExtTextOut(double x, double y, ::u32 nOptions, const ::rectangle_f64 & rectangle, const ::string & str, LPINT lpDxWidths)
   //{

   //   throw ::exception(todo);

   //}


   //::size_f64 graphics::TabbedTextOut(double x, double y, const ::string & lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions, i32 nTabOrigin)
   //{

   //   throw ::exception(todo);

   //}


   //::size_f64 graphics::TabbedTextOut(double x, double y, const ::string & str, count nTabPositions, LPINT lpnTabStopPositions, i32 nTabOrigin)
   //{

   //   throw ::exception(todo);

   //}


   //::size_f64 graphics::GetTabbedTextExtent(const ::string & lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions)
   //{

   //   throw ::exception(todo);

   //}


   //::size_f64 graphics::GetTabbedTextExtent(const ::string & str, count nTabPositions, LPINT lpnTabStopPositions)
   //{

   //   throw ::exception(todo);

   //}


   //::size_f64 graphics::GetOutputTabbedTextExtent(const ::string & lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions)
   //{

   //   throw ::exception(todo);

   //   return nullptr;

   //}


   //::size_f64 graphics::GetOutputTabbedTextExtent(const ::string & str, count nTabPositions, LPINT lpnTabStopPositions)
   //{

   //   throw ::exception(todo);

   //   return nullptr;

   //}


   //void graphics::GrayString(::draw2d::brush* pBrush, bool (CALLBACK* lpfnOutput)(HDC, LPARAM, int), LPARAM lpData, double dCount,double x, double y, double dWidth, double dHeight)
   //{

   //   throw ::exception(todo);

   //}


   ::u32 graphics::GetTextAlign()
   {

      throw ::exception(todo);

      return false;

   }


   //int graphics::GetTextFace(count nCount, char * lpszFacename)
   //{

   //   throw ::exception(todo);

   //}


   //int graphics::GetTextFace(string & rString)
   //{

   //   throw ::exception(todo);

   //}


   void graphics::get_text_metrics(::write_text::text_metric * pmetrics)
   {

      if (m_pfont.is_null())
      {

         __construct(((graphics*)this)->m_pfont);

      }

      if (m_pfont.is_null())
      {

         //return false;

         throw ::exception(error_failed);

      }

      m_pfont->get_os_data(this);

      memcpy(pmetrics, &m_pfont->m_textmetric2, sizeof(m_pfont->m_textmetric2));


      //return true;

   }


   void graphics::get_output_text_metrics(::write_text::text_metric * lpMetrics)
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::GetTextMetrics(get_handle1(), lpMetrics) != false;

      //return false;

   }


//   int graphics::GetTextCharacterExtra()
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle2() != nullptr);
//
//      //return ::GetTextCharacterExtra(get_handle2());
//
//   }
//
//
//   void graphics::GetCharWidth(::u32 nFirstChar, ::u32 nLastChar, LPINT lpBuffer)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle2() != nullptr);
//
//      //return ::GetCharWidth(get_handle2(), nFirstChar, nLastChar, lpBuffer) != false;
//
//   }
//
//   void graphics::GetOutputCharWidth(::u32 nFirstChar, ::u32 nLastChar, LPINT lpBuffer)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::GetCharWidth(get_handle1(), nFirstChar, nLastChar, lpBuffer) != false;
//
//   }
//
//   u32 graphics::GetFontLanguageInfo()
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::GetFontLanguageInfo(get_handle1());
//
//   }
//
//#ifdef WINDOWS_DESKTOP
//
//   /*   u32 graphics::GetCharacterPlacement(const ::string & lpString, double dCount, double dMaxExtent, LPGCP_RESULTS lpResults, u32 dwFlags)
//      {
//
//         throw ::exception(todo);
//
//         //ASSERT(get_handle1() != nullptr);
//
//         //return ::GetCharacterPlacement(get_handle1(), lpString, nCount, nMaxExtent, lpResults, dwFlags);
//
//      }
//
//      u32 graphics::GetCharacterPlacement(string & str, double dMaxExtent, LPGCP_RESULTS lpResults, u32 dwFlags)
//      {
//
//         throw ::exception(todo);
//
//         //ASSERT(get_handle1() != nullptr);
//
//         //return ::GetCharacterPlacement(get_handle1(), (const ::string &)str, (int) str.length(), nMaxExtent, lpResults, dwFlags);
//
//      }
//      */
//#endif
//
//   ::size_f64 graphics::GetAspectRatioFilter()
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle2() != nullptr);
//      //::size_f64::size_f64;
//      //VERIFY(::GetAspectRatioFilterEx(get_handle2(), &::size_f64));
//      //return ::size_f64;
//
//   }
//
//   void graphics::ScrollDC(int dx, int dy, const ::rectangle_i32 & rectangleScroll, const ::rectangle_i32 & rectangleClip, ::draw2d::region* pRgnUpdate, RECTANGLE_I32 * lpRectUpdate)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::ScrollDC(get_handle1(), dx, dy, lpRectScroll, lpRectClip, (HRGN)pRgnUpdate->get_os_data(), lpRectUpdate) != false;
//
//   }
//
//   // Printer Escape Functions
//   int graphics::Escape(double dEscape, double dCount, const ::string & lpszInData, LPVOID lpOutData)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::Escape(get_handle1(), nEscape, nCount, lpszInData, lpOutData);
//
//   }

   // graphics 3.1 Specific functions
   ::u32 graphics::SetBoundsRect(const ::rectangle_f64 & rectangleBounds, ::u32 flags)
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::SetBoundsRect(get_handle1(), rectangleBounds, flags);

      return false;

   }


   ::u32 graphics::GetBoundsRect(::rectangle_f64 * prectBounds, ::u32 flags)
   {

      throw ::exception(todo);

      //ASSERT(get_handle2() != nullptr);

      //return ::GetBoundsRect(get_handle2(), rectangleBounds, flags);

      return false;

   }


   //void graphics::ResetDC(const DEVMODE* lpDevMode)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle2() != nullptr);

   //   //return ::ResetDC(get_handle2(), lpDevMode) != nullptr;

   //}

#ifdef WINDOWS_DESKTOP

   /*   ::u32 graphics::GetOutlineTextMetrics(::u32 cbData, LPOUTLINETEXTMETRICW lpotm)
      {

         throw ::exception(todo);

         //ASSERT(get_handle2() != nullptr);

         //return ::GetOutlineTextMetrics(get_handle2(), cbData, lpotm);

      }

      void graphics::GetCharABCWidths(::u32 nFirstChar, ::u32 nLastChar, LPABC lpabc)
      {

         throw ::exception(todo);

         //ASSERT(get_handle2() != nullptr);

         //return ::GetCharABCWidths(get_handle2(), nFirstChar, nLastChar, lpabc) != false;

      }
      */
#endif

//   u32 graphics::GetFontData(u32 dwTable, u32 dwOffset, LPVOID lpData, u32 cbData)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle2() != nullptr);
//
//      //return ::GetFontData(get_handle2(), dwTable, dwOffset, lpData, cbData);
//
//   }
//
//#ifdef WINDOWS_DESKTOP
//
//   /*
//   int graphics::GetKerningPairs(double dPairs, LPKERNINGPAIR lpkrnpair)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle2() != nullptr);
//
//      //return ::GetKerningPairs(get_handle2(), nPairs, lpkrnpair);
//
//   }
//
//   u32 graphics::GetGlyphOutline(::u32 nChar, const ::e_align & ealign, const ::e_draw_text & edrawtext, LPGLYPHMETRICS lpgm,  u32 cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle2() != nullptr);
//
//      //return ::GetGlyphOutline(get_handle2(), nChar, nFormat, lpgm, cbBuffer, lpBuffer, lpmat2);
//
//   }
//
//   // ::userbase::document handling functions
//   int graphics::StartDoc(LPDOCINFO lpDocInfo)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::StartDoc(get_handle1(), lpDocInfo);
//
//   }
//   */
//#endif

   int graphics::StartPage()
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::StartPage(get_handle1());

      return 0;

   }

   int graphics::EndPage()
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::EndPage(get_handle1());
      return 0;

   }


   //int graphics::SetAbortProc(bool (CALLBACK* lpfn)(HDC, int))
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::SetAbortProc(get_handle1(), (ABORTPROC)lpfn);

   //}


   int graphics::AbortDoc()
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::AbortDoc(get_handle1());

      return 0;

   }


   int graphics::EndDoc()
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::EndDoc(get_handle1());

      return 0;

   }


//   void graphics::MaskBlt(double x, double y, double dWidth, double dHeight, ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc, ::draw2d::bitmap& maskBitmap, double xMask, double yMask)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::MaskBlt(get_handle1(), x, y, nWidth, nHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc,  (HBITMAP)maskBitmap.get_os_data(), xMask, yMask, dwRop) != false;
//
//   }
//
//
//   void graphics::PlgBlt(::point_f64 * lpPoint, ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc, double dWidth, double dHeight, ::draw2d::bitmap& maskBitmap, double xMask, double yMask)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::PlgBlt(get_handle1(), lpPoint, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nWidth, nHeight, (HBITMAP)maskBitmap.get_os_data(), xMask, yMask) != false;
//
//   }


   //void graphics::SetPixelV(double x, double y, const ::color::color & color)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::SetPixelV(get_handle1(), x, y, crColor) != false;

   //   return false;

   //}

   //void graphics::SetPixelV(const ::point_f64 & point, const ::color::color & color)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::SetPixelV(get_handle1(), point.x, point.y, crColor) != false;

   //   return false;

   //}

   void graphics::angle_arc(double x, double y, double nRadius, angle fStartAngle, angle fSweepAngle)
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::AngleArc(get_handle1(), x, y, nRadius, fStartAngle, fSweepAngle) != false;

      //return false;

   }


   //void graphics::arc_to(const ::rectangle_f64 & rectangle, const ::point_f64 & pointStart, const ::point_f64 & pointEnd)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ArcTo(rectangle.left, rectangle.top, rectangle.right, rectangle.bottom, pointStart.x, pointStart.y, pointEnd.x, pointEnd.y);
   //   //return false;


   //}

   //int graphics::GetArcDirection()
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle2() != nullptr);

   //   //return ::GetArcDirection(get_handle2());

   //}

   void graphics::poly_polyline(const ::point_f64* ppoints, const ::i32 * lpPolyPoints, count nCount)
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::poly_polyline(get_handle1(), ppoints, lpPolyPoints, nCount) != false;
      //return false;


   }

#ifdef WINDOWS_DESKTOP
   /*
      void graphics::GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust)
      {

         throw ::exception(todo);

         //ASSERT(get_handle2() != nullptr);

         //return ::GetColorAdjustment(get_handle2(), lpColorAdjust) != false;

      }
      */
#endif


   void graphics::poly_bezier(const ::point_f64* ppoints, count nCount)
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::poly_bezier(get_handle1(), ppoints, nCount) != false;

      //return false;

   }


//   int graphics::DrawEscape(double dEscape, double dInputSize, const ::string & lpszInputData)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::DrawEscape(get_handle1(), nEscape, nInputSize, lpszInputData);
//
//   }
//
//
//   int graphics::Escape(double dEscape, double dInputSize, const ::string & lpszInputData, double dOutputSize, char * lpszOutputData)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::ExtEscape(get_handle1(), nEscape, nInputSize, lpszInputData, nOutputSize, lpszOutputData);
//
//   }
//
//
//#ifdef WINDOWS_DESKTOP
//   /*
//      void graphics::GetCharABCWidths(::u32 nFirstChar, ::u32 nLastChar, LPABCFLOAT lpABCF)
//      {
//
//         throw ::exception(todo);
//
//         //ASSERT(get_handle2() != nullptr);
//         //
//         //return ::GetCharABCWidthsFloat(get_handle2(), nFirstChar, nLastChar, lpABCF) != false;
//
//      }
//      */
//#endif
//
//   void graphics::GetCharWidth(::u32 nFirstChar, ::u32 nLastChar, float* lpFloatBuffer)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle2() != nullptr);
//
//      //return ::GetCharWidthFloat(get_handle2(), nFirstChar, nLastChar, lpFloatBuffer) != false;
//
//   }


   void graphics::abort_path()
   {

      throw ::exception(todo);

      //if(m_ppath != nullptr)
      //{
      //   delete m_ppath;
      //   m_ppath = nullptr;
      //}
      //return true;

      //return false;

   }


   void graphics::begin_path()
   {

      throw ::exception(todo);

      //return false;

   }


   void graphics::close_figure()
   {

      throw ::exception(todo);

      //ASSERT(m_ppath != nullptr);
      //return m_ppath->CloseFigure() == Gdiplus::Status::Ok;

      //return false;

   }


   void graphics::end_path()
   {

      throw ::exception(todo);

      //if(m_ppath == nullptr)
      //   return false;

      //m_ppathPaint   = m_ppath;
      //m_ppath        = nullptr;

      //return true;

      //return false;

   }


   void graphics::fill_path()
   {

      throw ::exception(todo);

      //return m_prendertarget->FillPath(direct2d_brush(), m_ppath) == Gdiplus::Status::Ok;

      //return false;


   }


   void graphics::flatten_path()
   {

      throw ::exception(todo);

      //return m_ppath->Flatten() == Gdiplus::Status::Ok;
      //return false;


   }


   float graphics::GetMiterLimit()
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);
      //float fMiterLimit;
      //VERIFY(::GetMiterLimit(get_handle1(), &fMiterLimit));
      //return fMiterLimit;

      return 0.f;

   }


   //i32 graphics::GetPath(::point_f64 * ppoints, byte * lpTypes, ::count nCount)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr);
   //   //return ::GetPath(get_handle1(), ppoints, lpTypes, nCount);

   //}


   void graphics::SetMiterLimit(float fMiterLimit)
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);
      //return ::SetMiterLimit(get_handle1(), fMiterLimit, nullptr) != false;

      //return false;

   }


   void graphics::stroke_and_fill_path()
   {

      throw ::exception(todo);

      //bool bOk1 = m_prendertarget->FillPath(direct2d_brush(), m_ppathPaint) == Gdiplus::Status::Ok;

      //bool bOk2 = m_prendertarget->DrawPath(direct2d_pen(), m_ppathPaint) == Gdiplus::Status::Ok;

      //return bOk1 && bOk2;

      //return false;

   }


   void graphics::stroke_path()
   {

      throw ::exception(todo);

//      return m_prendertarget->DrawPath(direct2d_pen(), m_ppathPaint) == Gdiplus::Status::Ok;

      //return false;

   }


   void graphics::widen_path()
   {

      throw ::exception(todo);


//      return m_ppath->Widen(direct2d_pen()) == Gdiplus::Status::Ok;

      //return false;

   }


   void graphics::AddMetaFileComment(::u32 nDataSize, const byte* pCommentData)
   {

      throw ::exception(todo);


      //ASSERT(get_handle1() != nullptr);
      //return ::GdiComment(get_handle1(), nDataSize, pCommentData) != false;

      //return false;

   }


   /*bool CALLBACK metaCallback(
   EmfPlusRecordType recordType,
   unsigned int flags,
   unsigned int dataSize,
   const unsigned char* pStr,
   void* callbackData)
   {
   // Play only EmfPlusRecordTypeFillEllipse records.
   if (recordType == EmfPlusRecordTypeFillEllipse)
   {
   // Explicitly cast callbackData as a metafile pointer, and use it to call
   // the PlayRecord method.
   static_cast < Metafile* > (callbackData)->PlayRecord(recordType, flags, dataSize, pStr);
   }
   return true;
   }

   VOID Example_EnumerateMetafile9(HDC hdc)
   {
   Graphics graphics(hdc);
   // Create a Metafile object from an existing disk metafile.
   Metafile* pMeta = new Metafile(L"SampleMetafile.emf", hdc);
   {
   // Fill a rectangle_i32 and an ellipse in pMeta.
   Graphics metaGraphics(pMeta);
   metaGraphics.fill_rectangle(&SolidBrush(Color(255, 0, 0, 0)), 0, 0, 100, 100);
   metaGraphics.FillEllipse(&SolidBrush(Color(255, 255, 0, 0)), 100, 0, 200, 100);
   }
   // Enumerate pMeta to the destination rectangle, passing pMeta as the callback data.
   graphics.EnumerateMetafile(
   pMeta,
   Rect(0, 0, 300, 50),
   metaCallback,
   pMeta);
   // Draw pMeta as an pimage->
   graphics.DrawImage(pMeta, Point(0, 150));
   delete pMeta;;
   }*/
   //void graphics::PlayMetaFile(HENHMETAFILE hEnhMF, const ::rectangle_f64 & rectangleBounds)
   //{

   //   throw ::exception(todo);

   //   //Gdiplus::RectF rectangle_i32((Gdiplus::REAL) lpBounds->left, (Gdiplus::REAL) lpBounds->top, (Gdiplus::REAL) width(lpBounds), (Gdiplus::REAL) height(lpBounds));

   //   //Gdiplus::Metafile* pMeta = new Gdiplus::Metafile(hEnhMF, false);

   //   ////m_pgraphcis->EnumerateMetafile(pMeta, rectangle, metaCallback, PMETAHEADER);

   //   //bool bOk = m_prendertarget->DrawImage(pMeta, rectangle) == Gdiplus::Status::Ok;

   //   //delete pMeta;

   //   //return bOk ? true : false;
   //   ////return ::PlayEnhMetaFile(get_handle1(), hEnhMF, lpBounds);

   //}

   // true blend
   // COLOR_DEST = SRC_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA) * COLOR_DST

   // double blend
   // COLOR_DEST = SRC_ALPHA * BLEND_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA * BLEND_ALPHA) * COLOR_DST

   // Thank you
   // Jiju George T
   // Web Developer
   // India India
   // Member


   //void graphics::_alpha_blend_raw(const ::rectangle_f64 & rectangleTarget, ::draw2d::graphics * pgraphicsSrc, const ::rectangle_f64 & rectangleSource, double dRate)
   //{

   //   // ::draw2d::device_lock devicelock(this);

   //   /*      float fA = (float) dRate;

   //         Gdiplus::ColorMatrix matrix = {
   //            1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
   //            0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
   //            0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
   //            0.0f, 0.0f, 0.0f, fA  , 0.0f,
   //            0.0f, 0.0f, 0.0f, 0.0f, 1.0f
   //         };

   //         Gdiplus::ImageAttributes attributes;

   //         attributes.SetColorMatrix(&matrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);

   //         Gdiplus::RectF dstRect((Gdiplus::REAL) xDest, (Gdiplus::REAL) yDest, (Gdiplus::REAL) nDestWidth, (Gdiplus::REAL) nDestHeight);

   //         m_prendertarget->DrawImage((Gdiplus::Bitmap *) pgraphicsSrc->get_current_bitmap()->get_os_data(), dstRect,
   //            (Gdiplus::REAL) xSrc, (Gdiplus::REAL) ySrc, (Gdiplus::REAL) nSrcWidth, (Gdiplus::REAL) nSrcHeight, Gdiplus::UnitPixel, &attributes);*/

   //   try
   //   {

   //      if(pgraphicsSrc == nullptr)
   //         return false;

   //      if(pgraphicsSrc->get_current_bitmap() == nullptr)
   //         return false;

   //      if(pgraphicsSrc->get_current_bitmap()->m_osdata[0] == nullptr)
   //         return false;

   //      //D2D1_RECT_F rectangleTarget = D2D1::RectF((float) xDst, (float) yDst, (float) (xDst + nDstWidth), (float) (yDst + nDstHeight));
   //      //D2D1_RECT_F rectangleSource = D2D1::RectF((float) xSrc, (float) ySrc, (float) (xSrc + nSrcWidth), (float) (ySrc + nSrcHeight));
   //      /*
   //               if (get_current_bitmap() != nullptr && get_current_bitmap()->get_os_data() != nullptr)
   //               {

   //                  D2D1_SIZE_U sz = ((ID2D1Bitmap *)get_current_bitmap()->get_os_data())->GetPixelSize();

   //                  if (natural(nDstWidth + xDst) > sz.width)
   //                     nDstWidth = sz.width - xDst;

   //                  if (natural(nDstHeight + yDst) > sz.height)
   //                     nDstHeight = sz.height - yDst;

   //               }

   //               {

   //                  D2D1_SIZE_U sz = ((ID2D1Bitmap *)pgraphicsSrc->get_current_bitmap()->get_os_data())->GetPixelSize();

   //                  if (natural(nSrcWidth + xSrc) > sz.width)
   //                     nSrcWidth = sz.width - xSrc;

   //                  if (natural(nSrcHeight + ySrc) > sz.height)
   //                     nSrcHeight = sz.height - ySrc;

   //               }*/

   //      D2D1_RECT_F rDst = D2D1::RectF((float)rectangleTarget.left, (float)rectangleTarget.top, (float) rectangleTarget.right, (float) rectangleTarget.bottom);
   //      D2D1_RECT_F rSrc = D2D1::RectF((float)rectangleSource.left, (float)rectangleSource.top, (float) rectangleSource.right, (float) rectangleSource.bottom);


   //      //dynamic_cast <::draw2d_direct2d::graphics *> (pgraphicsSrc)->SaveClip();

   //      HRESULT hr = ((ID2D1DeviceContext *) pgraphicsSrc->get_os_data())->EndDraw();

   //      defer_primitive_blend();

   //      if(m_pdevicecontext != nullptr)
   //      {

   //         m_pdevicecontext->DrawBitmap((ID2D1Bitmap*)pgraphicsSrc->get_current_bitmap()->get_os_data(), rDst, (float)dRate, m_interpolationmode, rSrc);

   //      }
   //      else
   //      {

   //         m_prendertarget->DrawBitmap((ID2D1Bitmap*)pgraphicsSrc->get_current_bitmap()->get_os_data(), rDst, (float)dRate, m_bitmapinterpolationmode, rSrc);

   //      }


   //      //auto pimage = (ID2D1Bitmap*)pgraphicsSrc->get_current_bitmap()->get_os_data();
   //      //auto ecomposite = m_ealphamode == ::draw2d::e_alpha_mode_blend ? D2D1_COMPOSITE_MODE_SOURCE_OVER : D2D1_COMPOSITE_MODE_SOURCE_IN;

   //      //m_pdevicecontext->DrawImage(pimage, pointDst, rectangleSource, m_interpolationmode, ecomposite);


   //      //hr = m_prendertarget->Flush();
   //      //flush();

   //      if(SUCCEEDED(hr))
   //      {

   //         ((ID2D1DeviceContext *)pgraphicsSrc->get_os_data())->BeginDraw();

   //         //dynamic_cast <::draw2d_direct2d::graphics *> (pgraphicsSrc)->RestoreClip();

   //      }

   //      return true;

   //   }
   //   catch(...)
   //   {
   //      return false;
   //   }



   //   return true;

   //}


   /*void graphics::alpha_blend(double xDest, double yDest, double dDestWidth, double dDestHeight,
   ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc, double dSrcWidth, double dSrcHeight, BLENDFUNCTION blend)
   {

   throw ::exception(not_implemented_exception());
   //if(get_handle1() == nullptr)
   // return false;


   if(m_pimageAlphaBlend->is_set())
   {


   ::rectangle_i32 rectangleIntersect(m_pointAlphaBlend, m_pimageAlphaBlend->size());


   ::image_pointer pimageWork = nullptr;
   ::image_pointer pimageWork2 = nullptr;
   //         ::image_pointer pimageWork3 = nullptr;
   ::image_pointer pimageWork4 = nullptr;


   ::point_f64 pointSrc(xSrc, ySrc);
   ::point_f64 pointDest(xDest, yDest);
   ::size_f64::size_f64(nDestWidth, nDestHeight);



   ::image_pointer pimage;
   if(imageWork == nullptr)
   {
   pimage = create_image(this);
   imageWork = pimage;
   }
   if(imageWork == nullptr)
   return false;
   if(!imageWork = create_image(::size_f64))
   return false;
   if(!imageWork.from(::point_f64(), pgraphicsSrc, pointSrc, ::size_f64))
   return false;




   ::image_pointer pimage2;
   if(imageWork2 == nullptr)
   {
   pimage2 = create_image(this);
   imageWork2 = pimage2;
   }


   ::image_pointer pimage4;
   if(imageWork4 == nullptr)
   {
   image4 = create_image(this);
   imageWork4 = image4;
   }
   if(imageWork4 == nullptr)
   return false;
   if(!imageWork4 = create_image(::size_f64))
   return false;


   imageWork4.Fill(255, 0, 0, 0);

   imageWork4.from(point_i32(maximum(0, m_pointAlphaBlend.x - xDest), maximum(0, m_pointAlphaBlend.y - yDest)),
   m_pimageAlphaBlend->get_graphics(), point_i32(maximum(0, xDest - m_pointAlphaBlend.x), maximum(0, yDest - m_pointAlphaBlend.y)), ::size_f64);

   imageWork.channel_multiply(::color::e_channel_alpha, imageWork4);


   keep < image > keep(&m_pimageAlphaBlend, nullptr, m_pimageAlphaBlend, true);


   return papplication->m_visual.imaging().true_blend(this, pointDest, ::size_f64, imageWork.get_graphics(), pointSrc);


   }

   return ::AlphaBlend(get_handle1(), xDest, yDest,
   nDestWidth, nDestHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth,
   nSrcHeight, blend) != false;
   }*/


//   void graphics::TransparentBlt(double xDest, double yDest, double dDestWidth, double dDestHeight, ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc, double dSrcWidth, double dSrcHeight, ::u32 crTransparent)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//
//      //return ::TransparentBlt(get_handle1(), xDest, yDest, nDestWidth, nDestHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, nSrcHeight, crTransparent) != false;
//
//   }
//
//   void graphics::GradientFill(TRIVERTEX* pVertices, ULONG nVertices, void * pMesh, ULONG nMeshElements, u32 dwMode)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//      //
//      //return ::GradientFill(get_handle1(), pVertices, nVertices, pMesh, nMeshElements, dwMode) != false;
//
//   }

   // This is aura API library.
   //
   //
   //
   //
   //
   //
   //
   //

   // Always Inline. Functions only in Win98/Win2K or later

   //inline ::color::color graphics::GetDCBrushColor()
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::GetDCBrushColor(get_handle1());

   //}
   //inline ::color::color graphics::SetDCBrushColor(const ::color::color & color)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::SetDCBrushColor(get_handle1(), crColor);

   //}

   //inline ::color::color graphics::GetDCPenColor()
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::GetDCPenColor(get_handle1());

   //}
   //inline ::color::color graphics::SetDCPenColor(const ::color::color & color)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::SetDCPenColor(get_handle1(), crColor);

   //}

#ifdef WINDOWS_DESKTOP
   /*
      inline void graphics::GetCharABCWidthsI(::u32 giFirst, ::u32 cgi, LPWORD pgi, LPABC lpabc)
      {

         throw ::exception(todo);

         //ASSERT(get_handle1() != nullptr);

         //return ::GetCharABCWidthsI(get_handle1(), giFirst, cgi, pgi, lpabc) != false;

      }
      */
#endif

   //inline void graphics::GetCharWidthI(::u32 giFirst, ::u32 cgi, LPWORD pgi, LPINT lpBuffer)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::GetCharWidthI(get_handle1(), giFirst, cgi, pgi, lpBuffer) != false;

   //}

   //inline void graphics::GetTextExtentExPointI(LPWORD pgiIn, int cgi, double dMaxExtent, LPINT lpnFit, LPINT alpDx, LPSIZE32 LPSIZE32)
   //{

   //   throw ::exception(todo);

   //   //ENSURE(LPSIZE32 != nullptr);
   //   //ASSERT(get_handle1() != nullptr);
   //   //return ::GetTextExtentExPointI(get_handle1(), pgiIn, cgi, nMaxExtent, lpnFit, alpDx, LPSIZE32) != false;

   //}


   //inline void graphics::GetTextExtentPointI(LPWORD pgiIn, int cgi, LPSIZE32 LPSIZE32)
   //{

   //   throw ::exception(todo);

   //   //ENSURE(LPSIZE32 != nullptr);
   //   //ASSERT(get_handle1() != nullptr);
   //   //return ::GetTextExtentPointI(get_handle1(), pgiIn, cgi, LPSIZE32) != false;

   //}








#define HIMETRIC_INCH   2540    // HIMETRIC units per inch

   void graphics::DPtoHIMETRIC(::size_f64 * psize)
   {
//#ifdef WINDOWS_DESKTOP
//      ASSERT(is_memory_segment_ok(psize, sizeof(::size_f64)));
//
//      double dMapMode;
//      if (this != nullptr && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
//            nMapMode != MM_TEXT)
//      {
//         // when using a constrained ::map mode, ::map against physical inch
//         ((::draw2d::graphics *)this)->SetMapMode(MM_HIMETRIC);
//         DPtoLP(psize);
//         ((::draw2d::graphics *)this)->SetMapMode(nMapMode);
//      }
//      else
//      {
//         // ::map against logical inch for non-constrained mapping modes
//         int cxPerInch, cyPerInch;
//         if (this != nullptr)
//         {
//            ASSERT_VALID(this);
//            ASSERT(get_handle1() != nullptr);  // no HDC attached or created?
//            cxPerInch = GetDeviceCaps(LOGPIXELSX);
//            cyPerInch = GetDeviceCaps(LOGPIXELSY);
//         }
//         else
//         {

//         }
//         ASSERT(cxPerInch != 0 && cyPerInch != 0);
//         psize->cx = psize->cx * HIMETRIC_INCH / cxPerInch;
//         psize->cy = psize->cy * HIMETRIC_INCH / cyPerInch;
//      }
//#endif
   }

   void graphics::HIMETRICtoDP(::size_f64 * psize)
   {
//#ifdef WINDOWS_DESKTOP
//      ASSERT(is_memory_segment_ok(psize, sizeof(::size_f64)));
//
//      double dMapMode;
//      if (this != nullptr && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
//            nMapMode != MM_TEXT)
//      {
//         // when using a constrained ::map mode, ::map against physical inch
//         ((::draw2d::graphics *)this)->SetMapMode(MM_HIMETRIC);
//         LPtoDP(psize);
//         ((::draw2d::graphics *)this)->SetMapMode(nMapMode);
//      }
//      else
//      {
//         // ::map against logical inch for non-constrained mapping modes
//         int cxPerInch, cyPerInch;
//         if (this != nullptr)
//         {
//            ASSERT_VALID(this);
//            ASSERT(get_handle1() != nullptr);  // no HDC attached or created?
//            cxPerInch = GetDeviceCaps(LOGPIXELSX);
//            cyPerInch = GetDeviceCaps(LOGPIXELSY);
//         }
//         else
//         {

//         }
//         ASSERT(cxPerInch != 0 && cyPerInch != 0);
//         psize->cx = psize->cx * cxPerInch / HIMETRIC_INCH;
//         psize->cy = psize->cy * cyPerInch / HIMETRIC_INCH;
//      }
//#endif
   }

   /*void graphics::LPtoHIMETRIC(LPSIZE32 LPSIZE32)
   {
      ASSERT(is_memory_segment_ok(LPSIZE32, sizeof(SIZE_I32)));

      LPtoDP(LPSIZE32);
      DPtoHIMETRIC(LPSIZE32);
   }

   void graphics::HIMETRICtoLP(LPSIZE32 LPSIZE32)
   {
      ASSERT(is_memory_segment_ok(LPSIZE32, sizeof(SIZE_I32)));

      HIMETRICtoDP(LPSIZE32);
      DPtoLP(LPSIZE32);
   }*/

   /////////////////////////////////////////////////////////////////////////////
   // special graphics drawing primitives/helpers

   ::draw2d::brush* graphics::GetHalftoneBrush(::particle * pparticle)
   {
      /*
      ::aura::LockGlobals(CRIT_HALFTONEBRUSH);
      if (gen_HalftoneBrush == nullptr)
      {
      ::u16 grayPattern[8];
      for (int i = 0; i < 8; i++)
      grayPattern[i] = (::u16)(0x5555 << (i & 1));
      HBITMAP grayBitmap = CreateBitmap(8, 8, 1, 1, grayPattern);
      if (grayBitmap != nullptr)
      {
      gen_HalftoneBrush = ::CreatePatternBrush(grayBitmap);
      ::DeleteObject(grayBitmap);
      }
      }
      if (!gen_WingdixTerm)
      gen_WingdixTerm = (char)!atexit(&__win_gdi_x_term);
      ::aura::UnlockGlobals(CRIT_HALFTONEBRUSH);
      */

      return nullptr;
   }


   //void graphics::DrawDragRect(const ::rectangle_i32 & rectangle, const ::size_f64& ::size_f64, const ::rectangle_i32 & rectangleLast, const ::size_f64& sizeLast, ::draw2d::brush* pBrush, ::draw2d::brush* pBrushLast)
   //{

   //   throw ::interface_only();

   //   /*
   //   ASSERT(is_memory_segment_ok(rectangle, sizeof(::rectangle_f64), false));
   //   ASSERT(lpRectLast == nullptr ||
   //   is_memory_segment_ok(lpRectLast, sizeof(::rectangle_f64), false));

   //   // first, determine the update region and select it
   //   ::draw2d::region rgnNew;
   //   ::draw2d::region rgnOutside, rgnInside;
   //   rgnOutside.CreateRectRgnIndirect(rectangle);
   //   const ::rectangle_i32 & rectangle = *rectangle;
   //   rectangle.inflate(-size.cx, -size.cy);
   //   rectangle.intersect(rectangle, rectangle);
   //   rgnInside.CreateRectRgnIndirect(rectangle);
   //   rgnNew.CreateRectRgn(0, 0, 0, 0);
   //   rgnNew.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);

   //   ::draw2d::brush* pBrushOld = nullptr;
   //   if (pBrush == nullptr)
   //   {
   //   pBrush = graphics::GetHalftoneBrush(this);
   //   }

   //   ENSURE(pBrush);

   //   if (pBrushLast == nullptr)
   //   {
   //   pBrushLast = pBrush;
   //   }

   //   ::draw2d::region rgnLast, rgnUpdate;
   //   if (lpRectLast != nullptr)
   //   {
   //   // find difference between new region and old region
   //   rgnLast.CreateRectRgn(0, 0, 0, 0);
   //   rgnOutside.SetRectRgn(lpRectLast);
   //   rectangle = *lpRectLast;
   //   rectangle.inflate(-sizeLast.cx, -sizeLast.cy);
   //   rectangle.intersect(rectangle, lpRectLast);
   //   rgnInside.SetRectRgn(rectangle);
   //   rgnLast.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);

   //   // only diff them if brushes are the same
   //   if (pBrush->get_os_data() == pBrushLast->get_os_data())
   //   {
   //   rgnUpdate.CreateRectRgn(0, 0, 0, 0);
   //   rgnUpdate.CombineRgn(&rgnLast, &rgnNew, RGN_XOR);
   //   }
   //   }
   //   if (pBrush->get_os_data() != pBrushLast->get_os_data() && lpRectLast != nullptr)
   //   {
   //   // brushes are different -- erase old region first
   //   SelectClipRgn(&rgnLast);
   //   get_clip_box(&rectangle);
   //   pBrushOld = SelectObject(pBrushLast);
   //   PatBlt(rectangle.left, rectangle.top, rectangle.width(), rectangle.height(), PATINVERT);
   //   SelectObject(pBrushOld);
   //   pBrushOld = nullptr;
   //   }

   //   // draw into the update/new region
   //   SelectClipRgn(rgnUpdate.get_os_data() != nullptr ? &rgnUpdate : &rgnNew);
   //   get_clip_box(&rectangle);
   //   pBrushOld = SelectObject(pBrush);
   //   PatBlt(rectangle.left, rectangle.top, rectangle.width(), rectangle.height(), PATINVERT);

   //   // cleanup DC
   //   if (pBrushOld != nullptr)
   //   SelectObject(pBrushOld);
   //   SelectClipRgn(nullptr);

   //   */

   //}


   //void graphics::fill_rectangle(const ::rectangle_f64 & rectangle, const ::color::color & color)
   //{

   //   return fill_rectangle(rectangle_f64(rectangle), color);

   //}


   //void graphics::draw_inset_3d_rectangle(const ::rectangle_f64 & rectangle, const ::color::color & colorTopLeft, const ::color::color & colorBottomRight, const ::e_border & eborder)
   //{

   //   draw_inset_3d_rectangle(rectangle_f64(rectangle), colorTopLeft, colorBottomRight, eborder);

   //}


   //void graphics::draw_inset_3d_rectangle(const ::rectangle_f64 & rectangle,  const ::color::color & colorTopLeft, const ::color::color & colorBottomRight, const ::e_border & eborder)
   //{

   //   double x = rectangle.left;
   //   double y = rectangle.top;
   //   double cx = ::width(rectangle);
   //   double cy = ::height(rectangle);

   //   if (eborder & e_border_top)
   //   {

   //      fill_solid_rect_dim(x, y, cx - 1.0, 1.0, colorTopLeft);

   //   }

   //   if (eborder & e_border_left)
   //   {

   //      fill_solid_rect_dim(x, y, 1.0, cy - 1.0, colorTopLeft);

   //   }

   //   if (eborder & e_border_right)
   //   {

   //      fill_solid_rect_dim(x + cx, y, -1.0, cy, colorBottomRight);

   //   }

   //   if (eborder & e_border_bottom)
   //   {

   //      fill_solid_rect_dim(x, y + cy, cx, -1.0, colorBottomRight);

   //   }

   //}


   void graphics::DeleteDC()
   {

      return destroy();

      /* if(get_handle() == nullptr)
          return false;

       return ::DeleteDC(Detach()) != false;*/
   }


   //int graphics::StartDoc(const ::string & lpszDocName)
   //{

   //   throw ::exception(todo);

   //   //DOCINFO di;
   //   //memory_set(&di, 0, sizeof(DOCINFO));
   //   //di.cbSize = sizeof(DOCINFO);
   //   //di.lpszDocName = lpszDocName;
   //   //return StartDoc(&di);
   //}


   int graphics::SaveDC()
   {

      m_prendertarget->GetTransform(&m_state.m_m);

      ::count iSaveDC = m_statea.get_size();

      m_statea.add(m_state);

      m_state.m_iLayerIndex = m_iLayerCount;

      //m_pstate->m_layerparameters = D2D1::LayerParameters();

      //m_prendertarget->PushLayer(m_pstate->m_layerparameters, nullptr);

      return (int) iSaveDC;

   }


   void graphics::RestoreDC(int nSavedDC)
   {

      if (nSavedDC < 0)
      {

         //return false;

         throw ::exception(error_failed);

      }

      if (nSavedDC >= m_statea.get_count())
      {

         //return false;

         throw ::exception(error_failed);

      }

      //for (index iState = m_statea.get_upper_bound(); iState > nSavedDC; iState--)
      //{

      //   auto state = m_statea[iState];

      //   for (index iItem = state->m_maRegion.get_upper_bound(); iItem >= 0; iItem--)
      //   {

      //      m_prendertarget->PopLayer();

      //   }

      //   state->m_maRegion.erase_all();

      //   state->m_sparegionClip.erase_all();

      //   m_statea.erase_at(iState);

      //}

      m_state = m_statea[nSavedDC];


      while (m_iLayerCount > m_state.m_iLayerIndex)
      {

         _pop_layer();

      }

      m_prendertarget->SetTransform(&m_state.m_m);

      m_statea.set_size(nSavedDC);

      //return true;

   }


   //int graphics::SetPolyFillMode(int nPolyFillMode)
   //{

   //   throw ::exception(todo);

   //}


   //int graphics::SetROP2(int nDrawMode)
   //{

   //   throw ::exception(todo);

   //   //double dRetVal = 0;
   //   //if(get_handle1() != nullptr && get_handle1() != get_handle2())
   //   //   nRetVal = ::SetROP2(get_handle1(), nDrawMode);
   //   //if(get_handle2() != nullptr)
   //   //   nRetVal = ::SetROP2(get_handle2(), nDrawMode);
   //   //return nRetVal;
   //}

   void graphics::set_interpolation_mode(::draw2d::enum_interpolation_mode einterpolationmode)
   {
      if(einterpolationmode == ::draw2d::e_interpolation_mode_nearest_neighbor)
      {
         m_bitmapinterpolationmode     = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
         m_interpolationmode           = D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;;
      }
      else if(einterpolationmode == ::draw2d::e_interpolation_mode_high_quality_bicubic)
      {
         m_bitmapinterpolationmode     = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
         m_interpolationmode           = D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;
      }
      else
      {
         m_bitmapinterpolationmode     = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
         m_interpolationmode           = D2D1_INTERPOLATION_MODE_LINEAR;
      }
      //return 1;
      /*double dRetVal = 0;
      if(get_handle1() != nullptr && get_handle1() != get_handle2())
      nRetVal = ::set_interpolation_mode(get_handle1(), nStretchMode);
      if(get_handle2() != nullptr)
      nRetVal = ::set_interpolation_mode(get_handle2(), nStretchMode);
      return nRetVal;*/
   }


   // ::color::color graphics::SetTextColor(const ::color::color & color)
//   {
   //  return set_color(crColor);
   //const ::color::color & colorRetVal = m_colorColor;
   //m_colorColor = crColor;
   /*      const ::color::color & colorRetVal = CLR_INVALID;
   if(get_handle1() != nullptr && get_handle1() != get_handle2())
   crRetVal = ::SetTextColor(get_handle1(), crColor);
   if(get_handle2() != nullptr)
   crRetVal = ::SetTextColor(get_handle2(), crColor);*/
   //return crRetVal;
   //}

   //int graphics::SetGraphicsMode(int iMode)
   //{

   //   throw ::exception(todo);

   //   //double dRetVal = 0;
   //   //if(get_handle1() != nullptr && get_handle1() != get_handle2())
   //   //{
   //   //   nRetVal = ::SetGraphicsMode(get_handle1(), iMode);
   //   //}
   //   //if(get_handle2() != nullptr)
   //   //{
   //   //   nRetVal = ::SetGraphicsMode(get_handle2(), iMode);
   //   //}
   //   //return nRetVal;
   //}

   //void graphics::SetWorldTransform(const XFORM* pXform)
   //{

   //   throw ::exception(todo);

   //   //bool nRetVal = 0;
   //   //if(get_handle1() != nullptr && get_handle1() != get_handle2())
   //   //{
   //   //   nRetVal = ::SetWorldTransform(get_handle1(), pXform) != false;
   //   //}
   //   //if(get_handle2() != nullptr)
   //   //{
   //   //   nRetVal = ::SetWorldTransform(get_handle2(), pXform) != false;
   //   //}
   //   //return nRetVal;
   //}

   //void graphics::ModifyWorldTransform(const XFORM* pXform,u32 iMode)
   //{

   //   throw ::exception(todo);

   //   //bool nRetVal = 0;
   //   //if(get_handle1() != nullptr && get_handle1() != get_handle2())
   //   //{
   //   //   nRetVal = ::ModifyWorldTransform(get_handle1(), pXform, iMode) != false;
   //   //}
   //   //if(get_handle2() != nullptr)
   //   //{
   //   //   nRetVal = ::ModifyWorldTransform(get_handle2(), pXform, iMode) != false;
   //   //}
   //   //return nRetVal;
   //}


   //int graphics::SetMapMode(int nMapMode)
   //{

   //   return 0;

   //}


   point_f64 graphics::set_origin(double x, double y)
   {

      return ::draw2d::graphics::set_origin(x, y);

   }


   point_f64 graphics::offset_origin(double dWidth, double dHeight)
   {

      return ::draw2d::graphics::offset_origin(dWidth, dHeight);

   }


   ::size_f64 graphics::set_extents(double x, double y)
   {

      throw ::exception(todo);


      return nullptr;

   }


   ::size_f64 graphics::scale_extents(double xNum, double xDenom, double yNum, double yDenom)
   {

      return ::draw2d::graphics::scale_extents(xNum, xDenom, yNum, yDenom);

   }


   point_f64 graphics::SetWindowOrg(double x, double y)
   {

      throw ::exception(todo);

      return nullptr;

   }


   point_f64 graphics::offset_window_org(double dWidth, double dHeight)
   {

      throw ::exception(todo);

      return nullptr;

   }


   ::size_f64 graphics::set_window_ext(double x, double y)
   {

      throw ::exception(todo);

      return nullptr;

   }


   ::size_f64 graphics::scale_window_ext(double xNum, double xDenom, double yNum, double yDenom)
   {

      throw ::exception(todo);


      return nullptr;

   }


   int graphics::get_clip_box(::rectangle_f64 & rectangle)
   {
      return 0;
      //throw ::exception(todo);

      //return ::get_clip_box(get_handle1(), rectangle);
   }

   
   void graphics::reset_clip()
   {

      //::draw2d::lock draw2dlock;

      // ::draw2d::device_lock devicelock(this);

      while (m_iLayerCount > m_state.m_iLayerIndex)
      {

         _pop_layer();

      }

      //for (index iState = m_statea.get_upper_bound(); iState >= 0; iState--)
      //{

      //   auto state = m_statea[iState];

      //   for (index iItem = state->m_maRegion.get_upper_bound(); iItem >= 0; iItem--)
      //   {

      //      m_prendertarget->PopLayer();

      //   }

      //   state->m_maRegion.erase_all();

      //   state->m_sparegionClip.erase_all();

      //}

      //auto& pstate = m_pstate;

      //if (pstate)
      //{

      //   for (index iItem = pstate->m_maRegion.get_upper_bound(); iItem >= 0; iItem--)
      //   {

      //      m_prendertarget->PopLayer();

      //   }

      //   pstate->m_maRegion.erase_all();

      //   pstate->m_sparegionClip.erase_all();

      //}

      //return ::success;

   }


   void graphics::_push_layer(ID2D1Geometry * pgeometry)
   {

      auto layerparameters = D2D1::LayerParameters(
         D2D1::InfiniteRect(),
         pgeometry);

      m_prendertarget->PushLayer(layerparameters, nullptr);

      m_iLayerCount++;

   }


   void graphics::_pop_layer()
   {

      if (m_iLayerCount <= 0)
      {

         return;

      }

      m_prendertarget->PopLayer();

      m_iLayerCount--;

   }


   void graphics::_pop_all_layers()
   {

      while (m_iLayerCount > 0)
      {

         _pop_layer();

      }

   }


   //void graphics::add_shapes(const shape_array& shapea)
   //{

   //   for (int i = 0; i < shapea.get_count(); i++)
   //   {

   //      if (i + 1 < shapea.get_count())
   //      {

   //         if (shapea[i + 1]->eshape() == e_shape_intersect_clip)
   //         {

   //            switch (shapea[i]->eshape())
   //            {
   //            //case e_shape_rect:
   //            //   intersect_clip(shapea[i]->shape < ::rectangle_i32>());
   //            //   break;
   //            case e_shape_rectangle:
   //               intersect_clip(shapea[i]->shape < ::rectangle >());
   //               break;
   //            //case e_shape_oval:
   //            //   intersect_clip(shapea[i]->shape < ::oval>());
   //            //   break;
   //            case e_shape_ellipse:
   //               intersect_clip(shapea[i]->shape < ::ellipse >());
   //               break;
   //            //case e_shape_polygon:
   //            //   intersect_clip(shapea[i]->shape < ::polygon_i32>());
   //            //   break;
   //            case e_shape_polygon:
   //               intersect_clip(shapea[i]->shape < ::polygon >());
   //               break;

   //            }

   //            i++;

   //         }

   //      }

   //   }

   //   //return ::success;

   //}


   //void graphics::intersect_clip(const ::rectangle_f64 & rectangle)
   //{

   //   // ::draw2d::device_lock devicelock(this);

   //   {

   //      auto pregion = __create < ::draw2d::region > ();

   //      auto rectangleClip = rectangle_i32 + m_pointAddShapeTranslate;

   //      pregion->create_rect(rectangleClip);

   //      D2D1::Matrix3x2F m = {};

   //      m_prendertarget->GetTransform(&m);

   //      m_pstate->m_sparegionClip.add(pregion);

   //      m_pstate->m_maRegion.add(m);

   //      ID2D1Geometry* pgeometry = (ID2D1Geometry*)pregion->get_os_data(this);

   //      auto layerparameters = D2D1::LayerParameters(
   //         D2D1::InfiniteRect(),
   //         pgeometry);

   //      m_prendertarget->PushLayer(layerparameters, nullptr);

   //   }

   //   return 0;

   //}



   void graphics::_intersect_clip()
   {

   }
   

   void graphics::_add_clipping_shape(const ::rectangle_f64 & rectangle, ___shape<::draw2d::region > & shaperegion)
   {

      //::draw2d::lock draw2dlock;

      // ::draw2d::device_lock devicelock(this);

      if (!shaperegion.holdee())
      {

         auto pregion = __create < ::draw2d::region >();

         auto rectangleClip = rectangle + m_pointAddShapeTranslate;

         pregion->create_rectangle(rectangleClip);

         shaperegion.holdee(pregion);

      }

      //D2D1::Matrix3x2F m = {};

      //m_prendertarget->GetTransform(&m);

      //m_pstate->m_sparegionClip.add(pregion);

      //m_pstate->m_maRegion.add(m);

      ID2D1Geometry* pgeometry = (ID2D1Geometry*)shaperegion.holdee()->get_os_data(this);

      _push_layer(pgeometry);

      //return 0;


   }




   //void graphics::intersect_clip(const ::oval& oval)
   //{

   //   // ::draw2d::device_lock devicelock(this);

   //   {

   //      auto pregion = __create < ::draw2d::region > ();

   //      pregion->create_oval(oval);

   //      D2D1::Matrix3x2F m = {};

   //      m_prendertarget->GetTransform(&m);

   //      m_pstate->m_sparegionClip.add(pregion);

   //      m_pstate->m_maRegion.add(m);

   //      ID2D1Geometry* pgeometry = (ID2D1Geometry*)pregion->get_os_data(this);

   //      m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), nullptr);

   //   }

   //   return 0;


   //}


   void graphics::_add_clipping_shape(const ::ellipse & ellipse, ___shape<::draw2d::region > & shaperegion)
   {

      //::draw2d::lock draw2dlock;

      // ::draw2d::device_lock devicelock(this);

      {

         if (!shaperegion.holdee())
         {

            auto pregion = __create < ::draw2d::region >();

            auto rectangleClip = ellipse + m_pointAddShapeTranslate;

            pregion->create_ellipse(ellipse);

            shaperegion.holdee(pregion);

         }

         D2D1::Matrix3x2F m = {};

         m_prendertarget->GetTransform(&m);

         //m_pstate->m_sparegionClip.add(pregion);

         //m_pstate->m_maRegion.add(m);

         ID2D1Geometry* pgeometry = (ID2D1Geometry*)shaperegion.holdee()->get_os_data(this);

         //m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), nullptr);

         _push_layer(pgeometry);

      }

      //return 0;


   }


   //void graphics::intersect_clip(const ::polygon_i32& polygon_i32)
   //{

   //   // ::draw2d::device_lock devicelock(this);

   //   {

   //      auto pregion = __create < ::draw2d::region > ();

   //      pregion->create_polygon(polygon_i32.get_data(), (::i32) polygon_i32.get_count(), ::draw2d::e_fill_mode_winding);

   //      D2D1::Matrix3x2F m = {};

   //      m_prendertarget->GetTransform(&m);

   //      m_pstate->m_sparegionClip.add(pregion);

   //      m_pstate->m_maRegion.add(m);

   //      ID2D1Geometry* pgeometry = (ID2D1Geometry*)pregion->get_os_data(this);

   //      m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), nullptr);

   //   }

   //   return 0;


   //}


   void graphics::_add_clipping_shape(const ::polygon_f64& polygon_i32, ___shape<::draw2d::region > & shaperegion)
   {

      //::draw2d::lock draw2dlock;

      // ::draw2d::device_lock devicelock(this);

      {

         if (!shaperegion.holdee())
         {

            auto pregion = __create < ::draw2d::region >();

            pregion->m_pointOffset = m_pointAddShapeTranslate;

            pregion->create_polygon(polygon_i32.data(), (::i32)polygon_i32.get_count(), ::draw2d::e_fill_mode_winding);

            shaperegion.holdee(pregion);

         }

         D2D1::Matrix3x2F m = {};

         m_prendertarget->GetTransform(&m);

         //m_pstate->m_sparegionClip.add(pregion);

         //m_pstate->m_maRegion.add(m);

         ID2D1Geometry* pgeometry = (ID2D1Geometry*)shaperegion.holdee()->get_os_data(this);

         _push_layer(pgeometry);

         //m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), nullptr);

      }

      //return 0;


   }


   void graphics::intersect_clip(const ::rectangle_f64 & rectangle)
   {

      auto r = rectangle + m_pointAddShapeTranslate;

      D2D1_RECT_F rf;

      copy(rf, r);

      auto layerparameters = D2D1::LayerParameters(rf);

      m_prendertarget->PushLayer(layerparameters, nullptr);

      m_iLayerCount++;

   }


   //void graphics::

   //   }
   //   else
   //   {

   //      D2D1::Matrix3x2F m;

   //      m_prendertarget->GetTransform(&m);

   //      m_pstate->m_sparegionClip.add(pregion);

   //      m_pstate->m_maRegion.add(m);

   //      auto pgeometry = pregion->get_os_data < ID2D1Geometry * >(this);

   //      m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(),pgeometry),nullptr);

   //   }

   //   return 0;

   //   //throw ::exception(todo);


   //   //if(pregion == nullptr)
   //   //{
   //   //   m_prendertarget->ResetClip();
   //   //}
   //   //else
   //   //{
   //   //   m_prendertarget->SetClip((Gdiplus::Region *) pregion->get_os_data());
   //   //}

   //   //return 0;

   //   ///*      double dRetVal = ERROR;
   //   //if(get_handle1() != nullptr && get_handle1() != get_handle2())
   //   //nRetVal = ::SelectClipRgn(get_handle1(), pRgn == nullptr ? nullptr : (HRGN) pRgn->get_os_data());
   //   //if(get_handle2() != nullptr)
   //   //nRetVal = ::SelectClipRgn(get_handle2(), pRgn == nullptr ? nullptr : (HRGN) pRgn->get_os_data());
   //   //return nRetVal;*/
   //}

   //int graphics::ExcludeClipRect(double x1, double y1, double x2, double y2)
   //{

   //   // ::draw2d::device_lock devicelock(this);

   //   {

   //      ::u32 uMax = m_pdevicecontext->GetMaximumBitmapSize();

   //      ::draw2d::region_pointer regionBig(e_create);

   //      regionBig->create_rect_dim(-(int) uiMax/2, -(int)uiMax / 2, uiMax / 2, uiMax / 2);

   //      ::draw2d::region_pointer regionSmall(e_create);

   //      regionSmall->create_rect_dim(x1, y1, x2, y2);

   //      ::draw2d::region_pointer regionExclude(e_create);

   //      regionExclude->combine(regionBig, regionSmall, ::draw2d::e_combine_exclude);

   //      D2D1::Matrix3x2F m;

   //      m_prendertarget->GetTransform(&m);

   //      m_pstate->m_sparegionClip.add(regionExclude);

   //      m_pstate->m_maRegion.add(m);

   //      auto pgeometry = regionExclude->get_os_data < ID2D1Geometry * >(this);

   //      m_prendertarget->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), nullptr);

   //   }

   //   return 0;

   //}

   //int graphics::ExcludeClipRect(const ::rectangle_i32 & rectangle)
   //{

   //   return ExcludeClipRect(rectangle.left, rectangle.top, rectangle.right, rectangle.bottom);

   //}



   //int graphics::OffsetClipRgn(double x, double y)
   //{

   //   throw ::exception(todo);

   //}


   //int graphics::OffsetClipRgn(const ::size_f64& ::size_f64)
   //{

   //   throw ::exception(todo);

   //}


   ::u32 graphics::SetTextAlign(::u32 nFlags)
   {

      throw ::exception(todo);

      return 0;

   }


   //int graphics::SetTextJustification(double dBreakExtra, double dBreakCount)
   //{

   //   throw ::exception(todo);

   //}


   //int graphics::SetTextCharacterExtra(double dCharExtra)
   //{

   //   throw ::exception(todo);

   //}


   //u32 graphics::SetMapperFlags(u32 dwFlag)
   //{

   //   throw ::exception(todo);

   //}


   typedef u32 (CALLBACK* __GDIGETLAYOUTPROC)(HDC);
   typedef u32 (CALLBACK* __GDISETLAYOUTPROC)(HDC, u32);


   void graphics::apply_primitive_blend()
   {

      if (m_ealphamodeDevice != m_ealphamode)
      {

         m_ealphamodeDevice = m_ealphamode;

         if (m_ealphamodeDevice == ::draw2d::e_alpha_mode_blend)
         {

            m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);

         }
         else if (m_ealphamodeDevice == ::draw2d::e_alpha_mode_set)
         {

            m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);

         }

      }

   }


   void graphics::apply_text_rendering_hint()
   {

      try
      {

         if (m_prendertarget == nullptr)
         {

            return;

         }

         m_etextrenderinghintDevice = m_ewritetextrendering;

         switch (m_etextrenderinghintDevice)
         {
         case ::write_text::e_rendering_anti_alias:

            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

            break;

         case ::write_text::e_rendering_anti_alias_grid_fit:
            
            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

            break;

         case ::write_text::e_rendering_single_bit_per_pixel:
            
            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

            break;

         case ::write_text::e_rendering_clear_type_grid_fit:

            m_prendertarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

            break;

         }

      }
      catch (...)
      {

      }

   }


   void graphics::defer_text_primitive_blend()
   {

      if (m_ealphamodeDevice != ::draw2d::e_alpha_mode_blend)
      {

         m_ealphamodeDevice = m_ealphamode;

         m_pdevicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);

      }

   }


   //oswindow graphics::get_window_handle()
   //{

   //   if (::is_null(m_puserinteraction))
   //   {

   //      return nullptr;

   //   }

   //   return m_puserinteraction->get_oswindow();

   //}


   u32 graphics::GetLayout()
   {

      throw ::exception(todo);

      return 0;

   }


   u32 graphics::SetLayout(u32 dwSetLayout)
   {

      throw ::exception(todo);

      return 0;

   }


   //void graphics::arc_to(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
   //{

   //   throw ::exception(todo);

   //   //return false;

   //}


   //int graphics::SetArcDirection(double dArcDirection)
   //{

   //   throw ::exception(todo);

   //}


   void graphics::polydraw(const ::point_f64* ppoints, const byte* lpTypes, count nCount)
   {

      throw ::exception(todo);

      //return false;

   }


   void graphics::polyline_to(const ::point_f64* ppoints, count nCount)
   {

      throw ::exception(todo);

      //return false;

   }


   void graphics::poly_bezier_to(const ::point_f64* ppoints, count nCount)
   {

      throw ::exception(todo);

      //return false;

   }


   void graphics::SelectClipPath(int nMode)
   {

      //throw ::exception(todo);

      //return false;

   }


   //int graphics::SelectClipRgn(::draw2d::region* pRgn, ::draw2d::enum_combine ecombine)
   //{

   //   return 0;

   //}

   /////////////////////////////////////////////////////////////////////////////
   // Special handling for metafile playback
#ifdef WINDOWS_DESKTOP
   int CALLBACK __enum_meta_file_procedure(HDC hDC,
                                           HANDLETABLE* pHandleTable, METARECORD* pMetaRec, double dHandles, LPARAM lParam)
   {
      
      ::draw2d::graphics * pgraphics = (::draw2d::graphics *)lParam;

      ASSERT_VALID(pgraphics);

      switch (pMetaRec->rdFunction)
      {
      // these records have effects different for each graphics derived class
      case META_SETMAPMODE:
         //(dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SetMapMode((int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETWINDOWEXT:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->set_window_ext(
         (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETWINDOWORG:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SetWindowOrg(
         (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETVIEWPORTEXT:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->set_extents(
         (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETVIEWPORTORG:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->set_origin(
         (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SCALEWINDOWEXT:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->scale_window_ext(
         (int)(short)pMetaRec->rdParm[3], (int)(short)pMetaRec->rdParm[2],
         (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SCALEVIEWPORTEXT:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->scale_extents(
         (int)(short)pMetaRec->rdParm[3], (int)(short)pMetaRec->rdParm[2],
         (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_OFFSETVIEWPORTORG:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->offset_origin(
         (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SAVEDC:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SaveDC();
         break;
      case META_RESTOREDC:
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->RestoreDC((int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETBKCOLOR:
      {
         
         auto pbrush = pgraphics->__create < ::draw2d::brush >();
         
         pbrush->create_solid(*(UNALIGNED ::color::color*)&pMetaRec->rdParm[0]);
         
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->set(pbrush);

      }
      break;
      case META_SETTEXTCOLOR:
      {
         auto pbrush = pgraphics->__create < ::draw2d::brush >();
         pbrush->create_solid(*(UNALIGNED ::color::color*)&pMetaRec->rdParm[0]);
         (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->set(pbrush);
      }
      break;

      // need to watch out for SelectObject(HFONT), for custom font mapping
      case META_SELECTOBJECT:
      {
         HGDIOBJ hObject = pHandleTable->objectHandle[pMetaRec->rdParm[0]];
         ::u32 nObjType = GetObjectType(hObject);
         if (nObjType == 0)
         {
            // object type is unknown, determine if it is a font
            HFONT hStockFont = (HFONT)::GetStockObject(SYSTEM_FONT);
            //HFONT hFontOld = (HFONT)::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->get_handle1(), hStockFont);
            //HGDIOBJ hObjOld = ::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->get_handle1(), hObject);
            //if (hObjOld == hStockFont)
            //{
            //   // got the stock object back, so must be selecting a font
            //   throw ::not_implemented();
            //   //                  (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SelectObject(::draw2d_direct2d::font::from_handle_dup(pgraphics->get_application(), (HFONT)hObject));
            //   break;  // don't play the default record
            //}
            //else
            //{
            //   // didn't get the stock object back, so restore everything
            //   ::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->get_handle1(), hFontOld);
            //   ::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->get_handle1(), hObjOld);
            //}
            // and fall through to PlayMetaFileRecord...
         }
         else if (nObjType == OBJ_FONT)
         {
            // play back as graphics::SelectObject(::write_text::font*)
            //               (dynamic_cast<::draw2d_direct2d::graphics * >(pgraphics))->SelectObject(::draw2d_direct2d::font::from_handle_dup(pgraphics->get_application(), (HFONT)hObject));
            throw ::not_implemented();
            break;  // don't play the default record
         }
      }
      // fall through...

      default:
         //::PlayMetaFileRecord(hDC, pHandleTable, pMetaRec, nHandles);
         break;
      }

      return 1;
   }
#endif


   //void graphics::PlayMetaFile(HMETAFILE hMF)
   //{

   //   throw ::exception(todo);

   //}


   void graphics::LPtoDP(::size_f64 * psize)
   {

      ASSERT(is_memory_segment_ok(psize, sizeof(::size_f64)));

      ::size_f64 sizeWinExt = GetWindowExt();
      ::size_f64 sizeVpExt = get_extents();
      psize->cx = psize->cx * abs(sizeVpExt.cx) / abs(sizeWinExt.cx);
      psize->cy = psize->cy * abs(sizeVpExt.cy) / abs(sizeWinExt.cy);

   }


   void graphics::DPtoLP(::size_f64 * psize)
   {
   
      ASSERT(is_memory_segment_ok(psize, sizeof(::size_f64)));

      ::size_f64 sizeWinExt = GetWindowExt();
      ::size_f64 sizeVpExt = get_extents();
      psize->cx = psize->cx * abs(sizeWinExt.cx) / abs(sizeVpExt.cx);
      psize->cy = psize->cy * abs(sizeWinExt.cy) / abs(sizeVpExt.cy);

   }


   //void graphics::draw_text(const ::string & lpszString, strsize nCount, const ::rectangle_i32 & rectangle, const ::e_align & ealign, const ::e_draw_text & edrawtext)
   //{

   //   if (nCount < 0)
   //   {

   //      nCount = strlen(lpszString) + nCount + 1;

   //   }

   //   return draw_text(string(lpszString, nCount), rectangle, nFormat);

   //}


   void graphics::draw_text(const ::string & str,const ::rectangle_f64 & rectangle, const ::e_align & ealign, const ::e_draw_text & edrawtext)
   {

      if (str.is_empty())
      {

         //return true;

         return;

      }

      if (str.is_empty())
      {

         //return false;

         throw ::exception(error_invalid_empty_argument);

      }

      if (m_pfont.is_null())
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      if (m_pbrush.is_null())
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      ID2D1Brush* pbrush = m_pbrush->get_os_data < ID2D1Brush * >(this);

      if (::is_null(pbrush))
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      IDWriteTextFormat* pfont = m_pfont->get_os_data < IDWriteTextFormat * >(this);

      if (::is_null(pfont))
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      if (ealign & e_align_right)
      {

         pfont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

      }
      else if (ealign & e_align_horizontal_center)
      {

         pfont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

      }
      else
      {

         pfont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

      }

      if (ealign & e_align_bottom)
      {

         pfont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);

      }
      else if (ealign & e_align_vertical_center)
      {

         pfont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

      }
      else
      {

         pfont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

      }

      auto& text = m_pfont->m_mapText[str];

      if (text.m_wstr.is_empty())
      {

         text.m_wstr = str;

      }

      defer_text_primitive_blend();

      defer_text_rendering_hint();

      if (m_pfont->m_dFontWidth == 1.0)
      {

         D2D1_RECT_F rectangle_f32 = D2D1::RectF((FLOAT)rectangle.left, (FLOAT)rectangle.top, (FLOAT)rectangle.right, (FLOAT)rectangle.bottom);

         m_prendertarget->DrawText(text.m_wstr, (::u32)text.m_wstr.length(), pfont, &rectangle_f32, pbrush);

      }
      else
      {
         D2D1::Matrix3x2F m;

         m_prendertarget->GetTransform(&m);

         D2D1::Matrix3x2F mOriginal(m);

         D2D1_RECT_F rectangle_f32 = D2D1::RectF((FLOAT)0, (FLOAT)0, (FLOAT)width(rectangle), (FLOAT)height(rectangle));

         m = m * D2D1::Matrix3x2F::Translation((FLOAT)rectangle.left, (FLOAT)rectangle.top);

         m = m * D2D1::Matrix3x2F::Scale((FLOAT)m_pfont->m_dFontWidth, 1.f);

         m_prendertarget->SetTransform(&m);

         m_prendertarget->DrawText(text.m_wstr, (::u32)text.m_wstr.length(), pfont, &rectangle_f32, pbrush);

         m_prendertarget->SetTransform(&mOriginal);

      }

      //return 1;

   }


   //size_f64 graphics::get_text_extent(const ::scoped_string & scopedstr, strsize iIndex)
   //{

   //   size_f64 sz;

   //   //bool bOk = 
   //   
   //   get_text_extent(sz, lpszString, nCount, iIndex);

   //   //if (!bOk)
   //   //{

   //   //   return ::size_f64(0, 0);

   //   //}
   //   //else
   //   {

   //      return ::size_f64(sz.cx, sz.cy);

   //   }

   //}


   //size_f64 graphics::get_text_extent(const block & block)
   //{

   //   size_f64 s;

   //   get_text_extent(s, (const char*)block.get_data(), block.get_size());

   //   //if (!get_text_extent(s, (const char *) block.get_data(), block.get_size()))
   //   //{

   //   //   return ::size_f64(0, 0);

   //   //}

   //   return s;

   //}


   //::size_f64 graphics::GetOutputTextExtent(const char * lpszString, strsize nCount)
   //{

   //   throw ::exception(todo);

   //   return nullptr;

   //}


   //::size_f64 graphics::GetOutputTextExtent(const ::string & str)
   //{

   //   throw ::exception(todo);

   //   return nullptr;

   //}


   //void graphics::get_text_extent(size_f64 & size, const char * lpszString, strsize nCount, strsize iIndex)
   size_f64 graphics::get_text_extent(const ::scoped_string & scopedstr, strsize iIndex)
   {

      //if (iIndex <= 0)
      //{

      //   size.cx = 0;

      //   size.cy = 0;

      //   //return true;

      //   return;

      //}

      //string str;

      //if (nCount < 0)
      //{

      //   nCount = strlen(lpszString) + nCount + 1;

      //}

      if (iIndex < 0)
      {

         iIndex = scopedstr.size() + iIndex + 1;

      }

      if (iIndex > scopedstr.size())
      {

         iIndex = scopedstr.size();

      }

      if (iIndex <= 0)
      {

         return {};

      }

      auto range = scopedstr(0, iIndex);

      if (m_pfont.is_null())
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      auto pfont = m_pfont->get_os_data < IDWriteTextFormat * >(this);

      if (pfont == nullptr)
      {

         //return false;
         throw ::exception(error_null_pointer);

      }

      auto& text = m_pfont->m_mapText[range];

      if (text.m_bSize)
      {

         return text.m_size;

      }

      if (text.m_wstr.is_empty())
      {

         text.m_wstr = range;

      }

      ::size_f64 size;

      comptr<IDWriteTextLayout> playout1;

      HRESULT hr;

      comptr<IDWriteTextLayout> playout;

      ::u32 uLength = (::u32)text.m_wstr.length();

      hr = ::direct2d::direct2d()->dwrite_factory()->CreateTextLayout(
           text.m_wstr,                // The string to be laid out and formatted.
           uLength,   // The length of the string.
           pfont,    // The text format to apply to the string (contains font information, etc).
           1024.f * 1024.f,               // The width of the on_layout box.
           1024.f * 1024.f,        // The height of the on_layout box.
           &playout  // The IDWriteTextLayout interface pointer.
           );

      if(playout == nullptr || FAILED(hr))
      {

         size.cx = 0;

         size.cy = 0;

         //return false;

         throw ::exception(error_null_pointer);

      }

      DWRITE_TEXT_METRICS m;

      playout->GetMetrics(&m);

      size.cx = (::i32) (m.widthIncludingTrailingWhitespace  * m_pfont->m_dFontWidth);

      size.cy = (::i32) m.height;

      text.m_size = size;

      text.m_bSize = true;

      return size;

   }


   //void graphics::get_text_extent(size_f64 & size, const char * lpszString, strsize nCount)
   size_f64 graphics::get_text_extent(const ::scoped_string & scopedstr)
   {

      //if (nCount < 0)
      //{

      //   nCount = strlen(lpszString) + nCount + 1;

      //}

      //return get_text_extent(size, lpszString, nCount, nCount);

      return get_text_extent(scopedstr, scopedstr.size());

   }


   //void graphics::get_text_extent(size_f64 & size, const ::string & str)
   //{

   //   return get_text_extent(size, str, str.length());

   //}


   void graphics::fill_rectangle(const ::rectangle_f64 & rectangleParam, const ::color::color & color)
   {

      if (!m_pdevicecontext)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      D2D1_COLOR_F d2d1color;

      copy(d2d1color, color);

      comptr< ID2D1SolidColorBrush> psolidbrush;

      m_prendertarget->CreateSolidColorBrush(d2d1color, &psolidbrush);

      D2D1_RECT_F rectangle;

      copy(rectangle, rectangleParam);

      defer_primitive_blend();

      m_pdevicecontext->FillRectangle(&rectangle, psolidbrush);

   }


   //void graphics::text_out(double x, double y, const block & block)
   //{

   //   if (block.is_empty())
   //   {

   //      return false;

   //   }

   //   return ::draw2d::graphics::text_out((double)x, (double)y, string(lpszString, nCount));

   //}


   void graphics::TextOutRaw(double x, double y, const ::scoped_string & scopedstr)
   {

      if (scopedstr.is_empty())
      {

         return;
         ////return false;

         //throw ::exception(error_invalid_empty_argument);

      }

      if (m_pfont.is_null())
      {

         //return false;

         throw ::exception(error_wrong_state);

      }

      IDWriteTextFormat* pfont = m_pfont->get_os_data < IDWriteTextFormat * >(this);

      if (pfont == nullptr)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      if (m_pbrush.is_null())
      {

         throw ::exception(error_null_pointer);

      }

      ID2D1Brush* pbrush = m_pbrush->get_os_data < ID2D1Brush * >(this);

      if (::is_null(pbrush))
      {

         throw ::exception(error_null_pointer);

      }

      D2D1::Matrix3x2F m;

      D2D1::Matrix3x2F mOriginal;

      auto & text = m_pfont->m_mapText[scopedstr];

      ::size_f64 sizeText;
         
      if (text.m_bSize)
      {

         sizeText = text.m_size;

      }
      else
      {

         sizeText = get_text_extent(scopedstr);

      }

      D2D1_RECT_F rectangle_f32 = D2D1::RectF((FLOAT) 0, (FLOAT)0, (FLOAT)(0 + sizeText.cx * 2), (FLOAT)(0 + sizeText.cy * 2));

      HRESULT  hr = pfont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

      if(FAILED(hr))
      {

         WARNING("text_out, SetTextAlignment" << hresult_text(hr));

      }

      hr = pfont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

      if(FAILED(hr))
      {

         WARNING("text_out, SetTextAlignment" << hresult_text(hr));

      }

      DWRITE_TRIMMING trim;

      zero(trim);

      trim.granularity = DWRITE_TRIMMING_GRANULARITY_NONE;

      hr = pfont->SetTrimming(&trim, nullptr);

      if (FAILED(hr))
      {

         WARNING("text_out, SetTextAlignment" << hresult_text(hr));

      }

      hr = pfont->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

      if (m_pfont->m_dFontWidth != 1.0)
      {

         m_prendertarget->GetTransform(&mOriginal);

         m = mOriginal;

         m._11 *= (FLOAT)m_pfont->m_dFontWidth;
         m._31 += (FLOAT)x;
         m._32 += (FLOAT)y;

      }
      else
      {

         rectangle_f32.left += (FLOAT)x;
         rectangle_f32.top += (FLOAT)y;
         rectangle_f32.right += (FLOAT)x;
         rectangle_f32.bottom += (FLOAT)y;

      }

      if (m_pfont->m_dFontWidth != 1.0)
      {

         m_prendertarget->SetTransform(&m);

      }

      const ::wide_character * lpcwsz = text.m_wstr;

      strsize uiLen = text.m_wstr.length();

      defer_text_primitive_blend();

      defer_text_rendering_hint();

      if(lpcwsz != nullptr && uiLen > 0)
      {

         m_prendertarget->DrawText(lpcwsz,(int) uiLen,pfont,&rectangle_f32,pbrush);

      }

      if (m_pfont->m_dFontWidth != 1.0)
      {

         m_prendertarget->SetTransform(mOriginal);

      }

      //return true;

   }


   void graphics::draw_line(double x1, double y1, double x2, double y2, ::draw2d::pen * ppen)
   {

      D2D1_POINT_2F p1;

      p1.x = (FLOAT) x1;

      p1.y = (FLOAT) y1;

      D2D1_POINT_2F p2;

      p2.x = (FLOAT) x2;

      p2.y = (FLOAT) y2;

      ID2D1Brush * pbrush = m_ppen->get_os_data < ID2D1Brush * >(this);

      if (pbrush == nullptr)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      defer_primitive_blend();

      m_prendertarget->DrawLine(p1, p2, pbrush, (FLOAT)(dynamic_cast < ::draw2d_direct2d::pen * > (m_ppen.m_p))->m_dWidth);

      m_point.x = x2;

      m_point.y = y2;

      //return true;

   }


   void graphics::line_to(double x, double y)
   {

      D2D1_POINT_2F p1;

      copy(p1, m_point);

      D2D1_POINT_2F p2;

      p2.x = (FLOAT) x;

      p2.y = (FLOAT) y;

      ID2D1Brush * pbrush =  m_ppen->get_os_data < ID2D1Brush * >(this);

      if (pbrush == nullptr)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      defer_primitive_blend();

      FLOAT fWidth = (FLOAT)(dynamic_cast <::draw2d_direct2d::pen *> (m_ppen.m_p))->m_dWidth;

      m_prendertarget->DrawLine(p1, p2, pbrush, fWidth);

      m_point.x = x;

      m_point.y = y;

      //return true;

   }


   void graphics::on_begin_draw()
   {

      reset_clip();

      ::draw2d::graphics::on_begin_draw();

      m_ealphamodeDevice = ::draw2d::e_alpha_mode_none;

      set_alpha_mode(::draw2d::e_alpha_mode_blend);

      //return true;

   }


   void graphics::on_end_draw(oswindow wnd)
   {

      ::draw2d::graphics::on_end_draw(wnd);

      if (m_iLayerCount > 0)
      {

         WARNING("Layers left to pop on end draw!");

         _pop_all_layers();

      }

   }


   void graphics::set_alpha_mode(::draw2d::enum_alpha_mode ealphamode)
   {

      if (m_ealphamode != ealphamode)
      {

        ::draw2d::graphics::set_alpha_mode(ealphamode);

      }

      //return true;

   }


   //HDC graphics::get_handle() const
   //{

   //   throw ::exception(todo);

   //   return

   //}


   //HDC graphics::get_handle1() const
   //{

   //   return get_handle();

   //}


   //HDC graphics::get_handle2() const
   //{

   //   return get_handle();

   //}


   void graphics::attach(void * pdata)
   {

      if(m_pdevicecontext != nullptr)
      {

         m_pdevicecontext = nullptr;

      }

      if(m_prendertarget != nullptr)
      {

         m_prendertarget = nullptr;

      }

      if(m_pbitmaprendertarget != nullptr)
      {

         m_pbitmaprendertarget = nullptr;

      }

      m_pdevicecontext = (ID2D1DeviceContext *) pdata;

      HRESULT hr = m_pdevicecontext.as(m_pdevicecontext1);

      if (FAILED(hr))
      {

         m_pdevicecontext = nullptr;

         m_pdevicecontext1 = nullptr;

         throw ::exception(error_null_pointer);

      }

      hr = m_pdevicecontext.as(m_prendertarget);

      if(FAILED(hr))
      {

         m_pdevicecontext = nullptr;

         m_pdevicecontext1 = nullptr;

         m_prendertarget = nullptr;

         throw ::exception(error_null_pointer);

      }

      hr = m_prendertarget.as(m_pbitmaprendertarget);

      if(FAILED(hr))
      {
         m_pbitmaprendertarget = nullptr;
      }

      m_osdata[data_device_context] = m_pdevicecontext;

      m_osdata[data_render_target] = m_prendertarget;

      //return true;

   }


   void * graphics::detach()
   {

      m_prendertarget = nullptr;

      m_pbitmaprendertarget = nullptr;

      m_osdata[data_device_context] = nullptr;

      m_osdata[data_render_target] = nullptr;

      return m_pdevicecontext.detach();

   }


//#ifdef WINDOWS_DESKTOP
//
//
//   void graphics::attach_hdc(HDC hdc)
//   {
//
//      HWND hwnd = ::WindowFromDC(hdc);
//
//      if (hwnd == nullptr)
//      {
//
//         return false;
//
//      }
//
//      ::rectangle_i32 rectangleClient;
//
//      ::GetClientRect(hwnd, rectangleClient);
//
//      // Create a DC render target.
//      D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
//                                            D2D1_RENDER_TARGET_TYPE_DEFAULT,
//                                            D2D1::PixelFormat(
//                                            DXGI_FORMAT_B8G8R8A8_UNORM,
//                                            D2D1_ALPHA_MODE_IGNORE),
//                                            0,
//                                            0,
//                                            D2D1_RENDER_TARGET_USAGE_NONE,
//                                            D2D1_FEATURE_LEVEL_DEFAULT
//                                            );
//
//      HRESULT hr = ::direct2d::direct2d()->d2d1_factory1()->CreateDCRenderTarget(&props,&m_pdcrendertarget);
//
//      if (FAILED(hr))
//      {
//
//         return false;
//
//      }
//
//      hr = m_pdcrendertarget->BindDC(hdc, rectangleClient);
//
//      m_pdcrendertarget.As(&m_prendertarget);
//
//      m_hdcAttach = hdc;
//
//      return true;
//
//   }
//
//
//   HDC graphics::detach_hdc()
//   {
//
//      if (m_hdcAttach == nullptr)
//      {
//
//         return nullptr;
//
//      }
//
//      HDC hdc = m_hdcAttach;
//
//      m_hdcAttach = nullptr;
//
//      return hdc;
//   }
//
//#endif


   void graphics::blur(bool bExpand, double dRadius, const ::rectangle_f64 & rectangle)
   {

      throw ::exception(todo);

      //return false;

   }


   double graphics::get_dpix()
   {
      
      return 72.0;

   }


   void graphics::destroy()
   {


      destroy_os_data();
    
      ::draw2d::graphics::destroy();

   }


   void graphics::destroy_os_data()
   {


      // ::draw2d::device_lock devicelock(this);

      _pop_all_layers();

      m_ppathgeometryClip = nullptr;

      m_prendertarget = nullptr;

      m_pdevicecontext = nullptr;

      m_pbitmaprendertarget = nullptr;

      ::draw2d::graphics::destroy_os_data();

   }


   void graphics::clear_current_point()
   {

      //return ::success;

   }


   void graphics::draw(::draw2d::path * ppath)
   {

      return draw(ppath, m_ppen);

   }


   bool graphics::draw(ID2D1PathGeometry * pgeometry, ::draw2d::pen * ppen)
   {

      ::ID2D1Brush * pbrush = ppen->get_os_data < ID2D1Brush * >(this);

      if (pbrush == nullptr)
      {

         return false;

      }

      ::ID2D1StrokeStyle1 * pstrokestyle = ppen->get_os_data < ID2D1StrokeStyle1 * >(this, 1);

      m_prendertarget->DrawGeometry(pgeometry, pbrush, (FLOAT)ppen->m_dWidth, pstrokestyle);

      return true;

   }


   bool graphics::fill(ID2D1PathGeometry * pgeometry, ::draw2d::brush * pbrush)
   {

      if (pbrush->m_ebrush == ::draw2d::e_brush_box_gradient)
      {

         layer layerShape(m_prendertarget, pgeometry);

         double radius = pbrush->m_dRadius;

         double radius2 = radius * 2.0;

         auto w = pbrush->m_size.cx;

         auto h = pbrush->m_size.cy;

         if (radius2 > w || radius2 > h)
         {

            return false;

         }

         ::rectangle_f64 outer(pbrush->m_point, pbrush->m_size);

         ::rectangle_f64 inner(outer);

         inner.deflate(radius);

         ::rectangle_f64 cornerClip(pbrush->m_point, ::size_f64(radius, radius));
         
         ::rectangle_f64 cornerBrush(pbrush->m_point, ::size_f64(radius * 2.0, radius * 2.0));

         auto pstopcollection = _create_simple_full_range_flat_gradient_stop_collection(pbrush->m_color1, pbrush->m_color2);

         {

            //layer layerShape(m_prendertarget, pgeometry);

            auto pbrush = _create_simple_radial_gradient(cornerBrush, pstopcollection);

            m_prendertarget->FillRectangle(
               { (FLOAT)cornerClip.left, (FLOAT)cornerClip.top,
               (FLOAT)cornerClip.right, (FLOAT)cornerClip.bottom },
               pbrush);

         }

         cornerClip.Align(e_align_top_right, outer);

         cornerBrush.Align(e_align_top_right, outer);

         {

            //layer layerShape(m_prendertarget, pgeometry);

            auto pbrush = _create_simple_radial_gradient(cornerBrush, pstopcollection);

            m_prendertarget->FillRectangle(
               { (FLOAT)cornerClip.left, (FLOAT)cornerClip.top,
               (FLOAT)cornerClip.right, (FLOAT)cornerClip.bottom },
               pbrush);

         }

         cornerClip.Align(e_align_bottom_right, outer);

         cornerBrush.Align(e_align_bottom_right, outer);

         {

            //layer layerShape(m_prendertarget, pgeometry);

            auto pbrush = _create_simple_radial_gradient(cornerBrush, pstopcollection);

            m_prendertarget->FillRectangle(
               { (FLOAT)cornerClip.left, (FLOAT)cornerClip.top,
               (FLOAT)cornerClip.right, (FLOAT)cornerClip.bottom },
               pbrush);

         }

         cornerClip.Align(e_align_bottom_left, outer);

         cornerBrush.Align(e_align_bottom_left, outer);

         {

            //layer layerShape(m_prendertarget, pgeometry);

            auto pbrush = _create_simple_radial_gradient(cornerBrush, pstopcollection);

            m_prendertarget->FillRectangle(
               { (FLOAT)cornerClip.left, (FLOAT)cornerClip.top,
               (FLOAT)cornerClip.right, (FLOAT)cornerClip.bottom },
               pbrush);

         }


         cornerClip.Align(e_align_top_right, outer);

         cornerBrush.Align(e_align_top_right, outer);

         {

            //layer layerShape(m_prendertarget, pgeometry);

            auto pd2d1brush = _create_solid_brush(pbrush->m_color1);

            m_prendertarget->FillRectangle(
               { (FLOAT)inner.left, (FLOAT)inner.top,
               (FLOAT)inner.right, (FLOAT)inner.bottom },
               pd2d1brush);

         }

         {

            ::rectangle_f64 side(0, 0, inner.width(), radius);

            side.Align(e_align_top_center, outer);

            {

               auto pbrush = _create_simple_linear_gradient(side.bottom_left(), side.top_left(), pstopcollection);

               m_prendertarget->FillRectangle(
                  { (FLOAT)side.left, (FLOAT)side.top,
                  (FLOAT)side.right, (FLOAT)side.bottom },
                  pbrush);

            }

            side.Align(e_align_bottom_center, outer);

            {

               auto pbrush = _create_simple_linear_gradient(side.top_left(), side.bottom_left(), pstopcollection);

               m_prendertarget->FillRectangle(
                  { (FLOAT)side.left, (FLOAT)side.top,
                  (FLOAT)side.right, (FLOAT)side.bottom },
                  pbrush);

            }

         }


         {

            ::rectangle_f64 side(0, 0, radius, inner.height());

            side.Align(e_align_left_center, outer);

            {

               auto pbrush = _create_simple_linear_gradient(side.top_right(), side.top_left(), pstopcollection);

               m_prendertarget->FillRectangle(
                  { (FLOAT)side.left, (FLOAT)side.top,
                  (FLOAT)side.right, (FLOAT)side.bottom },
                  pbrush);

            }

            side.Align(e_align_right_center, outer);

            {

               auto pbrush = _create_simple_linear_gradient(side.top_left(), side.top_right(), pstopcollection);

               m_prendertarget->FillRectangle(
                  { (FLOAT)side.left, (FLOAT)side.top,
                  (FLOAT)side.right, (FLOAT)side.bottom },
                  pbrush);

            }

         }
         return true;

      }

      ::ID2D1Brush * pd2d1brush = pbrush->get_os_data < ID2D1Brush * >(this);

      if (pd2d1brush == nullptr)
      {

         return false;

      }

      m_prendertarget->FillGeometry(pgeometry, pd2d1brush);

      return true;

   }


   bool graphics::draw(ID2D1GeometryRealization * prealization, ::draw2d::pen * ppen)
   {

      ::ID2D1Brush * pbrush = ppen->get_os_data < ID2D1Brush * >(this);

      if (pbrush == nullptr)
      {

         return false;

      }

      m_pdevicecontext1->DrawGeometryRealization(prealization, pbrush);

      return true;

   }


   bool graphics::fill(ID2D1GeometryRealization * prealization, ::draw2d::brush * pbrush)
   {

      ::ID2D1Brush * pd2d1brush = pbrush->get_os_data < ID2D1Brush * >(this);

      if (pd2d1brush == nullptr)
      {

         return false;

      }

      m_pdevicecontext1->DrawGeometryRealization(prealization, pd2d1brush);

      return true;

   }


   void graphics::draw(::draw2d::path * ppathParam, ::draw2d::pen * ppen)
   {

      scoped_restore(m_bOutline);

      m_bOutline = true;

      ::pointer<class path> ppath = ppathParam;

      ID2D1PathGeometry * pgeometry = ppath->get_os_data < ID2D1PathGeometry * >(this, path_hollow);

      if(pgeometry != nullptr)
      {

         if (ppath && ppath->m_bUseGeometryRealization)
         {

            auto prealization = ppath->_get_stroked_geometry_realization(this,(::i32) ppen->m_dWidth);

            draw(prealization, ppen);

            return;

         }

         defer_primitive_blend();

         draw(pgeometry, ppen);

      }

      //for(index i = 0; i < ppath->m_shapea.get_size(); i++)
      //{

      //   if(ppath->m_shapea[i]->eshape() == e_shape_text_out)
      //   {

      //      draw(ppath->m_shapea[i]->shape < ::write_text::text_out>(), ppen);

      //   }
      //   else if (ppath->m_shapea[i]->eshape() == e_shape_draw_text)
      //   {

      //      draw(ppath->m_shapea[i]->shape < ::write_text::draw_text>(), ppen);

      //   }

      //}

      //return true;

   }


   void graphics::fill(::draw2d::path * ppath)
   {

      fill(ppath, m_pbrush);

      //::pointer<class path> ppath = ppathParam;

      //if (!ppath)
      //{

      //   throw ::exception(error_null_pointer);

      //}

      //if (ppath->is_empty())
      //{

      //   return;

      //}

      //__stack(m_bOutline, false);

      //ID2D1PathGeometry * pgeometry = ppath->get_os_data < ID2D1PathGeometry * >(this, path_filled);

      //if (pgeometry != nullptr)
      //{

      //   if (ppath && ppath->m_bUseGeometryRealization)
      //   {

      //      auto prealization = ppath->_get_filled_geometry_realization(this);

      //      fill(prealization, m_pbrush);

      //      return;

      //   }

      //   defer_primitive_blend();

      //   fill(pgeometry, m_pbrush);

      //}

      ////for (index i = 0; i < ppath->m_shapea.get_size(); i++)
      ////{

      ////   if (ppath->m_shapea[i]->eshape() == ::e_shape_text_out)
      ////   {

      ////      fill(ppath->m_shapea[i]->shape < ::write_text::text_out >(), m_pbrush);

      ////   }
      ////   else if (ppath->m_shapea[i]->eshape() == ::e_shape_draw_text)
      ////   {

      ////      fill(ppath->m_shapea[i]->shape < ::write_text::draw_text >(), m_pbrush);

      ////   }

      ////}

      ////return true;

   }


   void graphics::fill(::draw2d::path * ppathParam, ::draw2d::brush * pbrush)
   {

      ::pointer<class path> ppath = ppathParam;

      if (!ppath)
      {

         throw ::exception(error_null_pointer);

      }

      if (ppath->is_empty())
      {

         return;

      }



      //ID2D1Brush * pbrush = pbrushParam->get_os_data < ID2D1Brush * >(this);

      //if (pbrush == nullptr)
      //{

      //   //return false;

      //   throw ::exception(error_null_pointer);

      //}

      scoped_restore(m_bOutline);

      m_bOutline = false;

      ID2D1PathGeometry * pgeometry = ppath->get_os_data < ID2D1PathGeometry * >(this, path_filled);

      if (pgeometry != nullptr)
      {

         if (ppath && ppath->m_bUseGeometryRealization && pbrush->m_ebrush != ::draw2d::e_brush_box_gradient)
         {

            auto prealization = ppath->_get_filled_geometry_realization(this);

            fill(prealization, m_pbrush);

            return;

         }



         defer_primitive_blend();

         fill(pgeometry, pbrush);

         //m_prendertarget->FillGeometry(pgeometry, pbrush);

      }

      //for (index i = 0; i < ppath->m_shapea.get_size(); i++)
      //{

      //   if (ppath->m_shapea[i]->eshape() == ::e_shape_text_out)
      //   {

      //      fill(ppath->m_shapea[i]->shape < ::write_text::text_out >(), pbrushParam);

      //   }
      //   else if (ppath->m_shapea[i]->eshape() == ::e_shape_draw_text)
      //   {

      //      fill(ppath->m_shapea[i]->shape < ::write_text::draw_text >(), pbrushParam);

      //   }

      //}

      //return true;

   }


   void graphics::path(::draw2d::path * ppath)
   {

      if (ppath == nullptr)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      //bool bOk1 = 
      
      fill(ppath);

      //bool bOk2 = 
      
      draw(ppath);

      //return bOk1 && bOk2;

   }


   bool graphics::draw(const ::write_text::text_out & textout, ::draw2d::pen * ppen)
   {

      //wstring szOutline(textout.m_strText);

      //IDWriteTextFormat * pformat = textout.m_pfont->get_os_data < IDWriteTextFormat * > (this);

      //IDWriteFactory * pfactory = ::direct2d::direct2d()->dwrite_factory();

      //comptr<IDWriteTextLayout> playout;

      //HRESULT hr = pfactory->CreateTextLayout(
      //             szOutline,      // The string to be laid out and formatted.
      //             (::u32) szOutline.length(),  // The length of the string.
      //             pformat,  // The text format to apply to the string (contains font information, etc).
      //             4096,         // The width of the on_layout box.
      //             4096,        // The height of the on_layout box.
      //             &playout  // The IDWriteTextLayout interface pointer.
      //             );

      //if(playout == nullptr)
      //{

      //   return false;

      //}

      //CustomTextRenderer renderer(::direct2d::direct2d()->d2d1_factory1(),m_prendertarget,ppen->get_os_data < ID2D1Brush * >(this));

      //defer_text_primitive_blend();

      //defer_text_rendering_hint();

      //playout->Draw(nullptr, &renderer, (FLOAT) textout.m_point.x, (FLOAT) textout.m_point.y);

      return true;

   }


   bool graphics::fill(const ::write_text::text_out & textout, ::draw2d::brush * pbrush)
   {

      //wstring szOutline(textout.m_strText);

      //IDWriteTextFormat * pformat = textout.m_pfont->get_os_data < IDWriteTextFormat * >(this);

      //IDWriteFactory * pfactory = ::direct2d::direct2d()->dwrite_factory();

      //comptr<IDWriteTextLayout> playout;

      //HRESULT hr = pfactory->CreateTextLayout(
      //             szOutline,      // The string to be laid out and formatted.
      //             (::u32) szOutline.length(),  // The length of the string.
      //             pformat,  // The text format to apply to the string (contains font information, etc).
      //             4096,         // The width of the on_layout box.
      //             4096,        // The height of the on_layout box.
      //             &playout  // The IDWriteTextLayout interface pointer.
      //             );

      //if (playout == nullptr)
      //{
      //   
      //   return false;

      //}

      //auto posbrush = pbrush->get_os_data < ID2D1Brush * >(this);

      //if (posbrush)
      //{

      //   CustomTextRenderer renderer(::direct2d::direct2d()->d2d1_factory1(), m_prendertarget, nullptr, posbrush);

      //   defer_text_primitive_blend();

      //   defer_text_rendering_hint();

      //   playout->Draw(nullptr, &renderer, (FLOAT)textout.m_point.x, (FLOAT)textout.m_point.y);

      //}

      return true;

   }


   bool graphics::draw(const ::write_text::draw_text & drawtext, ::draw2d::pen* ppen)
   {

      //wstring szOutline(drawtext.m_strText);

      //IDWriteTextFormat* pformat = drawtext.m_pfont->get_os_data < IDWriteTextFormat* >(this);

      //IDWriteFactory* pfactory = ::direct2d::direct2d()->dwrite_factory();

      //IDWriteTextLayout* playout = nullptr;

      //HRESULT hr = pfactory->CreateTextLayout(
      //   szOutline,      // The string to be laid out and formatted.
      //   (::u32)szOutline.length(),  // The length of the string.
      //   pformat,  // The text format to apply to the string (contains font information, etc).
      //   4096,         // The width of the on_layout box.
      //   4096,        // The height of the on_layout box.
      //   &playout  // The IDWriteTextLayout interface pointer.
      //);

      //if (playout == nullptr)
      //{

      //   return false;

      //}

      //CustomTextRenderer renderer(::direct2d::direct2d()->d2d1_factory1(), m_prendertarget, ppen->get_os_data < ID2D1Brush* >(this));

      //defer_text_primitive_blend();

      //defer_text_rendering_hint();

      //playout->Draw(nullptr, &renderer, (FLOAT)drawtext.m_rectangle.left, (FLOAT)drawtext.m_rectangle.top);

      return true;

   }


   bool graphics::fill(const ::write_text::draw_text & drawtext, ::draw2d::brush* pbrush)
   {

      //wstring szOutline(drawtext.m_strText);

      //IDWriteTextFormat* pformat = drawtext.m_pfont->get_os_data < IDWriteTextFormat* >(this);

      //IDWriteFactory* pfactory = ::direct2d::direct2d()->dwrite_factory();

      //IDWriteTextLayout* playout = nullptr;

      //HRESULT hr = pfactory->CreateTextLayout(
      //   szOutline,      // The string to be laid out and formatted.
      //   (::u32)szOutline.length(),  // The length of the string.
      //   pformat,  // The text format to apply to the string (contains font information, etc).
      //   4096,         // The width of the on_layout box.
      //   4096,        // The height of the on_layout box.
      //   &playout  // The IDWriteTextLayout interface pointer.
      //);

      //if (playout == nullptr)
      //{

      //   return false;

      //}

      //auto posbrush = pbrush->get_os_data < ID2D1Brush* >(this);

      //if (posbrush)
      //{

      //   CustomTextRenderer renderer(::direct2d::direct2d()->d2d1_factory1(), m_prendertarget, nullptr, posbrush);

      //   defer_text_primitive_blend();

      //   defer_text_rendering_hint();

      //   playout->Draw(nullptr, &renderer, (FLOAT)drawtext.m_rectangle.left, (FLOAT)drawtext.m_rectangle.top);

      //}

      return true;

   }


   void graphics::flush()
   {

      if (!m_bBeginDraw)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      HRESULT hr = m_prendertarget->Flush();

      //return SUCCEEDED(hr);

   }


   void graphics::debug()
   {

#ifdef _DEBUG
      directx_debug();
#endif

   }


   //void graphics::enum_fonts(::write_text::font_enumeration_item_array & itema)
   //{


   //}


   //void graphics::set_direct2d_plugin(::draw2d_direct2d::plugin * pplugin)
   //{

   //   ::direct2d::direct2d() = pplugin;

   //}

   comptr < ID2D1SolidColorBrush > graphics::_create_solid_brush(const ::color::color & color)
   {

      comptr<ID2D1SolidColorBrush> pbrush;

      D2D1_COLOR_F d2d1color;

      copy(d2d1color, color);

      HRESULT hr = m_prendertarget->CreateSolidColorBrush(d2d1color, &pbrush);

      if (FAILED(hr))
      {

         throw hresult_exception(hr);

      }

      return pbrush;

   }

   comptr<ID2D1RadialGradientBrush> graphics::_create_simple_radial_gradient(const ::rectangle_f64 & r, ID2D1GradientStopCollection * pstopcollection)
   {

      comptr<ID2D1RadialGradientBrush> pbrush;

      double centerx = r.center_x();
      double centery = r.center_y();
      double offsetx = 0.0;
      double offsety = 0.0;
      double radiusx = r.width() / 2.0;
      double radiusy = r.height() / 2.0;

      HRESULT hr = m_prendertarget->CreateRadialGradientBrush(
         D2D1::RadialGradientBrushProperties(
            D2D1::Point2F((FLOAT)(centerx), (FLOAT)(centery)),
            D2D1::Point2F((FLOAT)(offsetx), (FLOAT)(offsety)),
            (FLOAT)(radiusx), (FLOAT)(radiusy)),
         pstopcollection,
         &pbrush
      );

      if (FAILED(hr))
      {

         throw hresult_exception(hr);

      }

      return pbrush;

   }


   comptr<ID2D1LinearGradientBrush> graphics::_create_simple_linear_gradient(const ::point_f64 & p1, const ::point_f64 & p2, ID2D1GradientStopCollection * pstopcollection)
   {

      comptr<ID2D1LinearGradientBrush> pbrush;

      D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES prop{};

      ::point_i32 pointViewport(0, 0);

      prop.startPoint.x = (FLOAT)p1.x + pointViewport.x;
      prop.startPoint.y = (FLOAT)p1.y + pointViewport.y;
      prop.endPoint.x = (FLOAT)p2.x + pointViewport.x;
      prop.endPoint.y = (FLOAT)p2.y + pointViewport.y;

      D2D1_BRUSH_PROPERTIES brushproperties = {};

      brushproperties.opacity = 1.0f;
      brushproperties.transform = D2D1::IdentityMatrix();

      HRESULT hr = m_prendertarget->CreateLinearGradientBrush(&prop, &brushproperties, pstopcollection, &pbrush);

      if (FAILED(hr))
      {

         throw hresult_exception(hr);

      }

      return pbrush;

   }



   comptr <ID2D1GradientStopCollection > graphics::_create_simple_full_range_flat_gradient_stop_collection(const ::color::color & color1, const ::color::color & color2)
   {

      // Create an array of gradient stops to put in the gradient stop
      // collection that will be used in the gradient brush.
      comptr <ID2D1GradientStopCollection > pgradientstops;

      D2D1_GRADIENT_STOP gradientstops[2];

      copy(gradientstops[0].color, color1);
      gradientstops[0].position = 0.0f;

      copy(gradientstops[1].color, color2);
      gradientstops[1].position = 1.0f;

      // Create the ID2D1GradientStopCollection from a previously
      // declared array of D2D1_GRADIENT_STOP structs.
      HRESULT hr = m_prendertarget->CreateGradientStopCollection(
         gradientstops,
         2,
         D2D1_GAMMA_2_2,
         D2D1_EXTEND_MODE_CLAMP,
         &pgradientstops
      );

      if (FAILED(hr))
      {

         throw hresult_exception(hr);

      }

      return pgradientstops;

   }





} // namespace draw2d_direct2d





