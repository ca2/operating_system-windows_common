#include "framework.h"
#include "direct2d.h"
#include "directx/directx.h"
#include "acme/graphics/write_text/font_weight.h"
#include "acme/prototype/geometry2d/rectangle.h"
#include "acme_windows_common/hresult_exception.h"



#define d2d1_fax_options D2D1_FACTORY_OPTIONS // fax of merde
#define d2d1_thread_model D2D1_FACTORY_TYPE_MULTI_THREADED // ???? muliple performance multi thread hidden option there exists cost uses?


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


   class direct2d* direct2d::s_pdirect2d = nullptr;


   direct2d::direct2d()
   {
      

   }


   direct2d::~direct2d()
   {

   }


   void direct2d::initialize(::particle * pparticle)
   {

      ::directx::defer_initialize(pparticle);

      auto& pdxgidevice = ::directx::directx()->m_pdxgidevice;

      // Create the Direct2D device object and a corresponding context.
      ::defer_throw_hresult(d2d1_factory1()->CreateDevice(pdxgidevice, &m_pd2device));

      d2d1_factory1()->QueryInterface(IID_PPV_ARGS(&m_d2dMultithread));

      //return ::success;

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


      if (m_pd2factory != nullptr || !bCreate)
      {

         return m_pd2factory;

      }

      d2d1_fax_options options;

      memory_set(&options, 0, sizeof(options));

      HRESULT hr = ::D2D1CreateFactory(d2d1_thread_model, __uuidof(ID2D1Factory1), &options, (void **) &m_pd2factory);

      if (FAILED(hr))
      {

         return nullptr;

      }

      m_geometrysinktextrenderer.m_pfactory = m_pd2factory;

      return m_pd2factory;

   }

   
   ID2D1Device * direct2d::draw_get_d2d1_device()
   {

      return m_pd2device;

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



   CLASS_DECL_DIRECT2D void defer_initialize(::particle * pparticle)
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


      direct2d::s_pdirect2d = ___new class direct2d;

      direct2d::s_pdirect2d->initialize(pparticle);

   }


   CLASS_DECL_DIRECT2D void finalize()
   {

      ::acme::del(direct2d::s_pdirect2d);

      //::acme::del(g_pdxgidebug);

   }


} // namespace draw2d_direct2d



CLASS_DECL_DIRECT2D DWRITE_FONT_WEIGHT dwrite_font_weight(const write_text::font_weight & fontweight)
{

   return (DWRITE_FONT_WEIGHT)fontweight.as_int();

}


