#include "framework.h"


#define d2d1_fax_options D2D1_FACTORY_OPTIONS // fax of merde
#define d2d1_thread_model D2D1_FACTORY_TYPE_MULTI_THREADED // ???? muliple performance multi thread hidden option there exists cost uses?


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

      ::directx::defer_initialize();

      auto& pdxgidevice = ::directx::directx()->m_pdxgidevice;

      // Create the Direct2D device object and a corresponding context.
      ::directx::throw_if_failed(d2d1_factory1()->CreateDevice(pdxgidevice.Get(), &m_pd2device));

      d2d1_factory1()->QueryInterface(IID_PPV_ARGS(&m_d2dMultithread));

      return ::success;

   }


   IDWriteFactory * direct2d::dwrite_factory(bool bCreate)
   {

      if (m_pwritefactory != nullptr || !bCreate)
      {

         return m_pwritefactory.Get();

      }

      HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &m_pwritefactory);

      if (FAILED(hr))
      {

         return nullptr;

      }

      return m_pwritefactory.Get();

   }


   ID2D1Factory1 * direct2d::d2d1_factory1(bool bCreate)
   {


      if (m_pd2factory != nullptr || !bCreate)
      {

         return m_pd2factory.Get();

      }

      d2d1_fax_options options;

      __memset(&options, 0, sizeof(options));

      HRESULT hr = ::D2D1CreateFactory(d2d1_thread_model, __uuidof(ID2D1Factory1), &options, &m_pd2factory);

      if (FAILED(hr))
      {

         return nullptr;

      }

      return m_pd2factory.Get();

   }

   
   ID2D1Device * direct2d::draw_get_d2d1_device()
   {

      return m_pd2device.Get();

   }


   CLASS_DECL_DIRECT2D void defer_initialize()
   {

      if (::is_set(direct2d::s_pdirect2d))
      {

         return;

      }

      direct2d::s_pdirect2d = new class direct2d;

   }


   CLASS_DECL_DIRECT2D void terminate()
   {

      ::acme::del(direct2d::s_pdirect2d);

   }


} // namespace draw2d_direct2d



