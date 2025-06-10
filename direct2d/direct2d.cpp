#include "framework.h"
#include "direct2d.h"
//#include "directx/directx.h"
#include "gpu_directx11/approach.h"
#include "gpu_directx11/device.h"
#include "acme/graphics/write_text/font_weight.h"
#include "acme/prototype/geometry2d/rectangle.h"
#include "acme_windows_common/hresult_exception.h"
#include "aura/windowing/window.h"




typedef HRESULT WINAPI FN_DXGIGetDebugInterface(REFIID riid, void ** ppDebug);

typedef FN_DXGIGetDebugInterface * PFN_DXGIGetDebugInterface;

//class dxgidebug
//{
//public:
//
//   comptr<IDXGIDebug> d;
//
//   void init()
//   {
//
//#ifdef UNIVERSAL_WINDOWS
//      HRESULT hr = DXGIGetDebugInterface1(0, IID_IDXGIDebug1, &d);
//#else
//
//      comptr<IDXGIInfoQueue> dxgiInfoQueue;
//
//      typedef HRESULT(WINAPI * LPDXGIGETDEBUGINTERFACE)(REFIID, void **);
//
//      HMODULE dxgidebug = LoadLibraryExW(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
//      if (dxgidebug)
//      {
//         auto dxgiGetDebugInterface = reinterpret_cast<LPDXGIGETDEBUGINTERFACE>(
//            reinterpret_cast<void *>(GetProcAddress(dxgidebug, "DXGIGetDebugInterface")));
//         if (SUCCEEDED(dxgiGetDebugInterface(IID_IDXGIDebug, &d)))
//         {
//
//         }
//         //d.Attach(dxgiGetDebugInterface);
//         //if (SUCCEEDED(dxgiGetDebugInterface(IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
//         //{
//         // dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
//         //dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
//         //}
//      }
//
//#endif
//
//
//   }
//
//   void debug()
//   {
//
//      if (d.Get() != nullptr)
//      {
//
//         ::output_debug_string("MY_DEBUG : IDXGIDebug::ReportLiveObjects");
//
//         d->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
//
//      }
//      else
//      {
//
//         init();
//
//      }
//
//   }
//
//} *g_pdxgidebug;




namespace direct2d
{


   ::pointer < class direct2d > direct2d::s_pdirect2d;


   direct2d::direct2d()
   {

      if (s_pdirect2d)
      {

         throw ::exception(error_wrong_state);

      }

      s_pdirect2d = this;

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


   void direct2d::initialize(::particle * pparticle)
   {

      ::app_consumer<::aura::application>::initialize(pparticle);

   }


   comptr<ID2D1Device> direct2d::create_device(::windowing::window* pwindow, const ::int_rectangle& rectanglePlacement)
   {

      ::cast < ::gpu_directx11::approach > papproach = m_papplication->get_gpu();

      ::cast < ::gpu_directx11::device > pgpudevice = papproach->get_device(pwindow, rectanglePlacement);

      auto& pdxgidevice = pgpudevice->m_pdxgidevice;

      comptr<ID2D1Device> pd2d1device;

      // Create the Direct2D device object and a corresponding context.
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

   
   ID2D1Device * direct2d::d2d1_device(::windowing::window* pwindow, const ::int_rectangle& rectanglePlacement)
   {

      if (!m_pd2d1device)
      {

         m_pd2d1device = create_device(pwindow, rectanglePlacement);

      }

      return m_pd2d1device;

   }

   
   ID2D1DeviceContext* direct2d::default_d2d1_device_context(::windowing::window* pwindow, const ::int_rectangle& rectanglePlacement)
   {

      if (!m_pd2d1devicecontextDefault)
      {

         m_pd2d1devicecontextDefault = create_d2d1_device_context(pwindow, rectanglePlacement);

      }

      return m_pd2d1devicecontextDefault;

   }


   comptr<ID2D1DeviceContext> direct2d::create_d2d1_device_context(::windowing::window* pwindow, const ::int_rectangle& rectanglePlacement)
   {

      auto pd2d1device = d2d1_device(pwindow, rectanglePlacement);

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


   ID2D1DeviceContext* direct2d::default_d2d1_device_context()
   {

      return m_pd2d1devicecontextDefault;

   }


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



   CLASS_DECL_DIRECT2D void defer_initialize(::windowing::window* pwindow, const ::int_rectangle& rectanglePlacement)
   {

      if (::is_set(direct2d::s_pdirect2d))
      {

         return;

      }

      //if (g_pdxgidebug == nullptr)
      //{

      //   g_pdxgidebug = ___new dxgidebug;

      //   //g_pdirect2dplugin = ___new plugin;

      //   //g_pdirect2dplugin->initialize();

      //}


      pwindow->__defer_construct(direct2d::s_pdirect2d);

      //direct2d::s_pdirect2d->initialize(pwindow);

      //direct2d::s_pdirect2d->initialize_direct2d(pwindow, rectanglePlacement);

   }


   CLASS_DECL_DIRECT2D void finalize()
   {

      direct2d::s_pdirect2d.release();

      //::acme::del(g_pdxgidebug);

   }


} // namespace draw2d_direct2d



CLASS_DECL_DIRECT2D DWRITE_FONT_WEIGHT dwrite_font_weight(const write_text::font_weight & fontweight)
{

   return (DWRITE_FONT_WEIGHT)fontweight.as_int();

}




direct2d_lock::direct2d_lock()
{

   auto pdirect2d = ::direct2d::get();

   if (::is_null(pdirect2d))
   {

      return;

   }

   auto pmultithread = pdirect2d->m_pd2d1multithread.m_p;

   if (::is_null(pmultithread))
   {

      return;

   }

   pmultithread->Enter();

   m_bLocked = true;

}


direct2d_lock::~direct2d_lock()
{

   if (m_bLocked)
   {

      ::direct2d::direct2d::s_pdirect2d->m_pd2d1multithread->Leave();

   }

}
