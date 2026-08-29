#include "platform.h"
#include "bitmap.h"
#include "brush.h"
#include "font.h"
#include "graphics.h"
#include "image.h"
#include "path.h"
#include "pen.h"
#include "region.h"
#include "window_attachment.h"
#include "direct2d/direct2d.h"
#include "direct2d/geometry.h"
#include "CustomRenderer.h"
#include "acme/exception/not_implemented.h"
#include "acme/parallelization/synchronous_lock.h"
#include "acme/platform/node.h"
#include "acme/platform/scoped_restore.h"
#include "acme/prototype/geometry2d/ellipse.h"
#include "aura/windowing/windowing.h"
#include "aura/windowing/display.h"
#include "aura/user/user/user.h"
#include "aura/graphics/draw2d/clip.h"
#include "aura/graphics/draw2d/lock.h"
#include "aura/graphics/draw2d/region.h"
#include "aura/graphics/draw2d/device_lock.h"
#include "aura/graphics/graphics/buffer_item.h"
#include "aura/graphics/image/context.h"
#include "aura/graphics/image/drawing.h"
#include "acme/graphics/image/frame_array.h"
#include "aura/platform/session.h"
#include "aura/windowing/window.h"
//#include "bred/gpu/bred_approach.h"
//#include "bred/gpu/context.h"
//#include "bred/gpu/context_lock.h"
//#include "bred/gpu/device.h"
//#include "bred/gpu/renderer.h"
//#include "bred/gpu/swap_chain.h"
//#include "bred/gpu/types.h"
#include "operating_system-windows_common/direct2d/direct2d.h"
#include <math.h>
#include "acme_windows_common/dxgi_device_source.h"
#include <atomic>


void dpi_initialize(ID2D1Factory * pFactory);


#define d2d1_fax_options D2D1_FACTORY_OPTIONS // fax of merde
#define multi_threaded D2D1_FACTORY_TYPE_MULTI_THREADED // ???? muliple performance multi thread hidden option there exists cost uses?


//#if defined(WINDOWS_DESKTOP
//
//CLASS_DECL_AURA void directx11_debug();
//
//#endif


namespace draw2d_direct2d
{


   // graphics::state::state()
   //{

   //}


   // graphics::state::~state()
   //{

   //}


   graphics::graphics()
   {

      //m_bDraw2dNeedsD3D11onD12 = true;

      m_iLayerCount = 0;

      m_state.m_iLayerIndex = 0;

      m_ealphamodeDevice = ::draw2d::e_alpha_mode_none;

      //clear_os_data();

      m_pthis = this;

      defer_create_synchronization();

      m_bSaveClip = false;

      m_hdcAttach = nullptr;

      m_iType = 0;

      m_d2d1interpolationmode = D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;

      m_d2d1bitmapinterpolationmode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;

   }


   graphics::~graphics() { destroy(); }


   // void graphics::assert_ok() const
   //{

   //   object::assert_ok();

   //}


   // void graphics::dump(dump_context & dumpcontext) const
   //{

   //   object::dump(dumpcontext);

   //   ////dumpcontext << "get_handle1() = " << (::iptr) get_handle1();
   //   ////dumpcontext << "\nm_hAttribDC = " << (::iptr) get_handle2();
   //   //dumpcontext << "\nm_bPrinting = " << m_bPrinting;

   //   //dumpcontext << "\n";

   //}


   // void graphics::IsPrinting()
   //{

   //   return m_bPrinting;

   //}


   // void graphics::CreateDC(const ::scoped_string & scopedstrDriverName, const ::scoped_string & scopedstrDeviceName,
   // const ::scoped_string & scopedstrOutput, const void * lpInitData)
   //{
   //    throw ::exception(todo);
   //    //return Attach(::CreateDC(lpszDriverName, lpszDeviceName, lpszOutput, (const DEVMODE*)lpInitData));
   // }


   // void graphics::CreateIC(const ::scoped_string & scopedstrDriverName, const ::scoped_string & scopedstrDeviceName,
   // const ::scoped_string & scopedstrOutput, const void * lpInitData)
   //{

   //   throw ::exception(todo);

   //}


   void graphics::_create_d2d1_device_context()
   {

      defer_constructø(direct2d()->m_pdxgidevicesource);

      ::cast<::dxgi_device_source> pdxgidevicesource = direct2d()->m_pdxgidevicesource;

      m_pd2d1devicecontext = direct2d()->create_d2d1_device_context(pdxgidevicesource);

      ::defer_throw_hresult(m_pd2d1devicecontext.as(m_pd2d1devicecontext1));

      void * pDataDeviceContext = m_pd2d1devicecontext.m_p;
      //m_osdata[::draw2d_direct2d::data_device_context] = pDataDeviceContext;


      //comptr<ID2D1DeviceContext> pd2dDeviceContext;

      //HRESULT hr = pd2dDevice->CreateDeviceContext(
      //   D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
      //   &pd2dDeviceContext);

      //::defer_throw_hresult(hr);

   }



   void graphics::create_for_window_draw2d(::user::interaction * puserinteraction, const ::i32_size & size)
   {

      m_bForWindowDraw2d = true;

      m_pacmeuserinteractionAffinity = puserinteraction;

      m_sizeTotal2 = size;

      //if (m_pgraphicsbufferitem)
      //{

         constructø(m_pgraphicsbufferitem->m_pimageBufferItem);

         m_pgraphicsbufferitem->m_pimageBufferItem->update_as_render_target(size, puserinteraction, this);

         //create_for_image(m_pgraphicsbufferitem->m_pimageBufferItem);

         m_pdraw2dbitmapTarget = m_pgraphicsbufferitem->m_pimageBufferItem->m_pdraw2dbitmap;

         //::memory memory;

         //m_pdraw2dbitmapTarget->create_bitmap(this, size, memory, nullptr);
         //
         create_bitmap_graphics(m_pdraw2dbitmapTarget);


      //}

   }


   
   void graphics::update_as_image_render_target(::image::image * pimage)
   {

      m_pacmeuserinteractionAffinity = pimage->m_pacmeuserinteractionAffinity;

      ::cast<::draw2d_direct2d::image> pdirect2dimage = pimage;

      auto hdcMemory = pdirect2dimage->m_hdcMemory;

      if (hdcMemory)
      {

         RECT bounds{ 0, 0, pdirect2dimage->width(), pdirect2dimage->height() };

         auto properties = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
            96.0f, 96.0f);

         auto hrCreateDCRenderTarget = direct2d()->d2d1_factory1()->CreateDCRenderTarget(&properties, &m_pd2d1dcrendertarget);

         if (FAILED(hrCreateDCRenderTarget))
         {

            throw hresult_exception(hrCreateDCRenderTarget);

         }

         if (::is_null(pimage->m_pacmeuserinteractionAffinity))
         {

            throw ::exception(error_wrong_state, "graphics::create_for_image : pimage->m_puserinteraction is null");

         }

         auto hrBindDC = m_pd2d1dcrendertarget->BindDC(hdcMemory, &bounds);

         if (FAILED(hrBindDC))
         {

            throw hresult_exception(hrBindDC);

         }

         m_pd2d1dcrendertarget.as(m_pd2d1devicecontext);

         m_pd2d1dcrendertarget.as(m_pd2d1devicecontext);

         m_pd2d1dcrendertarget.as(m_pd2d1devicecontext1);

      }
      else
      {
         
         pimage->update_bitmap_as_render_target(pimage->m_pacmeuserinteractionAffinity, this);

         ::cast < ::draw2d_direct2d::bitmap > pdraw2dbitmap = pimage->m_pdraw2dbitmap;

         pdraw2dbitmap->m_pd2d1devicecontext.as(m_pd2d1devicecontext);

         m_pd2d1devicecontext.as(m_pd2d1devicecontext);

         m_pd2d1devicecontext.as(m_pd2d1devicecontext1);

      }

      //m_osdata[data_device_context] = m_pd2d1devicecontext;

      //m_osdata[data_render_target] = m_pd2d1devicecontext;

      set_ok_flag();

   }


   void graphics::_create_memory_graphics(const ::i32_size & sizeParam, ::acme::user::interaction * pacmeuserinteractionAffinity)
   {

      //constructø(m_pdraw2dbitmap);

      //if (!pacmeuserinteractionAffinity)
      //{

      //   pacmeuserinteractionAffinity = m_pacmeuserinteractionAffinity;

      //}

      if (!m_pacmeuserinteractionAffinity)
      {

         m_pacmeuserinteractionAffinity = pacmeuserinteractionAffinity;

      }

      //m_pdraw2dbitmap->create_bitmap_for_image(
      //   this,
      //   pacmeuserinteractionAffinity);
      ////::draw2d::lock draw2dlock;

      //__UNREFERENCED_PARAMETER(pdraw2dgraphics);
      ////__UNREFERENCED_PARAMETER(pbits);
      ////__UNREFERENCED_PARAMETER(stride);

      ::draw2d::device_lock devicelock(this);

      if (m_pd2d1devicecontext)
      {

         destroy();

      }



      //D2D1_SIZE_F size;

      //size.width = (FLOAT) sizeParam.cx;
      //size.height = (FLOAT) sizeParam.cy;

      //D2D1_SIZE_U sizeu;

      //sizeu.width = sizeParam.cx;
      //sizeu.height = sizeParam.cy;

      //D2D1_PIXEL_FORMAT pixelformat{};

      //pixelformat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
      //pixelformat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
      ////properties.dpiX = 96.0;
      ////properties.dpiY = 96.0;

      //D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS options;

      //options = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE;

      ::cast < ::windowing::window > pwindow;

      if (m_pacmeuserinteractionAffinity)
      {

         pwindow = m_pacmeuserinteractionAffinity->acme_windowing_window();

      }

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

      //auto hrCreateCompatibleRenderTarget = pd2d1devicecontext->CreateCompatibleRenderTarget(
      //   size,
      //   sizeu,
      //   pixelformat,
      //   options,
      //   &m_pd2d1bitmaprendertargetCompatibleMemoryGraphics);

      //if (FAILED(hrCreateCompatibleRenderTarget))
      //{

      //   throw hresult_exception(hrCreateCompatibleRenderTarget, "Failed to create compatible bitmap render target");

      //}

      //D2D1_BITMAP_PROPERTIES1 bitmapproperties1 = {};

      //bitmapproperties1.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;
      //bitmapproperties1.pixelFormat = pixelformat;
      ////props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
      //bitmapproperties1.dpiX = 96.0f;
      //bitmapproperties1.dpiY = 96.0f;
      //bitmapproperties1.colorContext = nullptr;

      //auto hrQueryDeviceContext = m_pd2d1bitmaprendertargetCompatibleMemoryGraphics.as(m_pd2d1devicecontext);
      ////if(ppdata != nullptr)
      //{
      //   // g.m_pdc->CreateBitmap(size, *ppdata, cx * sizeof(::color32_t), props, &m_pdraw2dbitmap);
      //}
      ////else
      ////{
      ////HRESULT hr = ((ID2D1DeviceContext *)pdraw2dgraphics->get_os_data())->CreateBitmap(size, lpBits, stride, props, &m_pbitmap1);
      ////HRESULT hrCreateBitmap = m_pbitmaprendertarget->GetBitmap(&m_pdraw2dbitmap);

      //////}

      ////if (FAILED(hrCreateBitmap))
      ////{

      ////   throw ::exception(error_failed);

      ////}


      ////if (pbits && stride >= size.width * 4)
      ////{

      ////   D2D1_RECT_U r;

      ////   r.left = 0;
      ////   r.top = 0;
      ////   r.right = size.width;
      ////   r.bottom = size.height;

      ////   m_pdraw2dbitmap->CopyFromMemory(&r, pbits, stride);

      ////}


      ////auto hrQueryBitmap1 = m_pdraw2dbitmap.as(m_pbitmap1);

      ////static ::std::atomic<unsigned int> s_uBitmapCreationDiagnosticCount{ 0 };
      ////auto uBitmapCreationDiagnosticCount = s_uBitmapCreationDiagnosticCount.fetch_add(1, ::std::memory_order_relaxed);

      ////if (uBitmapCreationDiagnosticCount < 32
      ////   || FAILED(hrQueryDeviceContext)
      ////   || FAILED(hrQueryBitmap1))
      ////{

      ////   informationf(
      ////      "Direct2DBitmapCreationDiagnostic compatible=0x%08lx queryContext=0x%08lx getBitmap=0x%08lx "
      ////      "queryBitmap1=0x%08lx thread=%lu requested=(%d,%d) sourceContext=%p bitmapTarget=%p "
      ////      "bitmap=%p bitmap1=%p bitmapContext=%p",
      ////      (unsigned long)hrCreateCompatibleRenderTarget,
      ////      (unsigned long)hrQueryDeviceContext,
      ////      (unsigned long)hrCreateBitmap,
      ////      (unsigned long)hrQueryBitmap1,
      ////      (unsigned long)::GetCurrentThreadId(),
      ////      sizeParam.cx,
      ////      sizeParam.cy,
      ////      pd2d1devicecontext,
      ////      (ID2D1BitmapRenderTarget *)m_pbitmaprendertarget,
      ////      (ID2D1Bitmap *)m_pdraw2dbitmap,
      ////      (ID2D1Bitmap1 *)m_pbitmap1,
      ////      (ID2D1DeviceContext *)m_pdevicecontext);

      ////}

      ////if (FAILED(hrQueryDeviceContext))
      ////{

      ////   throw hresult_exception(hrQueryDeviceContext, "Failed to query bitmap device context");

      ////}

      ////if (FAILED(hrQueryBitmap1))
      ////{

      ////   throw hresult_exception(hrQueryBitmap1, "Failed to query ID2D1Bitmap1");

      ////}


      ////m_pdevicecontext->SetTarget(m_pbitmap1);

      ////zero(m_map);
      ////    m_pdraw2dbitmap->Map(D2D1_MAP_OPTIONS_READ | D2D1_MAP_OPTIONS_WRITE, &m_map);
      ////
      ////if(ppdata != nullptr)
      //// *ppdata = (::color::color *) m_map.bits;
      ////m_osdata[0] = m_pdraw2dbitmap;

      ////return true;

      /////set_ok_flag();

      m_estatus = success;


      //::cast < ::draw2d_direct2d::bitmap > pdraw2dbitmap = pimage->m_pdraw2dbitmap;

      //pdraw2dbitmap->m_pdevicecontext.as(m_pdevicecontext);

      m_pd2d1devicecontext.as(m_pd2d1devicecontext);

      m_pd2d1devicecontext.as(m_pd2d1devicecontext1);

   //m_osdata[data_device_context] = m_pd2d1devicecontext;

   //m_osdata[data_render_target] = m_pd2d1devicecontext;

   set_ok_flag();

   }


   //void graphics::create_compatible_graphics(::draw2d::graphics *pdraw2dgraphics)
   //{
   //
   //   _create_memory_graphics({256, 256}, pdraw2dgraphics->m_pacmeuserinteractionAffinity); 
   //
   //}


   ::image::image_pointer graphics::get_current_target_image()
   {

      return m_pgraphicsbufferitem->m_pimageBufferItem;

   }

   void graphics::defer_set_size(const ::i32_size &size)
   {
      if (m_pimage)
      {

         if (m_pimage->m_size == size)
         {

            return;

         }

      }

      //_create_memory_graphics(size);
      /*m_pgpucontextCompositor->sendø() << [this, size]()
         {
            m_pgpucontextCompositor->sendø() << [this, size]()
               {
         });*/
   }


   //void graphics::create_for_window_draw2d(::user::interaction *puserinteraction, const ::i32_size &size)
   //{

   //   ::gpu::graphics::create_for_window_draw2d(puserinteraction, size);

   //   auto pgpuapproach = m_papplication->get_gpu_approach();

   //   auto pgpudevice = pgpuapproach->get_gpu_device(m_puserinteractionDraw2dGraphics->m_pacmewindowingwindow);

   //   direct2d() = ::direct2d::from_gpu_device(pgpudevice);

   //   auto pgpucontextNew = pgpudevice->main_draw2d_context();

   //   pgpucontextNew->m_pgpucompositor = this;

   //   set_gpu_context(pgpucontextNew);

   //   ::cast<::dxgi_device_source> pdxgidevicesource = gpu_context();

   //   m_pdevicecontext = direct2d()->default_d2d1_device_context(pdxgidevicesource);

   //   m_pdevicecontext.as(m_pdevicecontext1);

   //   m_pdevicecontext.as(m_pd2d1devicecontext);

   //   defer_create_swap_chain(puserinteraction);

   //   //// for now create a "fake" memory graphics

   //   // create_memory_graphics({ 1920,1080 });

   //   // m_pgpucontextCompositor->m_iOverrideFrame = 0;
   //   //
   //   // bind_draw2d_compositor();

   //   // m_pgpucontextCompositor->m_iOverrideFrame = -1;

   //   set_ok_flag();

   //   // m_osdata[0] = (void*)1;

   //   ////::draw2d::lock draw2dlock;

   //   //// ::draw2d::device_lock devicelock(this);

   //   // if (m_iType != 0)
   //   //{

   //   //   destroy();

   //   //}

   //   //::user::interaction* puserinteraction = m_puserinteractionDraw2dGraphics;

   //   // if (::is_null(puserinteraction))
   //   //{

   //   //   puserinteraction = dynamic_cast <::user::interaction*>(m_pacmeuserinteractionMain.m_p);

   //   //}

   //   // auto pwindow = puserinteraction->window();

   //   // auto rectanglePlacement = pwindow->get_window_rectangle();

   //   // auto pgpuapproach = m_papplication->get_gpu_approach();

   //   // auto pgpudevice = pgpuapproach->get_gpu_device();

   //   // m_pgpucontextCompositor = pgpudevice->create_draw2d_context(::gpu::e_output_gpu_buffer, size);

   //   /*_create_memory_graphics(size);

   //   m_pgpucontextCompositor->sendø() << [this, size]()
   //      {

   //         m_pgpucontextCompositor->create_offscreen_graphics_for_swap_chain_blitting(this, size);

   //      });*/
   //}


   //void graphics::_create_from_dxgi_surface(::i32 iIndex, ::i32 iLayerIndex, IDXGISurface *pdxgisurface)
   //{

   //   auto &pd2d1rendertarget = m_d2d1rendertargeta.element_at_grow(iIndex).element_at_grow(iLayerIndex);

   //   D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
   //      D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
   //      96.0f, 96.0f);

   //   HRESULT hrCreateDxgiSurfaceRenderTarget =
   //      direct2d()->d2d1_factory1()->CreateDxgiSurfaceRenderTarget(pdxgisurface, &props, &pd2d1rendertarget);

   //   ::defer_throw_hresult(hrCreateDxgiSurfaceRenderTarget);

   //   m_pd2d1devicecontext = pd2d1rendertarget;

   //   ::defer_throw_hresult(m_pd2d1devicecontext.as(m_pdevicecontext));

   //   ::defer_throw_hresult(m_pdevicecontext.as(m_pdevicecontext1));
   //}


   ::draw2d::bitmap * graphics::get_target_bitmap()
   {

      //if (!m_pd2d1bitmaprendertarget)
      //{

      //   throw ::exception(error_wrong_state);

      //}

      constructø(m_pdraw2dbitmapTarget);

      ::cast<::draw2d_direct2d::bitmap> pdraw2dbitmap = m_pdraw2dbitmapTarget;

      //auto hrGetBitmap = m_pd2d1bitmaprendertarget->GetBitmap(&pdraw2dbitmap->m_pd2d1bitmap);

      //if (FAILED(hrGetBitmap))
      //{

      //   throw ::hresult_exception(hrGetBitmap);

      //}

      D2D1_BITMAP_PROPERTIES1 properties{};

      properties.pixelFormat =
         D2D1::PixelFormat(
            DXGI_FORMAT_B8G8R8A8_UNORM,
            D2D1_ALPHA_MODE_PREMULTIPLIED);

      properties.dpiX = 96.0f;
      properties.dpiY = 96.0f;

      properties.bitmapOptions =
         D2D1_BITMAP_OPTIONS_TARGET;

      comptr<ID2D1Bitmap1> pd2d1bitmap;

      HRESULT hr =
         m_pd2d1devicecontext->CreateBitmap(
            D2D1::SizeU((UINT32) m_sizeTotal2.cx,(UINT32)  m_sizeTotal2.cy),
            nullptr,
            0,
            &properties,
            &pd2d1bitmap);

      ::defer_throw_hresult(hr);

      pd2d1bitmap.as(pdraw2dbitmap->m_pd2d1bitmap);

      pdraw2dbitmap->m_pd2d1bitmap.as(pdraw2dbitmap->m_pd2d1bitmap1);

      auto size = pdraw2dbitmap->m_pd2d1bitmap->GetSize();

      pdraw2dbitmap->m_size.cx = (::i32) size.width;

      pdraw2dbitmap->m_size.cy = (::i32) size.height;

      return pdraw2dbitmap;

   }


   void graphics::create_bitmap_graphics(::draw2d::bitmap *pdraw2dbitmap)
   {

      throw ::interface_only();
   //{
   //
   //   auto pcontext = gpu_context();

   //   ::i32_size ;
   //   size.cx = pdraw2dbitmap->m_size.cx;
   //   size.cy = pdraw2dbitmap->m_size.cy;

   //   if (pcontext)
   //   {

   //      if (pcontext->size() == size)
   //      {

   //         return;
   //      }
   //   }

   //   //::draw2d::lock draw2dlock;

   //   // ::draw2d::device_lock devicelock(this);

   //   if (m_iType != 0)
   //   {

   //      destroy();
   //   }

   //   ::user::interaction *puserinteraction = m_puserinteractionDraw2dGraphics;

   //   if (::is_null(puserinteraction))
   //   {

   //      puserinteraction = dynamic_cast<::user::interaction *>(m_pacmeuserinteractionMain.m_p);
   //   }

   //   auto pwindow = puserinteraction->window();

   //   auto rectanglePlacement = pwindow->get_window_rectangle();

   //   auto pgpuapproach = m_papplication->get_gpu_approach();

   //   ::acme::windowing::window *pacmewindowingwindow = nullptr;

   //   if (::is_set(m_puserinteractionDraw2dGraphics))
   //   {

   //      pacmewindowingwindow = m_puserinteractionDraw2dGraphics->m_pacmewindowingwindow;
   //   }

   //   if (::is_null(pacmewindowingwindow))
   //   {

   //      pacmewindowingwindow = m_pacmeuserinteractionMain->m_pacmewindowingwindow;
   //   }

   //   auto pgpudevice = pgpuapproach->get_gpu_device(pacmewindowingwindow);

   //   auto pgpucontextNew = pgpudevice->create_draw2d_context(::gpu::e_output_gpu_buffer, size);

   //   set_gpu_context(pgpucontextNew);

   //   pcontext = gpu_context();

   //   {

   //      ::gpu::context_lock context_lock(pcontext);

   //      pcontext->m_pgpucompositor = this;

   //      auto pdirect2d = ::direct2d::from_gpu_device(pcontext->m_pgpudevice);

   //      initialize_direct2d_object(pdirect2d);
   //   }

   //   // auto pgpucontext = pgpudevice->get_main_context();

   //   // m_pgpucontextDraw2d->m_pgpurenderer = pgpucontext->get_output_renderer();
   //   //{

   //   //   m_pgpucontextCompositor->start_gpu_context(
   //   //      ::gpu::start_gpu_output_context_t
   //   //      {
   //   //         this,
   //   //         pdevice,
   //   //         ::gpu::e_output_gpu_buffer,
   //   //         rectanglePlacement
   //   //      });

   //   //}

   //   // auto pcontext = gpu_context();

   //   pcontext->sendø() << [this, size]()
   //   {
   //      auto pcontext = gpu_context();

   //      ::gpu::context_lock context_lock(pcontext);
   //      /*::direct2d::direct2d() = allocateø ::draw2d_direct2d::plugin();

   //      ::direct2d::get()->initialize();*/

   //      HRESULT hr;

   //      ::user::interaction *puserinteraction = m_puserinteractionDraw2dGraphics;

   //      if (!puserinteraction)
   //      {

   //         puserinteraction = dynamic_cast<::user::interaction *>(m_pacmeuserinteractionMain.m_p);
   //      }

   //      auto pwindow = puserinteraction->window();

   //      auto rectanglePlacement = pwindow->get_window_rectangle();

   //      auto pdirect2d = direct2d();

   //      ::direct2d_lock lock(pdirect2d);

   //      // auto pcontext = gpu_context();

   //      ::cast<::dxgi_device_source> pdxgidevicesource = pcontext;

   //      auto pdevicecontextDefault = pdirect2d->default_d2d1_device_context(pdxgidevicesource);

   //      // comptr < ID2D1DeviceContext > pdevicecontextTemplate;

   //      // hr = m_pdevice->CreateDeviceContext(
   //      //    //D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
   //      //    D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
   //      //    &pdevicecontextTemplate);

   //      // if (FAILED(hr))
   //      //{

   //      //   warning() << "graphics::CreateCompatibleDC, CreateDeviceContext (1) " << hresult_text(hr);

   //      //   //return false;

   //      //   throw ::exception(error_failed);

   //      //}

   //      auto psession = session();

   //      auto paurasession = psession;

   //      auto puser = paurasession->user();

   //      auto pwindowing = system()->windowing();

   //      auto pdisplay = pwindowing->display();

   //      auto dpi = pdisplay->get_dpi();

   //      if (dpi <= 0.0)
   //      {

   //         ASSERT(false);

   //         throw ::exception(error_failed);
   //      }

   //      pdevicecontextDefault->SetDpi(dpi, dpi);

   //      // comptr < ID2D1RenderTarget > prendertargetTemplate;

   //      // hr = pdevicecontextDefault->QueryInterface(IID_ID2D1RenderTarget, (void**)&prendertargetTemplate);

   //      // if (FAILED(hr))
   //      //{

   //      //   warning() << "graphics::CreateCompatibleDC, QueryInterface (2) " << hresult_text(hr);

   //      //   throw ::exception(error_failed);

   //      //}

   //      D2D1_SIZE_U sizeu = D2D1::SizeU(size.cx, size.cy);

   //      if (sizeu.width <= 0)
   //      {

   //         sizeu.width = 800;
   //      }

   //      if (sizeu.width <= 0)
   //      {

   //         sizeu.width = 600;
   //      }

   //      D2D1_PIXEL_FORMAT pixelformat;

   //      pixelformat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

   //      pixelformat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
   //      if (m_pbitmaprendertarget)
   //      {

   //         ID2D1Bitmap *pdraw2dbitmap;


   //         hr = m_pbitmaprendertarget->GetBitmap(&pdraw2dbitmap);
   //         if (SUCCEEDED(hr) &&

   //             pdraw2dbitmap)
   //         {


   //            auto s = pdraw2dbitmap->GetSize();

   //            if (s.width == size.width() && s.height == size.height())
   //            {

   //               return;
   //            }
   //         }
   //      }

   //      hr = pdevicecontextDefault->CreateCompatibleRenderTarget(
   //         nullptr, &sizeu, &pixelformat, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE, &m_pbitmaprendertarget);

   //      if (FAILED(hr))
   //      {

   //         warning() << "graphics::CreateCompatibleDC, CreateCompatibleRenderTarget (3) " << hresult_text(hr);

   //         throw ::exception(error_failed);
   //      }

   //      hr = m_pbitmaprendertarget.as(m_pd2d1devicecontext);

   //      if (FAILED(hr))
   //      {

   //         m_pbitmaprendertarget = nullptr;

   //         throw ::exception(error_failed);
   //      }

   //      hr = m_pbitmaprendertarget.as(m_pdevicecontext);

   //      m_pdevicecontext.as(m_pdevicecontext1);

   //      if (FAILED(hr))
   //      {

   //         m_pd2d1devicecontext = nullptr;

   //         m_pbitmaprendertarget = nullptr;

   //         throw ::exception(error_failed);
   //      }

   //      defer_constructø(m_pdraw2dbitmap);

   //      ID2D1Bitmap *pdraw2dbitmap;

   //      hr = m_pbitmaprendertarget->GetBitmap(&pdraw2dbitmap);

   //      if (FAILED(hr))
   //      {

   //         m_pbitmaprendertarget = nullptr;

   //         throw ::exception(error_failed);
   //      }

   //      m_pdraw2dbitmap->attach(pdraw2dbitmap);

   //      m_iType = 3;


   //      void *pDataDeviceContext = m_pdevicecontext.m_p;
   //      m_osdata[data_device_context] = pDataDeviceContext;

   //      void *pDataRenderTarget = m_pd2d1devicecontext.m_p;
   //      m_osdata[data_render_target] = pDataRenderTarget;

   //      set_ok_flag();
   //   };

   //   // return true;
   //}
}


   //void graphics::_create_memory_graphics(const ::i32_size & size)
   //{

   //   auto pcontext = gpu_context();

   //   if (pcontext)
   //   {

   //      if (pcontext->size() == size)
   //      {

   //         return;

   //      }

   //   }

   //   //::draw2d::lock draw2dlock;

   //   // ::draw2d::device_lock devicelock(this);

   //   if (m_iType != 0)
   //   {

   //      destroy();

   //   }

   //   ::user::interaction* puserinteraction = m_puserinteractionDraw2dGraphics;

   //   if (::is_null(puserinteraction))
   //   {

   //      auto pacmeuserinteractionMain = m_papplication->main_acme_user_interaction();

   //      puserinteraction = dynamic_cast <::user::interaction*>(pacmeuserinteractionMain);

   //   }

   //   auto pwindow = puserinteraction->window();

   //   auto rectanglePlacement = pwindow->get_window_rectangle();

   //   auto pgpuapproach = m_papplication->get_gpu_approach();

   //   ::acme::windowing::window *pacmewindowingwindow = nullptr;

   //   if (::is_set(m_puserinteractionDraw2dGraphics))
   //   {

   //      pacmewindowingwindow = m_puserinteractionDraw2dGraphics->m_pacmewindowingwindow;

   //   }

   //   if (::is_null(pacmewindowingwindow))
   //   {

   //      auto pacmeuserinteractionMain = m_papplication->main_acme_user_interaction();

   //      pacmewindowingwindow = pacmeuserinteractionMain->m_pacmewindowingwindow;

   //   }

   //   auto pgpudevice = pgpuapproach->get_gpu_device(pacmewindowingwindow);

   //   auto pgpucontextNew = pgpudevice->allocate_gpu_context();

   //   pgpucontextNew->create_draw2d_gpu_context(pgpudevice, pacmewindowingwindow, size);

   //   set_gpu_context(pgpucontextNew);

   //   pcontext = gpu_context();

   //   {

   //      ::gpu::context_lock context_lock(pcontext);

   //      pcontext->m_pgpucompositor = this;

   //      auto pdirect2d = ::direct2d::from_gpu_device(pcontext->m_pgpudevice);

   //      initialize_direct2d_object(pdirect2d);

   //   }

   //   //auto pgpucontext = pgpudevice->get_main_context();

   //   //m_pgpucontextDraw2d->m_pgpurenderer = pgpucontext->get_output_renderer();
   //   //{

   //   //   m_pgpucontextCompositor->start_gpu_context(
   //   //      ::gpu::start_gpu_output_context_t
   //   //      {
   //   //         this,
   //   //         pdevice,
   //   //         ::gpu::e_output_gpu_buffer,
   //   //         rectanglePlacement
   //   //      });

   //   //}

   //   //auto pcontext = gpu_context();

   //   pcontext->sendø() << [this, size]()
   //      {

   //         auto pcontext = gpu_context();

   //         ::gpu::context_lock context_lock(pcontext);
   //         /*::direct2d::direct2d() = allocateø ::draw2d_direct2d::plugin();

   //         ::direct2d::get()->initialize();*/

   //         HRESULT hr;

   //         ::user::interaction* puserinteraction = m_puserinteractionDraw2dGraphics;

   //         if (!puserinteraction)
   //         {

   //            auto pacmeuserinteractionMain = m_papplication->main_acme_user_interaction();

   //            puserinteraction = dynamic_cast <::user::interaction*>(pacmeuserinteractionMain);

   //         }

   //         auto pwindow = puserinteraction->window();

   //         auto rectanglePlacement = pwindow->get_window_rectangle();

   //         auto pdirect2d = direct2d();

   //         ::direct2d_lock lock(pdirect2d);

   //         //auto pcontext = gpu_context();

   //         ::cast < ::dxgi_device_source > pdxgidevicesource = pcontext;

   //         auto pdevicecontextDefault = pdirect2d->default_d2d1_device_context(pdxgidevicesource);

   //         //comptr < ID2D1DeviceContext > pdevicecontextTemplate;

   //         //hr = m_pdevice->CreateDeviceContext(
   //         //   //D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
   //         //   D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
   //         //   &pdevicecontextTemplate);

   //         //if (FAILED(hr))
   //         //{

   //         //   warning() << "graphics::CreateCompatibleDC, CreateDeviceContext (1) " << hresult_text(hr);

   //         //   //return false;

   //         //   throw ::exception(error_failed);

   //         //}

   //         auto psession = session();

   //         auto paurasession = psession;

   //         auto puser = paurasession->user();

   //         auto pwindowing = system()->windowing();

   //         auto pdisplay = pwindowing->display();

   //         auto dpi = pdisplay->get_dpi();

   //         if (dpi <= 0.0)
   //         {

   //            ASSERT(false);

   //            throw ::exception(error_failed);

   //         }

   //         pdevicecontextDefault->SetDpi(dpi, dpi);

   //         //comptr < ID2D1RenderTarget > prendertargetTemplate;

   //         //hr = pdevicecontextDefault->QueryInterface(IID_ID2D1RenderTarget, (void**)&prendertargetTemplate);

   //         //if (FAILED(hr))
   //         //{

   //         //   warning() << "graphics::CreateCompatibleDC, QueryInterface (2) " << hresult_text(hr);

   //         //   throw ::exception(error_failed);

   //         //}

   //         D2D1_SIZE_U sizeu = D2D1::SizeU(size.cx, size.cy);

   //         if (sizeu.width <= 0)
   //         {

   //            sizeu.width = 800;

   //         }

   //         if (sizeu.width <= 0)
   //         {

   //            sizeu.width = 600;

   //         }

   //         D2D1_PIXEL_FORMAT pixelformat;

   //         pixelformat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

   //         pixelformat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
   //         if (m_pbitmaprendertarget)
   //         {

   //            ID2D1Bitmap* pdraw2dbitmap;



   //            hr = m_pbitmaprendertarget->GetBitmap(&pdraw2dbitmap);
   //            if (SUCCEEDED(hr) &&

   //               pdraw2dbitmap)
   //            {


   //               auto s = pdraw2dbitmap->GetSize();

   //               if (s.width == size.width()
   //                  && s.height == size.height())
   //               {

   //                  return;

   //               }

   //            }

   //         }

   //         hr = pdevicecontextDefault->CreateCompatibleRenderTarget(
   //            nullptr,
   //            &sizeu,
   //            &pixelformat,
   //            D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE,
   //            &m_pbitmaprendertarget);

   //         if (FAILED(hr))
   //         {

   //            warning() << "graphics::CreateCompatibleDC, CreateCompatibleRenderTarget (3) " << hresult_text(hr);

   //            throw ::exception(error_failed);

   //         }

   //         hr = m_pbitmaprendertarget.as(m_pd2d1devicecontext);

   //         if (FAILED(hr))
   //         {

   //            m_pbitmaprendertarget = nullptr;

   //            throw ::exception(error_failed);

   //         }

   //         hr = m_pbitmaprendertarget.as(m_pdevicecontext);

   //         m_pdevicecontext.as(m_pdevicecontext1);

   //         if (FAILED(hr))
   //         {

   //            m_pd2d1devicecontext = nullptr;

   //            m_pbitmaprendertarget = nullptr;

   //            throw ::exception(error_failed);

   //         }

   //         defer_constructø(m_pdraw2dbitmap);

   //         ID2D1Bitmap* pdraw2dbitmap;

   //         hr = m_pbitmaprendertarget->GetBitmap(&pdraw2dbitmap);

   //         if (FAILED(hr))
   //         {

   //            m_pbitmaprendertarget = nullptr;

   //            throw ::exception(error_failed);

   //         }

   //         m_pdraw2dbitmap->attach(pdraw2dbitmap);

   //         m_iType = 3;


   //         void* pDataDeviceContext = m_pdevicecontext.m_p;
   //         m_osdata[data_device_context] = pDataDeviceContext;

   //         void* pDataRenderTarget = m_pd2d1devicecontext.m_p;
   //         m_osdata[data_render_target] = pDataRenderTarget;

   //         set_ok_flag();

   //      };

   //   //return true;

   //}


   //::i32 graphics::GetDeviceCaps(::f64 dIndex)
   //{

   //   throw ::exception(todo);

   //}


   //void graphics::gpu_layer_on_after_begin_render()
   //{
   //   
   //   ////m_bInLayer = true;
   //   //
   //   //direct2d()->m_pd2d1multithread->Enter();

   //   //bind_draw2d_compositor();

   //   //m_pdevicecontext->BeginDraw();

   //   //m_pdevicecontext->Clear();

   //}


   //void graphics::gpu_layer_on_before_end_render()
   //{

   //   //m_pdevicecontext->EndDraw();

   //   ////m_pdevicecontext->Clear();

   //   //soft_unbind_draw2d_compositor();

   //   //direct2d()->m_pd2d1multithread->Leave();

   //   ////m_bInLayer = false;

   //}


   ::f64_point graphics::GetBrushOrg()
   {
      //throw ::exception(todo);
      //ASSERT(get_handle1() != nullptr);
      //::f64_point point;
      //VERIFY(::GetBrushOrgEx(get_handle1(), &point));
      //return point;

      return ::f64_point();

   }


   ::f64_point graphics::SetBrushOrg(::f64 x, ::f64 y)
   {
      //throw ::exception(todo);
      //ASSERT(get_handle1() != nullptr);
      //::f64_point point;
      //VERIFY(::SetBrushOrgEx(get_handle1(), x, y, &point));
      //return point;

      return ::f64_point();

   }


   ::f64_point graphics::SetBrushOrg(const ::f64_point & point)
   {

      //throw ::exception(todo);
      //ASSERT(get_handle1() != nullptr);
      //VERIFY(::SetBrushOrgEx(get_handle1(), point.x, point.y, &point));
      //return point;

      return ::f64_point();

   }


   //::i32 graphics::EnumObjects(::f64 dObjectType, ::i32 (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData)
   //{
   //   throw ::exception(todo);
   //   //ASSERT(get_handle2() != nullptr);
   //   //return ::EnumObjects(get_handle2(), nObjectType, (GOBJENUMPROC)lpfn, lpData);
   //}


   bool graphics::_draw_blend(const ::image::image_drawing & imagedrawing)
   {

      //return ::draw2d::graphics::BitBltAlphaBlend(x, y, nWidth, nHeight, pgraphicsSrc, xSrc, ySrc);

      //if (m_pimageAlphaBlend->is_set())
      //{
      //   //return true;

      //   // Reference implementation

      //   ::i32_rectangle rectangleAlphaBlend(m_pointAlphaBlend, m_pimageAlphaBlend->size());

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

      //   i32_point ::f64_point(x, y);

      //   ::f64_size size(nWidth, nHeight);

      //   ::i32_rectangle rectangleBlend(point, ::f64_size);

      //   ::i32_rectangle rectangleIntersect;

      //   if (rectangleIntersect.intersect(rectangleAlphaBlend, rectangleBlend))
      //   {

      //      ::image::image_pointer pimage1;

      //      pimage1 = create_image(::f64_size);

      //      ::i32_rectangle rectangleDib1(::f64_point(), pimage1->m_size);

      //      pgraphicsImage1->set_alpha_mode(::draw2d::e_alpha_mode_set);

      //      pgraphicsImage1->fill_rectangle(rectangleDib1, argb(0, 0, 0, 0));

      //      if (!pimage1->from(::f64_point(), pgraphicsSrc, ::f64_point(xSrc, ySrc), ::f64_size))
      //      {

      //         return false;

      //      }

      //      ::image::image_pointer pimage2;

      //      pimage2 = create_image(::f64_size);

      //      pgraphicsImage2->set_alpha_mode(::draw2d::e_alpha_mode_set);

      //      pgraphicsImage2->fill_rectangle(rectangleDib1, argb(255, 0, 0, 0));

      //      if (!pimage2->from(::f64_point(), m_pimageAlphaBlend, i32_point - m_pointAlphaBlend, rectangleIntersect.size()))
      //      {

      //         return false;

      //      }

      //      ::pointer<::draw2d_direct2d::graphics>pgraphicsDib1 = pgraphicsImage1;

      //      ::pointer<::draw2d_direct2d::graphics>pgraphicsDib2 = pgraphicsImage2;

      //      HRESULT hr = ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->EndDraw();

      //      pgraphicsDib2->m_pdevicecontext->DrawImage(
      //      (ID2D1Bitmap *)pgraphicsDib1->get_current_bitmap()->get_os_data(),
      //      D2D1::Point2F(0.f, 0.f),
      //      d2d1::f32_rectangle(rectangleDib1),
      //      D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
      //      D2D1_COMPOSITE_MODE_SOURCE_OVER);

      //      if (SUCCEEDED(hr))
      //      {

      //         ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->BeginDraw();

      //      }

      //      set_alpha_mode(::draw2d::e_alpha_mode_blend);

      //      BitBltRaw(x, y, nWidth, nHeight, pgraphicsImage2, 0, 0);

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


         ::f64_rectangle rectangleIntersect(m_pointAlphaBlend, m_pimageAlphaBlend->size());

         ::f64_rectangle rectangleBlt(::f64_point(x, y), ::f64_size(nWidth, nHeight));

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

            //   ::f64_point pointOff = get_origin();

            //   x += pointOff.x;

            //   y += pointOff.y;

            //   return m_pimage->blend(::f64_point(x, y), pgraphicsSrc->m_pimage, ::f64_point(xSrc, ySrc), m_pimageAlphaBlend, i32_point(m_pointAlphaBlend.x - x, m_pointAlphaBlend.y - y), rectangleBlt.size());

            //}
            //else
            //{

            auto pimage1 = image()->create_image(rectangleBlt.size());

            auto pgraphicsImage1 = pimage1->acquire_graphics();

            pgraphicsImage1->set_alpha_mode(::draw2d::e_alpha_mode_set);

            auto pimage = imagedrawing.image();

            pimage1->_draw_raw(::f64_rectangle(rectangleBlt.size()), pimage, ::f64_point(xSrc, ySrc));

            //if (!pimage1->_draw_raw(::f64_rectangle(rectangleBlt.size()), pimage , ::f64_point(xSrc, ySrc)))
            //{

            //   return false;

            //}

            {
               auto ppixmapImage1 = pimage1->map();

               auto ppixmapImageAlphaBlend = m_pimageAlphaBlend->map();

               ppixmapImage1->blend2(::f64_point(), ppixmapImageAlphaBlend, ::f64_point(x - m_pointAlphaBlend.x, y - m_pointAlphaBlend.y), rectangleBlt.size(), 255);
            }

            ::image::image_drawing_options imagedrawingoptions;

            _draw_raw(rectangleTarget, pimage1, imagedrawingoptions, ::f64_point());

            //}

            return true;

         }

      }

      return false;

   }


   bool graphics::TextOutAlphaBlend(::f64 x, ::f64 y, const ::scoped_string & scopedstr)
   {

      if (scopedstr.is_empty())
      {

         throw ::exception(error_invalid_empty_argument);

      }

      ASSERT(m_pimageAlphaBlend->is_ok());

      // "Reference" implementation for TextOutAlphaBlend

      auto size = get_text_extent(scopedstr);

      ::f64_rectangle rectangleAlphaBlend(m_pointAlphaBlend, m_pimageAlphaBlend->size());

      ::f64_point point(x, y);

      ::f64_rectangle rectangleText(point, size);

      ::f64_rectangle rectangleIntersect;

      if (rectangleIntersect.intersect(rectangleAlphaBlend, rectangleText))
      {

         //::draw2d::lock draw2dlock;

         // ::draw2d::device_lock devicelock(this);

         auto pimage1 = image()->create_image(size);

         ::f64_rectangle rectangleDib1(::f64_point(), size);

         auto pgraphicsImage1 = pimage1->acquire_graphics();

         pgraphicsImage1->set(get_current_font());

         pgraphicsImage1->set(get_current_brush());

         pgraphicsImage1->set_alpha_mode(::draw2d::e_alpha_mode_set);

         pgraphicsImage1->fill_rectangle(rectangleDib1, argb(0, 0, 0, 0));

         pgraphicsImage1->text_out(0, 0, scopedstr);

         auto pimage2 = image()->create_image(size);

         auto pgraphicsImage2 = pimage2->acquire_graphics();

         pgraphicsImage2->set_alpha_mode(::draw2d::e_alpha_mode_set);

         pgraphicsImage2->fill_rectangle(rectangleDib1, argb(255, 0, 0, 0));

         pimage2->_draw_raw(rectangleIntersect.size(), m_pimageAlphaBlend, ::f64_point(point - m_pointAlphaBlend));

         ::pointer<::draw2d_direct2d::graphics>pgraphicsDib1 = pgraphicsImage1.get();

         ::pointer<::draw2d_direct2d::graphics>pgraphicsDib2 = pgraphicsImage2.get();

         //pimage2->unmap();

         //HRESULT hrFlush = ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->Flush();

         //HRESULT hrEndDraw = ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->EndDraw();

         //if (FAILED(hrEndDraw))
         //{

         //   warning("graphics::on_release_memory_graphics : EndDraw failed: {}", hresult_text(hrEndDraw));

         //}

         D2D1_RECT_F rectfDib1;

         copy(&rectfDib1, &rectangleDib1);

         ::cast < ::draw2d_direct2d::bitmap > pdraw2ddirect2dbitmap = pgraphicsDib2->get_current_bitmap();

         auto pd2d1bitmap = pdraw2ddirect2dbitmap->m_pd2d1bitmap;

         pgraphicsDib1->m_pd2d1devicecontext->DrawImage(
         pd2d1bitmap,
         D2D1::Point2F(0.f, 0.f),
         rectfDib1,
         D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
         D2D1_COMPOSITE_MODE_DESTINATION_IN);

         //if (SUCCEEDED(hrEndDraw))
         //{

         //   ((ID2D1DeviceContext *)pgraphicsDib2->get_os_data())->BeginDraw();

         //}

         set_alpha_mode(::draw2d::e_alpha_mode_blend);

         ::image::image_source imagesource(pimage1);

         ::f64_rectangle rectangleTarget(::f64_point(x, y), pimage1->size());

         ::image::image_drawing_options imagedrawingoptions(rectangleTarget);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         draw(imagedrawing);

         return true;

      }

      return false;

   }


   void graphics::set(::draw2d::bitmap * pbitmapParam)
   {

      //::draw2d::lock draw2dlock;

      // ::draw2d::device_lock devicelock(this);

      ::pointer<::draw2d_direct2d::bitmap>pdraw2dbitmap = pbitmapParam;

      if (::is_null(pbitmapParam))
      {

         throw ::exception(error_invalid_empty_argument);

      }


      //if(m_pd2d1devicecontext == nullptr)
      //{

        // create_memory_graphics({}, nullptr); // create_compatible_graphics(nullptr);

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

         //if (m_pdraw2dbitmap.is_null()
         //   || m_pbitmapMap.is_null()
         //   || m_pgraphics.is_null()
         //   || m_pgraphicsMap.is_null())
         //{

         //   return false;

         //}

         //::pointer<::draw2d_direct2d::graphics>pgraphicsMap = m_pgraphicsMap;

         //::pointer<::draw2d_direct2d::graphics>pdraw2dgraphics = m_pgraphics;

         //::pointer<::draw2d_direct2d::bitmap>pdraw2dbitmap = m_pdraw2dbitmap;

      //   m_pbitmaprendertarget = nullptr;

      //   m_iType = 11;

      //   ::i32_size size = pdraw2dbitmap->GetBitmapDimension();

      //   D2D1_SIZE_U sizeu = D2D1::SizeU(size.cx, size.cy);

      //   D2D1_PIXEL_FORMAT pixelformat;

      //   pixelformat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

      //   pixelformat.format = DXGI_FORMAT_B8G8R8A8_UNORM;

      //   auto & prendertarget = m_pd2d1devicecontext;

      //   HRESULT hr = prendertarget->CreateCompatibleRenderTarget(nullptr, &sizeu, &pixelformat, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE, &m_pbitmaprendertarget);

      //   if (m_pbitmaprendertarget == nullptr)
      //   {

      //      return false;

      //   }

      //   if (FAILED(m_pbitmaprendertarget.as(m_pd2d1devicecontext)))
      //   {

      //      m_pbitmaprendertarget = nullptr;

      //      return false;

      //   }

      //   if (FAILED(m_pbitmaprendertarget.as(m_pdevicecontext)))
      //   {

      //      m_pbitmaprendertarget = nullptr;

      //      m_pd2d1devicecontext = nullptr;

      //      return false;

      //   }

      //   comptr<ID2D1Bitmap> pd2d1bitmap;

      //   m_pbitmaprendertarget->GetBitmap(&pd2d1bitmap);

      //   if (pdraw2dbitmap->m_pdraw2dbitmap == nullptr)
      //   {

      //      return false;

      //   }

      //   //pdraw2dgraphics->m_pplugin = pgraphicsMap->m_pplugin;

      //   pdraw2dbitmap->m_pdraw2dbitmap = pd2d1bitmap;

      //   pdraw2dbitmap->m_pdraw2dbitmap.as(pdraw2dbitmap->m_pbitmap1);

      //   pdraw2dbitmap->m_osdata[0] = pdraw2dbitmap->m_pdraw2dbitmap;

      //   pdraw2dbitmap->m_osdata[1] = pdraw2dbitmap->m_pbitmap1;

      //   m_osdata[0] = m_pdevicecontext;

      //   m_osdata[1] = m_pd2d1devicecontext;

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

      m_pd2d1devicecontext->SetTarget(pdraw2dbitmap->m_pd2d1bitmap);

      m_pdraw2dbitmap = pdraw2dbitmap;

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


   //::i32 graphics::GetPolyFillMode()
   //{
   //   throw ::exception(todo);
   //   //return ::GetPolyFillMode(get_handle2());
   //}

   //::i32 graphics::GetROP2()
   //{
   //   throw ::exception(todo);
   //   //return ::GetROP2(get_handle2());
   //}

   //::i32 graphics::GetStretchBltMode()
   //{
   //   throw ::exception(todo);
   //   //return ::GetStretchBltMode(get_handle2());
   //}

   //::i32 graphics::GetMapMode()
   //{
   //   throw ::exception(todo);
   //   //return ::GetMapMode(get_handle2());
   //}

   //::i32 graphics::GetGraphicsMode()
   //{
   //   throw ::exception(todo);
   //   //return ::GetGraphicsMode(get_handle2());
   //}


   //void graphics::GetWorldTransform(XFORM* pXform)
   //{

   //   throw ::exception(todo);

   //}


   void graphics::_get(::geometry2d::matrix & matrix)
   {

      D2D1::Matrix3x2F m;

      m_pd2d1devicecontext->GetTransform(&m);

      matrix = ::geometry2d::matrix();

      matrix.a1 = m._11;
      matrix.a2 = m._12;
      matrix.b1 = m._21;
      matrix.b2 = m._22;
      matrix.c1 = m._31;
      matrix.c2 = m._32;

      //return true;

   }


   void graphics::_set(const ::geometry2d::matrix & matrix)
   {

      if (!m_pd2d1devicecontext)
      {

         throw ::exception(error_null_pointer);

      }

      D2D1::Matrix3x2F m;

      m._11 = (FLOAT)matrix.a1;
      m._12 = (FLOAT)matrix.a2;
      m._21 = (FLOAT)matrix.b1;
      m._22 = (FLOAT)matrix.b2;
      m._31 = (FLOAT)matrix.c1;
      m._32 = (FLOAT)matrix.c2;

      m_pd2d1devicecontext->SetTransform(&m);

      //return true;

   }


   //::f64_point graphics::get_origin()
   //{

   //   return ::draw2d::graphics::get_origin();

   //}


   ::f64_size graphics::get_extents()
   {

      throw ::exception(todo);

      return {};

   }


   ::f64_point graphics::GetWindowOrg()
   {

      throw ::exception(todo);

      return {};

   }


   ::f64_size graphics::GetWindowExt()
   {

      throw ::exception(todo);
      //::f64_size::f64_size;
      //::GetWindowExtEx(get_handle2(), &::f64_size);
      //return ::f64_size;
      return {};

   }


   //::f64_point graphics::set_origin(const ::f64_point & point)
   //{

   //   return ::draw2d::graphics::set_origin(point.x, point.y);

   //}


   ::f64_size graphics::set_extents(const ::f64_size & size)
   {

      return ::draw2d::graphics::set_extents(size.cx, size.cy);

   }


   ::f64_point graphics::SetWindowOrg(const ::f64_point & point)
   {

      return SetWindowOrg(point.x, point.y);

   }


   //::f64_size graphics::set_window_ext(const ::f64_size & size)
   //{

   //   return set_window_ext(size.cx, size.cy);

   //}


   void graphics::DPtoLP(::f64_point * ppoints, ::collection::count nCount)
   {

      throw ::exception(todo);
      //::DPtoLP(get_handle2(), ppoints, nCount);

   }


   void graphics::DPtoLP(::f64_rectangle * prectangle)
   {

      throw ::exception(todo);
      //::DPtoLP(get_handle2(), (::f64_point *)rectangle, 2);

   }


   void graphics::LPtoDP(::f64_point * ppoints, ::collection::count nCount)
   {

      throw ::exception(todo);
      //::LPtoDP(get_handle2(), ppoints, nCount);

   }


   void graphics::LPtoDP(::f64_rectangle * prectangle)
   {

      throw ::exception(todo);
      //::LPtoDP(get_handle2(), (::f64_point *)rectangle, 2);

   }


   void graphics::fill_region(::draw2d::region * pRgn, ::draw2d::brush * pBrush)
   {

      throw ::exception(todo);
      //return ::FillRgn(get_handle1(), (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data()) != false;

      //return false;

   }


   void graphics::frame_region(::draw2d::region * pRgn, ::draw2d::brush * pBrush, ::f64 nWidth, ::f64 nHeight)
   {

      throw ::exception(todo);
      //return ::FrameRgn(get_handle1(), (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data(), nWidth, nHeight) != false;

      //return false;

   }


   void graphics::invert_region(::draw2d::region * pregion)
   {
      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::InvertRgn(get_handle1(), (HRGN)pRgn->get_os_data()) != false;

      //return false;

   }


   void graphics::paint_region(::draw2d::region * pregion)
   {

      warning() << "look! this is a warning! paint_region isn't yet implemented here in draw2d_direct2d";
      //throw ::exception(todo);
      //ASSERT(get_handle1() != nullptr);

      //return ::PaintRgn(get_handle1(), (HRGN)pRgn->get_os_data())  != false;

      //return false;

   }

   //void graphics::PtVisible(::f64 x, ::f64 y)
   //{

   //   throw ::exception(todo);
   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::PtVisible(get_handle1(), x, y) != false;

   //}

   //void graphics::PtVisible(const ::f64_point & point)
   //{

   //   ASSERT(get_handle1() != nullptr);

   //   return PtVisible(point.x, point.y);

   //} // call virtual


   //void graphics::rectVisible(const ::i32_rectangle & rectangle)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::rectVisible(get_handle1(), rectangle) != false;

   //}


   ::f64_point graphics::current_position()
   {

      throw ::exception(todo);
      //ASSERT(get_handle2() != nullptr);
      //::f64_point point;
      //VERIFY(::GetCurrentPositionEx(get_handle2(), &point));
      //return point;

      return {};

   }


   //void graphics::Arc(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2, ::f64 x3, ::f64 y3, ::f64 x4, ::f64 y4)
   //{

   //   auto pdraw2dpath = createø < ::draw2d::path > ();

   //   ::f64 pi = 3.1415927f;

   //   ::f64_rectangle rectangle(x1, y1, x2, y2);

   //   ::f64 centerx    = (x2 + x1) / 2.0;
   //   ::f64 centery    = (y2 + y1) / 2.0;

   //   ::f64 start      = atan2(y3 - centery, x3 - centerx) * 180.0 / pi;
   //   ::f64 end        = atan2(y4 - centery, x4 - centerx) * 180.0 / pi;

   //   //pdraw2dpath->begin_figure(false, ::draw2d::e_fill_mode_winding);
   //   pdraw2dpath->begin_figure();
   //   pdraw2dpath->add_arc(rectangle, (::i32) start, (::i32) fmod(end + 360.0 - start, 360.0));
   //   //pdraw2dpath->end_figure(false);
   //   //pdraw2dpath->close_figure();

   //   return this->path(pdraw2dpath);

   //}


   void graphics::arc(::f64 x1, ::f64 y1, ::f64 w, ::f64 h, ::f64_angle start, ::f64_angle extends)
   {

      auto pdraw2dpath = createø < ::draw2d::path >();

      ::i32_rectangle rectangle((::i32)x1, (::i32)y1, (::i32)(x1 + w), (::i32)(y1 + h));

      //pdraw2dpath->begin_figure(false, ::draw2d::e_fill_mode_winding);
      pdraw2dpath->begin_figure();
      pdraw2dpath->add_arc(rectangle, start, extends);
      //pdraw2dpath->end_figure(false);
      //pdraw2dpath->close(false);

      return this->path(pdraw2dpath);

   }


   void graphics::arc(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2, ::f64 x3, ::f64 y3, ::f64 x4, ::f64 y4)
   {

      auto pdraw2dpath = createø < ::draw2d::path >();

      ::f64 pi = 3.1415927f;

      ::i32_rectangle rectangle((::i32)x1, (::i32)y1, (::i32)x2, (::i32)y2);

      ::f64 centerx = (x2 + x1) / 2.0;
      ::f64 centery = (y2 + y1) / 2.0;

      //f64_angle start{ with_t{}, atan2(y3 - centery, x3 - centerx) * 180.0 / pi };
      //f64_angle end{ with_t{}, atan2(y4 - centery, x4 - centerx) * 180.0 / pi };

      f64_angle start;
      f64_angle end;

      start.atan(y3 - centery, x3 - centerx);
      end.atan(y4 - centery, x4 - centerx);

      //pdraw2dpath->begin_figure(false, ::draw2d::e_fill_mode_winding);
      pdraw2dpath->begin_figure();
      pdraw2dpath->add_arc(rectangle, start, end - start);
      //pdraw2dpath->end_figure(false);

      return this->path(pdraw2dpath);

   }


   void graphics::arc(const ::f64_rectangle & rectangle, const ::f64_point & pointStart, const ::f64_point & pointEnd)
   {

      //ASSERT(get_handle1() != nullptr);

      arc(rectangle.left, rectangle.top, rectangle.right, rectangle.bottom, pointStart.x, pointStart.y, pointEnd.x, pointEnd.y);


   }


   void graphics::polyline(const ::f64_point * ppoints, ::collection::count nCount)
   {

      auto pdraw2dpath = createø < ::draw2d::path >();

      //pdraw2dpath->begin_figure(false, ::draw2d::e_fill_mode_winding);

      pdraw2dpath->begin_figure();

      pdraw2dpath->add_lines(ppoints, nCount);

      //pdraw2dpath->end_figure(false);

      //pdraw2dpath->end_figure();

      this->draw(pdraw2dpath);

   }


   void graphics::frame_rectangle(const ::f64_rectangle & rectangleParam, ::draw2d::brush * pdraw2dbrush)
   {

      if (m_pd2d1devicecontext == nullptr)
      {

         throw ::exception(error_null_pointer);

      }

      D2D1_RECT_F rectangle;

      copy(&rectangle, &rectangleParam);

      pdraw2dbrush->defer_update(this);

      ::cast < ::draw2d_direct2d::brush>pdraw2ddirect2dbrush = pdraw2dbrush;

      auto pd2d1brush = pdraw2ddirect2dbrush->m_pd2d1brush;

      if (::is_null(pd2d1brush))
      {

         throw ::exception(error_null_pointer);

      }

      m_pd2d1devicecontext->DrawRectangle(rectangle, pd2d1brush);

   }


   void graphics::invert_rectangle(const ::f64_rectangle & rectangle)
   {

      throw ::exception(todo);

      //return false;

   }


   //void graphics::DrawIcon(::f64 x, ::f64 y, ::image::icon * picon)
   //{
   //   throw ::exception(todo);
   //   //ASSERT(get_handle1() != nullptr);

   //   //if(picon == nullptr)
   //   //   return false;

   //   //return ::DrawIcon(get_handle1(), x, y, picon->m_hicon) != false;

   //}

   //void graphics::DrawIcon(const ::f64_point & point, ::image::icon * picon)
   //{
   //   throw ::exception(todo);
   //   //ASSERT(get_handle1() != nullptr);

   //   //if(picon == nullptr)
   //   //   return false;

   //   //return ::DrawIcon(get_handle1(), point.x, point.y, picon->m_hicon) != false;

   //}

//   void graphics::draw(const ::f64_rectangle & rectangleTarget, ::image::icon * picon)
//   {
//
//#ifdef UNIVERSAL_WINDOWS
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
//         if(m_pd2d1devicecontext == nullptr)
//            return false;
//
//         bool bOk = false;
//
//         BITMAPINFO info;
//         ::color::color * pimage32;
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
//         HBITMAP hbitmap = ::CreateDIBSection(nullptr, &info, DIB_RGB_COLORS, (void **) &pimage32, nullptr, 0);
//
//         HDC hdc = ::CreateCompatibleDC(nullptr);
//
//         HBITMAP hbitmapOld = (HBITMAP) ::SelectObject(hdc, hbitmap);
//
//         ::i32 istepIfAniCur = 0;
//         if(::DrawIconEx(hdc, 0, 0, (HICON) picon->m_picon, rectangleTarget.width(), rectangleTarget.height(), istepIfAniCur, nullptr, DI_IMAGE | DI_MASK))
//         {
//
//            ::SelectObject(hdc, hbitmapOld);
//
//            try
//            {
//
//               //Gdiplus::Bitmap b(cx, cy, cx * 4 , PixelFormat32bppARGB, (::u8 *) pimage32);
//
//               ::draw2d::bitmap_pointer b(e_create);
//
//               b->CreateBitmap(this, rectangleTarget.size(), 1, 32, pimage32, rectangleTarget.width() * sizeof(::color32_t));
//
//               D2D1_RECT_F i32_rectangle;
//
//               __copy(rectangle, rectangleTarget);
//               //rectangle.left   = (FLOAT) i32_rectangle;
//               //rectangle.top    = (FLOAT) y;
//               //rectangle.right  = rectangle.left    + cx;
//               //rectangle.bottom = rectangle.top     + cy;
//
//               m_pd2d1devicecontext->DrawBitmap((ID2D1Bitmap *) b->get_os_data(), rectangle);
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
//   void graphics::DrawState(const ::f64_point & point, const ::f64_size& ::f64_size, HBITMAP hBitmap, ::u32 nFlags, HBRUSH hBrush)
//   {
//
//      throw ::exception(todo);
//      //ASSERT(get_handle1() != nullptr);
//      //return ::DrawState(get_handle1(), hBrush, nullptr, (LPARAM)hBitmap, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_BITMAP) != false;
//
//   }
//
//   void graphics::DrawState(const ::f64_point & point, const ::f64_size& ::f64_size, ::draw2d::bitmap* pBitmap, ::u32 nFlags, ::draw2d::brush* pBrush)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//      //return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), nullptr, (LPARAM)pBitmap->get_os_data(), 0, point.x, point.y, size.cx, size.cy, nFlags|DST_BITMAP) != false;
//
//   }
//
//   void graphics::DrawState(const ::f64_point & point, const ::f64_size& ::f64_size, HICON hIcon, ::u32 nFlags, HBRUSH hBrush)
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
//   void graphics::DrawState(const ::f64_point & point, const ::f64_size& ::f64_size, HICON hIcon, ::u32 nFlags, ::draw2d::brush* pBrush)
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
//   void graphics::DrawState(const ::f64_point & point, const ::f64_size& ::f64_size, const ::scoped_string & scopedstrText, ::u32 nFlags, bool bPrefixText, ::f64 dTextLen, HBRUSH hBrush)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr);
//      //return ::DrawState(get_handle1(), hBrush,  nullptr, (LPARAM)lpszText, (WPARAM)nTextLen, point.x, point.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)) != false;
//
//   }
//
//   void graphics::DrawState(const ::f64_point & point, const ::f64_size& ::f64_size, const ::scoped_string & scopedstrText, ::u32 nFlags, bool bPrefixText, ::f64 dTextLen, ::draw2d::brush* pBrush)
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
//   void graphics::DrawState(const ::f64_point & point, const ::f64_size& ::f64_size, DRAWSTATEPROC lpDrawProc, LPARAM lData, ::u32 nFlags, HBRUSH hBrush)
//   {
//
//      throw ::exception(todo);
//
//      //ASSERT(get_handle1() != nullptr); return ::DrawState(get_handle1(), hBrush, lpDrawProc, lData, 0, point.x, point.y, size.cx, size.cy, nFlags|DST_COMPLEX) != false;
//
//   }
//
//   void graphics::DrawState(const ::f64_point & point, const ::f64_size& ::f64_size, DRAWSTATEPROC lpDrawProc, LPARAM lData, ::u32 nFlags, ::draw2d::brush* pBrush)
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
   //void graphics::DrawEdge(::f64_rectangle * prectangle, ::u32 nEdge, ::u32 nFlags)
   //{

   //   throw ::exception(todo);

   //   ASSERT(get_handle1() != nullptr);

   //   //return ::DrawEdge(get_handle1(), rectangle, nEdge, nFlags) != false;

   //}


   //void graphics::DrawFrameControl(::f64_rectangle * prectangle, ::u32 nType, ::u32 nState)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr); return ::DrawFrameControl(get_handle1(), rectangle, nType, nState) != false;

   //}

   void graphics::Chord(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2, ::f64 x3, ::f64 y3, ::f64 x4, ::f64 y4)
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::Chord(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4) != false;

      //return false;

   }

   void graphics::Chord(const ::f64_rectangle & rectangle, const ::f64_point & pointStart, const ::f64_point & pointEnd)
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::Chord(get_handle1(), rectangle.left, rectangle.top, rectangle.right, rectangle.bottom, pointStart.x, pointStart.y, pointEnd.x, pointEnd.y) != false;

      //return false;

   }


   void graphics::DrawFocusRect(const ::f64_rectangle & rectangle)
   {

      throw ::exception(todo);

      //      ASSERT(get_handle1() != nullptr);

   }


   void graphics::fill_ellipse(const ::f64_rectangle & rectangle)
   {

      if (m_pdraw2dbrush.is_null())
      {

         throw ::exception(error_null_pointer);

      }

      if (m_bTargetRectangleModified)
      {

         defer_on_target_rectangle_update();

      }

      if (m_pdraw2dbrush->m_ebrush == ::draw2d::e_brush_box_gradient)
      {

         //comptr < ID2D1PathGeometry1 > pgeometry;

         //HRESULT hr = direct2d()->d2d1_factory1()->CreatePathGeometry(&pgeometry);

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

      m_pdraw2dbrush->defer_update(this);

      ::cast < ::draw2d_direct2d::brush>pdraw2ddirect2dbrush = m_pdraw2dbrush;

      auto pd2d1brush = pdraw2ddirect2dbrush->m_pd2d1brush;

      if (!pd2d1brush)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      D2D1_ELLIPSE ellipse;

      ellipse.point.x = (::f32)(rectangle.right + rectangle.left) / 2.f;
      ellipse.point.y = (::f32)(rectangle.bottom + rectangle.top) / 2.f;
      ellipse.radiusX = (::f32)(rectangle.right - rectangle.left) / 2.f;
      ellipse.radiusY = (::f32)(rectangle.bottom - rectangle.top) / 2.f;

      defer_primitive_blend();

      m_pd2d1devicecontext->FillEllipse(&ellipse, pd2d1brush);

      //return true;

   }


   void graphics::draw_ellipse(const ::f64_rectangle & rectangle)
   {

      if (m_pdraw2dpen.is_null())
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      m_pdraw2dpen->defer_update(this);

      auto pdraw2dbrush = m_pdraw2dpen->m_pdraw2dbrush;

      pdraw2dbrush->defer_update(this);

      ::cast < ::draw2d_direct2d::brush>pdraw2ddirect2dbrush = pdraw2dbrush;

      auto pd2d1brush = pdraw2ddirect2dbrush->m_pd2d1brush;

      if (::is_null(pd2d1brush))
      {

         throw ::exception(error_null_pointer);

      }

      D2D1_ELLIPSE ellipse;

      ellipse.point.x = (::f32)(rectangle.right + rectangle.left) / 2.f;
      ellipse.point.y = (::f32)(rectangle.bottom + rectangle.top) / 2.f;
      ellipse.radiusX = (::f32)(rectangle.right - rectangle.left) / 2.f;
      ellipse.radiusY = (::f32)(rectangle.bottom - rectangle.top) / 2.f;

      defer_primitive_blend();

      m_pd2d1devicecontext->DrawEllipse(&ellipse, pd2d1brush, (FLOAT)m_pdraw2dpen->m_dWidth);

      //return true;

   }


   void graphics::Pie(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2, ::f64 x3, ::f64 y3, ::f64 x4, ::f64 y4)
   {

      throw ::exception(todo);

      //return false;

   }


   void graphics::Pie(const ::f64_rectangle & rectangle, const ::f64_point & pointStart, const ::f64_point & pointEnd)
   {

      throw ::exception(todo);

      //return false;

   }


   void graphics::polygon(const ::f64_point * ppoints, ::collection::count count)
   {

      if (count < 3)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      comptr<ID2D1PathGeometry> pgeometry;

      HRESULT hr = direct2d()->d2d1_factory1()->CreatePathGeometry(&pgeometry);

      {

         comptr<ID2D1GeometrySink> psink;

         pgeometry->Open(&psink);

         psink->SetFillMode(D2D1_FILL_MODE_WINDING);

         psink->BeginFigure(D2D1::Point2F((FLOAT)ppoints->x, (FLOAT)ppoints->y), D2D1_FIGURE_BEGIN_FILLED);

         for (::collection::index i = 1; i < count; i++)
         {

            psink->AddLine({ (FLOAT)ppoints[i].x, (FLOAT)ppoints[i].y });

         }

         psink->EndFigure(D2D1_FIGURE_END_CLOSED);

         psink->Close();

      }

      //bool bOk = 

      fill(pgeometry, m_pdraw2dbrush);

      //bOk = bOk && 

      draw(pgeometry, m_pdraw2dpen);

      //return bOk;

   }


   void graphics::draw_polygon(const ::f64_point * ppoints, ::collection::count count)
   {

      if (count < 3)
      {

         //return false;

         throw ::exception(error_invalid_empty_argument);

      }

      comptr<ID2D1PathGeometry> pgeometry;

      HRESULT hr = direct2d()->d2d1_factory1()->CreatePathGeometry(&pgeometry);

      {

         comptr<ID2D1GeometrySink> psink;

         pgeometry->Open(&psink);

         psink->SetFillMode(D2D1_FILL_MODE_WINDING);

         psink->BeginFigure(D2D1::Point2F((FLOAT)ppoints->x, (FLOAT)ppoints->y), D2D1_FIGURE_BEGIN_FILLED);

         for (::collection::index i = 1; i < count; i++)
         {

            psink->AddLine({ (FLOAT)ppoints[i].x, (FLOAT)ppoints[i].y });

         }

         psink->EndFigure(D2D1_FIGURE_END_CLOSED);

         psink->Close();

      }

      //bool bOk = 

      draw(pgeometry, m_pdraw2dpen);

      //return bOk;

   }


   void graphics::fill_polygon(const ::f64_point * ppoints, ::collection::count count)
   {

      if (count < 3)
      {

         //return false;

         throw ::exception(error_invalid_empty_argument);

      }

      comptr<ID2D1PathGeometry> pgeometry;

      HRESULT hr = direct2d()->d2d1_factory1()->CreatePathGeometry(&pgeometry);

      {

         comptr<ID2D1GeometrySink> psink;

         pgeometry->Open(&psink);

         psink->SetFillMode(D2D1_FILL_MODE_WINDING);

         psink->BeginFigure(D2D1::Point2F((FLOAT)ppoints->x, (FLOAT)ppoints->y), D2D1_FIGURE_BEGIN_FILLED);

         for (::collection::index i = 1; i < count; i++)
         {

            psink->AddLine({ (FLOAT)ppoints[i].x, (FLOAT)ppoints[i].y });

         }

         psink->EndFigure(D2D1_FIGURE_END_CLOSED);

         psink->Close();

      }

      //bool bOk = 

      fill(pgeometry, m_pdraw2dbrush);

      //return bOk;

   }


   void graphics::rectangle(const ::f64_rectangle & rectangle)
   {

      fill_rectangle(rectangle);

      draw_rectangle(rectangle);

   }


   void graphics::draw_rectangle(const ::f64_rectangle & rectangleParam, ::draw2d::pen * pdraw2dpen)
   {

      if (pdraw2dpen == nullptr)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      if (pdraw2dpen->m_epen == ::draw2d::e_pen_null)
      {

         //return true;

         return;

      }

      D2D1_RECT_F rectangle;

      copy(&rectangle, &rectangleParam);

      pdraw2dpen->defer_update(this);

      auto pdraw2dbrush = pdraw2dpen->m_pdraw2dbrush;

      pdraw2dbrush->defer_update(this);

      ::cast < ::draw2d_direct2d::brush>pdraw2ddirect2dbrush = pdraw2dbrush;

      auto pd2d1brush = pdraw2ddirect2dbrush->m_pd2d1brush;

      if (::is_null(pd2d1brush))
      {

         throw ::exception(error_null_pointer);

      }

      defer_primitive_blend();

      m_pd2d1devicecontext->DrawRectangle(&rectangle, pd2d1brush, (FLOAT)pdraw2dpen->m_dWidth);

   }


   void graphics::fill_rectangle(const ::f64_rectangle & rectangleParam, ::draw2d::brush * pdraw2dbrush)
   {

      if (pdraw2dbrush == nullptr)
      {

         throw ::exception(error_null_pointer);

      }

      if (pdraw2dbrush->m_ebrush == ::draw2d::e_brush_null)
      {

         return;

      }

      if (pdraw2dbrush->m_ebrush == ::draw2d::e_brush_box_gradient)
      {

         return;

      }

      D2D1_RECT_F rectangle;

      copy(&rectangle, &rectangleParam);

      pdraw2dbrush->defer_update(this);

      ::cast < ::draw2d_direct2d::brush>pdraw2ddirect2dbrush = pdraw2dbrush;

      auto pd2d1brush = pdraw2ddirect2dbrush->m_pd2d1brush;

      defer_primitive_blend();

      m_pd2d1devicecontext->FillRectangle(rectangle, pd2d1brush);

   }


   void graphics::fill_round_rectangle(const ::f64_rectangle & rectangleParam, ::draw2d::brush * pdraw2dbrush, ::f64 dRadius)
   {

      if (pdraw2dbrush->m_ebrush == ::draw2d::e_brush_box_gradient)
      {

         ::draw2d::graphics::fill_round_rectangle(rectangleParam, pdraw2dbrush, dRadius);

         return;

      }

      if (m_pdraw2dbrush.is_set() && m_pdraw2dbrush->m_ebrush != ::draw2d::e_brush_null)
      {

         D2D1_ROUNDED_RECT rectangle;

         copy(&rectangle.rect, &rectangleParam);

         rectangle.radiusX = (FLOAT)dRadius;

         rectangle.radiusY = (FLOAT)dRadius;

         //auto pdraw2dbrush = m_pdraw2dbrush.cast <::draw2d_direct2d::brush >();

         m_pdraw2dbrush->defer_update(this);

         ::cast < ::draw2d_direct2d::brush>pdraw2ddirect2dbrush = m_pdraw2dbrush;

         auto pd2d1brush = pdraw2ddirect2dbrush->m_pd2d1brush;

         defer_primitive_blend();

         m_pd2d1devicecontext->FillRoundedRectangle(rectangle, pd2d1brush);

      }

   }


   void graphics::draw_round_rectangle(const ::f64_rectangle & rectangleParam, ::draw2d::pen * pdraw2dpen, ::f64 dRadius)
   {

      if (::is_set(pdraw2dpen) && m_pdraw2dpen->m_epen != ::draw2d::e_pen_null)
      {

         D2D1_ROUNDED_RECT rectangle;

         copy(&rectangle.rect, &rectangleParam);

         rectangle.radiusX = (FLOAT)dRadius;

         rectangle.radiusY = (FLOAT)dRadius;

         pdraw2dpen->defer_update(this);

         auto pdraw2dbrush = pdraw2dpen->m_pdraw2dbrush;

         pdraw2dbrush->defer_update(this);

         ::cast < ::draw2d_direct2d::brush>pdraw2ddirect2dbrush = pdraw2dbrush;

         auto pd2d1brush = pdraw2ddirect2dbrush->m_pd2d1brush;

         if (::is_null(pd2d1brush))
         {

            throw ::exception(error_null_pointer);

         }

         defer_primitive_blend();

         m_pd2d1devicecontext->DrawRoundedRectangle(rectangle, pd2d1brush, (FLOAT)pdraw2dpen->m_dWidth);

      }

   }


   //void graphics::PatBlt(::f64 x, ::f64 y, ::f64 dWidth, ::f64 dHeight)
   //{

   //   throw ::exception(todo);

   //}


   void graphics::_draw_raw(const ::f64_rectangle & rectangleTarget, ::image::image * pimageSource, const ::image::image_drawing_options & imagedrawingoptions, const ::f64_point & pointSrc)
   {

      //::draw2d::lock draw2dlock;

      // ::draw2d::device_lock devicelock(this);

      auto pimage = pimageSource->get_source_image();

      //try
      //{

      if (pimage == nullptr)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      //pimage->create_bitmap(m_pacmeuserinteractionAffinity);

      //if (pimage->get_bitmap() == nullptr)
      //{

      //   throw ::exception(error_null_pointer);

      //}

      //if (pgraphicsSrc->get_current_bitmap() == nullptr)
      //{

      //   return false;

      //}

      //if (pgraphicsSrc->get_current_bitmap()->nok())
      //{

      //   return false;

      //}

      ::f64 x = rectangleTarget.left;
      ::f64 y = rectangleTarget.top;
      ::f64 nWidth = rectangleTarget.width();
      ::f64 nHeight = rectangleTarget.height();
      ::f64 xSrc = pointSrc.x;
      ::f64 ySrc = pointSrc.y;

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

      ::comptr < ::ID2D1Image > pd2d1imageTarget;

      m_pd2d1devicecontext->GetTarget(&pd2d1imageTarget);

      ::comptr<ID2D1Bitmap > pd2d1bitmapTarget;

      pd2d1imageTarget.as(pd2d1bitmapTarget);

      if (pd2d1imageTarget)
      {

         D2D1_SIZE_U sz = pd2d1bitmapTarget->GetPixelSize();

         if (nWidth + x + m_pointTarget.x > sz.width)
         {

            nWidth = sz.width - x - m_pointTarget.x;

         }

         if (nHeight + y + m_pointTarget.y > sz.height)
         {

            nHeight = sz.height - y - m_pointTarget.y;

         }

      }

      {

         ::cast < ::draw2d_direct2d::bitmap > pdirect2dbitmapSource = pimage->get_bitmap_as_source();

         auto pd2d1bitmapSource = pdirect2dbitmapSource->m_pd2d1bitmap;

         D2D1_SIZE_U sz = pd2d1bitmapSource->GetPixelSize();

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

         D2D1_RECT_F rectangleTarget = D2D1::RectF((::f32)x, (::f32)y, (::f32)(x + nWidth), (::f32)(y + nHeight));

         D2D1_RECT_F rectangleSource = D2D1::RectF((::f32)xSrc, (::f32)ySrc, (::f32)(xSrc + nWidth), (::f32)(ySrc + nHeight));

         ::cast < ::draw2d_direct2d::bitmap > pdirect2dbitmapSource = pimage->get_bitmap_as_source();

         auto pd2d1bitmapSource = pdirect2dbitmapSource->m_pd2d1bitmap;

//         auto pd2d1bitmap = ((ID2D1Bitmap *)pimage->get_bitmap_as_source()->get_os_data());

         ::i32 cx = pd2d1bitmapSource->GetPixelSize().width;

         ::i32 cy = pd2d1bitmapSource->GetPixelSize().height;

         //pimage->unmap();

         //HRESULT hrFlush = ((ID2D1DeviceContext *)pgraphicsImage->get_os_data())->Flush();

         //HRESULT hrEndDraw = ((ID2D1DeviceContext *)pgraphicsImage->get_os_data())->EndDraw();

         defer_primitive_blend();

         if (imagedrawingoptions.is_identity())
         {

            m_pd2d1devicecontext->DrawBitmap(pd2d1bitmapSource, rectangleTarget, (FLOAT)imagedrawingoptions.opacity().f32_opacity(), D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR, rectangleSource);

         }
         else
         {

            comptr<ID2D1Effect> colorMatrixEffect;
            HRESULT hr = m_pd2d1devicecontext->CreateEffect(CLSID_D2D1ColorMatrix, &colorMatrixEffect);

            defer_throw_hresult(hr);

            colorMatrixEffect->SetInput(0, pd2d1bitmapSource);
            D2D1_MATRIX_5X4_F matrix =
               D2D1::Matrix5x4F(
                  (FLOAT)imagedrawingoptions.matrix().a1,
                  (FLOAT)imagedrawingoptions.matrix().a2,
                  (FLOAT)imagedrawingoptions.matrix().a3,
                  (FLOAT)imagedrawingoptions.matrix().a4,
                  (FLOAT)imagedrawingoptions.matrix().b1,
                  (FLOAT)imagedrawingoptions.matrix().b2,
                  (FLOAT)imagedrawingoptions.matrix().b3,
                  (FLOAT)imagedrawingoptions.matrix().b4,
                  (FLOAT)imagedrawingoptions.matrix().c1,
                  (FLOAT)imagedrawingoptions.matrix().c2,
                  (FLOAT)imagedrawingoptions.matrix().c3,
                  (FLOAT)imagedrawingoptions.matrix().c4,
                  (FLOAT)imagedrawingoptions.matrix().d1,
                  (FLOAT)imagedrawingoptions.matrix().d2,
                  (FLOAT)imagedrawingoptions.matrix().d3,
                  (FLOAT)imagedrawingoptions.matrix().d4,
                  (FLOAT)imagedrawingoptions.matrix().e1,
                  (FLOAT)imagedrawingoptions.matrix().e2,
                  (FLOAT)imagedrawingoptions.matrix().e3,
                  (FLOAT)imagedrawingoptions.matrix().e4);
            colorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, matrix);
            D2D1_POINT_2F pointTarget;
            pointTarget.x = rectangleTarget.left;
            pointTarget.y = rectangleTarget.top;
            //m_pdevicecontext->BeginDraw();
            m_pd2d1devicecontext->DrawImage(colorMatrixEffect, &pointTarget, &rectangleSource, D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
            //m_pdevicecontext->EndDraw();

         }


         //if (SUCCEEDED(hrEndDraw))
         //{

         //   ((ID2D1DeviceContext *)pgraphicsImage->get_os_data())->BeginDraw();

         //}

      }

      //return true;

   //}
   //catch (...)
   //{

   //   return false;

   //}

   }


   void graphics::_stretch_raw(const ::f64_rectangle & rectangleTarget, ::image::image * pimage, const ::image::image_drawing_options & imagedrawingoptions, const ::f64_rectangle & rectangleSource)
   {

      //try
      //{

      ::f64 xDst = rectangleTarget.left;
      ::f64 yDst = rectangleTarget.top;
      ::f64 nDstWidth = rectangleTarget.width();
      ::f64 nDstHeight = rectangleTarget.height();
      ::f64 xSrc = rectangleSource.left;
      ::f64 ySrc = rectangleSource.top;
      ::f64 nSrcWidth = rectangleSource.width();
      ::f64 nSrcHeight = rectangleSource.height();

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

            defer_construct_newø(pframeaTarget);

            if (pframeaTarget->get_size() != pframeaSource->get_size())
            {

               pframeaTarget->set_size(pframeaSource->get_size());

               for (::collection::index i = 0; i < pframeaTarget->get_size(); i++)
               {

                  auto & pframeSource = pframeaSource->element_at(i);

                  auto & pframeTarget = pframeaTarget->element_at(i);

                  defer_construct_newø(pframeTarget);

                  pframeTarget->m_time = pframeSource->m_time;

                  pframeTarget->m_iFrame = pframeSource->m_iFrame;

                  auto & ppixmapSource = pframeSource->m_ppixmap;

                  ppixmapSource->set_ok_flag();

                  auto pimageSource = createø<::image::image>();

                  pimageSource->create_as_descriptor(ppixmapSource->size(), DEFAULT_CREATE_IMAGE_FLAG, ppixmapSource->m_iScan);

                  pimageSource->m_ppixmapOwned = ppixmapSource;

                  auto pimageTarget = createø<::image::image>();

                  pframeTarget->m_pparticleImage = pimageTarget;

                  pimageTarget->create_as_descriptor(m_pimage->size());

                  auto pgraphicsImageTarget = pimageTarget->acquire_graphics();

                  pgraphicsImageTarget->_stretch_raw(rectangleTarget, pimageSource, imagedrawingoptions, rectangleSource);

               }

               pframeaTarget->m_timeTotal = pframeaSource->m_timeTotal;

            }

            return;

         }

      }

      pimage->update_bitmap_as_render_target(m_pacmeuserinteractionAffinity, this);

      if (pimage->get_bitmap_as_source() == nullptr)
      {

         //return false;

         throw ::exception(error_failed);

      }

      //auto pgraphicsImage = pimage->acquire_graphics();

      //if (pgraphicsImage == nullptr)
      //{

      //   //return false;

      //   throw ::exception(error_failed);

      //}

      {

         D2D1_RECT_F rectangleTarget = D2D1::RectF((::f32)xDst, (::f32)yDst, (::f32)(xDst + nDstWidth), (::f32)(yDst + nDstHeight));

         D2D1_RECT_F rectangleSource = D2D1::RectF((::f32)xSrc, (::f32)ySrc, (::f32)(xSrc + nSrcWidth), (::f32)(ySrc + nSrcHeight));

         //pimage->unmap();

         //::cast < graphics > pgraphicsImage = pgraphicsImage;

         //auto pd2d1contextImage = pgraphicsImage->m_pdevicecontext;

         //auto pcontext = gpu_context();

         //::direct2d_lock direct2dlock(::direct2d::from_gpu_device(pcontext->m_pgpudevice));

         //HRESULT hrFlush = pd2d1contextImage->Flush();

         //HRESULT hrEndDraw = pd2d1contextImage->EndDraw();

         //if (FAILED(hrEndDraw))
         //{

         //   warning("graphics::on_release_memory_graphics : EndDraw failed: {}", hresult_text(hrEndDraw));

         //}

         defer_primitive_blend();

         ::cast < ::draw2d_direct2d::bitmap > pdirect2dbitmapSource = pimage->get_bitmap_as_source();

         auto pd2d1bitmapSource = pdirect2dbitmapSource->m_pd2d1bitmap;

         //if (m_pd2d1devicecontext != nullptr)
         //{

            m_pd2d1devicecontext->DrawBitmap(pd2d1bitmapSource, &rectangleTarget, 1.0, m_d2d1bitmapinterpolationmode, &rectangleSource);

         //}
         //else
         //{

         //   m_pd2d1devicecontext->DrawBitmap(pd2d1bitmap, rectangleTarget, 1.0, m_d2d1interpolationmode, rectangleSource);

         //}

         //if (SUCCEEDED(hrEndDraw))
         //{

         //   ((ID2D1DeviceContext *)pgraphicsImage->get_os_data())->BeginDraw();

         //}

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


   //void graphics::_draw_raw(const ::f64_rectangle & rectangleTarget, ::draw2d::graphics * pgraphicsSrc, const ::f64_point & pointSrc)
   //{


   //}


   //void graphics::_stretch_raw(const ::f64_rectangle & rectangleTarget, ::draw2d::graphics * pgraphicsSrc, const ::f64_rectangle & rectangleSource)
   //{


   //}


   ::color::color graphics::GetPixel(::f64 x, ::f64 y)
   {

      throw ::exception(todo);


      return {};

   }


   ::color::color graphics::GetPixel(const ::f64_point & point)
   {

      throw ::exception(todo);

      return {};

   }


   ::color::color graphics::SetPixel(::f64 x, ::f64 y, const ::color::color & color)
   {

      throw ::exception(todo);

      return {};

   }


   ::color::color graphics::SetPixel(const ::f64_point & point, const ::color::color & color)
   {

      throw ::exception(todo);

      return {};

   }


   //   void graphics::FloodFill(::f64 x, ::f64 y, const ::color::color & color)
   //   {
   //
   //      throw ::exception(todo);
   //
   //   }
   //
   //
   //   void graphics::ExtFloodFill(::f64 x, ::f64 y, const ::color::color & color, ::u32 nFillType)
   //   {
   //
   //      throw ::exception(todo);
   //
   //   }


      //void graphics::text_out(::f64 x, ::f64 y, const block & block)
      //{

      //   return text_out(x, y, (const ::string &) block.get_data(), block.get_size());

      //}


      //void graphics::text_out(::f64 x, ::f64 y, const ::scoped_string & scopedstr)
      //{

      //   return text_out((::i32) x, (::i32) y, str, (::i32) str.length());

      //}


      //void graphics::ExtTextOut(::f64 x, ::f64 y, ::u32 nOptions, const ::f64_rectangle & rectangle, const ::scoped_string & scopedstrString, character_count nCount, LPINT lpDxWidths)
      //{

      //   throw ::exception(todo);

      //}


      //void graphics::ExtTextOut(::f64 x, ::f64 y, ::u32 nOptions, const ::f64_rectangle & rectangle, const ::scoped_string & scopedstr, LPINT lpDxWidths)
      //{

      //   throw ::exception(todo);

      //}


      //::f64_size graphics::TabbedTextOut(::f64 x, ::f64 y, const ::scoped_string & scopedstrString, character_count nCount, ::collection::count nTabPositions, LPINT lpnTabStopPositions, ::i32 nTabOrigin)
      //{

      //   throw ::exception(todo);

      //}


      //::f64_size graphics::TabbedTextOut(::f64 x, ::f64 y, const ::scoped_string & scopedstr, ::collection::count nTabPositions, LPINT lpnTabStopPositions, ::i32 nTabOrigin)
      //{

      //   throw ::exception(todo);

      //}


      //::f64_size graphics::GetTabbedTextExtent(const ::scoped_string & scopedstrString, character_count nCount, ::collection::count nTabPositions, LPINT lpnTabStopPositions)
      //{

      //   throw ::exception(todo);

      //}


      //::f64_size graphics::GetTabbedTextExtent(const ::scoped_string & scopedstr, ::collection::count nTabPositions, LPINT lpnTabStopPositions)
      //{

      //   throw ::exception(todo);

      //}


      //::f64_size graphics::GetOutputTabbedTextExtent(const ::scoped_string & scopedstrString, character_count nCount, ::collection::count nTabPositions, LPINT lpnTabStopPositions)
      //{

      //   throw ::exception(todo);

      //   return nullptr;

      //}


      //::f64_size graphics::GetOutputTabbedTextExtent(const ::scoped_string & scopedstr, ::collection::count nTabPositions, LPINT lpnTabStopPositions)
      //{

      //   throw ::exception(todo);

      //   return nullptr;

      //}


      //void graphics::GrayString(::draw2d::brush* pBrush, bool (CALLBACK* lpfnOutput)(HDC, LPARAM, ::i32), LPARAM lpData, ::f64 dCount,::f64 x, ::f64 y, ::f64 dWidth, ::f64 dHeight)
      //{

      //   throw ::exception(todo);

      //}


   ::u32 graphics::GetTextAlign()
   {

      throw ::exception(todo);

      return false;

   }


   //::i32 graphics::GetTextFace(::collection::count nCount, ::i8 * lpszFacename)
   //{

   //   throw ::exception(todo);

   //}


   //::i32 graphics::GetTextFace(string & rString)
   //{

   //   throw ::exception(todo);

   //}


   void graphics::get_text_metrics(::write_text::text_metric * pmetrics)
   {

      if (m_pwritetextfont.is_null())
      {

         constructø(((graphics *)this)->m_pwritetextfont);

      }

      if (m_pwritetextfont.is_null())
      {

         //return false;

         throw ::exception(error_failed);

      }

      m_pwritetextfont->defer_update(this);

      m_pwritetextfont->get_text_metric(this, *pmetrics);

      //memory_copy(pmetrics, &m_pwritetextfont->m_textmetric2, sizeof(m_pwritetextfont->m_textmetric2));


      //return true;

   }


   void graphics::get_output_text_metrics(::write_text::text_metric * lpMetrics)
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::GetTextMetrics(get_handle1(), lpMetrics) != false;

      //return false;

   }



   void graphics::set_smooth_mode(::draw2d::enum_smooth_mode esmoothmode)
   {

      //if (esmoothmode != m_esmoothmode)
      //{

         m_esmoothmode = esmoothmode;

      //   if (esmoothmode != ::draw2d::e_smooth_mode_none)
      //   {

      //      m_pd2d1devicecontext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

      //   }
      //   else
      //   {

      //      m_pd2d1devicecontext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_NONE);

      //   }

      //}

      //return true;

   }




   //   ::i32 graphics::GetTextCharacterExtra()
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
   //   ::u32 graphics::GetFontLanguageInfo()
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
   //   /*   ::u32 graphics::GetCharacterPlacement(const ::string & lpString, ::f64 dCount, ::f64 dMaxExtent, LPGCP_RESULTS lpResults, ::u32 dwFlags)
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
   //      ::u32 graphics::GetCharacterPlacement(string & str, ::f64 dMaxExtent, LPGCP_RESULTS lpResults, ::u32 dwFlags)
   //      {
   //
   //         throw ::exception(todo);
   //
   //         //ASSERT(get_handle1() != nullptr);
   //
   //         //return ::GetCharacterPlacement(get_handle1(), (const ::string &)str, (::i32) str.length(), nMaxExtent, lpResults, dwFlags);
   //
   //      }
   //      */
   //#endif
   //
   //   ::f64_size graphics::GetAspectRatioFilter()
   //   {
   //
   //      throw ::exception(todo);
   //
   //      //ASSERT(get_handle2() != nullptr);
   //      //::f64_size::f64_size;
   //      //VERIFY(::GetAspectRatioFilterEx(get_handle2(), &::f64_size));
   //      //return ::f64_size;
   //
   //   }
   //
   //   void graphics::ScrollDC(::i32 dx, ::i32 dy, const ::i32_rectangle & rectangleScroll, const ::i32_rectangle & rectangleClip, ::draw2d::region* pRgnUpdate, ::i32_rectangle * lpRectUpdate)
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
   //   ::i32 graphics::Escape(::f64 dEscape, ::f64 dCount, const ::scoped_string & scopedstrInData, LPVOID lpOutData)
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
   ::u32 graphics::SetBoundsRect(const ::f64_rectangle & rectangleBounds, ::u32 flags)
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::SetBoundsRect(get_handle1(), rectangleBounds, flags);

      return false;

   }


   ::u32 graphics::GetBoundsRect(::f64_rectangle * prectBounds, ::u32 flags)
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

      //   ::u32 graphics::GetFontData(::u32 dwTable, ::u32 dwOffset, LPVOID lpData, ::u32 cbData)
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
      //   ::i32 graphics::GetKerningPairs(::f64 dPairs, LPKERNINGPAIR lpkrnpair)
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
      //   ::u32 graphics::GetGlyphOutline(::u32 nChar, const ::e_align & ealign, const ::e_draw_text & edrawtext, LPGLYPHMETRICS lpgm,  ::u32 cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2)
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
      //   ::i32 graphics::StartDoc(LPDOCINFO lpDocInfo)
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

   ::i32 graphics::StartPage()
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::StartPage(get_handle1());

      return 0;

   }

   ::i32 graphics::EndPage()
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::EndPage(get_handle1());
      return 0;

   }


   //::i32 graphics::SetAbortProc(bool (CALLBACK* lpfn)(HDC, ::i32))
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ::SetAbortProc(get_handle1(), (ABORTPROC)lpfn);

   //}


   ::i32 graphics::AbortDoc()
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::AbortDoc(get_handle1());

      return 0;

   }


   ::i32 graphics::EndDoc()
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::EndDoc(get_handle1());

      return 0;

   }


   //   void graphics::MaskBlt(::f64 x, ::f64 y, ::f64 dWidth, ::f64 dHeight, ::draw2d::graphics * pgraphicsSrc, ::f64 xSrc, ::f64 ySrc, ::draw2d::bitmap& maskBitmap, ::f64 xMask, ::f64 yMask)
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
   //   void graphics::PlgBlt(::f64_point * lpPoint, ::draw2d::graphics * pgraphicsSrc, ::f64 xSrc, ::f64 ySrc, ::f64 dWidth, ::f64 dHeight, ::draw2d::bitmap& maskBitmap, ::f64 xMask, ::f64 yMask)
   //   {
   //
   //      throw ::exception(todo);
   //
   //      //ASSERT(get_handle1() != nullptr);
   //
   //      //return ::PlgBlt(get_handle1(), lpPoint, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nWidth, nHeight, (HBITMAP)maskBitmap.get_os_data(), xMask, yMask) != false;
   //
   //   }


      //void graphics::SetPixelV(::f64 x, ::f64 y, const ::color::color & color)
      //{

      //   throw ::exception(todo);

      //   //ASSERT(get_handle1() != nullptr);

      //   //return ::SetPixelV(get_handle1(), x, y, crColor) != false;

      //   return false;

      //}

      //void graphics::SetPixelV(const ::f64_point & point, const ::color::color & color)
      //{

      //   throw ::exception(todo);

      //   //ASSERT(get_handle1() != nullptr);

      //   //return ::SetPixelV(get_handle1(), point.x, point.y, crColor) != false;

      //   return false;

      //}

   void graphics::angle_arc(::f64 x, ::f64 y, ::f64 nRadius, f64_angle fStartAngle, f64_angle fSweepAngle)
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::AngleArc(get_handle1(), x, y, nRadius, fStartAngle, fSweepAngle) != false;

      //return false;

   }


   //void graphics::arc_to(const ::f64_rectangle & rectangle, const ::f64_point & pointStart, const ::f64_point & pointEnd)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr);

   //   //return ArcTo(rectangle.left, rectangle.top, rectangle.right, rectangle.bottom, pointStart.x, pointStart.y, pointEnd.x, pointEnd.y);
   //   //return false;


   //}

   //::i32 graphics::GetArcDirection()
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle2() != nullptr);

   //   //return ::GetArcDirection(get_handle2());

   //}

   void graphics::poly_polyline(const ::f64_point * ppoints, const ::i32 * lpPolyPoints, ::collection::count nCount)
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


   void graphics::poly_bezier(const ::f64_point * ppoints, ::collection::count nCount)
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);

      //return ::poly_bezier(get_handle1(), ppoints, nCount) != false;

      //return false;

   }


   //   ::i32 graphics::DrawEscape(::f64 dEscape, ::f64 dInputSize, const ::scoped_string & scopedstrInputData)
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
   //   ::i32 graphics::Escape(::f64 dEscape, ::f64 dInputSize, const ::scoped_string & scopedstrInputData, ::f64 dOutputSize, ::i8 * lpszOutputData)
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
   //   void graphics::GetCharWidth(::u32 nFirstChar, ::u32 nLastChar, ::f32* lpFloatBuffer)
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

      //return m_pd2d1devicecontext->FillPath(direct2d_brush(), m_ppath) == Gdiplus::Status::Ok;

      //return false;


   }


   void graphics::flatten_path()
   {

      throw ::exception(todo);

      //return m_ppath->Flatten() == Gdiplus::Status::Ok;
      //return false;


   }


   ::f32 graphics::GetMiterLimit()
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);
      //::f32 fMiterLimit;
      //VERIFY(::GetMiterLimit(get_handle1(), &fMiterLimit));
      //return fMiterLimit;

      return 0.f;

   }


   //::i32 graphics::GetPath(::f64_point * ppoints, ::u8 * lpTypes, ::collection::count nCount)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_handle1() != nullptr);
   //   //return ::GetPath(get_handle1(), ppoints, lpTypes, nCount);

   //}


   void graphics::SetMiterLimit(::f32 fMiterLimit)
   {

      throw ::exception(todo);

      //ASSERT(get_handle1() != nullptr);
      //return ::SetMiterLimit(get_handle1(), fMiterLimit, nullptr) != false;

      //return false;

   }


   void graphics::stroke_and_fill_path()
   {

      throw ::exception(todo);

      //bool bOk1 = m_pd2d1devicecontext->FillPath(direct2d_brush(), m_ppathPaint) == Gdiplus::Status::Ok;

      //bool bOk2 = m_pd2d1devicecontext->DrawPath(direct2d_pen(), m_ppathPaint) == Gdiplus::Status::Ok;

      //return bOk1 && bOk2;

      //return false;

   }


   void graphics::stroke_path()
   {

      throw ::exception(todo);

      //      return m_pd2d1devicecontext->DrawPath(direct2d_pen(), m_ppathPaint) == Gdiplus::Status::Ok;

            //return false;

   }


   void graphics::widen_path()
   {

      throw ::exception(todo);


      //      return m_ppath->Widen(direct2d_pen()) == Gdiplus::Status::Ok;

            //return false;

   }


   void graphics::AddMetaFileComment(::u32 nDataSize, const ::u8 * pCommentData)
   {

      throw ::exception(todo);


      //ASSERT(get_handle1() != nullptr);
      //return ::GdiComment(get_handle1(), nDataSize, pCommentData) != false;

      //return false;

   }


   /*bool CALLBACK metaCallback(
   EmfPlusRecordType recordType,
   ::u32 flags,
   ::u32 dataSize,
   const ::u8* pStr,
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
   Metafile* pMeta = ___new Metafile(L"SampleMetafile.emf", hdc);
   {
   // Fill a i32_rectangle and an ellipse in pMeta.
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
   //void graphics::PlayMetaFile(HENHMETAFILE hEnhMF, const ::f64_rectangle & rectangleBounds)
   //{

   //   throw ::exception(todo);

   //   //Gdiplus::RectF i32_rectangle((Gdiplus::REAL) lpBounds->left, (Gdiplus::REAL) lpBounds->top, (Gdiplus::REAL) width(lpBounds), (Gdiplus::REAL) height(lpBounds));

   //   //Gdiplus::Metafile* pMeta = ___new Gdiplus::Metafile(hEnhMF, false);

   //   ////m_pgraphcis->EnumerateMetafile(pMeta, rectangle, metaCallback, PMETAHEADER);

   //   //bool bOk = m_pd2d1devicecontext->DrawImage(pMeta, rectangle) == Gdiplus::Status::Ok;

   //   //delete pMeta;

   //   //return bOk ? true : false;
   //   ////return ::PlayEnhMetaFile(get_handle1(), hEnhMF, lpBounds);

   //}

   // true blend
   // COLOR_DEST = SRC_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA) * COLOR_DST

   // ::f64 blend
   // COLOR_DEST = SRC_ALPHA * BLEND_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA * BLEND_ALPHA) * COLOR_DST

   // Thank you
   // Jiju George T
   // Web Developer
   // India India
   // Member


   //void graphics::_alpha_blend_raw(const ::f64_rectangle & rectangleTarget, ::draw2d::graphics * pgraphicsSrc, const ::f64_rectangle & rectangleSource, ::f64 dRate)
   //{

   //   // ::draw2d::device_lock devicelock(this);

   //   /*      ::f32 fA = (::f32) dRate;

   //         Gdiplus::ColorMatrix matrix = {
   //            1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
   //            0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
   //            0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
   //            0.0f, 0.0f, 0.0f, fA  , 0.0f,
   //            0.0f, 0.0f, 0.0f, 0.0f, 1.0f
   //         };

   //         Gdiplus::ImageAttributes attributes;

   //         attributes.SetColorMatrix(&matrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);

   //         Gdiplus::RectF rectangleTarget((Gdiplus::REAL) xDest, (Gdiplus::REAL) yDest, (Gdiplus::REAL) nDestWidth, (Gdiplus::REAL) nDestHeight);

   //         m_pd2d1devicecontext->DrawImage((Gdiplus::Bitmap *) pgraphicsSrc->get_current_bitmap()->get_os_data(), rectangleTarget,
   //            (Gdiplus::REAL) xSrc, (Gdiplus::REAL) ySrc, (Gdiplus::REAL) nSrcWidth, (Gdiplus::REAL) nSrcHeight, Gdiplus::UnitPixel, &attributes);*/

   //   try
   //   {

   //      if(pgraphicsSrc == nullptr)
   //         return false;

   //      if(pgraphicsSrc->get_current_bitmap() == nullptr)
   //         return false;

   //      if(pgraphicsSrc->get_current_bitmap()->m_osdata[0] == nullptr)
   //         return false;

   //      //D2D1_RECT_F rectangleTarget = D2D1::RectF((::f32) xDst, (::f32) yDst, (::f32) (xDst + nDstWidth), (::f32) (yDst + nDstHeight));
   //      //D2D1_RECT_F rectangleSource = D2D1::RectF((::f32) xSrc, (::f32) ySrc, (::f32) (xSrc + nSrcWidth), (::f32) (ySrc + nSrcHeight));
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

   //      D2D1_RECT_F rDst = D2D1::RectF((::f32)rectangleTarget.left, (::f32)rectangleTarget.top, (::f32) rectangleTarget.right, (::f32) rectangleTarget.bottom);
   //      D2D1_RECT_F rSrc = D2D1::RectF((::f32)rectangleSource.left, (::f32)rectangleSource.top, (::f32) rectangleSource.right, (::f32) rectangleSource.bottom);


   //      //dynamic_cast <::draw2d_direct2d::graphics *> (pgraphicsSrc)->SaveClip();

   //      HRESULT hr = ((ID2D1DeviceContext *) pgraphicsSrc->get_os_data())->EndDraw();

   //      defer_primitive_blend();

   //      if(m_pdevicecontext != nullptr)
   //      {

   //         m_pdevicecontext->DrawBitmap((ID2D1Bitmap*)pgraphicsSrc->get_current_bitmap()->get_os_data(), rDst, (::f32)dRate, m_interpolationmode, rSrc);

   //      }
   //      else
   //      {

   //         m_pd2d1devicecontext->DrawBitmap((ID2D1Bitmap*)pgraphicsSrc->get_current_bitmap()->get_os_data(), rDst, (::f32)dRate, m_bitmapinterpolationmode, rSrc);

   //      }


   //      //auto pimage = (ID2D1Bitmap*)pgraphicsSrc->get_current_bitmap()->get_os_data();
   //      //auto ecomposite = m_ealphamode == ::draw2d::e_alpha_mode_blend ? D2D1_COMPOSITE_MODE_SOURCE_OVER : D2D1_COMPOSITE_MODE_SOURCE_IN;

   //      //m_pdevicecontext->DrawImage(pimage, pointDst, rectangleSource, m_interpolationmode, ecomposite);


   //      //hr = m_pd2d1devicecontext->Flush();
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


   /*void graphics::alpha_blend(::f64 xDest, ::f64 yDest, ::f64 dDestWidth, ::f64 dDestHeight,
   ::draw2d::graphics * pgraphicsSrc, ::f64 xSrc, ::f64 ySrc, ::f64 dSrcWidth, ::f64 dSrcHeight, BLENDFUNCTION blend)
   {

   throw ::exception(not_implemented_exception());
   //if(get_handle1() == nullptr)
   // return false;


   if(m_pimageAlphaBlend->is_set())
   {


   ::i32_rectangle rectangleIntersect(m_pointAlphaBlend, m_pimageAlphaBlend->size());


   ::image::image_pointer pimageWork = nullptr;
   ::image::image_pointer pimageWork2 = nullptr;
   //         ::image::image_pointer pimageWork3 = nullptr;
   ::image::image_pointer pimageWork4 = nullptr;


   ::f64_point pointSrc(xSrc, ySrc);
   ::f64_point pointDest(xDest, yDest);
   ::f64_size::f64_size(nDestWidth, nDestHeight);



   ::image::image_pointer pimage;
   if(imageWork == nullptr)
   {
   pimage = create_image(this);
   imageWork = pimage;
   }
   if(imageWork == nullptr)
   return false;
   if(!imageWork = create_image(::f64_size))
   return false;
   if(!imageWork.from(::f64_point(), pgraphicsSrc, pointSrc, ::f64_size))
   return false;




   ::image::image_pointer pimage2;
   if(imageWork2 == nullptr)
   {
   pimage2 = create_image(this);
   imageWork2 = pimage2;
   }


   ::image::image_pointer pimage4;
   if(imageWork4 == nullptr)
   {
   image4 = create_image(this);
   imageWork4 = image4;
   }
   if(imageWork4 == nullptr)
   return false;
   if(!imageWork4 = create_image(::f64_size))
   return false;


   imageWork4.Fill(255, 0, 0, 0);

   imageWork4.from(i32_point(maximum(0, m_pointAlphaBlend.x - xDest), maximum(0, m_pointAlphaBlend.y - yDest)),
   m_pimageAlphaBlend->get_graphics(), i32_point(maximum(0, xDest - m_pointAlphaBlend.x), maximum(0, yDest - m_pointAlphaBlend.y)), ::f64_size);

   imageWork.channel_multiply(::color::e_channel_opacity, imageWork4);


   keep < image > keep(&m_pimageAlphaBlend, nullptr, m_pimageAlphaBlend, true);


   return papplication->m_visual.imaging().true_blend(this, pointDest, ::f64_size, imageWork.get_graphics(), pointSrc);


   }

   return ::AlphaBlend(get_handle1(), xDest, yDest,
   nDestWidth, nDestHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth,
   nSrcHeight, blend) != false;
   }*/


   //   void graphics::TransparentBlt(::f64 xDest, ::f64 yDest, ::f64 dDestWidth, ::f64 dDestHeight, ::draw2d::graphics * pgraphicsSrc, ::f64 xSrc, ::f64 ySrc, ::f64 dSrcWidth, ::f64 dSrcHeight, ::u32 crTransparent)
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
   //   void graphics::GradientFill(TRIVERTEX* pVertices, ULONG nVertices, void * pMesh, ULONG nMeshElements, ::u32 dwMode)
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

      //inline void graphics::GetTextExtentExPointI(LPWORD pgiIn, ::i32 cgi, ::f64 dMaxExtent, LPINT lpnFit, LPINT alpDx, LPSIZE32 LPSIZE32)
      //{

      //   throw ::exception(todo);

      //   //ENSURE(LPSIZE32 != nullptr);
      //   //ASSERT(get_handle1() != nullptr);
      //   //return ::GetTextExtentExPointI(get_handle1(), pgiIn, cgi, nMaxExtent, lpnFit, alpDx, LPSIZE32) != false;

      //}


      //inline void graphics::GetTextExtentPointI(LPWORD pgiIn, ::i32 cgi, LPSIZE32 LPSIZE32)
      //{

      //   throw ::exception(todo);

      //   //ENSURE(LPSIZE32 != nullptr);
      //   //ASSERT(get_handle1() != nullptr);
      //   //return ::GetTextExtentPointI(get_handle1(), pgiIn, cgi, LPSIZE32) != false;

      //}








#define HIMETRIC_INCH   2540    // HIMETRIC units per inch

   void graphics::DPtoHIMETRIC(::f64_size * psize)
   {
      //#ifdef WINDOWS_DESKTOP
      //      ASSERT(is_memory_segment_ok(psize, sizeof(::f64_size)));
      //
      //      ::f64 dMapMode;
      //      if (this != nullptr && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
      //            nMapMode != MM_TEXT)
      //      {
      //         // when using a constrained ::map_base mode, ::map_base against physical inch
      //         ((::draw2d::graphics *)this)->SetMapMode(MM_HIMETRIC);
      //         DPtoLP(psize);
      //         ((::draw2d::graphics *)this)->SetMapMode(nMapMode);
      //      }
      //      else
      //      {
      //         // ::map_base against logical inch for non-constrained mapping modes
      //         ::i32 cxPerInch, cyPerInch;
      //         if (this != nullptr)
      //         {
      //            ASSERT_OK(this);
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

   void graphics::HIMETRICtoDP(::f64_size * psize)
   {
      //#ifdef WINDOWS_DESKTOP
      //      ASSERT(is_memory_segment_ok(psize, sizeof(::f64_size)));
      //
      //      ::f64 dMapMode;
      //      if (this != nullptr && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
      //            nMapMode != MM_TEXT)
      //      {
      //         // when using a constrained ::map_base mode, ::map_base against physical inch
      //         ((::draw2d::graphics *)this)->SetMapMode(MM_HIMETRIC);
      //         LPtoDP(psize);
      //         ((::draw2d::graphics *)this)->SetMapMode(nMapMode);
      //      }
      //      else
      //      {
      //         // ::map_base against logical inch for non-constrained mapping modes
      //         ::i32 cxPerInch, cyPerInch;
      //         if (this != nullptr)
      //         {
      //            ASSERT_OK(this);
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
      ASSERT(is_memory_segment_ok(LPSIZE32, sizeof(::i32_size)));

      LPtoDP(LPSIZE32);
      DPtoHIMETRIC(LPSIZE32);
   }

   void graphics::HIMETRICtoLP(LPSIZE32 LPSIZE32)
   {
      ASSERT(is_memory_segment_ok(LPSIZE32, sizeof(::i32_size)));

      HIMETRICtoDP(LPSIZE32);
      DPtoLP(LPSIZE32);
   }*/

   /////////////////////////////////////////////////////////////////////////////
   // special graphics drawing prototypes/helpers

   ::draw2d::brush * graphics::GetHalftoneBrush(::particle * pparticle)
   {
      /*
      ::aura::LockGlobals(CRIT_HALFTONEBRUSH);
      if (gen_HalftoneBrush == nullptr)
      {
      ::u16 grayPattern[8];
      for (::i32 i = 0; i < 8; i++)
      grayPattern[i] = (::u16)(0x5555 << (i & 1));
      HBITMAP grayBitmap = CreateBitmap(8, 8, 1, 1, grayPattern);
      if (grayBitmap != nullptr)
      {
      gen_HalftoneBrush = ::CreatePatternBrush(grayBitmap);
      ::DeleteObject(grayBitmap);
      }
      }
      if (!gen_WingdixTerm)
      gen_WingdixTerm = (::i8)!atexit(&__win_gdi_x_term);
      ::aura::UnlockGlobals(CRIT_HALFTONEBRUSH);
      */

      return nullptr;
   }


   //void graphics::DrawDragRect(const ::i32_rectangle & rectangle, const ::f64_size& ::f64_size, const ::i32_rectangle & rectangleLast, const ::f64_size& sizeLast, ::draw2d::brush* pBrush, ::draw2d::brush* pBrushLast)
   //{

   //   throw ::interface_only();

   //   /*
   //   ASSERT(is_memory_segment_ok(rectangle, sizeof(::f64_rectangle), false));
   //   ASSERT(lpRectLast == nullptr ||
   //   is_memory_segment_ok(lpRectLast, sizeof(::f64_rectangle), false));

   //   // first, determine the update region and select it
   //   ::draw2d::region rgnNew;
   //   ::draw2d::region rgnOutside, rgnInside;
   //   rgnOutside.CreateRectRgnIndirect(rectangle);
   //   const ::i32_rectangle & rectangle = *rectangle;
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
   //   // find difference between ___new region and old region
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

   //   // draw into the update/___new region
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


   //void graphics::fill_rectangle(const ::f64_rectangle & rectangle, const ::color::color & color)
   //{

   //   return fill_rectangle(::f64_rectangle(rectangle), color);

   //}


   //void graphics::draw_inset_3d_rectangle(const ::f64_rectangle & rectangle, const ::color::color & colorTopLeft, const ::color::color & colorBottomRight, const ::e_border & eborder)
   //{

   //   draw_inset_3d_rectangle(::f64_rectangle(rectangle), colorTopLeft, colorBottomRight, eborder);

   //}


   //void graphics::draw_inset_3d_rectangle(const ::f64_rectangle & rectangle,  const ::color::color & colorTopLeft, const ::color::color & colorBottomRight, const ::e_border & eborder)
   //{

   //   ::f64 x = rectangle.left;
   //   ::f64 y = rectangle.top;
   //   ::f64 cx = ::width(rectangle);
   //   ::f64 cy = ::height(rectangle);

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


   //::i32 graphics::StartDoc(const ::scoped_string & scopedstrDocName)
   //{

   //   throw ::exception(todo);

   //   //DOCINFO di;
   //   //memory_set(&di, 0, sizeof(DOCINFO));
   //   //di.cbSize = sizeof(DOCINFO);
   //   //di.lpszDocName = lpszDocName;
   //   //return StartDoc(&di);
   //}


   ::i32 graphics::save_graphics_context()
   {

      m_pd2d1devicecontext->GetTransform(&m_state.m_m);

      ::collection::count iSaveDC = m_statea.get_size();

      m_statea.add(m_state);

      m_state.m_iLayerIndex = m_iLayerCount;

      //m_pstate->m_layerparameters = D2D1::LayerParameters();

      //m_pd2d1devicecontext->PushLayer(m_pstate->m_layerparameters, nullptr);

      return (::i32)iSaveDC;

   }


   void graphics::restore_graphics_context(::i32 iSavedContext)
   {

      if (iSavedContext < 0)
      {

         //return false;

         throw ::exception(error_failed);

      }

      if (iSavedContext >= m_statea.get_count())
      {

         //return false;

         throw ::exception(error_failed);

      }

      //for (::collection::index iState = m_statea.get_upper_bound(); iState > nSavedDC; iState--)
      //{

      //   auto state = m_statea[iState];

      //   for (::collection::index iItem = state->m_maRegion.get_upper_bound(); iItem >= 0; iItem--)
      //   {

      //      m_pd2d1devicecontext->PopLayer();

      //   }

      //   state->m_maRegion.erase_all();

      //   state->m_sparegionClip.erase_all();

      //   m_statea.erase_at(iState);

      //}

      m_state = m_statea[iSavedContext];


      while (m_iLayerCount > m_state.m_iLayerIndex)
      {

         _pop_layer();

      }

      m_pd2d1devicecontext->SetTransform(&m_state.m_m);

      m_statea.set_size(iSavedContext);

      //return true;

   }


   //::i32 graphics::SetPolyFillMode(::i32 nPolyFillMode)
   //{

   //   throw ::exception(todo);

   //}


   //::i32 graphics::SetROP2(::i32 nDrawMode)
   //{

   //   throw ::exception(todo);

   //   //::f64 dRetVal = 0;
   //   //if(get_handle1() != nullptr && get_handle1() != get_handle2())
   //   //   nRetVal = ::SetROP2(get_handle1(), nDrawMode);
   //   //if(get_handle2() != nullptr)
   //   //   nRetVal = ::SetROP2(get_handle2(), nDrawMode);
   //   //return nRetVal;
   //}

   void graphics::set_interpolation_mode(::draw2d::enum_interpolation_mode einterpolationmode)
   {
      if (einterpolationmode == ::draw2d::e_interpolation_mode_nearest_neighbor)
      {
         m_d2d1bitmapinterpolationmode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
         m_d2d1interpolationmode = D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;;
      }
      else if (einterpolationmode == ::draw2d::e_interpolation_mode_high_quality_bicubic)
      {
         m_d2d1bitmapinterpolationmode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
         m_d2d1interpolationmode = D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;
      }
      else
      {
         m_d2d1bitmapinterpolationmode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
         m_d2d1interpolationmode = D2D1_INTERPOLATION_MODE_LINEAR;
      }
      //return 1;
      /*::f64 dRetVal = 0;
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

   //::i32 graphics::SetGraphicsMode(::i32 iMode)
   //{

   //   throw ::exception(todo);

   //   //::f64 dRetVal = 0;
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

   //void graphics::ModifyWorldTransform(const XFORM* pXform,::u32 iMode)
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


   //::i32 graphics::SetMapMode(::i32 nMapMode)
   //{

   //   return 0;

   //}


   //::f64_point graphics::set_origin(::f64 x, ::f64 y)
   //{

   //   return ::draw2d::graphics::set_origin(x, y);

   //}


   //::f64_point graphics::offset_origin(::f64 dWidth, ::f64 dHeight)
   //{

   //   return ::draw2d::graphics::offset_origin(dWidth, dHeight);

   //}


   ::f64_size graphics::set_extents(::f64 x, ::f64 y)
   {

      throw ::exception(todo);

      return {};

   }


   ::f64_size graphics::scale_extents(::f64 xNum, ::f64 xDenom, ::f64 yNum, ::f64 yDenom)
   {

      return ::draw2d::graphics::scale_extents(xNum, xDenom, yNum, yDenom);

   }


   ::f64_point graphics::SetWindowOrg(::f64 x, ::f64 y)
   {

      throw ::exception(todo);

      return {};

   }


   ::f64_point graphics::offset_window_org(::f64 dWidth, ::f64 dHeight)
   {

      throw ::exception(todo);

      return {};

   }


   //::f64_size graphics::set_window_ext(::f64 x, ::f64 y)
   //{

   //   throw ::exception(todo);

   //   return {};

   //}


   //::f64_size graphics::scale_window_ext(::f64 xNum, ::f64 xDenom, ::f64 yNum, ::f64 yDenom)
   //{

   //   throw ::exception(todo);

   //   return {};

   //}


   ::i32 graphics::get_clip_box(::f64_rectangle & rectangle)
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

      //for (::collection::index iState = m_statea.get_upper_bound(); iState >= 0; iState--)
      //{

      //   auto state = m_statea[iState];

      //   for (::collection::index iItem = state->m_maRegion.get_upper_bound(); iItem >= 0; iItem--)
      //   {

      //      m_pd2d1devicecontext->PopLayer();

      //   }

      //   state->m_maRegion.erase_all();

      //   state->m_sparegionClip.erase_all();

      //}

      //auto& pstate = m_pstate;

      //if (pstate)
      //{

      //   for (::collection::index iItem = pstate->m_maRegion.get_upper_bound(); iItem >= 0; iItem--)
      //   {

      //      m_pd2d1devicecontext->PopLayer();

      //   }

      //   pstate->m_maRegion.erase_all();

      //   pstate->m_sparegionClip.erase_all();

      //}

      //return ::success;

   }


   bool IsAxisAlignedRectGeometry(ID2D1Geometry* geometry, D2D1_RECT_F* outRect = nullptr)
   {
      
      comptr<ID2D1RectangleGeometry> rectGeom;

      if (SUCCEEDED(geometry->QueryInterface(IID_PPV_ARGS(&rectGeom))))
      {

         if (outRect)
         {

            rectGeom->GetRect(outRect);

         }
         
         return true;

      }
      
      return false;

   }


   bool IsAxisAligned(const D2D1_MATRIX_3X2_F& m)
   {
      
      return (m._21 == 0.0f && m._12 == 0.0f);

   }


   void graphics::_push_layer(const ::i32_rectangle_array_base& rectanglea)
   {

      m_iaPushLayer.add(1);

      m_iaPushLayerCount.add((::i32) rectanglea.size());

      //::f64_point pointImage;

      //if (m_pgraphicsbufferitem)
      //{

      //   if (m_pgraphicsbufferitem->m_pimageBufferItem)
      //   {

      //      pointImage = m_pgraphicsbufferitem->m_pimageBufferItem->m_point;

      //   }

      //}

      for (::i32 i = 0; i < rectanglea.size(); i++)
      {

         auto r = rectanglea[i];

         //r.offset(pointImage.x, pointImage.y);

         D2D1_RECT_F clipRect{(FLOAT) r.left, (FLOAT)r.top, (FLOAT)r.right, (FLOAT)r.bottom};

         m_pd2d1devicecontext->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

      }

      m_iLayerCount++;

   }


   void graphics::_push_layer(ID2D1Geometry * pgeometry)
   {

      D2D1_MATRIX_3X2_F transform;
      
      m_pd2d1devicecontext->GetTransform(&transform);

      bool isAxisAligned = IsAxisAligned(transform);

      D2D1_RECT_F clipRect;


      if (isAxisAligned && IsAxisAlignedRectGeometry(pgeometry, &clipRect))
      {

         m_iaPushLayer.add(1);

         m_iaPushLayerCount.add(1);

         m_pd2d1devicecontext->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

      }
      else
      {

         auto layerparameters = D2D1::LayerParameters(
            D2D1::InfiniteRect(),
            pgeometry);

         m_pd2d1devicecontext->PushLayer(layerparameters, nullptr);

         m_iaPushLayer.add(0);

         m_iaPushLayerCount.add(1);

      }

      m_iLayerCount++;

   }


   void graphics::_pop_layer()
   {

      if (m_iLayerCount <= 0)
      {

         return;

      }

      ::i32 iPop = m_iaPushLayer.pop();

      ::i32 iCount = m_iaPushLayerCount.pop();

      if (iPop == 1)
      {

         for (::i32 i = 0; i < iCount; i++)
         {
            m_pd2d1devicecontext->PopAxisAlignedClip();

         }
         
      }
      else
      {

         m_pd2d1devicecontext->PopLayer();
      }

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

   //   for (::i32 i = 0; i < shapea.get_count(); i++)
   //   {

   //      if (i + 1 < shapea.get_count())
   //      {

   //         if (shapea[i + 1]->eshape() == e_shape_intersect_clip)
   //         {

   //            switch (shapea[i]->eshape())
   //            {
   //            //case e_shape_rect:
   //            //   intersect_clip(shapea[i]->shape < ::i32_rectangle>());
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
   //            //   intersect_clip(shapea[i]->shape < ::i32_polygon>());
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


   //void graphics::intersect_clip(const ::f64_rectangle & rectangle)
   //{

   //   // ::draw2d::device_lock devicelock(this);

   //   {

   //      auto pregion = createø < ::draw2d::region > ();

   //      auto rectangleClip = i32_rectangle + m_pointAddShapeTranslate;

   //      pregion->create_rect(rectangleClip);

   //      D2D1::Matrix3x2F m = {};

   //      m_pd2d1devicecontext->GetTransform(&m);

   //      m_pstate->m_sparegionClip.add(pregion);

   //      m_pstate->m_maRegion.add(m);

   //      ID2D1Geometry* pgeometry = (ID2D1Geometry*)pregion->get_os_data(this);

   //      auto layerparameters = D2D1::LayerParameters(
   //         D2D1::InfiniteRect(),
   //         pgeometry);

   //      m_pd2d1devicecontext->PushLayer(layerparameters, nullptr);

   //   }

   //   return 0;

   //}



   void graphics::_intersect_clip()
   {

   }


   void graphics::_add_shape(const ::f64_rectangle & rectangle)
   {

      throw ::exception(error_not_supported);

   }


   void graphics::_add_shape(const ::f64_ellipse & ellipse)
   {

      throw ::exception(error_not_supported);

   }


   void graphics::_add_shape(const ::f64_polygon_base & polygon)
   {

      throw ::exception(error_not_supported);

   }




   ////void graphics::_add_clipping_shape(const ::f64_rectangle & rectangle, ___shape<::draw2d::region > & shaperegion)
   //void graphics::_add_clipping_shape(const ::f64_rectangle & rectangle, ::draw2d::region * pregion)
   //{

   //   //::draw2d::lock draw2dlock;

   //   // ::draw2d::device_lock devicelock(this);

   //   //if (!shaperegion.holdee())
   //   //{

   //   //   auto pregion = createø < ::draw2d::region >();

   //   //   auto rectangleClip = rectangle + m_pointAddShapeTranslate;

   //   //   pregion->create_rectangle(rectangleClip);

   //   //   shaperegion.holdee(pregion);

   //   //}

   //   //D2D1::Matrix3x2F m = {};

   //   //m_pd2d1devicecontext->GetTransform(&m);

   //   //m_pstate->m_sparegionClip.add(pregion);

   //   //m_pstate->m_maRegion.add(m);

   //   comptr<ID2D1RectangleGeometry> pgeometry;

   //   D2D1_RECT_F r;

   //   copy(r, rectangle);

   //   direct2d()->d2d1_factory1()->CreateRectangleGeometry(r, &pgeometry);

   //   //return pgeometry;


   //   //      ID2D1Geometry * pgeometry = (ID2D1Geometry *)shaperegion.holdee()->get_os_data(this);

   //   _push_layer(pgeometry);

   //   //return 0;


   //}




   ////void graphics::intersect_clip(const ::oval& oval)
   ////{

   ////   // ::draw2d::device_lock devicelock(this);

   ////   {

   ////      auto pregion = createø < ::draw2d::region > ();

   ////      pregion->create_oval(oval);

   ////      D2D1::Matrix3x2F m = {};

   ////      m_pd2d1devicecontext->GetTransform(&m);

   ////      m_pstate->m_sparegionClip.add(pregion);

   ////      m_pstate->m_maRegion.add(m);

   ////      ID2D1Geometry* pgeometry = (ID2D1Geometry*)pregion->get_os_data(this);

   ////      m_pd2d1devicecontext->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), nullptr);

   ////   }

   ////   return 0;


   ////}


   //void graphics::_add_clipping_shape(const ::f64_ellipse & ellipse, ::draw2d::region * pdraw2dregion)
   //{

   //   //::draw2d::lock draw2dlock;

   //   // ::draw2d::device_lock devicelock(this);

   //   {

   //      //if (!shaperegion.holdee())
   //      //{

   //      //   auto pregion = createø < ::draw2d::region >();

   //      //   auto rectangleClip = ellipse + m_pointAddShapeTranslate;

   //      //   pregion->create_ellipse(ellipse);

   //      //   shaperegion.holdee(pregion);

   //      //}

   //      //D2D1::Matrix3x2F m = {};

   //      //m_pd2d1devicecontext->GetTransform(&m);

   //      //m_pstate->m_sparegionClip.add(pregion);

   //      //m_pstate->m_maRegion.add(m);

   //      //ID2D1Geometry* pgeometry = (ID2D1Geometry*)shaperegion.holdee()->get_os_data(this);

   //      auto pgeometry = ::direct2d::geometry::create_ellipse(ellipse);

   //      //m_pd2d1devicecontext->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), nullptr);

   //      _push_layer(pgeometry);

   //   }

   //   //return 0;


   //}

   void graphics::intersect_clip(const ::draw2d::clip_group& clipgroup)
   {

      comptr<ID2D1PathGeometry> ppathgeometry;

      HRESULT hr = direct2d()->d2d1_factory1()->CreatePathGeometry(&ppathgeometry);

      ::i32_rectangle_array_base rectanglea;

      if (clipgroup.is_rectangle_only(rectanglea))
      {

         _push_layer(rectanglea);

      }
      else
      {

         comptr<ID2D1GeometrySink> pgeometrysink;

         ppathgeometry->Open(&pgeometrysink);

         pgeometrysink->SetFillMode(D2D1_FILL_MODE_WINDING);

         for (auto& pclipitem : clipgroup)
         {

            _add_clip_item(pgeometrysink, pclipitem);

         }

         pgeometrysink->Close();

         _push_layer(ppathgeometry);

      }

   }


   void graphics::_add_clip_item(::draw2d::clip_item * pclipitem)
   {

      throw ::exception(error_not_supported);

   }


   void graphics::_add_clip_item(ID2D1GeometrySink * pgeometrysink, ::draw2d::clip_item * pclipitem)
   {

      switch (pclipitem->clip_item_type())
      {
      case ::draw2d::e_clip_item_rectangle:
         ::direct2d::geometry::add_rectangle(pgeometrysink, dynamic_cast<::draw2d::clip_rectangle *>(pclipitem)->m_item);
         break;
      case ::draw2d::e_clip_item_ellipse:
         ::direct2d::geometry::add_ellipse(pgeometrysink, dynamic_cast<::draw2d::clip_ellipse *>(pclipitem)->m_item);
         break;
      case ::draw2d::e_clip_item_polygon:
         ::direct2d::geometry::add_polygon(pgeometrysink, dynamic_cast<::draw2d::clip_polygon *>(pclipitem)->m_item);
         break;
      default:
         break;
      };

   }



   //void graphics::intersect_clip(const ::i32_polygon& i32_polygon)
   //{

   //   // ::draw2d::device_lock devicelock(this);

   //   {

   //      auto pregion = createø < ::draw2d::region > ();

   //      pregion->create_polygon(i32_polygon.get_data(), (::i32) i32_polygon.get_count(), ::draw2d::e_fill_mode_winding);

   //      D2D1::Matrix3x2F m = {};

   //      m_pd2d1devicecontext->GetTransform(&m);

   //      m_pstate->m_sparegionClip.add(pregion);

   //      m_pstate->m_maRegion.add(m);

   //      ID2D1Geometry* pgeometry = (ID2D1Geometry*)pregion->get_os_data(this);

   //      m_pd2d1devicecontext->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), nullptr);

   //   }

   //   return 0;


   //}


   //void graphics::_add_clipping_shape(const ::f64_polygon& polygon, ::draw2d::region * pdraw2dregion)
   //{

   //   //::draw2d::lock draw2dlock;

   //   // ::draw2d::device_lock devicelock(this);

   //   {

   //      //if (!shaperegion.holdee())
   //      //{

   //      //   auto pregion = createø < ::draw2d::region >();

   //      //   pregion->m_pointOffset = m_pointAddShapeTranslate;

   //      //   pregion->create_polygon(i32_polygon.data(), (::i32)i32_polygon.get_count(), ::draw2d::e_fill_mode_winding);

   //      //   shaperegion.holdee(pregion);

   //      //}

   //      //D2D1::Matrix3x2F m = {};

   //      //m_pd2d1devicecontext->GetTransform(&m);

   //      //m_pstate->m_sparegionClip.add(pregion);

   //      //m_pstate->m_maRegion.add(m);

   //      //auto pellipsegeometry = ::direct2d::geometry::;

   //      auto pgeometry = ::direct2d::geometry::create_polygon(polygon);

   //      _push_layer(pgeometry);

   //      //m_pd2d1devicecontext->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), nullptr);

   //   }

   //   //return 0;


   //}


   void graphics::intersect_clip(const ::f64_rectangle & rectangle)
   {

      if (m_bTargetRectangleModified)
      {

         defer_on_target_rectangle_update();

      }
      //auto r = rectangle + m_pointAddShapeTranslate;

      auto r = rectangle;

      //::f64_point pointImage;

      //if (m_pgraphicsbufferitem)
      //{

      //   if (m_pgraphicsbufferitem->m_pimageBufferItem)
      //   {

      //      pointImage = m_pgraphicsbufferitem->m_pimageBufferItem->m_point;

      //   }

      //}

      //r += pointImage;

      D2D1_RECT_F rf;

      copy(&rf, &r);

      auto layerparameters = D2D1::LayerParameters(rf);

      //m_pd2d1devicecontext->PushLayer(layerparameters, nullptr);

      m_iaPushLayer.add(1);
      m_iaPushLayerCount.add(1);

      m_pd2d1devicecontext->PushAxisAlignedClip(rf, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);


      m_iLayerCount++;

   }


   //void graphics::

   //   }
   //   else
   //   {

   //      D2D1::Matrix3x2F m;

   //      m_pd2d1devicecontext->GetTransform(&m);

   //      m_pstate->m_sparegionClip.add(pregion);

   //      m_pstate->m_maRegion.add(m);

   //      auto pgeometry = pregion->get_os_data < ID2D1Geometry * >(this);

   //      m_pd2d1devicecontext->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(),pgeometry),nullptr);

   //   }

   //   return 0;

   //   //throw ::exception(todo);


   //   //if(pregion == nullptr)
   //   //{
   //   //   m_pd2d1devicecontext->ResetClip();
   //   //}
   //   //else
   //   //{
   //   //   m_pd2d1devicecontext->SetClip((Gdiplus::Region *) pregion->get_os_data());
   //   //}

   //   //return 0;

   //   ///*      ::f64 dRetVal = ERROR;
   //   //if(get_handle1() != nullptr && get_handle1() != get_handle2())
   //   //nRetVal = ::SelectClipRgn(get_handle1(), pRgn == nullptr ? nullptr : (HRGN) pRgn->get_os_data());
   //   //if(get_handle2() != nullptr)
   //   //nRetVal = ::SelectClipRgn(get_handle2(), pRgn == nullptr ? nullptr : (HRGN) pRgn->get_os_data());
   //   //return nRetVal;*/
   //}

   //::i32 graphics::ExcludeClipRect(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2)
   //{

   //   // ::draw2d::device_lock devicelock(this);

   //   {

   //      ::u32 uMax = m_pdevicecontext->GetMaximumBitmapSize();

   //      ::draw2d::region_pointer regionBig(e_create);

   //      regionBig->create_rect_dim(-(::i32) uiMax/2, -(::i32)uiMax / 2, uiMax / 2, uiMax / 2);

   //      ::draw2d::region_pointer regionSmall(e_create);

   //      regionSmall->create_rect_dim(x1, y1, x2, y2);

   //      ::draw2d::region_pointer regionExclude(e_create);

   //      regionExclude->combine(regionBig, regionSmall, ::draw2d::e_combine_exclude);

   //      D2D1::Matrix3x2F m;

   //      m_pd2d1devicecontext->GetTransform(&m);

   //      m_pstate->m_sparegionClip.add(regionExclude);

   //      m_pstate->m_maRegion.add(m);

   //      auto pgeometry = regionExclude->get_os_data < ID2D1Geometry * >(this);

   //      m_pd2d1devicecontext->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pgeometry), nullptr);

   //   }

   //   return 0;

   //}

   //::i32 graphics::ExcludeClipRect(const ::i32_rectangle & rectangle)
   //{

   //   return ExcludeClipRect(rectangle.left, rectangle.top, rectangle.right, rectangle.bottom);

   //}



   //::i32 graphics::OffsetClipRgn(::f64 x, ::f64 y)
   //{

   //   throw ::exception(todo);

   //}


   //::i32 graphics::OffsetClipRgn(const ::f64_size& ::f64_size)
   //{

   //   throw ::exception(todo);

   //}


   ::u32 graphics::SetTextAlign(::u32 nFlags)
   {

      throw ::exception(todo);

      return 0;

   }


   //::i32 graphics::SetTextJustification(::f64 dBreakExtra, ::f64 dBreakCount)
   //{

   //   throw ::exception(todo);

   //}


   //::i32 graphics::SetTextCharacterExtra(::f64 dCharExtra)
   //{

   //   throw ::exception(todo);

   //}


   //::u32 graphics::SetMapperFlags(::u32 dwFlag)
   //{

   //   throw ::exception(todo);

   //}


   typedef ::u32(CALLBACK * __GDIGETLAYOUTPROC)(HDC);
   typedef ::u32(CALLBACK * __GDISETLAYOUTPROC)(HDC, ::u32);


   void graphics::apply_primitive_blend()
   {

      if (m_ealphamodeDevice != m_ealphamode)
      {

         m_ealphamodeDevice = m_ealphamode;

         if (m_ealphamodeDevice == ::draw2d::e_alpha_mode_blend)
         {

            m_pd2d1devicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);

         }
         else if (m_ealphamodeDevice == ::draw2d::e_alpha_mode_set)
         {

            m_pd2d1devicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);

         }

      }

   }


   void graphics::apply_text_rendering_hint()
   {

      try
      {

         if (m_pd2d1devicecontext == nullptr)
         {

            return;

         }

         m_etextrenderinghintDevice = m_ewritetextrendering;

         switch (m_etextrenderinghintDevice)
         {
         case ::write_text::e_rendering_anti_alias:

            m_pd2d1devicecontext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

            break;

         case ::write_text::e_rendering_anti_alias_grid_fit:

            m_pd2d1devicecontext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

            break;

         case ::write_text::e_rendering_single_bit_per_pixel:

            m_pd2d1devicecontext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

            break;

         case ::write_text::e_rendering_clear_type_grid_fit:

            m_pd2d1devicecontext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

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

         m_pd2d1devicecontext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_SOURCE_OVER);

      }

   }


   //oswindow graphics::get_window_handle()
   //{

   //   if (::is_null(m_puserinteractionDraw2dGraphics))
   //   {

   //      return nullptr;

   //   }

   //   return m_puserinteractionDraw2dGraphics->get_oswindow();

   //}


   ::u32 graphics::GetLayout()
   {

      throw ::exception(todo);

      return 0;

   }


   ::u32 graphics::SetLayout(::u32 dwSetLayout)
   {

      throw ::exception(todo);

      return 0;

   }


   //void graphics::on_start_layer(::gpu::layer* pgpulayer)
   //{

   //   //if (m_pdevicecontext && m_bInLayer)
   //   //if (m_pdevicecontext)
   //   {

   //      direct2d()->m_pd2d1multithread->Enter();

   //      m_pdevicecontext->BeginDraw();

   //      m_pdevicecontext->Clear();

   //   }

   //}


   //void graphics::on_end_layer(::gpu::layer* pgpulayer)
   //{

   //   //if (m_pdevicecontext)
   //   {

   //      m_pdevicecontext->Flush();

   //      m_pdevicecontext->EndDraw();

   //      direct2d()->m_pd2d1multithread->Leave();



   //   }

   //   defer_soft_unbind_draw2d_compositor(pgpulayer);

   //}

   //void graphics::arc_to(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2, ::f64 x3, ::f64 y3, ::f64 x4, ::f64 y4)
   //{

   //   throw ::exception(todo);

   //   //return false;

   //}


   //::i32 graphics::SetArcDirection(::f64 dArcDirection)
   //{

   //   throw ::exception(todo);

   //}


   void graphics::polydraw(const ::f64_point * ppoints, const ::u8 * lpTypes, ::collection::count nCount)
   {

      throw ::exception(todo);

      //return false;

   }


   void graphics::polyline_to(const ::f64_point * ppoints, ::collection::count nCount)
   {

      throw ::exception(todo);

      //return false;

   }


   void graphics::poly_bezier_to(const ::f64_point * ppoints, ::collection::count nCount)
   {

      throw ::exception(todo);

      //return false;

   }


   void graphics::SelectClipPath(::i32 nMode)
   {

      //throw ::exception(todo);

      //return false;

   }


   //::i32 graphics::SelectClipRgn(::draw2d::region* pRgn, ::draw2d::enum_combine ecombine)
   //{

   //   return 0;

   //}

//   /////////////////////////////////////////////////////////////////////////////
//   // Special handling for metafile playback
//#ifdef WINDOWS_DESKTOP
//   ::i32 CALLBACK __enum_meta_file_procedure(HDC hDC,
//                                           HANDLETABLE * pHandleTable, METARECORD * pMetaRec, ::f64 dHandles, LPARAM lParam)
//   {
//
//      ::draw2d::graphics * pdraw2dgraphics = (::draw2d::graphics *)lParam;
//
//      ASSERT_OK(pdraw2dgraphics);
//
//      switch (pMetaRec->rdFunction)
//      {
//         // these records have effects different for each graphics derived class
//      case META_SETMAPMODE:
//         //(dynamic_cast<::draw2d_direct2d::graphics * >(pdraw2dgraphics))->SetMapMode((::i32)(::i16)pMetaRec->rdParm[0]);
//         break;
//      case META_SETWINDOWEXT:
//         (dynamic_cast<::draw2d_direct2d::graphics *>(pdraw2dgraphics))->set_window_ext(
//         (::i32)(::i16)pMetaRec->rdParm[1], (::i32)(::i16)pMetaRec->rdParm[0]);
//         break;
//      case META_SETWINDOWORG:
//         (dynamic_cast<::draw2d_direct2d::graphics *>(pdraw2dgraphics))->SetWindowOrg(
//         (::i32)(::i16)pMetaRec->rdParm[1], (::i32)(::i16)pMetaRec->rdParm[0]);
//         break;
//      case META_SETVIEWPORTEXT:
//         (dynamic_cast<::draw2d_direct2d::graphics *>(pdraw2dgraphics))->set_extents(
//         (::i32)(::i16)pMetaRec->rdParm[1], (::i32)(::i16)pMetaRec->rdParm[0]);
//         break;
//      case META_SETVIEWPORTORG:
//         (dynamic_cast<::draw2d_direct2d::graphics *>(pdraw2dgraphics))->set_origin(
//         (::i32)(::i16)pMetaRec->rdParm[1], (::i32)(::i16)pMetaRec->rdParm[0]);
//         break;
//      case META_SCALEWINDOWEXT:
//         (dynamic_cast<::draw2d_direct2d::graphics *>(pdraw2dgraphics))->scale_window_ext(
//         (::i32)(::i16)pMetaRec->rdParm[3], (::i32)(::i16)pMetaRec->rdParm[2],
//         (::i32)(::i16)pMetaRec->rdParm[1], (::i32)(::i16)pMetaRec->rdParm[0]);
//         break;
//      case META_SCALEVIEWPORTEXT:
//         (dynamic_cast<::draw2d_direct2d::graphics *>(pdraw2dgraphics))->scale_extents(
//         (::i32)(::i16)pMetaRec->rdParm[3], (::i32)(::i16)pMetaRec->rdParm[2],
//         (::i32)(::i16)pMetaRec->rdParm[1], (::i32)(::i16)pMetaRec->rdParm[0]);
//         break;
//      case META_OFFSETVIEWPORTORG:
//         (dynamic_cast<::draw2d_direct2d::graphics *>(pdraw2dgraphics))->offset_origin(
//         (::i32)(::i16)pMetaRec->rdParm[1], (::i32)(::i16)pMetaRec->rdParm[0]);
//         break;
//      case META_SAVEDC:
//         (dynamic_cast<::draw2d_direct2d::graphics *>(pdraw2dgraphics))->save_graphics_context();
//         break;
//      case META_RESTOREDC:
//         (dynamic_cast<::draw2d_direct2d::graphics *>(pdraw2dgraphics))->restore_graphics_context((::i32)(::i16)pMetaRec->rdParm[0]);
//         break;
//      case META_SETBKCOLOR:
//      {
//
//         auto pdraw2dbrush = pdraw2dgraphics->createø < ::draw2d::brush >();
//
//         pdraw2dbrush->create_solid(*(UNALIGNED::color::color *)&pMetaRec->rdParm[0]);
//
//         (dynamic_cast<::draw2d_direct2d::graphics *>(pdraw2dgraphics))->set(pdraw2dbrush);
//
//      }
//      break;
//      case META_SETTEXTCOLOR:
//      {
//         auto pdraw2dbrush = pdraw2dgraphics->createø < ::draw2d::brush >();
//         pdraw2dbrush->create_solid(*(UNALIGNED::color::color *)&pMetaRec->rdParm[0]);
//         (dynamic_cast<::draw2d_direct2d::graphics *>(pdraw2dgraphics))->set(pdraw2dbrush);
//      }
//      break;
//
//      // need to watch out for SelectObject(HFONT), for custom font mapping
//      case META_SELECTOBJECT:
//      {
//         HGDIOBJ hObject = pHandleTable->objectHandle[pMetaRec->rdParm[0]];
//         ::u32 nObjType = GetObjectType(hObject);
//         if (nObjType == 0)
//         {
//            // object type is unknown, determine if it is a font
//            HFONT hStockFont = (HFONT)::GetStockObject(SYSTEM_FONT);
//            //HFONT hFontOld = (HFONT)::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pdraw2dgraphics))->get_handle1(), hStockFont);
//            //HGDIOBJ hObjOld = ::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pdraw2dgraphics))->get_handle1(), hObject);
//            //if (hObjOld == hStockFont)
//            //{
//            //   // got the stock object back, so must be selecting a font
//            //   throw ::not_implemented();
//            //   //                  (dynamic_cast<::draw2d_direct2d::graphics * >(pdraw2dgraphics))->SelectObject(::draw2d_direct2d::font::from_handle_dup(pdraw2dgraphics->get_application(), (HFONT)hObject));
//            //   break;  // don't play the default record
//            //}
//            //else
//            //{
//            //   // didn't get the stock object back, so restore everything
//            //   ::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pdraw2dgraphics))->get_handle1(), hFontOld);
//            //   ::SelectObject((dynamic_cast<::draw2d_direct2d::graphics * >(pdraw2dgraphics))->get_handle1(), hObjOld);
//            //}
//            // and fall through to PlayMetaFileRecord...
//         }
//         else if (nObjType == OBJ_FONT)
//         {
//            // play back as graphics::SelectObject(::write_text::font*)
//            //               (dynamic_cast<::draw2d_direct2d::graphics * >(pdraw2dgraphics))->SelectObject(::draw2d_direct2d::font::from_handle_dup(pdraw2dgraphics->get_application(), (HFONT)hObject));
//            throw ::not_implemented();
//            break;  // don't play the default record
//         }
//      }
//      // fall through...
//
//      default:
//         //::PlayMetaFileRecord(hDC, pHandleTable, pMetaRec, nHandles);
//         break;
//      }
//
//      return 1;
//   }
//#endif


   //void graphics::PlayMetaFile(HMETAFILE hMF)
   //{

   //   throw ::exception(todo);

   //}


   void graphics::LPtoDP(::f64_size * psize)
   {

      ASSERT(is_memory_segment_ok(psize, sizeof(::f64_size)));

      ::f64_size sizeWinExt = GetWindowExt();
      ::f64_size sizeVpExt = get_extents();
      psize->cx = psize->cx * abs(sizeVpExt.cx) / abs(sizeWinExt.cx);
      psize->cy = psize->cy * abs(sizeVpExt.cy) / abs(sizeWinExt.cy);

   }


   void graphics::DPtoLP(::f64_size * psize)
   {

      ASSERT(is_memory_segment_ok(psize, sizeof(::f64_size)));

      ::f64_size sizeWinExt = GetWindowExt();
      ::f64_size sizeVpExt = get_extents();
      psize->cx = psize->cx * abs(sizeWinExt.cx) / abs(sizeVpExt.cx);
      psize->cy = psize->cy * abs(sizeWinExt.cy) / abs(sizeVpExt.cy);

   }


   //void graphics::draw_text(const ::scoped_string & scopedstrString, character_count nCount, const ::i32_rectangle & rectangle, const ::e_align & ealign, const ::e_draw_text & edrawtext)
   //{

   //   if (nCount < 0)
   //   {

   //      nCount = strlen(lpszString) + nCount + 1;

   //   }

   //   return draw_text(string(lpszString, nCount), rectangle, nFormat);

   //}


   void graphics::draw_text(const ::scoped_string & scopedstr, const ::f64_rectangle & rectangle, const ::e_align & ealign, const ::e_draw_text & edrawtext)
   {

      if (scopedstr.is_empty())
      {

         //return true;

         return;

      }

      if (scopedstr.is_empty())
      {

         //return false;

         throw ::exception(error_invalid_empty_argument);

      }

      if (m_pwritetextfont.is_null())
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      if (m_pdraw2dbrush.is_null())
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      if (m_bTargetRectangleModified)
      {

         defer_on_target_rectangle_update();

      }

      m_pdraw2dbrush->defer_update(this);

      ::cast < ::draw2d_direct2d::brush>pdraw2ddirect2dbrush = m_pdraw2dbrush;

      auto pd2d1brush = pdraw2ddirect2dbrush->m_pd2d1brush;

      if (::is_null(pd2d1brush))
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      m_pwritetextfont->update(this);

      ::cast < ::draw2d_direct2d::font > pdraw2ddirect2dfont = m_pwritetextfont;

      auto pdwritetextformat = pdraw2ddirect2dfont->m_pdwritetextformat;

      if (::is_null(pdwritetextformat))
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      if (ealign & e_align_right)
      {

         pdwritetextformat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

      }
      else if (ealign & e_align_horizontal_center)
      {

         pdwritetextformat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

      }
      else
      {

         pdwritetextformat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

      }

      if (ealign & e_align_bottom)
      {

         pdwritetextformat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);

      }
      else if (ealign & e_align_vertical_center)
      {

         pdwritetextformat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

      }
      else
      {

         pdwritetextformat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

      }

      synchronous_lock synchronouslock(this->synchronization());

      synchronous_lock synchronouslockFontTextMap(::write_text::font::s_pmutexFontTextMap);

      auto & text = m_pwritetextfont->m_mapFontText[scopedstr];

      auto ptextitem = text.get_item(::write_text::font::text::e_size_backend_draw_text);

      if (ptextitem->get_text().is_empty())
      {

         ptextitem->set_text(scopedstr);

      }

      defer_text_primitive_blend();

      defer_text_rendering_hint();

      if (m_pwritetextfont->m_dFontWidth == 1.0)
      {

         D2D1_RECT_F rectf = D2D1::RectF((FLOAT)rectangle.left, (FLOAT)rectangle.top, (FLOAT)rectangle.right, (FLOAT)rectangle.bottom);

         m_pd2d1devicecontext->DrawText(ptextitem->get_text(), (::u32)ptextitem->get_text().length(), pdwritetextformat,
                                       &rectf, pd2d1brush);

      }
      else
      {
         D2D1::Matrix3x2F m;

         m_pd2d1devicecontext->GetTransform(&m);

         D2D1::Matrix3x2F mOriginal(m);

         D2D1_RECT_F rectf = D2D1::RectF((FLOAT)0, (FLOAT)0, (FLOAT)width(rectangle), (FLOAT)height(rectangle));

         m = m * D2D1::Matrix3x2F::Translation((FLOAT)rectangle.left, (FLOAT)rectangle.top);

         m = m * D2D1::Matrix3x2F::Scale((FLOAT)m_pwritetextfont->m_dFontWidth, 1.f);

         m_pd2d1devicecontext->SetTransform(&m);

         m_pd2d1devicecontext->DrawText(ptextitem->get_text(), (::u32)ptextitem->get_text().length(),
                                       pdwritetextformat,
                                       &rectf,
                                       pd2d1brush);

         m_pd2d1devicecontext->SetTransform(&mOriginal);

      }

      //return 1;

   }


   //::f64_size graphics::get_text_extent(const ::scoped_string & scopedstr, character_count iIndex)
   //{

   //   ::f64_size sz;

   //   //bool bOk = 
   //   
   //   get_text_extent(sz, lpszString, nCount, iIndex);

   //   //if (!bOk)
   //   //{

   //   //   return ::f64_size(0, 0);

   //   //}
   //   //else
   //   {

   //      return ::f64_size(sz.cx, sz.cy);

   //   }

   //}


   //::f64_size graphics::get_text_extent(const block & block)
   //{

   //   ::f64_size s;

   //   get_text_extent(s, (const_char_pointer )block.get_data(), block.get_size());

   //   //if (!get_text_extent(s, (const_char_pointer )block.get_data(), block.get_size()))
   //   //{

   //   //   return ::f64_size(0, 0);

   //   //}

   //   return s;

   //}


   //::f64_size graphics::GetOutputTextExtent(const_char_pointer pszString, character_count nCount)
   //{

   //   throw ::exception(todo);

   //   return nullptr;

   //}


   //::f64_size graphics::GetOutputTextExtent(const ::scoped_string & scopedstr)
   //{

   //   throw ::exception(todo);

   //   return nullptr;

   //}


   //void graphics::get_text_extent(::f64_size & size, const_char_pointer pszString, character_count nCount, character_count iIndex)
   ::f64_size graphics::_get_text_extent(const ::scoped_string & scopedstr, character_count iIndex)
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

      if (m_pwritetextfont.is_null())
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      m_pwritetextfont->update(this);

      ::cast < ::draw2d_direct2d::font > pdraw2ddirect2dfont = m_pwritetextfont;

      auto pdwritetextformat = pdraw2ddirect2dfont->m_pdwritetextformat;

      if (pdwritetextformat == nullptr)
      {

         //return false;
         throw ::exception(error_null_pointer);

      }

      synchronous_lock synchronouslock(this->synchronization());

      synchronous_lock synchronouslockFontTextMap(::write_text::font::s_pmutexFontTextMap);

      auto & text = m_pwritetextfont->m_mapFontText[range];

      auto ptextitem = text.get_item(::write_text::font::text::e_size_backend_draw_text);

      if (ptextitem->has_size())
      {

         return ptextitem->get_size();

      }

      if (ptextitem->get_text().is_empty())
      {

         ptextitem->set_text(range);

      }

      ::f64_size size;

      comptr<IDWriteTextLayout> playout1;

      HRESULT hr;

      comptr<IDWriteTextLayout> playout;

      ::u32 uLength = (::u32)ptextitem->get_text().length();

      hr = direct2d()->dwrite_factory()->CreateTextLayout(
         ptextitem->get_text(), // The string to be laid out and formatted.
           uLength,   // The length of the string.
           pdwritetextformat,    // The text format to apply to the string (contains font information, etc).
           1024.f * 1024.f,               // The width of the on_layout box.
           1024.f * 1024.f,        // The height of the on_layout box.
           &playout  // The IDWriteTextLayout interface pointer.
      );

      if (playout == nullptr || FAILED(hr))
      {

         size.cx = 0;

         size.cy = 0;

         //return false;

         throw ::exception(error_null_pointer);

      }

      DWRITE_TEXT_METRICS m;

      playout->GetMetrics(&m);

      size.cx = (::f64)(m.widthIncludingTrailingWhitespace * m_pwritetextfont->m_dFontWidth);

      size.cy = m.height;

      ptextitem->set_size(size);

      return size;

   }


   //void graphics::get_text_extent(::f64_size & size, const_char_pointer pszString, character_count nCount)
   ::f64_size graphics::_get_text_extent(const ::scoped_string & scopedstr)
   {

      //if (nCount < 0)
      //{

      //   nCount = strlen(lpszString) + nCount + 1;

      //}

      //return get_text_extent(size, lpszString, nCount, nCount);

      return _get_text_extent(scopedstr, scopedstr.size());

   }


   //void graphics::get_text_extent(::f64_size & size, const ::scoped_string & scopedstr)
   //{

   //   return get_text_extent(size, str, str.length());

   //}


   void graphics::fill_rectangle(const ::f64_rectangle & rectangleParam, const ::color::color & color)
   {

      if (!m_pd2d1devicecontext)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      if (m_bTargetRectangleModified)
      {

         defer_on_target_rectangle_update();

      }

      D2D1_COLOR_F d2d1color;

      copy(d2d1color, color);

      comptr< ID2D1SolidColorBrush> psolidbrush;

      m_pd2d1devicecontext->CreateSolidColorBrush(d2d1color, &psolidbrush);

      D2D1_RECT_F rectangle;

      copy(&rectangle, &rectangleParam);

      defer_primitive_blend();

      m_pd2d1devicecontext->FillRectangle(&rectangle, psolidbrush);

   }


   bool graphics::is_memory_graphics_pool_compatible(
      ::acme::user::interaction * pacmeuserinteractionAffinity) const
   {

      if (!pacmeuserinteractionAffinity
         || m_bBeginDraw
         || m_pimage
         || m_pdraw2dbitmap
         || m_iLayerCount != 0
         || m_pd2d1devicecontext
         || m_pd2d1devicecontext)
      {

         return false;

      }

      ::cast < ::windowing::window > pwindow =
         pacmeuserinteractionAffinity->acme_windowing_window();

      if (!pwindow)
      {

         return false;

      }

      ::cast < ::draw2d_direct2d::window_attachment > pwindowattachment =
         pwindow->m_pdraw2dwindowattachment;

      if (!pwindowattachment)
      {

         return false;

      }

      _synchronous_lock synchronouslock(
         pwindowattachment->_d2d1_device_context_mutex());

      auto pdevicecontext = pwindowattachment->_d2d1_device_context();

      if (!pdevicecontext)
      {

         return false;

      }

      comptr < ID2D1Device > pdevice;

      pdevicecontext->GetDevice(&pdevice);

      return pdevice
         && m_pd2d1deviceMemoryGraphicsPool
         && pdevice.m_p == m_pd2d1deviceMemoryGraphicsPool.m_p;

   }


   void graphics::on_acquire_memory_graphics(
      ::image::image * pimage,
      const ::i32_size & size,
      ::acme::user::interaction * pacmeuserinteractionAffinity)
   {

      if (m_bBeginDraw)
      {

         throw ::exception(
            error_wrong_state,
            "pooled Direct2D graphics was acquired with an active drawing scope");

      }

      if (pimage)
      {

         if (!pimage->m_pdraw2dbitmap)
         {

            pimage->update_bitmap_as_render_target(pacmeuserinteractionAffinity, this);

         }

         ::cast < ::draw2d_direct2d::bitmap > pdraw2dbitmap = pimage->m_pdraw2dbitmap;

         if (!pdraw2dbitmap
            || !pdraw2dbitmap->m_pd2d1bitmap
            || !pdraw2dbitmap->m_pd2d1devicecontext)
         {

            throw ::exception(
               error_wrong_state,
               "Direct2D image has no usable bitmap render target");

         }

         if (m_pd2d1devicecontext
            && m_pd2d1devicecontext.m_p != pdraw2dbitmap->m_pd2d1devicecontext.m_p)
         {

            throw ::exception(
               error_wrong_state,
               "Direct2D graphics retained a different image render target");

         }

         if (!m_pd2d1devicecontext && m_pd2d1devicecontext)
         {

            throw ::exception(
               error_wrong_state,
               "Direct2D graphics retained an incomplete render target");

         }

         //m_pd2d1bitmaprendertargetCompatibleMemoryGraphics.release();
         m_pd2d1dcrendertarget.release();
         m_pd2d1devicecontext = pdraw2dbitmap->m_pd2d1devicecontext;
         //m_osdata[0] = m_pd2d1devicecontext.m_p;
         m_pd2d1devicecontext.as(m_pd2d1devicecontext);
         m_pd2d1devicecontext.as(m_pd2d1devicecontext1);
         m_pd2d1devicecontext->SetTarget(pdraw2dbitmap->m_pd2d1bitmap);

      }
      else
      {

         if (m_pd2d1devicecontext || m_pd2d1devicecontext)
         {

            throw ::exception(
               error_wrong_state,
               "pooled Direct2D graphics retained a previous render target");

         }

         _create_memory_graphics(size, pacmeuserinteractionAffinity);

      }

      if (!m_pd2d1devicecontext || !m_pd2d1devicecontext)
      {

         throw ::exception(
            error_wrong_state,
            "Direct2D memory graphics has no render target");

      }

      m_pd2d1devicecontext->GetDevice(&m_pd2d1deviceMemoryGraphicsPool);
      //m_osdata[data_device_context] = m_pd2d1devicecontext;
      //m_osdata[data_render_target] = m_pd2d1devicecontext;
      m_iLayerCount = 0;
      m_iaPushLayer.erase_all();
      m_iaPushLayerCount.erase_all();
      m_statea.erase_all();
      m_state.m_iLayerIndex = 0;
      m_matrix = {};
      m_sizeScaling = { 1.0, 1.0 };
      m_pointTranslateOutput = {};
      m_sizeScaleOutput = { 1.0, 1.0 };
      m_iYFlipHeight = 0;
      m_dSizeScaler = 1.0;

      m_pd2d1devicecontext->BeginDraw();
      m_bBeginDraw = true;

      try
      {

         ::draw2d::graphics::on_acquire_memory_graphics(
            pimage,
            size,
            pacmeuserinteractionAffinity);

         defer_on_target_rectangle_update();
         m_pd2d1devicecontext->SetPrimitiveBlend(
            D2D1_PRIMITIVE_BLEND_SOURCE_OVER);
         m_pd2d1devicecontext->SetAntialiasMode(
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
         m_pd2d1devicecontext->SetTextAntialiasMode(
            D2D1_TEXT_ANTIALIAS_MODE_DEFAULT);
         m_ealphamodeDevice = ::draw2d::e_alpha_mode_blend;
         m_etextrenderinghintDevice =
            ::write_text::e_rendering_undefined;

      }
      catch (...)
      {

         _pop_all_layers();
         m_pd2d1devicecontext->EndDraw();
         m_bBeginDraw = false;
         m_pd2d1devicecontext1.release();
         m_pd2d1devicecontext.release();
         m_pd2d1devicecontext.release();
         //clear_os_data();
         throw;

      }

   }


   void graphics::on_release_memory_graphics()
   {


      auto pimageBeforeRelease = m_pimage;
      auto pdevicecontextBeforeRelease = m_pd2d1devicecontext;

      //if (m_egraphics == e_graphics_draw && (!m_bBeginDraw || !pdevicecontextBeforeRelease))
      //{

      //   m_bBeginDraw = false;
      //   m_pd2d1devicecontext1.release();
      //   m_pd2d1devicecontext.release();
      //   m_pd2d1devicecontext.release();
      //   clear_os_data();

      //   throw ::exception(
      //      error_wrong_state,
      //      "Direct2D memory graphics was released outside its drawing scope");

      //}

      try
      {

         ::draw2d::graphics::on_release_memory_graphics();
         _pop_all_layers();

      }
      catch (...)
      {

         //pdevicecontextBeforeRelease->EndDraw();
         m_bBeginDraw = false;
         m_pd2d1devicecontext1.release();
         m_pd2d1devicecontext.release();
         m_pd2d1devicecontext.release();
         //clear_os_data();
         throw;

      }

      HRESULT hrEndDraw = S_OK;

      if (m_bBeginDraw)
      {

         hrEndDraw = pdevicecontextBeforeRelease->EndDraw();

         m_bBeginDraw = false;

      }
      m_iLayerCount = 0;
      m_iaPushLayer.erase_all();
      m_iaPushLayerCount.erase_all();
      m_statea.erase_all();
      m_state.m_iLayerIndex = 0;
      m_pd2d1layer.release();
      //m_ppathgeometryClip.release();
      //m_pd2d1devicecontext1.release();
      //m_pd2d1devicecontext.release();
      //m_pd2d1devicecontext.release();
      //m_pd2d1dcrendertarget.release();
      //m_pd2d1bitmaprendertargetCompatibleMemoryGraphics.release();
      //clear_os_data();

      //static ::std::atomic<unsigned int> s_uEndDrawDiagnosticCount{ 0 };
      //auto uEndDrawDiagnosticCount = s_uEndDrawDiagnosticCount.fetch_add(1, ::std::memory_order_relaxed);

      //if (uEndDrawDiagnosticCount < 32 || FAILED(hrEndDraw))
      //{

      //   informationf(
      //      "Direct2DEndDrawDiagnostic result=0x%08lx graphics=%p image=%p target=%p",
      //      (unsigned long)hrEndDraw,
      //      this,
      //      pimageBeforeRelease,
      //      (ID2D1DeviceContext *)pdevicecontextBeforeRelease);

      //}

//      if (FAILED(hrEndDraw))
      //{

      //   throw hresult_exception(
      //      hrEndDraw,
      //      "Direct2D memory graphics EndDraw failed");

      //}

   }



   //void graphics::text_out(::f64 x, ::f64 y, const block & block)
   //{

   //   if (block.is_empty())
   //   {

   //      return false;

   //   }

   //   return ::draw2d::graphics::text_out((::f64)x, (::f64)y, string(lpszString, nCount));

   //}


   void graphics::text_out(::f64 x, ::f64 y, const ::scoped_string & scopedstr)
   {

      if (scopedstr.is_empty())
      {

         return;
         ////return false;

         //throw ::exception(error_invalid_empty_argument);

      }

      if (m_pwritetextfont.is_null())
      {

         //return false;

         throw ::exception(error_wrong_state);

      }

      if (m_bTargetRectangleModified)
      {

         defer_on_target_rectangle_update();

      }

      m_pwritetextfont->update(this);

      ::cast < ::draw2d_direct2d::font > pdraw2ddirect2dfont = m_pwritetextfont;

      auto pdwritetextformat = pdraw2ddirect2dfont->m_pdwritetextformat;

      if (pdwritetextformat == nullptr)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      if (m_pdraw2dbrush.is_null())
      {

         throw ::exception(error_null_pointer);

      }

      m_pdraw2dbrush->defer_update(this);

      ::cast < ::draw2d_direct2d::brush>pdraw2ddirect2dbrush = m_pdraw2dbrush;

      auto pd2d1brush = pdraw2ddirect2dbrush->m_pd2d1brush;

      if (::is_null(pd2d1brush))
      {

         throw ::exception(error_null_pointer);

      }

      synchronous_lock synchronouslock(this->synchronization());

      D2D1::Matrix3x2F m;

      D2D1::Matrix3x2F mOriginal;

      synchronous_lock synchronouslockFontTextMap(::write_text::font::s_pmutexFontTextMap);

      auto & text = m_pwritetextfont->m_mapFontText[scopedstr];

      auto ptextitem = text.get_item(::write_text::font::text::e_size_backend_draw_text);

      ::f64_size sizeText;

      if (ptextitem->has_size())
      {

         sizeText = ptextitem->get_size();

      }
      else
      {

         sizeText = get_text_extent(scopedstr);

      }

      D2D1_RECT_F rectf = D2D1::RectF((FLOAT)0, (FLOAT)0, (FLOAT)(0 + sizeText.cx * 2), (FLOAT)(0 + sizeText.cy * 2));

      HRESULT  hr = pdwritetextformat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

      if (FAILED(hr))
      {

         warning() << "text_out, SetTextAlignment" << hresult_text(hr);

      }

      hr = pdwritetextformat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

      if (FAILED(hr))
      {

         warning() << "text_out, SetTextAlignment" << hresult_text(hr);

      }

      DWRITE_TRIMMING trim;

      zero(trim);

      trim.granularity = DWRITE_TRIMMING_GRANULARITY_NONE;

      hr = pdwritetextformat->SetTrimming(&trim, nullptr);

      if (FAILED(hr))
      {

         warning() << "text_out, SetTextAlignment" << hresult_text(hr);

      }

      hr = pdwritetextformat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

      if (m_pwritetextfont->m_dFontWidth != 1.0)
      {

         m_pd2d1devicecontext->GetTransform(&mOriginal);

         m = mOriginal;

         m._11 *= (FLOAT)m_pwritetextfont->m_dFontWidth;
         m._31 += (FLOAT)x;
         m._32 += (FLOAT)y;

      }
      else
      {

         rectf.left += (FLOAT)x;
         rectf.top += (FLOAT)y;
         rectf.right += (FLOAT)x;
         rectf.bottom += (FLOAT)y;

      }

      if (m_pwritetextfont->m_dFontWidth != 1.0)
      {

         m_pd2d1devicecontext->SetTransform(&m);

      }

      auto lpcwsz = ptextitem->get_text().c_str();

      character_count uiLen = ptextitem->get_text().length();

      defer_text_primitive_blend();

      defer_text_rendering_hint();

      if (lpcwsz != nullptr && uiLen > 0)
      {

         m_pd2d1devicecontext->DrawText(lpcwsz, (::i32)uiLen, pdwritetextformat, &rectf, pd2d1brush);

      }

      if (m_pwritetextfont->m_dFontWidth != 1.0)
      {

         m_pd2d1devicecontext->SetTransform(mOriginal);

      }

   }


   void graphics::line(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2, ::draw2d::pen * pdraw2dpen)
   {

      if (m_bTargetRectangleModified)
      {

         defer_on_target_rectangle_update();

      }

      D2D1_POINT_2F p1;

      p1.x = (FLOAT)x1;

      p1.y = (FLOAT)y1;

      D2D1_POINT_2F p2;

      p2.x = (FLOAT)x2;

      p2.y = (FLOAT)y2;

      pdraw2dpen->defer_update(this);

      auto pdraw2dbrush = pdraw2dpen->m_pdraw2dbrush;

      pdraw2dbrush->defer_update(this);

      ::cast < ::draw2d_direct2d::brush>pdraw2ddirect2dbrush = pdraw2dbrush;

      auto pd2d1brush = pdraw2ddirect2dbrush->m_pd2d1brush;

      if (::is_null(pd2d1brush))
      {

         throw ::exception(error_null_pointer);

      }

      defer_primitive_blend();

      m_pd2d1devicecontext->DrawLine(p1, p2, pd2d1brush, (FLOAT)m_pdraw2dpen->m_dWidth);

      m_pointCurrent.x = x2;

      m_pointCurrent.y = y2;

      //return true;

   }


   void graphics::line(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2)
   {

      if (m_bTargetRectangleModified)
      {

         defer_on_target_rectangle_update();

      }

      D2D1_POINT_2F p1;

      p1.x = (FLOAT) x1;

      p1.y = (FLOAT) y1;

      D2D1_POINT_2F p2;

      p2.x = (FLOAT)x2;

      p2.y = (FLOAT)y2;

      m_pdraw2dpen->defer_update(this);

      auto pdraw2dbrush = m_pdraw2dpen->m_pdraw2dbrush;

      pdraw2dbrush->defer_update(this);

      ::cast < ::draw2d_direct2d::brush>pdraw2ddirect2dbrush = pdraw2dbrush;

      auto pd2d1brush = pdraw2ddirect2dbrush->m_pd2d1brush;

      if (::is_null(pd2d1brush))
      {

         throw ::exception(error_null_pointer);

      }

      defer_primitive_blend();

      FLOAT fWidth = (FLOAT)m_pdraw2dpen->m_dWidth;

      m_pd2d1devicecontext->DrawLine(p1, p2, pd2d1brush, fWidth);

      m_pointCurrent.x = x2;

      m_pointCurrent.y = y2;

      //return true;

   }


   void graphics::begin_draw()
   {

      ::gpu::graphics::begin_draw();

      //auto pgputexturesiteTarget = current_target_texture(::gpu::current_layer());

      //prepare_nanovg_render_target(pgputexturesiteTarget->gpu_texture());

      //auto size = m_size;

      //nvgBeginFrame(m_pdc, (float)size.width(), (float)size.height(), 1.0f);

      if (!m_bBeginDraw)
      {

         if (::gpu::current_layer())
         {

            m_bBeginDraw = true;

            auto pgputexturesiteTarget = ::gpu::current_layer()->texture(true);

            //prepare_gpu_draw2d_graphics_render_target(pgputexturesiteTarget->gpu_texture());

            //::cast < ::draw2d_direct2d_for_directx11::image > pimage = m_pimage;

            //if (::is_set(pimage))
            //{

            //   if (!pimage->m_pdraw2dbitmap)
            //   {

            //      pimage->create_bitmap(m_pacmeuserinteractionAffinity, this);

            //   }

            //   ::cast <::draw2d_direct2d::bitmap> pdraw2dbitmap = pimage->m_pdraw2dbitmap;

            //   auto & pd2d1bitmap = pdraw2dbitmap->m_pdraw2dbitmap;

            //   m_pdevicecontext->SetTarget(pd2d1bitmap);

            //}

            auto ptexture = pgputexturesiteTarget->gpu_texture();

            defer_constructø(ptexture->m_pimageGpuTexture);

            ptexture->m_pimageGpuTexture->update_as_backed_by_gpu_texture(ptexture->m_textureattributes.m_sizeRaw, ptexture, this);

            ::cast <::draw2d_direct2d_for_directx11::bitmap> pdraw2dbitmap = ptexture->m_pimageGpuTexture->m_pdraw2dbitmap;

            m_pd2d1devicecontext->SetTarget(pdraw2dbitmap->m_pd2d1bitmap);

            m_pd2d1devicecontext->BeginDraw();

            m_pd2d1devicecontext->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

            //set_alpha_mode(::draw2d::e_alpha_mode_set);

            //fill_solid_rectangle({ ::i32_point(), m_sizeTarget }, ::color::transparent);

            //auto pgputexturesiteTarget = current_target_texture(::gpu::current_layer());

            //prepare_gpu_draw2d_graphics_render_target(pgputexturesiteTarget->gpu_texture());

            //::cast <::draw2d_direct2d_for_directx11::bitmap> pdraw2dbitmap = m_pimage->m_pdraw2dbitmap;

            //m_pd2d1devicecontext->SetTarget(pdraw2dbitmap->m_pd2d1bitmap);

            //m_pd2d1devicecontext->BeginDraw();

            //set_alpha_mode(::draw2d::e_alpha_mode_set);

            //fill_solid_rectangle({ ::i32_point(), m_sizeTarget }, ::color::transparent);

         }

      }

   }


   void graphics::end_draw()
   {

      if (m_bBeginDraw)
      {

         m_bBeginDraw = false;


         //int iLayerIndex = ::gpu::current_layer()->m_iLayerIndex;

//if (iLayerIndex == 0)
//{

         //m_bBeginDraw = false;

         if (0)
         {
            auto pgpucontext = gpu_context();

            ::gpu::context_lock context_lock(pgpucontext);


            auto ptexturesite = pgpucontext->m_pgpurenderer->m_pgpurendertarget2->current_texture(::gpu::current_layer(), false);

            ::cast < ::gpu_directx11::texture > ptexture = ptexturesite->gpu_texture();

            auto pd3d11texture = ptexture->m_ptextureOffscreen;


            comptr<ID2D1Image > pd2d1image;

            m_pd2d1devicecontext->GetTarget(&pd2d1image);

            comptr<ID2D1Bitmap1>pd2d1bitmap;

            pd2d1image.as(pd2d1bitmap);

            comptr<IDXGISurface> psurfaceFromBitmap;

            HRESULT hr =
               pd2d1bitmap->GetSurface(
                  &psurfaceFromBitmap);

            ::defer_throw_hresult(hr);

            comptr<ID3D11Texture2D> ptextureFromBitmap;

            hr =
               psurfaceFromBitmap->QueryInterface(
                  IID_PPV_ARGS(&ptextureFromBitmap));

            ::defer_throw_hresult(hr);


            comptr<IUnknown> punknownOriginal;
            comptr<IUnknown> punknownFromD2D;

            pd3d11texture.as(punknownOriginal);

            ptextureFromBitmap.as(punknownFromD2D);

            informationf(
               "original=%p fromD2D=%p same=%d",
               punknownOriginal.m_p,
               punknownFromD2D.m_p,
               punknownOriginal.m_p == punknownFromD2D.m_p);

            //   D2D1_COLOR_F color;
            //   color.r = 0.7f;
            //   color.g = 0.7f;
            //   color.b = 0.2f;
            //   color.a = 0.5f;

            //   m_pd2d1devicecontext->Clear(color);

            //}

   //           m_pd2d1devicecontext->Clear(
   //D2D1::ColorF(
   //   D2D1::ColorF::Green,
   //   1.0f));

         }
         D2D1_TAG tag1 = 0;
         D2D1_TAG tag2 = 0;

         HRESULT hrEndDraw = m_pd2d1devicecontext->EndDraw(&tag1, &tag2);

         //informationf("hrEndDraw %d", hrEndDraw);
         informationf(
"D2D EndDraw hr=0x%08X tag1=%llu tag2=%llu",
(unsigned int)hrEndDraw,
(unsigned long long) tag1,
(unsigned long long) tag2);




         //if (1)
         //{

         //   pgpucontext->m_pd3d11devicecontext->Flush();

         //}

         //m_pdevicecontext->Flush();


         //m_pd2d1devicecontext->EndDraw();

         m_pd2d1devicecontext->SetTarget(nullptr);



         //if (1)
         //{

         //   ::cast < ::gpu_directx11::context > pgpucontext = gpu_context();

         //   ::gpu::context_lock context_lock(pgpucontext);

         //   pgpucontext->m_pd3d11devicecontext->Flush();

         //}


      }

      ::gpu::graphics::end_draw();

   }



   void graphics::start_layer(bool bFirstLayer, ::user::interaction * puserinteraction)
   {

      m_pacmeuserinteractionAffinity = puserinteraction;

      reset_clip();

      m_ealphamodeDevice = ::draw2d::e_alpha_mode_none;

      set_alpha_mode(::draw2d::e_alpha_mode_blend);

      set_smooth_mode(::draw2d::e_smooth_mode_high);

      //::gpu::graphics::start_layer(bFirstLayer);
      m_pointTarget = m_pacmeuserinteractionAffinity->m_pacmewindowingwindow->m_pointWindow;
      m_sizeTarget = m_pacmeuserinteractionAffinity->m_pacmewindowingwindow->m_sizeWindow;
      m_bTargetRectangleModified = false;
      update_matrix();



      //if (m_egraphics == ::e_graphics_draw)
      {

      //   //auto rectangleHost = m_puserinteractionDraw2dGraphics->raw_rectangle();

      //   //m_pgpucontextDraw2d->m_pgpudevice->start_stacking_layers();

      //   //m_pgpucontextDraw2d->m_pgpurenderer->start_layer(rectangleHost);



         if (m_pgraphicsbufferitem)
         {

            m_pointTarget = m_pgraphicsbufferitem->m_pointBufferItem;
            m_sizeTarget = m_pgraphicsbufferitem->m_sizeBufferItem;
            m_bTargetRectangleModified = false;
            update_matrix();


            //m_pgraphicsbufferitem->m_pimageBufferItem = m_pimage;

            auto pimage = m_pgraphicsbufferitem->m_pimageBufferItem;

            if (pimage)
            {

               //auto point = m_pacmeuserinteractionAffinity->m_pacmewindowingwindow->m_pointWindow;
               //auto size = m_pacmeuserinteractionAffinity->m_pacmewindowingwindow->m_sizeWindow;
               auto point = m_pgraphicsbufferitem->m_pointBufferItem;
               auto size = m_pgraphicsbufferitem->m_sizeBufferItem;

            //   ::f64_point pointf64Image = pimage->m_point;
            //   ::f64_size sizef64Image = pimage->m_size;
            //   ::f64_size sizef64ImageRaw = pimage->m_sizeRaw;

               if (pimage->m_point != point || pimage->m_size != size)
               {

                  pimage->m_point = point;

                  pimage->m_size = size;

                  pimage->set_ok_flag();

                  pimage->m_estatus = success;

               }

            }

            set_target_image(m_pgraphicsbufferitem->m_pimageBufferItem);

            update_matrix();

         }


         if (!m_bBeginDraw && !bFirstLayer)
         {

            //m_bBeginDraw = true;

            //m_pd2d1devicecontext->BeginDraw();

            begin_draw();

         }

      }

   }


   void graphics::set_target_image(::image::image * pimage)
   {

      ::cast < ::draw2d_direct2d::bitmap > pdraw2dbitmap = pimage->m_pdraw2dbitmap;

      pdraw2dbitmap->m_pd2d1devicecontext.as(m_pd2d1devicecontext);
      
      //pdraw2dbitmap->m_pd2d1bitmaprendertarget.as(m_pd2d1bitmaprendertarget);

      m_pd2d1devicecontext.as(m_pd2d1devicecontext1);

      //m_pd2d1bitmaprendertarget.as(m_pd2d1devicecontext);

      m_pd2d1devicecontext.as(m_pd2d1dcrendertarget);

      //m_osdata[data_device_context] = m_pd2d1devicecontext;

      //m_osdata[data_render_target] = m_pd2d1devicecontext;

   }


   //void graphics::create_end_draw()
   //{

   //   if (m_penddraw)
   //   {

   //      return;

   //   }

   //   m_penddraw = allocateø swap_chain_end_draw();

   //   m_penddraw->initialize(this);

   //   m_penddraw->initialize_end_draw(this);

   //}


   void graphics::end_layer(bool bClosingLayer)
   {

      if (m_iLayerCount > 0)
      {

         warning() << "Layers left to pop on end draw!";

         _pop_all_layers();

      }

      //if (m_egraphics == ::e_graphics_draw)
      //{

      //   if (m_bBeginDraw)
      //   {

      //      m_bBeginDraw = false;

      //      m_pd2d1devicecontext->EndDraw();

      //   }

      //}

      //::gpu::graphics::end_layer(bClosingLayer);

   }


   //void graphics::just_after_new_frame()
   //{

   //   ::gpu::graphics::just_after_new_frame();

   //}


   //void graphics::start_gpu_layer(::gpu::layer * pgpulayer)
   //{

   //   ::gpu::graphics::start_gpu_layer(pgpulayer);
   //   //m_pgpucontextDraw2d->m_pgpudevice->start_stacking_layers();
   //   //m_pgpucontextDraw2d->m_pgpurenderer->start_layer(m_puserinteractionDraw2dGraphics->raw_rectangle());

   //   

   //}


   //::gpu::frame * graphics::end_gpu_layer(::gpu::layer * pgpulayer)
   //{

   //   return ::gpu::graphics::end_gpu_layer(pgpulayer);

   //}


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


   //void graphics::gpu_layer_on_after_begin_render()
   //{



   //}

   //void graphics::gpu_layer_on_before_end_render()
   //{



   //}
   

//   //void graphics::__attach(ID2D1DeviceContext* pdevicecontext)
//   void graphics::_bind(::i32 iIndex, ::i32 iLayerIndex, IDXGISurface * pdxgisurface)
//   {
//
//      if (::is_null(pdxgisurface))
//      {
//
//         throw ::exception(error_null_pointer);
//
//      }
//      if (iIndex < 0 || iIndex >= 16)
//      {
//
//         throw ::exception(error_bad_argument, "what?!");
//
//      }
//      if (iLayerIndex < 0 || iLayerIndex >= 8)
//      {
//
//         throw ::exception(error_bad_argument, "what?!");
//
//      }
//
//      auto& pdxgisurfaceBound = m_dxgisurfaceaBound.element_at_grow(iIndex).element_at_grow(iLayerIndex);
//
//      auto& pd2d1bitmap = m_d2d1bitmapa.element_at_grow(iIndex).element_at_grow(iLayerIndex);
//
//      auto & pd2d1rendertarget = m_d2d1rendertargeta.element_at_grow(iIndex).element_at_grow(iLayerIndex);
//
//      if (pdxgisurfaceBound != pdxgisurface)
//      {
//
//         pd2d1rendertarget.release();
//
//         pd2d1bitmap.release();
//
//      }
//
//      if (pd2d1bitmap)
//      {
//
//         auto size = pd2d1bitmap->GetSize();
//
//         DXGI_SURFACE_DESC desc{};
//
//         HRESULT hrDxgiSurfaceGetDesc = pdxgisurface->GetDesc(&desc);
//
//         ::defer_throw_hresult(hrDxgiSurfaceGetDesc);
//
//         if (size.width == desc.Width
//            && size.height == desc.Height
//            && pd2d1rendertarget)
//         {
//
//            pdxgisurfaceBound = pdxgisurface;
//
//            __attach(iIndex, iLayerIndex, pd2d1bitmap);
//
//            return;
//
//         }
//
//      }
//
//      //IDXGISurface* dxgiSurface = nullptr;
//      //auto hr = texture->QueryInterface(__uuidof(IDXGISurface), (void**)&dxgiSurface);
//
//      //ID2D1Factory1* d2dFactory = nullptr;
//      //D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);
//
//      //IDXGIDevice* dxgiDevice = nullptr;
////      d3d11Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
//
//      auto pcontext = gpu_context();
//
//      ::cast < ::dxgi_device_source > pdxgidevicesource = pcontext;
//
//      auto pd2d1device = direct2d()->d2d1_device(pdxgidevicesource);
//
//      /*ID2D1Device* d2dDevice = nullptr;
//      direct2d()->d2d1_factory1()->CreateDevice(pdxgidevice, &d2dDevice);*/
//
//      //ID2D1DeviceContext* d2dContext = nullptr;
//
//      //pd2d1device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, 
//        // &m_pdevicecontext);
//
//      //m_pdevicecontext = pdevicecontext;
//
//      D2D1_BITMAP_PROPERTIES1 bitmapProps = {
//    { DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED },
//    96.0f, 96.0f,
//    D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
//    nullptr
//      };
//      
//      _create_from_dxgi_surface(iIndex, iLayerIndex, pdxgisurface);
//      
//      HRESULT hrCreateBitmapFromDxgiSurface = 
//         m_pdevicecontext1->CreateBitmapFromDxgiSurface(
//         pdxgisurface, 
//         &bitmapProps, 
//         &pd2d1bitmap);
//
//      ::defer_throw_hresult(hrCreateBitmapFromDxgiSurface);
//
//      pdxgisurfaceBound = pdxgisurface;
//
//      __attach(iIndex, iLayerIndex, pd2d1bitmap);
//
//   }
//

   //void graphics::__attach(::i32 iIndex, ::i32 iLayerIndex, ID2D1Bitmap1* pd2d1bitmap)
   //{

   //   auto& pd2d1rendertarget = m_d2d1rendertargeta[iIndex][iLayerIndex];

   //   m_pd2d1devicecontext = pd2d1rendertarget;

   //   m_pd2d1devicecontext.as(m_pd2d1devicecontext);

   //   m_pd2d1devicecontext.as(m_pd2d1devicecontext1);

   //   m_pd2d1devicecontext->SetTarget(pd2d1bitmap);

   //}


   void graphics::attach(void * pdata)
   {

      if (m_pd2d1devicecontext != nullptr)
      {

         m_pd2d1devicecontext = nullptr;

      }

      //if (m_pd2d1devicecontext != nullptr)
      //{

      //   m_pd2d1devicecontext = nullptr;

      //}

      //if (m_pd2d1bitmaprendertarget != nullptr)
      //{

      //   m_pd2d1bitmaprendertarget = nullptr;

      //}

      m_pd2d1devicecontext = (ID2D1DeviceContext *)pdata;

      HRESULT hr = m_pd2d1devicecontext.as(m_pd2d1devicecontext1);

      if (FAILED(hr))
      {

         m_pd2d1devicecontext = nullptr;

         m_pd2d1devicecontext1 = nullptr;

         throw ::exception(error_null_pointer);

      }

      hr = m_pd2d1devicecontext.as(m_pd2d1devicecontext);

      if (FAILED(hr))
      {

         m_pd2d1devicecontext = nullptr;

         m_pd2d1devicecontext1 = nullptr;

         //m_pd2d1devicecontext = nullptr;

         throw ::exception(error_null_pointer);

      }

      //hr = m_pd2d1devicecontext.as(m_pd2d1bitmaprendertarget);

      //if (FAILED(hr))
      //{
      //   m_pd2d1bitmaprendertarget = nullptr;
      //}

      //m_osdata[data_device_context] = m_pd2d1devicecontext;

      //m_osdata[data_render_target] = m_pd2d1devicecontext;

      //return true;

   }


   void * graphics::detach()
   {

      m_pd2d1devicecontext = nullptr;

      //m_pd2d1bitmaprendertarget = nullptr;

      //m_osdata[data_device_context] = nullptr;

      //m_osdata[data_render_target] = nullptr;

      return m_pd2d1devicecontext.detach();

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
   //      ::i32_rectangle rectangleX;
   //
   //      ::GetClientRect(hwnd, rectangleX);
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
   //      HRESULT hr = direct2d()->d2d1_factory1()->CreateDCRenderTarget(&props,&m_pdcrendertarget);
   //
   //      if (FAILED(hr))
   //      {
   //
   //         return false;
   //
   //      }
   //
   //      hr = m_pdcrendertarget->BindDC(hdc, rectangleX);
   //
   //      m_pdcrendertarget.As(&m_pd2d1devicecontext);
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


   void graphics::blur(bool bExpand, ::f64 dRadius, const ::f64_rectangle & rectangle)
   {

      throw ::exception(todo);

      //return false;

   }


   ::f64 graphics::get_dpix()
   {

      return 72.0;

   }


   //void graphics::destroy()
   //{


   //   destroy_os_data();

   //   ::draw2d::graphics::destroy();

   //}


   void graphics::destroy()
   {


      // ::draw2d::device_lock devicelock(this);

      _pop_all_layers();

      m_pd2d1pathgeometryClip = nullptr;

      m_pd2d1devicecontext = nullptr;

      m_pd2d1devicecontext = nullptr;

      //m_pd2d1bitmaprendertarget = nullptr;

      //::draw2d::graphics::destroy_os_data();

   }


   void graphics::clear_current_point()
   {

      //return ::success;

   }


   void graphics::draw(::draw2d::path * pdraw2dpath)
   {

      return draw(pdraw2dpath, m_pdraw2dpen);

   }


   bool graphics::draw(ID2D1PathGeometry * pgeometry, ::draw2d::pen * pdraw2dpen)
   {

      pdraw2dpen->defer_update(this);

      auto pdraw2dbrush = pdraw2dpen->m_pdraw2dbrush;

      pdraw2dbrush->defer_update(this);

      ::cast < ::draw2d_direct2d::brush>pdraw2ddirect2dbrush = pdraw2dbrush;

      auto pd2d1brush = pdraw2ddirect2dbrush->m_pd2d1brush;

      if (::is_null(pd2d1brush))
      {

         throw ::exception(error_null_pointer);

      }

      ::cast < ::draw2d_direct2d::pen > pdraw2ddirect2dpen = pdraw2dpen;

      ::ID2D1StrokeStyle1 * pstrokestyle = pdraw2ddirect2dpen->m_pd2d1strokestyle1;

      m_pd2d1devicecontext->DrawGeometry(pgeometry, pd2d1brush, (FLOAT)pdraw2dpen->m_dWidth, pstrokestyle);

      return true;

   }


   bool graphics::fill(ID2D1PathGeometry * pgeometry, ::draw2d::brush * pdraw2dbrush)
   {

      if (pdraw2dbrush->m_ebrush == ::draw2d::e_brush_box_gradient)
      {

         layer layerShape(m_pd2d1devicecontext, pgeometry);

         ::f64 radius = pdraw2dbrush->m_dRadius;

         ::f64 radius2 = radius * 2.0;

         auto w = pdraw2dbrush->m_size.cx;

         auto h = pdraw2dbrush->m_size.cy;

         if (radius2 > w || radius2 > h)
         {

            return false;

         }

         ::f64_rectangle outer(pdraw2dbrush->m_point, pdraw2dbrush->m_size);

         ::f64_rectangle inner(outer);

         inner.deflate(radius);

         ::f64_rectangle cornerClip(pdraw2dbrush->m_point, ::f64_size(radius, radius));

         ::f64_rectangle cornerBrush(pdraw2dbrush->m_point, ::f64_size(radius * 2.0, radius * 2.0));

         auto pstopcollection = _create_simple_full_range_flat_gradient_stop_collection(pdraw2dbrush->m_color1, pdraw2dbrush->m_color2);

         {

            //layer layerShape(m_pd2d1devicecontext, pgeometry);

            auto pdraw2dbrush = _create_simple_radial_gradient(cornerBrush, pstopcollection);

            m_pd2d1devicecontext->FillRectangle(
               { (FLOAT)cornerClip.left, (FLOAT)cornerClip.top,
               (FLOAT)cornerClip.right, (FLOAT)cornerClip.bottom },
               pdraw2dbrush);

         }

         cornerClip.Align(e_align_top_right, outer);

         cornerBrush.Align(e_align_top_right, outer);

         {

            //layer layerShape(m_pd2d1devicecontext, pgeometry);

            auto pdraw2dbrush = _create_simple_radial_gradient(cornerBrush, pstopcollection);

            m_pd2d1devicecontext->FillRectangle(
               { (FLOAT)cornerClip.left, (FLOAT)cornerClip.top,
               (FLOAT)cornerClip.right, (FLOAT)cornerClip.bottom },
               pdraw2dbrush);

         }

         cornerClip.Align(e_align_bottom_right, outer);

         cornerBrush.Align(e_align_bottom_right, outer);

         {

            //layer layerShape(m_pd2d1devicecontext, pgeometry);

            auto pdraw2dbrush = _create_simple_radial_gradient(cornerBrush, pstopcollection);

            m_pd2d1devicecontext->FillRectangle(
               { (FLOAT)cornerClip.left, (FLOAT)cornerClip.top,
               (FLOAT)cornerClip.right, (FLOAT)cornerClip.bottom },
               pdraw2dbrush);

         }

         cornerClip.Align(e_align_bottom_left, outer);

         cornerBrush.Align(e_align_bottom_left, outer);

         {

            //layer layerShape(m_pd2d1devicecontext, pgeometry);

            auto pdraw2dbrush = _create_simple_radial_gradient(cornerBrush, pstopcollection);

            m_pd2d1devicecontext->FillRectangle(
               { (FLOAT)cornerClip.left, (FLOAT)cornerClip.top,
               (FLOAT)cornerClip.right, (FLOAT)cornerClip.bottom },
               pdraw2dbrush);

         }


         cornerClip.Align(e_align_top_right, outer);

         cornerBrush.Align(e_align_top_right, outer);

         {

            //layer layerShape(m_pd2d1devicecontext, pgeometry);

            auto pd2d1brush = _create_solid_brush(pdraw2dbrush->m_color1);

            m_pd2d1devicecontext->FillRectangle(
               { (FLOAT)inner.left, (FLOAT)inner.top,
               (FLOAT)inner.right, (FLOAT)inner.bottom },
               pd2d1brush);

         }

         {

            ::f64_rectangle side(0, 0, inner.width(), radius);

            side.Align(e_align_top_center, outer);

            {

               auto pdraw2dbrush = _create_simple_linear_gradient(side.bottom_left(), side.top_left(), pstopcollection);

               m_pd2d1devicecontext->FillRectangle(
                  { (FLOAT)side.left, (FLOAT)side.top,
                  (FLOAT)side.right, (FLOAT)side.bottom },
                  pdraw2dbrush);

            }

            side.Align(e_align_bottom_center, outer);

            {

               auto pdraw2dbrush = _create_simple_linear_gradient(side.top_left(), side.bottom_left(), pstopcollection);

               m_pd2d1devicecontext->FillRectangle(
                  { (FLOAT)side.left, (FLOAT)side.top,
                  (FLOAT)side.right, (FLOAT)side.bottom },
                  pdraw2dbrush);

            }

         }


         {

            ::f64_rectangle side(0, 0, radius, inner.height());

            side.Align(e_align_left_center, outer);

            {

               auto pdraw2dbrush = _create_simple_linear_gradient(side.top_right(), side.top_left(), pstopcollection);

               m_pd2d1devicecontext->FillRectangle(
                  { (FLOAT)side.left, (FLOAT)side.top,
                  (FLOAT)side.right, (FLOAT)side.bottom },
                  pdraw2dbrush);

            }

            side.Align(e_align_right_center, outer);

            {

               auto pdraw2dbrush = _create_simple_linear_gradient(side.top_left(), side.top_right(), pstopcollection);

               m_pd2d1devicecontext->FillRectangle(
                  { (FLOAT)side.left, (FLOAT)side.top,
                  (FLOAT)side.right, (FLOAT)side.bottom },
                  pdraw2dbrush);

            }

         }
         return true;

      }

      ::cast < ::draw2d_direct2d::brush > pdraw2ddirect2dbrush = pdraw2dbrush;

      ::ID2D1Brush * pd2d1brush = pdraw2ddirect2dbrush->m_pd2d1brush;

      if (pd2d1brush == nullptr)
      {

         return false;

      }

      m_pd2d1devicecontext->FillGeometry(pgeometry, pd2d1brush);

      return true;

   }


   bool graphics::draw(ID2D1GeometryRealization * prealization, ::draw2d::pen * pdraw2dpen)
   {

      pdraw2dpen->defer_update(this);

      auto pdraw2dbrush = pdraw2dpen->m_pdraw2dbrush;

      pdraw2dbrush->defer_update(this);

      ::cast < ::draw2d_direct2d::brush>pdraw2ddirect2dbrush = pdraw2dbrush;

      auto pd2d1brush = pdraw2ddirect2dbrush->m_pd2d1brush;

      if (::is_null(pd2d1brush))
      {

         throw ::exception(error_null_pointer);

      }

      m_pd2d1devicecontext1->DrawGeometryRealization(prealization, pd2d1brush);

      return true;

   }


   bool graphics::fill(ID2D1GeometryRealization * prealization, ::draw2d::brush * pdraw2dbrush)
   {

      ::cast < ::draw2d_direct2d::brush > pdraw2ddirect2dbrush = pdraw2dbrush;

      ::ID2D1Brush * pd2d1brush = pdraw2ddirect2dbrush->m_pd2d1brush;

      if (pd2d1brush == nullptr)
      {

         return false;

      }

      m_pd2d1devicecontext1->DrawGeometryRealization(prealization, pd2d1brush);

      return true;

   }


   void graphics::draw(::draw2d::path * ppathParam, ::draw2d::pen * pdraw2dpen)
   {

      if (m_bTargetRectangleModified)
      {

         defer_on_target_rectangle_update();

      }

      scoped_restore(m_bOutline);

      m_bOutline = true;

      ::pointer<::draw2d_direct2d::path> pdraw2ddirect2dpath = ppathParam;

      ID2D1PathGeometry * pgeometry = pdraw2ddirect2dpath->m_pd2d1pathgeometryHollow1;

      if (pgeometry != nullptr)
      {

         if (pdraw2ddirect2dpath && pdraw2ddirect2dpath->m_bUseGeometryRealization)
         {

            auto prealization = pdraw2ddirect2dpath->_get_stroked_geometry_realization(this, (::i32)pdraw2dpen->m_dWidth);

            draw(prealization, pdraw2dpen);

            return;

         }

         defer_primitive_blend();

         draw(pgeometry, pdraw2dpen);

      }

      //for(::collection::index i = 0; i < pdraw2dpath->m_shapea.get_size(); i++)
      //{

      //   if(pdraw2dpath->m_shapea[i]->eshape() == e_shape_text_out)
      //   {

      //      draw(pdraw2dpath->m_shapea[i]->shape < ::write_text::text_out>(), pdraw2dpen);

      //   }
      //   else if (pdraw2dpath->m_shapea[i]->eshape() == e_shape_draw_text)
      //   {

      //      draw(pdraw2dpath->m_shapea[i]->shape < ::write_text::draw_text>(), pdraw2dpen);

      //   }

      //}

      //return true;

   }


   void graphics::fill(::draw2d::path * pdraw2dpath)
   {

      fill(pdraw2dpath, m_pdraw2dbrush);

      //::pointer<class path> pdraw2dpath = ppathParam;

      //if (!pdraw2dpath)
      //{

      //   throw ::exception(error_null_pointer);

      //}

      //if (pdraw2dpath->is_empty())
      //{

      //   return;

      //}

      //__stack(m_bOutline, false);

      //ID2D1PathGeometry * pgeometry = pdraw2dpath->get_os_data < ID2D1PathGeometry * >(this, path_filled);

      //if (pgeometry != nullptr)
      //{

      //   if (pdraw2dpath && pdraw2dpath->m_bUseGeometryRealization)
      //   {

      //      auto prealization = pdraw2dpath->_get_filled_geometry_realization(this);

      //      fill(prealization, m_pdraw2dbrush);

      //      return;

      //   }

      //   defer_primitive_blend();

      //   fill(pgeometry, m_pdraw2dbrush);

      //}

      ////for (::collection::index i = 0; i < pdraw2dpath->m_shapea.get_size(); i++)
      ////{

      ////   if (pdraw2dpath->m_shapea[i]->eshape() == ::e_shape_text_out)
      ////   {

      ////      fill(pdraw2dpath->m_shapea[i]->shape < ::write_text::text_out >(), m_pdraw2dbrush);

      ////   }
      ////   else if (pdraw2dpath->m_shapea[i]->eshape() == ::e_shape_draw_text)
      ////   {

      ////      fill(pdraw2dpath->m_shapea[i]->shape < ::write_text::draw_text >(), m_pdraw2dbrush);

      ////   }

      ////}

      ////return true;

   }


   void graphics::fill(::draw2d::path * ppathParam, ::draw2d::brush * pdraw2dbrush)
   {

      ::pointer<class path> pdraw2dpath = ppathParam;

      if (!pdraw2dpath)
      {

         throw ::exception(error_null_pointer);

      }

      if (pdraw2dpath->is_empty())
      {

         return;

      }

      if (m_bTargetRectangleModified)
      {

         defer_on_target_rectangle_update();

      }

      //ID2D1Brush * pdraw2dbrush = pbrushParam->get_os_data < ID2D1Brush * >(this);

      //if (pdraw2dbrush == nullptr)
      //{

      //   //return false;

      //   throw ::exception(error_null_pointer);

      //}

      scoped_restore(m_bOutline);

      m_bOutline = false;

      ::cast < ::draw2d_direct2d::path > pdraw2ddirect2dpath = pdraw2dpath;

      ID2D1PathGeometry * pgeometry = pdraw2ddirect2dpath->m_pd2d1pathgeometryFilled1;

      if (pgeometry != nullptr)
      {

         if (pdraw2dpath && pdraw2dpath->m_bUseGeometryRealization && pdraw2dbrush->m_ebrush != ::draw2d::e_brush_box_gradient)
         {

            auto prealization = pdraw2dpath->_get_filled_geometry_realization(this);

            fill(prealization, m_pdraw2dbrush);

            return;

         }



         defer_primitive_blend();

         fill(pgeometry, pdraw2dbrush);

         //m_pd2d1devicecontext->FillGeometry(pgeometry, pdraw2dbrush);

      }

      //for (::collection::index i = 0; i < pdraw2dpath->m_shapea.get_size(); i++)
      //{

      //   if (pdraw2dpath->m_shapea[i]->eshape() == ::e_shape_text_out)
      //   {

      //      fill(pdraw2dpath->m_shapea[i]->shape < ::write_text::text_out >(), pbrushParam);

      //   }
      //   else if (pdraw2dpath->m_shapea[i]->eshape() == ::e_shape_draw_text)
      //   {

      //      fill(pdraw2dpath->m_shapea[i]->shape < ::write_text::draw_text >(), pbrushParam);

      //   }

      //}

      //return true;

   }


   void graphics::path(::draw2d::path * pdraw2dpath)
   {

      if (pdraw2dpath == nullptr)
      {

         //return false;

         throw ::exception(error_null_pointer);

      }

      //bool bOk1 = 

      fill(pdraw2dpath);

      //bool bOk2 = 

      draw(pdraw2dpath);

      //return bOk1 && bOk2;

   }


   bool graphics::draw(const ::write_text::text_out & textout, ::draw2d::pen * pdraw2dpen)
   {

      //wstring szOutline(textout.m_strText);

      //IDWriteTextFormat * pformat = textout.m_pwritetextfont->get_os_data < IDWriteTextFormat * > (this);

      //IDWriteFactory * pfactory = direct2d()->dwrite_factory();

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

      //CustomTextRenderer renderer(direct2d()->d2d1_factory1(),m_pd2d1devicecontext,pdraw2dpen->get_os_data < ID2D1Brush * >(this));

      //defer_text_primitive_blend();

      //defer_text_rendering_hint();

      //playout->Draw(nullptr, &renderer, (FLOAT) textout.m_point.x, (FLOAT) textout.m_point.y);

      return true;

   }


   bool graphics::fill(const ::write_text::text_out & textout, ::draw2d::brush * pdraw2dbrush)
   {

      //wstring szOutline(textout.m_strText);

      //IDWriteTextFormat * pformat = textout.m_pwritetextfont->get_os_data < IDWriteTextFormat * >(this);

      //IDWriteFactory * pfactory = direct2d()->dwrite_factory();

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

      //auto posbrush = pdraw2dbrush->get_os_data < ID2D1Brush * >(this);

      //if (posbrush)
      //{

      //   CustomTextRenderer renderer(direct2d()->d2d1_factory1(), m_pd2d1devicecontext, nullptr, posbrush);

      //   defer_text_primitive_blend();

      //   defer_text_rendering_hint();

      //   playout->Draw(nullptr, &renderer, (FLOAT)textout.m_point.x, (FLOAT)textout.m_point.y);

      //}

      return true;

   }


   bool graphics::draw(const ::write_text::draw_text & drawtext, ::draw2d::pen * pdraw2dpen)
   {

      //wstring szOutline(drawtext.m_strText);

      //IDWriteTextFormat* pformat = drawtext.m_pwritetextfont->get_os_data < IDWriteTextFormat* >(this);

      //IDWriteFactory* pfactory = direct2d()->dwrite_factory();

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

      //CustomTextRenderer renderer(direct2d()->d2d1_factory1(), m_pd2d1devicecontext, pdraw2dpen->get_os_data < ID2D1Brush* >(this));

      //defer_text_primitive_blend();

      //defer_text_rendering_hint();

      //playout->Draw(nullptr, &renderer, (FLOAT)drawtext.m_rectangle.left, (FLOAT)drawtext.m_rectangle.top);

      return true;

   }


   bool graphics::fill(const ::write_text::draw_text & drawtext, ::draw2d::brush * pdraw2dbrush)
   {

      //wstring szOutline(drawtext.m_strText);

      //IDWriteTextFormat* pformat = drawtext.m_pwritetextfont->get_os_data < IDWriteTextFormat* >(this);

      //IDWriteFactory* pfactory = direct2d()->dwrite_factory();

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

      //auto posbrush = pdraw2dbrush->get_os_data < ID2D1Brush* >(this);

      //if (posbrush)
      //{

      //   CustomTextRenderer renderer(direct2d()->d2d1_factory1(), m_pd2d1devicecontext, nullptr, posbrush);

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

         //throw ::exception(error_null_pointer);

      }

      if (m_egraphics == e_graphics_draw)
      {

         HRESULT hr = m_pd2d1devicecontext->Flush();

      }

      //return SUCCEEDED(hr);

   }


   void graphics::debug()
   {

//#ifdef _DEBUG
//      directx11_debug();
//#endif

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

      comptr<ID2D1SolidColorBrush> pdraw2dbrush;

      D2D1_COLOR_F d2d1color;

      copy(d2d1color, color);

      HRESULT hr = m_pd2d1devicecontext->CreateSolidColorBrush(d2d1color, &pdraw2dbrush);

      if (FAILED(hr))
      {

         throw hresult_exception(hr);

      }

      return pdraw2dbrush;

   }

   comptr<ID2D1RadialGradientBrush> graphics::_create_simple_radial_gradient(const ::f64_rectangle & r, ID2D1GradientStopCollection * pstopcollection)
   {

      comptr<ID2D1RadialGradientBrush> pdraw2dbrush;

      ::f64 centerx = r.center_x();
      ::f64 centery = r.center_y();
      ::f64 offsetx = 0.0;
      ::f64 offsety = 0.0;
      ::f64 radiusx = r.width() / 2.0;
      ::f64 radiusy = r.height() / 2.0;

      HRESULT hr = m_pd2d1devicecontext->CreateRadialGradientBrush(
         D2D1::RadialGradientBrushProperties(
            D2D1::Point2F((FLOAT)(centerx), (FLOAT)(centery)),
            D2D1::Point2F((FLOAT)(offsetx), (FLOAT)(offsety)),
            (FLOAT)(radiusx), (FLOAT)(radiusy)),
         pstopcollection,
         &pdraw2dbrush
      );

      if (FAILED(hr))
      {

         throw hresult_exception(hr);

      }

      return pdraw2dbrush;

   }


   comptr<ID2D1LinearGradientBrush> graphics::_create_simple_linear_gradient(const ::f64_point & p1, const ::f64_point & p2, ID2D1GradientStopCollection * pstopcollection)
   {

      comptr<ID2D1LinearGradientBrush> pdraw2dbrush;

      D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES prop{};

      ::i32_point pointViewport(0, 0);

      prop.startPoint.x = (FLOAT)p1.x + pointViewport.x;
      prop.startPoint.y = (FLOAT)p1.y + pointViewport.y;
      prop.endPoint.x = (FLOAT)p2.x + pointViewport.x;
      prop.endPoint.y = (FLOAT)p2.y + pointViewport.y;

      D2D1_BRUSH_PROPERTIES brushproperties = {};

      brushproperties.opacity = 1.0f;
      brushproperties.transform = D2D1::IdentityMatrix();

      HRESULT hr = m_pd2d1devicecontext->CreateLinearGradientBrush(&prop, &brushproperties, pstopcollection, &pdraw2dbrush);

      if (FAILED(hr))
      {

         throw hresult_exception(hr);

      }

      return pdraw2dbrush;

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
      HRESULT hr = m_pd2d1devicecontext->CreateGradientStopCollection(
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


   ID2D1Geometry * graphics::defer_update_os_data(::pointer < ::geometry2d::region > & pregion)
   {

      ::pointer < region > pdirect2dregion = pregion;

      if (!pdirect2dregion)
      {

         pdirect2dregion = create_newø < region >();

         //pdirect2dregion->m_eregion = pregion->m_eregion;

         pdirect2dregion->m_pitem = pregion->m_pitem;

         pregion = pdirect2dregion;

      }

      return (ID2D1Geometry *)pdirect2dregion->m_pd2d1geometry;

   }


} // namespace draw2d_direct2d





