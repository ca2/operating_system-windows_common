#include "framework.h"
#include "direct2d.h"
//#include "swap_chain.h"
#include "acme/exception/interface_only.h"
#include "acme/graphics/write_text/font_weight.h"
#include "acme/prototype/geometry2d/rectangle.h"
#include "acme_windows_common/dxgi_device_source.h"
#include "acme/operating_system/windows_common/com/hresult_exception.h"
#include "bred/gpu/bred_approach.h"
#include "bred/gpu/device.h"
#include "aura/windowing/window.h"
#include "directx11/directx11.h"
#include "gpu_directx11/swap_chain.h"




typedef HRESULT WINAPI FN_DXGIGetDebugInterface(REFIID riid, void ** ppDebug);

typedef FN_DXGIGetDebugInterface * PFN_DXGIGetDebugInterface;



namespace direct2d
{


//   ::pointer < class direct2d > direct2d::s_pdirect2d;


   direct2d::direct2d()
   {

      //if (s_pdirect2d)
      //{

      //   throw ::exception(error_wrong_state);

      //}

      //s_pdirect2d = this;

      d2d1_factory1(true);

      if (!m_pd2d1multithread)
      {

         HRESULT hrMultithread = m_pd2d1factory->QueryInterface(IID_PPV_ARGS(&m_pd2d1multithread));

         ::defer_throw_hresult(hrMultithread);

      }


   }


   direct2d::~direct2d()
   {

   }


   void direct2d::initialize_direct2d(::dxgi_device_source * pdxgidevicesource)
   {

      ::app_consumer<::aura::application>::initialize(pdxgidevicesource);

      m_pdxgidevicesource = pdxgidevicesource;

   }


   comptr<ID2D1Device> direct2d::create_d2d1_device(::dxgi_device_source* pdxgidevicesource)
   {

      direct2d_lock direct2dlock(this);

      /*::cast < ::gpu::approach > pgpuapproach = m_papplication->get_gpu_approach();

      ::cast < ::gpu::device > pgpudevice = pgpuapproach->get_gpu_device();*/

      //if (m_papplication->m_gpu.m_bUseSwapChainWindow)
      //{

      //   ::cast < ::gpu_directx11::swap_chain > pswapchain = pgpudevice->get_swap_chain();

      //   pdxgidevice = pswapchain->_get_dxgi_device();

      //}
      //else
      //{

//      ::cast < ::dxgi_device_source > pdxgidevicesource = m_ppgpudevice;

      auto pdxgidevice = pdxgidevicesource->_get_dxgi_device();

      comptr<ID2D1Device> pd2d1device;

      HRESULT hr = m_pd2d1factory->CreateDevice(pdxgidevice, &pd2d1device);
      
      ::defer_throw_hresult(hr);

      return pd2d1device;

   }


   IDWriteFactory * direct2d::dwrite_factory(bool bCreate)
   {

      if (m_pwritefactory != nullptr || !bCreate)
      {

         return m_pwritefactory;

      }

      HRESULT hr = DWriteCreateFactory(
         DWRITE_FACTORY_TYPE_SHARED,
         __uuidof(IDWriteFactory),
         reinterpret_cast < IUnknown ** > (&m_pwritefactory));

      if (FAILED(hr))
      {

         return nullptr;

      }
      return m_pwritefactory;

   }


   ID2D1Factory1 * direct2d::d2d1_factory1(bool bCreate)
   {

      if (m_pd2d1factory != nullptr || !bCreate)
      {

         return m_pd2d1factory;

      }

      D2D1_FACTORY_OPTIONS d2d1factoryoptions{};

#ifdef _DEBUG
      d2d1factoryoptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

      HRESULT hr = ::D2D1CreateFactory(
         D2D1_FACTORY_TYPE_MULTI_THREADED,
         __uuidof(ID2D1Factory1), 
         &d2d1factoryoptions,
         (void **) &m_pd2d1factory);

      if (FAILED(hr))
      {

         return nullptr;

      }

      m_geometrysinktextrenderer.m_pfactory = m_pd2d1factory;

      return m_pd2d1factory;

   }

   
   ID2D1Device * direct2d::d2d1_device(::dxgi_device_source* pdxgidevicesource)
   {

      if (!m_pd2d1device)
      {

         m_pd2d1device = create_d2d1_device(pdxgidevicesource);

      }

      return m_pd2d1device;

   }

   
   ID2D1DeviceContext* direct2d::default_d2d1_device_context(::dxgi_device_source* pdxgidevicesource)
   {

      if (!m_pd2d1devicecontextDefault)
      {

         m_pd2d1devicecontextDefault = create_d2d1_device_context(pdxgidevicesource);

      }

      return m_pd2d1devicecontextDefault;

   }


   comptr<ID2D1DeviceContext> direct2d::create_d2d1_device_context(::dxgi_device_source* pdxgidevicesource)
   {

      auto pd2d1device = d2d1_device(pdxgidevicesource);

      comptr<ID2D1DeviceContext> pd2d1devicecontext;
      
      HRESULT hr = pd2d1device->CreateDeviceContext(
         //D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
         D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
         &pd2d1devicecontext
      );

      if (FAILED(hr))
      {

         warning() << "graphics::CreateCompatibleDC, CreateDeviceContext (1) " << hresult_text(hr);

         //return false;

         throw ::exception(error_failed);

      }

      return pd2d1devicecontext;

   }


   //ID2D1DeviceContext* direct2d::default_d2d1_device_context()
   //{

   //   return m_pd2d1devicecontextDefault;

   //}


   comptr < ID2D1PathGeometry1 > direct2d::create_rectangle_path_geometry(const ::double_rectangle & rectangle)
   {

      comptr < ID2D1PathGeometry1 > ppathgeometry;

      HRESULT hr = d2d1_factory1()->CreatePathGeometry(&ppathgeometry);

      if (FAILED(hr))
      {

         throw ::hresult_exception(hr);

      }

      comptr < ID2D1GeometrySink > psink;

      ppathgeometry->Open(&psink);

      if (FAILED(hr))
      {

         throw ::hresult_exception(hr);

      }

      psink->SetFillMode(D2D1_FILL_MODE_WINDING);
      psink->BeginFigure(D2D1::Point2F((FLOAT) rectangle.left(), (FLOAT)rectangle.top()), D2D1_FIGURE_BEGIN_FILLED);
      psink->AddLine(D2D1::Point2F((FLOAT)rectangle.right(), (FLOAT)rectangle.top()));
      psink->AddLine(D2D1::Point2F((FLOAT)rectangle.right(), (FLOAT)rectangle.bottom()));
      psink->AddLine(D2D1::Point2F((FLOAT)rectangle.left(), (FLOAT)rectangle.bottom()));
      psink->EndFigure(D2D1_FIGURE_END_CLOSED);

      return ppathgeometry;

   }



   //CLASS_DECL_DIRECT2D void defer_initialize(::windowing::window* pwindow, const ::int_rectangle& rectanglePlacement)
   //{

   //   if (::is_set(direct2d::s_pdirect2d))
   //   {

   //      return;

   //   }

   //   //if (g_pdxgidebug == nullptr)
   //   //{

   //   //   g_pdxgidebug = ___new dxgidebug;

   //   //   //g_pdirect2dplugin = ___new plugin;

   //   //   //g_pdirect2dplugin->initialize();

   //   //}


   //   pwindow->__defer_construct(direct2d::s_pdirect2d);

   //   //direct2d::s_pdirect2d->initialize(pwindow);

   //   //direct2d::s_pdirect2d->initialize_direct2d(pwindow, rectanglePlacement);

   //}


   //CLASS_DECL_DIRECT2D void finalize()
   //{

   //   direct2d::s_pdirect2d.release();

   //   //::acme::del(g_pdxgidebug);

   //}



   CLASS_DECL_DIRECT2D direct2d* from_gpu_device(::gpu::device* pgpudevice)
   {

      ::cast < direct2d > pdirect2d = pgpudevice->payload("direct2d").get_subparticle();

      if (!pdirect2d)
      {

         ::pointer < direct2d > pdirect2dNew;

         pgpudevice->__construct_new(pdirect2dNew);

         ::cast < ::dxgi_device_source > pdxgidevicesource = pgpudevice;

         pdirect2dNew->initialize_direct2d(pdxgidevicesource);

         pgpudevice->payload("direct2d") = pdirect2dNew;
         
         pdirect2d = pgpudevice->payload("direct2d").get_subparticle();

      }

      return pdirect2d;

   }



} // namespace draw2d_direct2d



CLASS_DECL_DIRECT2D DWRITE_FONT_WEIGHT dwrite_font_weight(const write_text::font_weight & fontweight)
{

   return (DWRITE_FONT_WEIGHT)fontweight.as_int();

}




direct2d_lock::direct2d_lock(::direct2d::direct2d* pdirect2d)
{

   m_pdirect2d = pdirect2d;

   if (::is_null(m_pdirect2d))
   {

      m_bLocked = false;

      return;

   }

   auto pmultithread = pdirect2d->m_pd2d1multithread.m_p;

   if (::is_null(pmultithread))
   {

      m_bLocked = false;

      return;

   }

   pmultithread->Enter();

   m_bLocked = true;

}


direct2d_lock::~direct2d_lock()
{

   if (m_bLocked)
   {

      m_pdirect2d->m_pd2d1multithread->Leave();

   }

}
