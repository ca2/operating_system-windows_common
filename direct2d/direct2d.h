#pragma once


#include "geometry_sink_text_renderer.h"


namespace direct2d
{


   class CLASS_DECL_DIRECT2D direct2d :
      virtual public ::matter
   {
   public:


      comptr<IDWriteFactory>        m_pwritefactory;
      comptr<ID2D1Factory1>         m_pd2factory;

      geometry_sink_text_renderer   m_geometrysinktextrenderer;


      comptr<ID2D1Device>           m_pd2device;
      comptr<ID2D1DeviceContext>    m_pd2devicecontext;

      comptr<ID2D1Multithread>      m_d2dMultithread;

      //D3D_FEATURE_LEVEL             m_featurelevel;

      static direct2d *             s_pdirect2d;


      direct2d();
      ~direct2d() override;


      void initialize(::object * pobject) override;


      IDWriteFactory * dwrite_factory(bool bCreate = true);
      ID2D1Factory1 * d2d1_factory1(bool bCreate = true);


      ID2D1Device* draw_get_d2d1_device();


      comptr < ID2D1PathGeometry1 > create_rectangle_path_geometry(const ::rectangle_f64 & rectangle);



   };


   inline direct2d* direct2d()
   {

      return direct2d::s_pdirect2d;

   }


   CLASS_DECL_DIRECT2D void defer_initialize(::object * pobject);
   CLASS_DECL_DIRECT2D void finalize();


} // namespace direct2d



