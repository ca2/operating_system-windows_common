#include "platform.h"
#include "region.h"
#include "path.h"
#include "graphics.h"
#include "direct2d/geometry.h"
#include "acme/exception/interface_only.h"
#include "acme/prototype/geometry2d/item.h"
#include "acme/prototype/geometry2d/_defer_item.h"
#include "direct2d/direct2d.h"


namespace draw2d_direct2d_for_directx11
{


   region::region()
   {

      //m_bUpdated = false;

   }


   region::~region()
   {

      destroy();

   }


   //::i32 region::GetRegionData(LPRGNDATA lpRgnData, ::i32 nDataSize) const
   //{

   //   throw ::exception(todo);

   //   //   ASSERT(get_os_data() != nullptr); return (::i32)::GetRegionData((HRGN)get_os_data(), nDataSize, lpRgnData);

   //}
   void region::SetRectRgn(::i32 x1, ::i32 y1, ::i32 x2, ::i32 y2)
   {

      throw ::exception(todo);

      //ASSERT(get_os_data() != nullptr); ::SetRectRgn((HRGN)get_os_data(), x1, y1, x2, y2);
   }

   void region::SetRectRgn(const ::i32_rectangle & rectangle)
   {

      throw ::exception(todo);

      //   ::SetRectRgn((HRGN)get_os_data(), rectangle.left, rectangle.top, rectangle.right, rectangle.bottom);
   }

   //::i32 region::CombineRgn(const ::draw2d::region* pRgn1, const ::draw2d::region* pRgn2, ::i32 nCombineMode)
   //{

   //   throw ::exception(todo);

   //   //ASSERT(get_os_data() != nullptr); return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgn1->get_os_data(),
   //   //(HRGN)pRgn2->get_os_data(), nCombineMode);
   //}
   //::i32 region::CopyRgn(const ::draw2d::region* pRgnSrc)
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

   //::i32 region::OffsetRgn(::i32 x, ::i32 y)
   //{

   //   throw ::exception(todo);


   //   //ASSERT(get_os_data() != nullptr); return ::OffsetRgn((HRGN)get_os_data(), x, y);
   //}
   //::i32 region::OffsetRgn(const ::i32_point & point)
   //{

   //   throw ::exception(todo);


   //   //ASSERT(get_os_data() != nullptr); return ::OffsetRgn((HRGN)get_os_data(), point.x, point.y);
   //}
   //::i32 region::GetRgnBox(::i32_rectangle * prectangle) const
   //{

   //   throw ::exception(todo);


   //   //ASSERT(get_os_data() != nullptr); return ::GetRgnBox((HRGN)get_os_data(), rectangle);
   //}


   bool region::contains(::draw2d::graphics * pdraw2dgraphics, ::i32 x, ::i32 y) const
   {

      return ::draw2d_direct2d::region::contains(pdraw2dgraphics, x, y);

      //((region *) this)->defer_update(pdraw2dgraphics, 0);

      //BOOL b = false;

      //HRESULT hr = m_pgeometry->FillContainsPoint(D2D1::Point2F((FLOAT) x, (FLOAT) y), nullptr, &b);

      //if(FAILED(hr))
      //   return false;

      //return b != false;

      ////Gdiplus::PointF ::f32_point((Gdiplus::REAL) x, (Gdiplus::REAL) y);
      ////
      ////ASSERT(get_os_data() != nullptr); //return ::PtInRegion((HRGN)get_os_data(), x, y);

      ////return m_pgeometry->IsVisible(::f32_point)  != false;

   }


   bool region::contains(::draw2d::graphics * pdraw2dgraphics, const ::i32_point & point) const
   {

      return contains(pdraw2dgraphics, point.x, point.y);

      //throw ::exception(todo);


      //return ::PtInRegion((HRGN)get_os_data(), point.x, point.y);
      //Gdiplus::PointF ::f32_point((Gdiplus::REAL) point.x, (Gdiplus::REAL) point.y);
      //
      //ASSERT(get_os_data() != nullptr); //return ::PtInRegion((HRGN)get_os_data(), x, y);

      //return m_pgeometry->IsVisible(::f32_point)  != false;

   }


   bool region::RectInRegion(const ::i32_rectangle & rectangle) const
   {

      throw ::exception(todo);


      //ASSERT(get_os_data() != nullptr); return ::rectInRegion((HRGN)get_os_data(), rectangle);

      //Gdiplus::RectF ::f32_rectangle((Gdiplus::REAL) rectangle.left, (Gdiplus::REAL) rectangle.top, (Gdiplus::REAL) (rectangle.right - rectangle.left), (Gdiplus::REAL) (rectangle.bottom - rectangle.top));
      //
      //ASSERT(get_os_data() != nullptr); //return ::PtInRegion((HRGN)get_os_data(), x, y);

      //return m_pgeometry->IsVisible(::f32_rectangle)  != false;
      return false;

   }


   void region::update(::draw2d::graphics * pdraw2dgraphics)
   {

      //m_osdata[0] = get(pdraw2dgraphics);

      //return m_pgeometry != nullptr;

   }


   //comptr < ID2D1Geometry > region::get(::draw2d::graphics * pdraw2dgraphics)
   //{

   //   auto eitem = m_pitem->type();

   //   switch(eitem)
   //   {
   //   case ::draw2d::e_item_none:
   //   {

   //      ID2D1PathGeometry * ppathgeometry = nullptr;

   //      HRESULT hr = direct2d()->d2d1_factory1()->CreatePathGeometry(&ppathgeometry);

   //      if(FAILED(hr))
   //         return nullptr;

   //      return ppathgeometry;

   //   }
   //   case ::draw2d::e_item_rectangle:
   //      return get_rectangle(pdraw2dgraphics);
   //   case ::draw2d::e_item_ellipse:
   //      return get_ellipse(pdraw2dgraphics);
   //   case ::draw2d::e_item_polygon:
   //      return get_polygon(pdraw2dgraphics);
   //   case ::draw2d::e_item_poly_polygon:
   //      return get_polygon(pdraw2dgraphics);
   //   case ::draw2d::e_item_combine:
   //      return get_combine(pdraw2dgraphics);
   //   default:
   //      throw ::interface_only();
   //   }

   //   return nullptr;

   //}


   //ID2D1Geometry * region::get_rectangle(::draw2d::graphics * pdraw2dgraphics)
   //{

   //   //ID2D1RectangleGeometry * pgeometry = nullptr;

   //   auto pdraw2dgraphics = __graphics(pdraw2dgraphics);

   //   if (!pdraw2dgraphics)
   //   {

   //      return nullptr;

   //   }

   //   ::pointer<::geometry2d::rectangle_item>pitem = m_pitem;

   //   //D2D1_RECT_F r;
   //   //
   //   //copy(r, pitem->m_item);

   //   auto prectanglegeometry = ::direct2d::geometry::create_rectangle(direct2d(), pitem->m_item);

   //   //m_pdirect2d->d2d1_factory1()->CreateRectangleGeometry(r, &pgeometry);

   //   return prectanglegeometry;

   //}


   //ID2D1Geometry * region::get_ellipse(::draw2d::graphics * pdraw2dgraphics)
   //{

   //   //D2D1_ELLIPSE ellipse;

   //   ::pointer<::geometry2d::ellipse_item>pitem = m_pitem;

   //   //auto pointCenter = pitem->m_item.center();
   //   //auto sizeRadius = pitem->m_item.size() / 2.0;

   //   //ellipse.point.x = (::f32)pointCenter.x;
   //   //ellipse.point.y = (::f32)pointCenter.y;
   //   //ellipse.radiusX = (::f32)sizeRadius.cx;
   //   //ellipse.radiusY = (::f32)sizeRadius.cy;

   //   //ID2D1EllipseGeometry * pgeometry = nullptr;

   //   //m_pdirect2d->d2d1_factory1()->CreateEllipseGeometry(ellipse, &pgeometry);

   //   auto pellipsegeometry = ::direct2d::geometry::create_ellipse(direct2d(), pitem->m_item);

   //   return pellipsegeometry;

   //}


   //ID2D1Geometry * region::get_polygon(::draw2d::graphics * pdraw2dgraphics)
   //{

   //   //auto pdraw2dpath = pdraw2dgraphics->createø < ::draw2d::path > ();

   //   /*i32_point_array pa;

   //   for(::i32 i = 0; i < m_nCount; i++)
   //   {
   //      pa.add(Gdiplus::PointF((Gdiplus::REAL) m_lppoints[i].x, (Gdiplus::REAL) m_lppoints[i].y));
   //   }*/

   //   /*
   //   if(m_efillmode == ::draw2d::e_fill_mode_alternate)
   //   {
   //      path.SetFillMode(Gdiplus::FillModeAlternate);
   //   }
   //   else
   //   {
   //      path.SetFillMode(Gdiplus::FillModeWinding);
   //   }
   //   */

   //   ::pointer<::geometry2d::polygon_item>pitem = m_pitem;

   //   //pdraw2dpath->begin_figure();
   //   //pdraw2dpath->add_polygon(pitem->m_polygon.data(), pitem->m_polygon.size());
   //   //pdraw2dpath->close_figure();

   //   //pdraw2dpath->get_os_data(pdraw2dgraphics, path_filled);

   //   //m_pgeometry = ::transfer((ID2D1PathGeometry *) pdraw2dpath->detach());

   //   auto ppathgeometry = ::direct2d::geometry::create_polygon(direct2d(), pitem->m_polygon);

   //   return ppathgeometry;

   //}


   //ID2D1Geometry * region::get_poly_polygon(::draw2d::graphics * pdraw2dgraphics)
   //{

   //   auto pdraw2dpath = pdraw2dgraphics->createø < ::draw2d::path > ();

   //   f64_point_array pa;

   //   /*if(m_efillmode == ::draw2d::e_fill_mode_alternate)
   //   {
   //      path.SetFillMode(Gdiplus::FillModeAlternate);
   //   }
   //   else
   //   {
   //      path.SetFillMode(Gdiplus::FillModeWinding);
   //   }*/

   //   ::i32 n = 0;

   //   ::pointer<::geometry2d::poly_polygon_item>pitem = m_pitem;

   //   for(::i32 i = 0; i < pitem->m_polypolygon.get_size(); i++)
   //   {
   //      auto ppolygon = pitem->m_polypolygon[i];
   //      auto jCount = ppolygon->get_size();
   //      //pa.erase_all();
   //      //for(::i32 j = 0; j < jCount; j++)
   //      //{
   //      //   pa.add(::f64_point(m_lppoints[n].x, m_lppoints[n].y));
   //      //   n++;
   //      //}
   //      //pdraw2dpath->begin_figure(true, m_efillmode);
   //      pdraw2dpath->begin_figure();
   //      pdraw2dpath->add_polygon(ppolygon->data(), (::i32) ppolygon->size());
   //      //pdraw2dpath->end_figure(true);
   //      pdraw2dpath->close_figure();
   //   }

   //   return (ID2D1PathGeometry *) pdraw2dpath->detach();

   //}


   //ID2D1Geometry * region::get_combine(::draw2d::graphics * pdraw2dgraphics)
   //{

   //   comptr < ID2D1PathGeometry > ppathgeometry ;

   //   HRESULT hr = direct2d()->d2d1_factory1()->CreatePathGeometry(&ppathgeometry);

   //   comptr < ID2D1GeometrySink > psink ;

   //   if (FAILED(hr))
   //   {

   //      return nullptr;

   //   }

   //   hr = ppathgeometry->Open(&psink);

   //   if(FAILED(hr))
   //   {

   //      return nullptr;

   //   }

   //   ::pointer < ::geometry2d::combine_item > pitem = m_pitem;

   //   ::pointer < graphics > pdirect2dgraphics = pdraw2dgraphics;

   //   auto pgeometry1 = pdirect2dgraphics->defer_update_os_data(pitem->m_pregion1);

   //   auto pgeometry2 = pdirect2dgraphics->defer_update_os_data(pitem->m_pregion2);

   //   //auto pgeometry1 = pitem->m_pregion1->get_os_data < ID2D1Geometry * >(pdraw2dgraphics);

   //   //auto pgeometry2 = pitem->m_pregion2->get_os_data < ID2D1Geometry * >(pdraw2dgraphics);

   //   if(pitem->m_ecombine == ::draw2d::e_combine_add)
   //   {

   //      hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_UNION, nullptr, 0.f, psink);

   //   }
   //   else if(pitem->m_ecombine == ::draw2d::e_combine_exclude)
   //   {

   //      hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_EXCLUDE, nullptr, 0.f, psink);

   //   }
   //   else if(pitem->m_ecombine == ::draw2d::e_combine_intersect)
   //   {

   //      hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_INTERSECT, nullptr, 0.f, psink);

   //   }
   //   else
   //   {

   //      hr = pgeometry1->CombineWithGeometry(pgeometry2, D2D1_COMBINE_MODE_UNION, nullptr, 0.f, psink);

   //   }

   //   if(FAILED(hr))
   //   {

   //      return nullptr;

   //   }

   //   hr = psink->Close();

   //   if(FAILED(hr))
   //   {

   //      return nullptr;

   //   }

   //   return ppathgeometry.detach();

   //}


   ////void region::destroy()
   ////{

   ////   destroy_os_data();

   ////}


   void region::destroy()
   {

      ::draw2d_direct2d::region::destroy();

      //m_pgeometry = nullptr;

      //object::destroy();

   }


} // namespace draw2d_direct2d_for_directx11



