#include "framework.h"


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
//#ifdef _UWP
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


   ::e_status direct2d::initialize(::object * pobject)
   {

      ::directx::defer_initialize(pobject);

      auto& pdxgidevice = ::directx::directx()->m_pdxgidevice;

      // Create the Direct2D device object and a corresponding context.
      ::throw_if_failed(d2d1_factory1()->CreateDevice(pdxgidevice, &m_pd2device));

      d2d1_factory1()->QueryInterface(IID_PPV_ARGS(&m_d2dMultithread));

      return ::success;

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

      __memset(&options, 0, sizeof(options));

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


   CLASS_DECL_DIRECT2D void defer_initialize(::object * pobject)
   {

      if (::is_set(direct2d::s_pdirect2d))
      {

         return;

      }

      //if (g_pdxgidebug == nullptr)
      //{

      //   g_pdxgidebug = new dxgidebug;

      //   //g_pdirect2dplugin = new plugin;

      //   //g_pdirect2dplugin->initialize();

      //}


      direct2d::s_pdirect2d = new class direct2d;

      direct2d::s_pdirect2d->initialize(pobject);



   }


   CLASS_DECL_DIRECT2D void finalize()
   {

      ::acme::del(direct2d::s_pdirect2d);

      //::acme::del(g_pdxgidebug);

   }


} // namespace draw2d_direct2d



