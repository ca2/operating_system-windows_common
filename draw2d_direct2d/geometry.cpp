// Created by camilo on 2023-06-13 03:14 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "acme/primitive/geometry2d/ellipse.h"
#include "acme/primitive/geometry2d/polygon.h"
//#include "acme/primitive/geometry2d/poly_polygon.h"
#include "direct2d/direct2d.h"


namespace direct2d
{


   namespace geometry
   {


      CLASS_DECL_DRAW2D_DIRECT2D comptr<ID2D1RectangleGeometry> create_rectangle(const ::rectangle_f32 & rectangle)
      {

         comptr<ID2D1RectangleGeometry> pgeometry;

         D2D1_RECT_F r;

         copy(r, rectangle);

         ::direct2d::direct2d()->d2d1_factory1()->CreateRectangleGeometry(r, &pgeometry);


         return pgeometry;

      }


      CLASS_DECL_DRAW2D_DIRECT2D comptr<ID2D1EllipseGeometry> create_ellipse(const ::ellipse_f32 & ellipse)
      {

         D2D1_ELLIPSE d2d1ellipse;

         comptr<ID2D1EllipseGeometry> pgeometry;

         auto pointCenter = ellipse.center();
         auto sizeRadius = ellipse.size() / 2.f;

         d2d1ellipse.point.x = pointCenter.x();
         d2d1ellipse.point.y = pointCenter.y();
         d2d1ellipse.radiusX = sizeRadius.cx();
         d2d1ellipse.radiusY = sizeRadius.cy();

         ::direct2d::direct2d()->d2d1_factory1()->CreateEllipseGeometry(d2d1ellipse, &pgeometry);

         return pgeometry;


      }


      CLASS_DECL_DRAW2D_DIRECT2D comptr<ID2D1PathGeometry> create_polygon(const ::polygon_f64 & polygon, bool bFilled)
      {

         if (polygon.size() <= 2)
         {

            return nullptr;

         }

         comptr<ID2D1PathGeometry> ppathgeometry;

         HRESULT hr = ::direct2d::direct2d()->d2d1_factory1()->CreatePathGeometry(&ppathgeometry);

         {

            comptr<ID2D1GeometrySink> psink;

            ppathgeometry->Open(&psink);

            psink->SetFillMode(D2D1_FILL_MODE_WINDING);

            psink->BeginFigure({ (FLOAT)polygon.first().x(), (FLOAT)polygon.first().y()}, bFilled ? D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN_HOLLOW);

            for (::index i = 1; i < polygon.size(); i++)
            {

               psink->AddLine({ (FLOAT)polygon[i].x(), (FLOAT)polygon[i].y()});

            }

            psink->EndFigure(D2D1_FIGURE_END_CLOSED);

            psink->Close();

         }

         return ppathgeometry;

      }


      CLASS_DECL_DRAW2D_DIRECT2D comptr<ID2D1PathGeometry> create_poly_polygon(const ::poly_polygon_f64 & polypolygon, bool bFilled)
      {

         comptr<ID2D1PathGeometry> ppathgeometry;

         HRESULT hr = ::direct2d::direct2d()->d2d1_factory1()->CreatePathGeometry(&ppathgeometry);

         {

            comptr<ID2D1GeometrySink> psink;

            ppathgeometry->Open(&psink);

            psink->SetFillMode(D2D1_FILL_MODE_WINDING);

            for (int i = 0; i < polypolygon.size(); i++)
            {
               
               auto & polygon = *polypolygon[i];

               auto jCount = polygon.size();

               psink->BeginFigure({ (FLOAT)polygon.first().x(), (FLOAT)polygon.first().y()}, bFilled ? D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN_HOLLOW);

               for (::index j = 1; j < jCount; j++)
               {

                  psink->AddLine({ (FLOAT)polygon[j].x(), (FLOAT)polygon[j].y()});

               }

               psink->EndFigure(D2D1_FIGURE_END_CLOSED);

            }


            psink->Close();

         }

         return ppathgeometry;

      }


   } // namespace geometry


} // namespace direct2d

