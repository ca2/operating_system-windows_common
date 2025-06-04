#include "framework.h"
#include "region.h"
#include "path.h"
#include "graphics.h"
#include "direct2d/geometry.h"
#include "acme/exception/interface_only.h"
#include "acme/prototype/geometry2d/item.h"
#include "acme/prototype/geometry2d/_defer_item.h"
#include "direct2d/direct2d.h"


namespace draw2d_direct2d
{


   region::region()
   {

      //m_bUpdated = false;

   }


   region::~region()
   {

      destroy();

   }


   //int region::GetRegionData(LPRGNDATA lpRgnData, int nDataSize) const
   //{

   //   throw ::exception(todo);

   //   //   ASSERT(get_os_data() != nullptr); return (int)::GetRegionData((HRGN)get_os_data(), nDataSize, lpRgnData);

   //}
   void region::SetRectRgn(int x1, int y1, int x2, int y2)
   {

      throw ::exception(todo);

      //ASSERT(get_os_data() != nullptr); ::SetRectRgn((HRGN)get_os_data(), x1, y1, x2, y2);
   }

   void region::SetRectRgn(const ::int_rectangle & rectangle)
   {

      throw ::exception(todo);

      //   ::SetRectRgn((HRGN)get_os_data(), rectangle.left(), rectangle.top(), rectangle.right(), rectangle.bottom());
   }

   //int region::CombineRgn(const ::draw2d::region* pRgn1, const ::draw2d::region* pRgn2, int nCombineMode)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_os_data() != nullptr); return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgn1->get_os_data(),
   //   //(HRGN)pRgn2->get_os_data(), nCombineMode);
   //}
   //int region::CopyRgn(const ::draw2d::region* pRgnSrc)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_os_data() != nullptr); return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgnSrc->get_os_data(), nullptr, RGN_COPY);
   //}

   //bool region::EqualRgn(const ::draw2d::region* pRgn) const
   //{

   //   throw ::exception(todo);


   //   //   ASSERT(get_os_data() != nullptr);

   //   // return ::EqualRgn((HRGN)get_os_data(), (HRGN)pRgn->get_os_data()) != false;

   //}

   //int region::OffsetRgn(int x, int y)
   //{

   //   throw ::exception(todo);


   //   //ASSERT(get_os_data() != nullptr); return ::OffsetRgn((HRGN)get_os_data(), x, y);
   //}
   //int region::OffsetRgn(const ::int_point & point)
   //{

   //   throw ::exception(todo);


   //   //ASSERT(get_os_data() != nullptr); return ::OffsetRgn((HRGN)get_os_data(), point.x(), point.y());
   //}
   //int region::GetRgnBox(::int_rectangle * prectangle) const
   //{

   //   throw ::exception(todo);


   //   //ASSERT(get_os_data() != nullptr); return ::GetRgnBox((HRGN)get_os_data(), rectangle);
   //}


   bool region::contains(::draw2d::graphics* pgraphics, int x, int y) const
   {

      ((region *) this)->defer_update(pgraphics, 0);

      BOOL b = false;

      HRESULT hr = m_pgeometry->FillContainsPoint(D2D1::Point2F((FLOAT) x, (FLOAT) y), nullptr, &b);

      if(FAILED(hr))
         return false;

      return b != false;

      //Gdiplus::PointF float_point((Gdiplus::REAL) x, (Gdiplus::REAL) y);
      //
      //ASSERT(get_os_data() != nullptr); //return ::PtInRegion((HRGN)get_os_data(), x, y);

      //return m_pgeometry->IsVisible(float_point)  != false;

   }


   bool region::contains(::draw2d::graphics* pgraphics, const ::int_point & point) const
   {

      return contains(pgraphics, point.x(), point.y());

      //throw ::exception(todo);


      //return ::PtInRegion((HRGN)get_os_data(), point.x(), point.y());
      //Gdiplus::PointF float_point((Gdiplus::REAL) point.x(), (Gdiplus::REAL) point.y());
      //
      //ASSERT(get_os_data() != nullptr); //return ::PtInRegion((HRGN)get_os_data(), x, y);

      //return m_pgeometry->IsVisible(float_point)  != false;

   }


   bool region::RectInRegion(const ::int_rectangle & rectangle) const
   {

      throw ::exception(todo);


      //ASSERT(get_os_data() != nullptr); return ::rectInRegion((HRGN)get_os_data(), rectangle);

      //Gdiplus::RectF float_rectangle((Gdiplus::REAL) rectangle.left(), (Gdiplus::REAL) rectangle.top(), (Gdiplus::REAL) (rectangle.right() - rectangle.left()), (Gdiplus::REAL) (rectangle.bottom() - rectangle.top()));
      //
      //ASSERT(get_os_data() != nullptr); //return ::PtInRegion((HRGN)get_os_data(), x, y);

      //return m_pgeometry->IsVisible(float_rectangle)  != false;
      return false;

   }


   void region::create(::draw2d::graphics * pgraphics, char iCreate)
   {

      m_osdata[0] = get(pgraphics);

      //return m_pgeometry != nullptr;

   }


   ID2D1Geometry * region::get(::draw2d::graphics* pgraphics)
   {

      auto eitem = m_pitem->type();

      switch(eitem)
      {
      case ::draw2d::e_item_none:
      {

         ID2D1PathGeometry * ppathgeometry = nullptr;

         HRESULT hr = ::direct2d::factory()->CreatePathGeometry(&ppathgeometry);

         if(FAILED(hr))
            return nullptr;

         return ppathgeometry;

      }
      case ::draw2d::e_item_rectangle:
         return get_rectangle(pgraphics);
      case ::draw2d::e_item_ellipse:
         return get_ellipse(pgraphics);
      case ::draw2d::e_item_polygon:
         return get_polygon(pgraphics);
      case ::draw2d::e_item_poly_polygon:
         return get_polygon(pgraphics);
      case ::draw2d::e_item_combine:
         return get_combine(pgraphics);
      default:
         throw ::interface_only();
      }

      return nullptr;

   }


   ID2D1Geometry * region::get_rectangle(::draw2d::graphics* pgraphicsParam)
   {

      //ID2D1RectangleGeometry * pgeometry = nullptr;

      auto pgraphics = __graphics(pgraphicsParam);

      if (!pgraphics)
      {

         return nullptr;

      }

      ::pointer<::geometry2d::rectangle_item>pitem = m_pitem;

      //D2D1_RECT_F r;
      //
      //copy(r, pitem->m_item);

      auto prectanglegeometry = ::direct2d::geometry::create_rectangle(pitem->m_item);

      //::direct2d::factory()->CreateRectangleGeometry(r, &pgeometry);

      return prectanglegeometry;

   }


   ID2D1Geometry * region::get_ellipse(::draw2d::graphics* pgraphics)
   {

      //D2D1_ELLIPSE ellipse;

      ::pointer<::geometry2d::ellipse_item>pitem = m_pitem;

      //auto pointCenter = pitem->m_item.center();
      //auto sizeRadius = pitem->m_item.size() / 2.0;

      //ellipse.point.x = (float)pointCenter.x();
      //ellipse.point.y = (float)pointCenter.y();
      //ellipse.radiusX = (float)sizeRadius.cx();
      //ellipse.radiusY = (float)sizeRadius.cy();

      //ID2D1EllipseGeometry * pgeometry = nullptr;

      //::direct2d::factory()->CreateEllipseGeometry(ellipse, &pgeometry);

      auto pellipsegeometry = ::direct2d::geometry::create_ellipse(pitem->m_item);

      return pellipsegeometry;

   }


   ID2D1Geometry * region::get_polygon(::draw2d::graphics* pgraphics)
   {

      //auto ppath = pgraphics->__øcreate < ::draw2d::path > ();

      /*int_point_array pa;

      for(int i = 0; i < m_nCount; i++)
      {
         pa.add(Gdiplus::PointF((Gdiplus::REAL) m_lppoints[i].x(), (Gdiplus::REAL) m_lppoints[i].y()));
      }*/

      /*
      if(m_efillmode == ::draw2d::e_fill_mode_alternate)
      {
         path.SetFillMode(Gdiplus::FillModeAlternate);
      }
      else
      {
         path.SetFillMode(Gdiplus::FillModeWinding);
      }
      */

      ::pointer<::geometry2d::polygon_item>pitem = m_pitem;

      //ppath->begin_figure();
      //ppath->add_polygon(pitem->m_polygon.data(), pitem->m_polygon.size());
      //ppath->close_figure();

      //ppath->get_os_data(pgraphics, path_filled);

      //m_pgeometry = ::transfer((ID2D1PathGeometry *) ppath->detach());

      auto ppathgeometry = ::direct2d::geometry::create_polygon(pitem->m_polygon);

      return ppathgeometry;

   }


   ID2D1Geometry * region::get_poly_polygon(::draw2d::graphics* pgraphics)
   {

      auto ppath = pgraphics->__øcreate < ::draw2d::path > ();

      double_point_array pa;

      /*if(m_efillmode == ::draw2d::e_fill_mode_alternate)
      {
         path.SetFillMode(Gdiplus::FillModeAlternate);
      }
      else
      {
         path.SetFillMode(Gdiplus::FillModeWinding);
      }*/

      int n = 0;

      ::pointer<::geometry2d::poly_polygon_item>pitem = m_pitem;

      for(int i = 0; i < pitem->m_polypolygon.get_size(); i++)
      {
         auto ppolygon = pitem->m_polypolygon[i];
         auto jCount = ppolygon->get_size();
         //pa.erase_all();
         //for(int j = 0; j < jCount; j++)
         //{
         //   pa.add(double_point(m_lppoints[n].x(), m_lppoints[n].y()));
         //   n++;
         //}
         //ppath->begin_figure(true, m_efillmode);
         ppath->begin_figure();
         ppath->add_polygon(ppolygon->data(), (int) ppolygon->size());
         //ppath->end_figure(true);
         ppath->close_figure();
      }

      return (ID2D1PathGeometry *) ppath->detach();

   }


   ID2D1Geometry * region::get_combine(::draw2d::graphics* pgraphics)
   {

      comptr < ID2D1PathGeometry > ppathgeometry ;

      HRESULT hr = ::direct2d::factory()->CreatePathGeometry(&ppathgeometry);

      comptr < ID2D1GeometrySink > psink ;

      if (FAILED(hr))
      {

         return nullptr;

      }

      hr = ppathgeometry->Open(&psink);

      if(FAILED(hr))
      {

         return nullptr;

      }

      ::pointer < ::geometry2d::combine_item > pitem = m_pitem;

      ::pointer < graphics > pdirect2dgraphics = pgraphics;

      auto pgeometry1 = pdirect2dgraphics->defer_update_os_data(pitem->m_pregion1);

      auto pgeometry2 = pdirect2dgraphics->defer_update_os_data(pitem->m_pregion2);

      //auto pgeometry1 = pitem->m_pregion1->get_os_data < ID2D1Geometry * >(pgraphics);

      //auto pgeometry2 = pitem->m_pregion2->get_os_data < ID2D1Geometry * >(pgraphics);

      if(pitem->m_ecombine == ::draw2d::e_combine_add)
      {

         hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_UNION, nullptr, 0.f, psink);

      }
      else if(pitem->m_ecombine == ::draw2d::e_combine_exclude)
      {

         hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_EXCLUDE, nullptr, 0.f, psink);

      }
      else if(pitem->m_ecombine == ::draw2d::e_combine_intersect)
      {

         hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_INTERSECT, nullptr, 0.f, psink);

      }
      else
      {

         hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_UNION, nullptr, 0.f, psink);

      }

      if(FAILED(hr))
      {

         return nullptr;

      }

      hr = psink->Close();

      if(FAILED(hr))
      {

         return nullptr;

      }

      return ppathgeometry.detach();

   }


   void region::destroy()
   {

      destroy_os_data();

   }


   void region::destroy_os_data()
   {

      m_pgeometry = nullptr;

      object::destroy_os_data();

   }


} // namespace draw2d_direct2d



