// Created by camilo on 2023-06-13 03:14 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "direct2d.h"
#include "geometry.h"
#include "acme/prototype/geometry2d/ellipse.h"
#include "acme/prototype/geometry2d/polygon.h"
#include "aura/platform/application.h"


namespace direct2d
{


   namespace geometry
   {


      comptr<ID2D1RectangleGeometry> create_rectangle(::direct2d::direct2d * pdirect2d, const ::float_rectangle & rectangle)
      {

         comptr<ID2D1RectangleGeometry> pgeometry;

         D2D1_RECT_F r;

         copy(&r, &rectangle);

         pdirect2d->d2d1_factory1()->CreateRectangleGeometry(r, &pgeometry);

         return pgeometry;

      }


      comptr<ID2D1EllipseGeometry> create_ellipse(::direct2d::direct2d* pdirect2d, const ::float_ellipse & ellipse)
      {

         D2D1_ELLIPSE d2d1ellipse;

         comptr<ID2D1EllipseGeometry> pgeometry;

         auto pointCenter = ellipse.center();
         auto sizeRadius = ellipse.size() / 2.f;

         d2d1ellipse.point.x = pointCenter.x();
         d2d1ellipse.point.y = pointCenter.y();
         d2d1ellipse.radiusX = sizeRadius.cx();
         d2d1ellipse.radiusY = sizeRadius.cy();

         pdirect2d->d2d1_factory1()->CreateEllipseGeometry(d2d1ellipse, &pgeometry);

         return pgeometry;


      }


      comptr<ID2D1PathGeometry> create_polygon(::direct2d::direct2d* pdirect2d, const ::double_polygon & polygon, bool bFilled)
      {

         if (polygon.size() <= 2)
         {

            return nullptr;

         }

         comptr<ID2D1PathGeometry> ppathgeometry;

         HRESULT hr = pdirect2d->d2d1_factory1()->CreatePathGeometry(&ppathgeometry);

         {

            comptr<ID2D1GeometrySink> psink;

            ppathgeometry->Open(&psink);

            psink->SetFillMode(D2D1_FILL_MODE_WINDING);

            add_polygon(psink, polygon, bFilled);

            psink->Close();

         }

         return ppathgeometry;

      }


      comptr<ID2D1PathGeometry> create_poly_polygon(::direct2d::direct2d* pdirect2d, const ::double_poly_polygon & polypolygon, bool bFilled)
      {

         comptr<ID2D1PathGeometry> ppathgeometry;

         HRESULT hr = pdirect2d->d2d1_factory1()->CreatePathGeometry(&ppathgeometry);

         {

            comptr<ID2D1GeometrySink> psink;

            ppathgeometry->Open(&psink);

            psink->SetFillMode(D2D1_FILL_MODE_WINDING);

            add_poly_polygon(psink, polypolygon, bFilled);

            psink->Close();

         }

         return ppathgeometry;

      }


      void add_rectangle(ID2D1GeometrySink * pgeometrysink, const ::double_rectangle & rectangle)
      {

         ::double_polygon polygon;

         polygon = rectangle;

         add_polygon(pgeometrysink, polygon, true);

      }


      void add_ellipse(ID2D1GeometrySink * pgeometrysink, const ::double_ellipse & ellipse, bool bFilled)
      {
         
         D2D1_ARC_SEGMENT arcsegment;

         arcsegment.point.x = (FLOAT) ellipse.center_y();
         arcsegment.point.y = (FLOAT) ellipse.right();

         arcsegment.rotationAngle = (FLOAT)(2.0 * π);

         arcsegment.size.width = (FLOAT)ellipse.width();
         arcsegment.size.height = (FLOAT)ellipse.height();

         arcsegment.sweepDirection = D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE;
         arcsegment.arcSize = D2D1_ARC_SIZE_LARGE;

         pgeometrysink->BeginFigure(arcsegment.point, bFilled ? D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN_HOLLOW);

         pgeometrysink->AddArc(arcsegment);

         pgeometrysink->EndFigure(D2D1_FIGURE_END_CLOSED);

      }


      void add_polygon(ID2D1GeometrySink * pgeometrysink, const ::double_polygon & polygon, bool bFilled)
      {

         pgeometrysink->BeginFigure({ (FLOAT)polygon.first().x(), (FLOAT)polygon.first().y() }, bFilled ? D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN_HOLLOW);

         for (::collection::index i = 1; i < polygon.size(); i++)
         {

            pgeometrysink->AddLine({ (FLOAT)polygon[i].x(), (FLOAT)polygon[i].y() });

         }

         pgeometrysink->EndFigure(D2D1_FIGURE_END_CLOSED);

      }


      void add_poly_polygon(ID2D1GeometrySink * pgeometrysink, const ::double_poly_polygon & polypolygon, bool bFilled)
      {

         for(auto & ppolygon : polypolygon)
         {

            add_polygon(pgeometrysink, *ppolygon, bFilled);

         }

      }


   } // namespace geometry


} // namespace direct2d

