#pragma once


//#include "acme/prototype/geometry2d/_geometry2d.h"
#include "geometry_sink_text_renderer.h"
#include "apex/platform/app_consumer.h"
#include "aura/platform/application.h"


class dxgi_device_source;


namespace direct2d
{


   class CLASS_DECL_DIRECT2D direct2d :
      virtual public ::app_consumer<::aura::application>
   {
   protected:
      comptr < ID2D1Device >        m_pd2d1device;
   public:


      ::pointer < ::dxgi_device_source >     m_pdxgidevicesource;

      comptr<IDWriteFactory>        m_pwritefactory;
      comptr<ID2D1Factory1>         m_pd2d1factory;
      geometry_sink_text_renderer   m_geometrysinktextrenderer;


      comptr<ID2D1DeviceContext>    m_pd2d1devicecontextDefault;

      comptr<ID2D1Multithread>      m_pd2d1multithread;

      //D3D_FEATURE_LEVEL             m_featurelevel;

      //static ::pointer < direct2d >             s_pdirect2d;


      direct2d();
      ~direct2d() override;


      virtual void initialize_direct2d(::dxgi_device_source * pdxgidevicesource);

   protected:
      virtual comptr<ID2D1Device> create_d2d1_device(::dxgi_device_source * pdxgidevicesource);
   public:

      IDWriteFactory * dwrite_factory(bool bCreate = true);
      ID2D1Factory1 * d2d1_factory1(bool bCreate = true);


      ID2D1Device* d2d1_device(::dxgi_device_source* pdxgidevicesource);

      comptr<ID2D1DeviceContext> create_d2d1_device_context(::dxgi_device_source* pdxgidevicesource);

      ID2D1DeviceContext * _default_d2d1_device_context(::dxgi_device_source* pdxgidevicesource);

      ID2D1DeviceContext* default_d2d1_device_context(::dxgi_device_source* pdxgidevicesource);


      comptr < ID2D1PathGeometry1 > create_rectangle_path_geometry(const ::double_rectangle & rectangle);

      



   };


   CLASS_DECL_DIRECT2D direct2d* from_gpu_device(::gpu::device* pgpudevice);


   ////inline ID2D1Factory1* factory()
   //{

     // return get()->d2d1_factory1();

   //}


   //inline IDWriteFactory* dwrite_factory()
   //{

     // return get()->dwrite_factory();

   //}



   //CLASS_DECL_DIRECT2D void defer_initialize(::windowing::window * pwindow, const ::int_rectangle & rectanglePlacement);
   //CLASS_DECL_DIRECT2D void finalize();


} // namespace direct2d




class CLASS_DECL_DIRECT2D direct2d_lock
{
public:


   bool m_bLocked = false;
   ::direct2d::direct2d* m_pdirect2d;


   direct2d_lock(::direct2d::direct2d * pdirect2d);
   ~direct2d_lock();


};
