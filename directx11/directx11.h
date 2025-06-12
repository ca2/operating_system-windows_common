#pragma once


//#include "acme/prototype/geometry2d/_geometry2d.h"
//#include "geometry_sink_text_renderer.h"
#include "apex/platform/app_consumer.h"
#include "aura/platform/application.h"


namespace directx11
{


   class CLASS_DECL_DIRECTX11 directx11 :
      virtual public ::app_consumer<::aura::application>
   {
   protected:
      //comptr < ID2D1Device >        m_pd2d1device;
   public:


      //comptr<IDWriteFactory>        m_pwritefactory;
      //comptr<ID2D1Factory1>         m_pd2d1factory;
      //geometry_sink_text_renderer   m_geometrysinktextrenderer;


      //comptr<ID2D1DeviceContext>    m_pd2d1devicecontextDefault;

      //comptr<ID2D1Multithread>      m_pd2d1multithread;

      //D3D_FEATURE_LEVEL             m_featurelevel;

      static ::pointer < directx11 >             s_pdirectx11;


      directx11();
      ~directx11() override;


      void initialize(::particle * pparticle) override;

   //protected:
   //   virtual comptr<ID2D1Device> create_device(::windowing::window* pwindow, const ::int_rectangle& rectanglePlacement);
   //public:

      //IDWriteFactory * dwrite_factory(bool bCreate = true);
      //ID2D1Factory1 * d2d1_factory1(bool bCreate = true);


      //ID2D1Device* d2d1_device(::windowing::window* pwindow, const ::int_rectangle& rectanglePlacement);

      //comptr<ID2D1DeviceContext> create_d2d1_device_context(::windowing::window* pwindow, const ::int_rectangle& rectanglePlacement);

      //ID2D1DeviceContext * default_d2d1_device_context(::windowing::window* pwindow, const ::int_rectangle& rectanglePlacement);

      //ID2D1DeviceContext* default_d2d1_device_context();


      //comptr < ID2D1PathGeometry1 > create_rectangle_path_geometry(const ::double_rectangle & rectangle);

      //virtual IDXGISurface* _get_dxgi_surface();



   };



   inline directx11* get()
   {

      return directx11::s_pdirectx11;

   }


   //inline ID2D1Factory1* factory()
   //{

   //   return get()->d2d1_factory1();

   //}


   //inline IDWriteFactory* dwrite_factory()
   //{

   //   return get()->dwrite_factory();

   //}



   CLASS_DECL_DIRECTX11 void defer_initialize(::windowing::window * pwindow, const ::int_rectangle & rectanglePlacement);
   CLASS_DECL_DIRECTX11 void finalize();


} // namespace directx11


//
//
//class CLASS_DECL_DIRECTX11 directx11_lock
//{
//public:
//
//
//   bool m_bLocked = false;
//
//
//   directx11_lock();
//   ~directx11_lock();
//
//
//};
