#include "framework.h"
#include "path.h"
#include "graphics.h"
#include "CustomRenderer.h"
#include "acme/parallelization/synchronous_lock.h"
#include "aura/graphics/write_text/font.h"
#include "aura/graphics/write_text/text_out.h"
#include "direct2d/direct2d.h"


#include <math.h>


namespace draw2d_direct2d
{


   path::path()
   {

      m_pthis = this;
      m_bUseGeometryRealization = true;

   }


   path::~path()
   {

      destroy();

   }


   bool path::internal_end_figure(bool bClose)
   {

      if (m_psink == nullptr)
      {

         return false;

      }

      if(bClose)
      {

         m_psink->EndFigure(D2D1_FIGURE_END_CLOSED);

      }
      else
      {

         m_psink->EndFigure(D2D1_FIGURE_END_OPEN);

      }

      m_bHasPoint = false;

      return true;

   }


   bool path::internal_add_arc(::draw2d::graphics* pgraphics, const ::arc & arc)
   {

      ::point_f64 point;

      D2D1_ARC_SEGMENT arcseg;

      internal_get_arc(point, arcseg, arc);

      if (!m_bHasPoint)
      {

         if (!internal_start_figure(pgraphics, point.x, point.y))
         {

            return false;

         }

      }
      else
      {

         auto dDistance = point.distance(m_pointEnd);

         if (dDistance > 0.001)
         {

            m_psink->AddLine({(FLOAT) point.x, (FLOAT) point.y});

         }


      }

      m_psink->AddArc(arcseg);

      m_pointEnd.x = arcseg.point.x;

      m_pointEnd.y = arcseg.point.y;

      m_estatus = ::success;

      return true;

   }


   bool path::internal_add_rectangle(::draw2d::graphics* pgraphics, double x, double y, double cx, double cy)
   {

      internal_start_figure(pgraphics, x, y);

      internal_add_line(pgraphics, x + cx,y);
      internal_add_line(pgraphics, x + cx,y + cy);
      internal_add_line(pgraphics, x,y + cy);
      
      internal_end_figure(true);

      return true;

   }


   bool path::internal_add_ellipse(::draw2d::graphics * pgraphics, double x, double y, double cx, double cy)
   {

      internal_start_figure(pgraphics, x + cx, y + cy / 2.0);

      ::arc arc{};

      arc.m_pointCenter.x = x + cx / 2.0;
      arc.m_pointCenter.y = y + cy / 2.0;
      arc.m_sizeRadius.cx = cx / 2.0;
      arc.m_sizeRadius.cy = cy / 2.0;
      arc.m_pointBegin.x = x + cx;
      arc.m_pointBegin.y = y + cy / 2.0;
      arc.m_pointEnd.x = x;
      arc.m_pointEnd.y = y + cy / 2.0;
      arc.m_angleBeg = 0.0;
      arc.m_angleEnd2 = MATH_PI;
      arc.m_angleExt = MATH_PI;

      internal_add_arc(pgraphics, arc);

      arc.m_pointCenter.x = x + cx / 2.0;
      arc.m_pointCenter.y = y + cy / 2.0;
      arc.m_sizeRadius.cx = cx / 2.0;
      arc.m_sizeRadius.cy = cy / 2.0;
      arc.m_pointBegin.x = x;
      arc.m_pointBegin.y = y + cy / 2.0;
      arc.m_pointEnd.x = x + cx;
      arc.m_pointEnd.y = y + cy / 2.0;
      arc.m_angleBeg = MATH_PI;
      arc.m_angleEnd2 = MATH_PI * 2.0;
      arc.m_angleExt = MATH_PI;

      internal_add_arc(pgraphics, arc);

      internal_end_figure(true);

      return true;

   }


   bool path::internal_add_lines(::draw2d::graphics* pgraphics, const ::point_i32_array& pointa, bool bClose)
   {

      if (pointa.get_count() < 1)
      {

         return false;

      }

      internal_start_figure(pgraphics, pointa[0].x, pointa[0].y);

      for (index i = 1; i < pointa.get_count(); i++)
      {

         internal_add_line(pgraphics, pointa[i].x, pointa[i].y);

      }

      internal_end_figure(bClose);

      return true;

   }


   bool path::internal_add_lines(::draw2d::graphics* pgraphics, const ::point_f64_array& pointa, bool bClose)
   {

      if (pointa.get_count() < 1)
      {

         return false;

      }

      internal_start_figure(pgraphics, pointa[0].x, pointa[0].y);

      for (index i = 1; i < pointa.get_count(); i++)
      {

         internal_add_line(pgraphics, pointa[i].x, pointa[i].y);

      }

      internal_end_figure(bClose);

      return true;

   }


   bool path::internal_add_string(::draw2d_direct2d::graphics * pgraphics, double x, double y, const ::string & strText, ::write_text::font * pfont)
   {

      if(!internal_start_figure(pgraphics))
      {

         return false;

      }

      IDWriteTextFormat * pformat = pfont->get_os_data < IDWriteTextFormat * >(pgraphics);

      IDWriteFactory * pfactory = ::direct2d::direct2d()->dwrite_factory();

      comptr<IDWriteTextLayout> playout;

      wstring wstr(strText);

      HRESULT hr = pfactory->CreateTextLayout(
         wstr,      // The string to be laid out and formatted.
         (::u32)wstr.length(),  // The length of the string.
         pformat,  // The text format to apply to the string (contains font information, etc).
         4096,         // The width of the on_layout box.
         4096,        // The height of the on_layout box.
         &playout  // The IDWriteTextLayout interface pointer.
      );

      if (playout == nullptr)
      {

         return false;

      }

      synchronous_lock lock(::direct2d::direct2d()->synchronization());

      auto & renderer = ::direct2d::direct2d()->m_geometrysinktextrenderer;

      renderer.m_pgeometrysink = m_psink;

      FLOAT dpix = 0.f;
      FLOAT dpiy = 0.f;

      pgraphics->m_prendertarget->GetDpi(&dpix, &dpiy);

      if (dpix <= 0)
      {

         renderer.m_dDpi = 96.0;

      }
      else
      {

         renderer.m_dDpi = dpix;

      }

      renderer.m_figurebeginOverride = pgraphics->m_bOutline ? D2D1_FIGURE_BEGIN_HOLLOW : D2D1_FIGURE_BEGIN_FILLED;

      playout->Draw(nullptr, &renderer, (FLOAT)x, (FLOAT)y);

      m_estatus = ::success;

      return true;

   }


   bool path::internal_add_line(::draw2d::graphics* pgraphics, double x, double y)
   {

      if (::is_null(m_psink))
      {

         return false;

      }

      m_psink->AddLine({ (FLOAT) x, (FLOAT)y });

      m_pointEnd.x = x;

      m_pointEnd.y = y;

      m_estatus = ::success;

      return true;

   }


   bool path::internal_start_figure(::draw2d::graphics* pgraphics)
   {

      if (m_bHasPoint)
      {

         internal_end_figure(false);

      }

      if (m_psink == nullptr)
      {

         m_ppath->Open(&m_psink);

         if (m_efillmode == ::draw2d::e_fill_mode_winding)
         {

            m_psink->SetFillMode(D2D1_FILL_MODE_WINDING);

         }
         else if (m_efillmode == ::draw2d::e_fill_mode_alternate)
         {

            m_psink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);

         }

      }

      return true;

   }


   bool path::internal_start_figure(::draw2d::graphics * pgraphics, double x, double y)
   {

      if (!internal_start_figure(pgraphics))
      {

         return false;

      }

      m_psink->BeginFigure({ (FLOAT)x, (FLOAT)y }, pgraphics->m_bOutline ? D2D1_FIGURE_BEGIN_HOLLOW : D2D1_FIGURE_BEGIN_FILLED);

      m_pointBegin.x = x;
      m_pointBegin.y = y;
      m_pointEnd.x = x;
      m_pointEnd.y = y;

      m_bHasPoint = true;

      return true;

   }


   void * path::detach()
   {

      return m_ppath.detach();

   }



   bool path::internal_get_arc(::point_f64 & pointStart,D2D1_ARC_SEGMENT & arcseg, const ::arc & arc)
   {

      D2D1_POINT_2F pointCenter;

      pointCenter.x = (FLOAT)arc.m_pointCenter.x;
      pointCenter.y = (FLOAT)arc.m_pointCenter.y;

      double rx = arc.m_sizeRadius.cx;
      double ry = arc.m_sizeRadius.cy;

      pointStart.x = arc.m_pointBegin.x;
      pointStart.y = arc.m_pointBegin.y;

      arcseg.point.x = (FLOAT)arc.m_pointEnd.x;
      arcseg.point.y = (FLOAT)arc.m_pointEnd.y;

      if(arc.m_angleEnd2 > arc.m_angleBeg)
      {

         arcseg.sweepDirection = D2D1_SWEEP_DIRECTION_CLOCKWISE;

      }
      else
      {

         arcseg.sweepDirection = D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE;

      }

      if(fabs(arc.m_angleEnd2 - arc.m_angleBeg) < MATH_PI)
      {

         arcseg.arcSize = D2D1_ARC_SIZE_SMALL;

      }
      else
      {

         arcseg.arcSize = D2D1_ARC_SIZE_LARGE;

      }

      arcseg.rotationAngle = (FLOAT) arc.m_angleRotation;

      arcseg.size.width    = (FLOAT) rx;

      arcseg.size.height   = (FLOAT) ry;

      return true;

   }


   void path::create(::draw2d::graphics* pgraphicsParam, ::i8 iCreate)
   {

      auto pgraphics = __graphics(pgraphicsParam);

      HRESULT hr = S_OK;

      if (pgraphics->m_bOutline)
      {

         if (iCreate == path_hollow)
         {

            if (m_ppathHollow)
            {

               m_osdata[path_hollow] = m_ppathHollow;

               return;

            }

         }

         hr = ::direct2d::direct2d()->d2d1_factory1()->CreatePathGeometry(&m_ppathHollow);

         m_ppath = m_ppathHollow;

      }
      else
      {

         if (m_ppathFilled)
         {

            m_osdata[path_filled] = m_ppathFilled;

            return;

         }

         hr = ::direct2d::direct2d()->d2d1_factory1()->CreatePathGeometry(&m_ppathFilled);
         
         m_ppath = m_ppathFilled;

      }

      ::defer_throw_hresult(hr);

      m_psink  = nullptr;

      m_estatus = error_failed;

      m_bHasPoint = false;

      _set_create(pgraphics);

      /*for(i32 i = 0; i < m_elementa.get_count(); i++)
      {

         if (m_elementa.ptr_at(i))
         {

            set(pgraphics, m_elementa(i));

         }

      }*/

      if (!m_estatus)
      {

         m_ppath = nullptr;

      }
      else
      {

         if (m_bHasPoint)
         {

            internal_end_figure(false);

         }

         if (m_psink != nullptr)
         {

            HRESULT hr = m_psink->Close();

            m_osdata[iCreate] = m_ppath;

         }
         else
         {

            m_ppath = nullptr;

         }

      }

      m_psink = nullptr;

      //return m_ppath != nullptr;

   }


   ID2D1GeometryRealization * path::_get_stroked_geometry_realization(::draw2d::graphics * pgraphicsParam, int iWidth)
   {

      auto & prealization = m_mapGeometryHollowRealization[iWidth];

      if (!prealization)
      {

         auto pgraphics = __graphics(pgraphicsParam);

         HRESULT hr = pgraphics->m_pdevicecontext1->CreateStrokedGeometryRealization(
            m_ppathHollow,
            1.0f,
            (FLOAT)iWidth,
            nullptr,
            &prealization);

         defer_throw_hresult(hr);

      }

      return prealization;

   }


   ID2D1GeometryRealization * path::_get_filled_geometry_realization(::draw2d::graphics * pgraphicsParam)
   {

      auto & prealization = m_geometryFilledRealization;

      if (!prealization)
      {

         auto pgraphics = __graphics(pgraphicsParam);

         HRESULT hr = pgraphics->m_pdevicecontext1->CreateFilledGeometryRealization(
            m_ppathFilled,
            1.0f,
            &prealization);

         defer_throw_hresult(hr);

      }

      return prealization;

   }



   void * path::detach(::draw2d::graphics* pgraphicsParam)
   {

      defer_update(pgraphicsParam, 0);

      return m_ppath.detach();

   }


   void path::destroy()
   {

      destroy_os_data();

      ::draw2d::path::destroy();

   }


   void path::destroy_os_data()
   {

      m_psink = nullptr;

      m_ppath = nullptr;

      m_ppathHollow = nullptr;

      m_ppathFilled = nullptr;

      object::destroy_os_data();

   }



   bool path::create()
   {

      return true;

   }


   //bool path::set(::draw2d_direct2d::graphics * pgraphics, const ::draw2d::path::matter & e)
   //{

   //   switch(e.m_etype)
   //   {
   //   case ::draw2d::path::matter::type_move:
   //      set(e.u.m_move);
   //      break;
   //   case ::draw2d::path::matter::type_arc:
   //      set(e.u.m_arc);
   //      break;
   //   case ::draw2d::path::matter::type_line:
   //      set(e.u.m_line);
   //      break;
   //   case ::draw2d::path::matter::type_rect:
   //      set(e.u.m_rectangle);
   //      break;
   //   case ::draw2d::path::matter::e_type_string:
   //      set(pgraphics,e.m_stringpath);
   //      break;
   //   case ::draw2d::path::matter::type_end:
   //      internal_end_figure(e.u.m_end.m_bClose);
   //      break;
   //   default:
   //      throw ::exception(::exception("unexpected simple os graphics matter type"));
   //   }

   //   return false;

   //}


   bool path::_set(::draw2d::graphics * pgraphics, const ::arc & arc)
   {

      //::rectangle_f64 rectangle;

      //rectangle.left      = (::i32) (arc.m_pointCenter.x - arc.m_sizeRadius.cx);
      //rectangle.right     = (::i32) (arc.m_pointCenter.x + arc.m_sizeRadius.cx);
      //rectangle.top       = (::i32) (arc.m_pointCenter.y - arc.m_sizeRadius.cy);
      //rectangle.bottom    = (::i32) (arc.m_pointCenter.y + arc.m_sizeRadius.cy);

      //bool bOk = internal_add_arc(pgraphics, parc);

      //return bOk;

      return internal_add_arc(pgraphics, arc);

   }


   bool path::_set(::draw2d::graphics* pgraphics, const enum_shape& eshape)
   {

      if (eshape == e_shape_begin_figure)
      {

         if (m_bHasPoint)
         {

            internal_end_figure(false);

         }

         return true;

      }
      else if (eshape == e_shape_close_figure)
      {

         if (m_bHasPoint)
         {

            internal_end_figure(true);

         }

         return true;

      }
      else if (eshape == e_shape_end_figure)
      {

         if (m_bHasPoint)
         {

            internal_end_figure(false);

         }

         return true;

      }
      else
      {

         return ::draw2d::path::_set(pgraphics, eshape);

      }


      return true;

   }


   //bool path::_set(const ::draw2d::path::move & move)
   //{

   //   return internal_add_move((int) move.m_x, (int) move.m_y);

   //}


   bool path::_set(::draw2d::graphics* pgraphics, const ::line & line)
   {

      if (line.m_p1 != m_pointEnd || ::is_null(m_psink))
      {

         if (!internal_start_figure(pgraphics, line.m_p1.x, line.m_p1.y))
         {

            return false;

         }

      }

      return internal_add_line(pgraphics, line.m_p2.x, line.m_p2.y);

   }


   //bool path::_set(::draw2d::graphics* pgraphics, const ::line & line)
   //{

   //   if (line.m_p1 != m_pointEnd || !m_bFigureOpened)
   //   {

   //      if (!internal_start_figure(pgraphics, line.m_p1.x, line.m_p1.y))
   //      {

   //         return false;

   //      }

   //      m_pointEnd = line.m_p1;

   //   }

   //   return internal_add_line(pgraphics, line.m_p2.x, line.m_p2.y);

   //}


   bool path::_set(::draw2d::graphics* pgraphics, const ::rectangle & rectangle)
   {

      return internal_add_rectangle(pgraphics, rectangle.left, rectangle.top, rectangle.width(), rectangle.height());

   }


   //bool path::_set(::draw2d::graphics* pgraphics, const ::rectangle_f64& rectangle)
   //{

   //   return internal_add_rect(pgraphics, rectangle.left, rectangle.top, rectangle.width(), rectangle.height());

   //}


   bool path::_set(::draw2d::graphics * pgraphics, const ::ellipse & ellipse)
   {

      return internal_add_ellipse(pgraphics, ellipse.left, ellipse.top, ellipse.width(), ellipse.height());

   }



   bool path::_set(::draw2d::graphics* pgraphics, const ::lines & lines)
   {

      return internal_add_lines(pgraphics, (const ::point_i32_array &) lines, false);

   }


   //bool path::_set(::draw2d::graphics* pgraphics, const ::linesd& lines)
   //{

   //   return internal_add_lines(pgraphics, (const ::point_f64_array&) lines, false);

   //}


   bool path::_set(::draw2d::graphics* pgraphics, const ::polygon & polygon)
   {

      return internal_add_lines(pgraphics, (const ::point_f64_array&)polygon, true);

   }

   //bool path::_set(::draw2d::graphics* pgraphics, const ::polygon_f64& polygon_i32)
   //{

   //   return internal_add_lines(pgraphics, (const ::point_f64_array&)polygon_i32, true);

   //}


   bool path::_set(::draw2d::graphics * pgraphics, const ::write_text::text_out & textout)
   {

      ::pointer < ::draw2d_direct2d::graphics > pgraphicsDraw2dDirect2d = pgraphics;

      auto bOk = internal_add_string(
         pgraphicsDraw2dDirect2d,
         textout.m_point.x,
         textout.m_point.y,
         textout.m_strText,
         textout.m_pfont);

      if (!bOk)
      {

         return false;

      }

      return true;

      //IDWriteTextFormat * pformat = textout.m_pfont->get_os_data < IDWriteTextFormat * >(pgraphicsParam);

      //IDWriteFactory * pfactory = ::direct2d::direct2d()->dwrite_factory();

      //comptr<IDWriteTextLayout> playout;

      //wstring wstr(textout.m_strText);

      //HRESULT hr = pfactory->CreateTextLayout(
      //   wstr,      // The string to be laid out and formatted.
      //   (::u32)wstr.length(),  // The length of the string.
      //   pformat,  // The text format to apply to the string (contains font information, etc).
      //   4096,         // The width of the on_layout box.
      //   4096,        // The height of the on_layout box.
      //   &playout  // The IDWriteTextLayout interface pointer.
      //);

      //if (playout == nullptr)
      //{

      //   return false;

      //}

      //auto pgraphics = pgraphicsParam->cast < ::draw2d_direct2d::graphics>();

      ////if (m_psink == nullptr)
      ////{

      ////   hr = m_ppath->Open(&m_psink);

      ////}

      ////CustomTextRenderer renderer(::direct2d::direct2d()->d2d1_factory1(), m_psink);

      ////defer_text_primitive_blend();

      ////defer_text_rendering_hint();

      //synchronous_lock lock(::direct2d::direct2d()->synchronization());

      //auto & renderer = ::direct2d::direct2d()->m_geometrysinktextrenderer;

      //renderer.m_pgeometrysink = m_psink;

      //FLOAT dpix, dpiy;

      //if (SUCCEEDED(pgraphics->m_prendertarget->GetDpi(dpix, dpiy))
      //{

      //   renderer.m_dDpi = dpix;

      //}
      //else
      //{

      //   renderer.m_dDpi = 96.0;

      //}

      //renderer.m_figurebeginOverride = pgraphics->m_bOutline ? D2D1_FIGURE_BEGIN_HOLLOW : D2D1_FIGURE_BEGIN_FILLED;

      //playout->Draw(nullptr, &renderer, (FLOAT)textout.m_point.x, (FLOAT)textout.m_point.y);

      //return true;

   }


   bool path::_set(::draw2d::graphics* pgraphics, const ::write_text::draw_text & drawtext)
   {

      return true;

   }


   //bool path::_set(::draw2d::graphics* pgraphics, ::draw2d::path::close* pclose)
   //{

   //   //::rectangle_f64 rectangle;

   //   //rectangle.left      = (::i32) (arc.m_pointCenter.x - arc.m_sizeRadius.cx);
   //   //rectangle.right     = (::i32) (arc.m_pointCenter.x + arc.m_sizeRadius.cx);
   //   //rectangle.top       = (::i32) (arc.m_pointCenter.y - arc.m_sizeRadius.cy);
   //   //rectangle.bottom    = (::i32) (arc.m_pointCenter.y + arc.m_sizeRadius.cy);

   //   //bool bOk = internal_add_arc(pgraphics, parc);

   //   //return bOk;

   //   if (m_bFigureOpened)
   //   {

   //      internal_end_figure(true);

   //   }

   //   return true;

   //}


} // namespace draw2d_direct2d



#define d2d1_fax_options D2D1_FACTORY_OPTIONS // fax of merde
#define multi_threaded D2D1_FACTORY_TYPE_MULTI_THREADED // ???? muliple performance multi thread hidden option there exists cost uses?


//class PathTextRenderer: public IDWriteTextRenderer
//{
//public:
//
//   PathTextRenderer(
//   FLOAT pixelsPerDip
//   );
//
//   STDMETHOD(DrawGlyphRun)(
//   _In_opt_ void* clientDrawingContext,
//   FLOAT baselineOriginX,
//   FLOAT baselineOriginY,
//   DWRITE_MEASURING_MODE measuringMode,
//   _In_ DWRITE_GLYPH_RUN const* glyphRun,
//   _In_ DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
//   _In_opt_ IUnknown* clientDrawingEffect
//   ) override;
//
//   STDMETHOD(DrawUnderline)(
//   _In_opt_ void* clientDrawingContext,
//   FLOAT baselineOriginX,
//   FLOAT baselineOriginY,
//   _In_ DWRITE_UNDERLINE const* underline,
//   _In_opt_ IUnknown* clientDrawingEffect
//   ) override;
//
//   STDMETHOD(DrawStrikethrough)(
//   _In_opt_ void* clientDrawingContext,
//   FLOAT baselineOriginX,
//   FLOAT baselineOriginY,
//   _In_ DWRITE_STRIKETHROUGH const* strikethrough,
//   _In_opt_ IUnknown* clientDrawingEffect
//   ) override;
//
//   STDMETHOD(DrawInlineObject)(
//   _In_opt_ void* clientDrawingContext,
//   FLOAT originX,
//   FLOAT originY,
//   IDWriteInlineObject* inlineObject,
//   BOOL isSideways,
//   BOOL isRightToLeft,
//   _In_opt_ IUnknown* clientDrawingEffect
//   ) override;
//
//   STDMETHOD(IsPixelSnappingDisabled)(
//   _In_opt_ void* clientDrawingContext,
//   _Out_ BOOL* isDisabled
//   ) override;
//
//   STDMETHOD(GetCurrentTransform)(
//   _In_opt_ void* clientDrawingContext,
//   _Out_ DWRITE_MATRIX* transform
//   ) override;
//
//   STDMETHOD(GetPixelsPerDip)(
//   _In_opt_ void* clientDrawingContext,
//   _Out_ FLOAT* pixelsPerDip
//   ) override;
//
//   STDMETHOD(QueryInterface)(
//   REFIID riid,
//   _Outptr_ void** object
//   ) override;
//
//   STDMETHOD_(ULONG,AddRef)() override;
//
//   STDMETHOD_(ULONG,Release)() override;
//
//private:
//   DWORD cRefCount_;
//   FLOAT m_pixelsPerDip;   // Number of pixels per DIP.
//   ::u32 m_ref;
//};
//
//

//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

//using namespace Microsoft::WRL;

//// An identity matrix for use by IDWritePixelSnapping::GetCurrentTransform.
//const DWRITE_MATRIX identityTransform =
//{
//   1,0,
//   0,1,
//   0,0
//};
//
//
//PathTextRenderer::PathTextRenderer(FLOAT pixelsPerDip):
//   m_pixelsPerDip(pixelsPerDip),
//   m_ref(0)
//{
//}
//
////
//// Draws a given glyph run along the geometry specified
//// in the given clientDrawingEffect.
////
//// This method calculates the horizontal displacement
//// of each glyph cluster in the run, then calculates the
//// tangent vector of the geometry at each of those distances.
//// It then renders the glyph cluster using the offset and angle
//// defined by that tangent, thereby placing each cluster on
//// the path and also rotated to the path.
////
//HRESULT PathTextRenderer::DrawGlyphRun(
//_In_opt_ void* clientDrawingContext,
//FLOAT baselineOriginX,
//FLOAT baselineOriginY,
//DWRITE_MEASURING_MODE measuringMode,
//_In_ DWRITE_GLYPH_RUN const* glyphRun,
//_In_ DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
//_In_opt_ IUnknown* clientDrawingEffect
//)
//{
//   if(clientDrawingContext == nullptr)
//   {
//      return S_OK;
//   }
//
//   ::draw2d_direct2d::path * dc = static_cast<::draw2d_direct2d::path*>(clientDrawingContext);
//   HRESULT hr = glyphRun->pfontFace->GetGlyphRunOutline(
//                glyphRun->pfontEmSize,
//                glyphRun->glyphIndices,
//                glyphRun->glyphAdvances,
//                glyphRun->glyphOffsets,
//                glyphRun->glyphCount,
//                glyphRun->isSideways,
//                glyphRun->bidiLevel % 2,
//                dc->m_psink
//                );
//
//   return hr;
//}
//
//HRESULT PathTextRenderer::DrawUnderline(
//_In_opt_ void* clientDrawingContext,
//FLOAT baselineOriginX,
//FLOAT baselineOriginY,
//_In_ DWRITE_UNDERLINE const* underline,
//_In_opt_ IUnknown* clientDrawingEffect
//)
//{
//   // We don't use underline in this application.
//   return E_NOTIMPL;
//}
//
//HRESULT PathTextRenderer::DrawStrikethrough(
//_In_opt_ void* clientDrawingContext,
//FLOAT baselineOriginX,
//FLOAT baselineOriginY,
//_In_ DWRITE_STRIKETHROUGH const* strikethrough,
//_In_opt_ IUnknown* clientDrawingEffect
//)
//{
//   // We don't use strikethrough in this application.
//   return E_NOTIMPL;
//}
//
//HRESULT PathTextRenderer::DrawInlineObject(
//_In_opt_ void* clientDrawingContext,
//FLOAT originX,
//FLOAT originY,
//IDWriteInlineObject* inlineObject,
//BOOL isSideways,
//BOOL isRightToLeft,
//_In_opt_ IUnknown* clientDrawingEffect
//)
//{
//   // We don't use inline objects in this application.
//   return E_NOTIMPL;
//}
//
////
//// IDWritePixelSnapping methods
////
//HRESULT PathTextRenderer::IsPixelSnappingDisabled(
//_In_opt_ void* clientDrawingContext,
//_Out_ BOOL* isDisabled
//)
//{
//   *isDisabled = false;
//   return S_OK;
//}
//
//HRESULT PathTextRenderer::GetCurrentTransform(
//_In_opt_ void* clientDrawingContext,
//_Out_ DWRITE_MATRIX* transform
//)
//{
//   *transform = identityTransform;
//   return S_OK;
//}
//
//HRESULT PathTextRenderer::GetPixelsPerDip(
//_In_opt_ void* clientDrawingContext,
//_Out_ FLOAT* pixelsPerDip
//)
//{
//   *pixelsPerDip = m_pixelsPerDip;
//   return S_OK;
//}
//
////
//// IUnknown methods
////
//// These use a basic, non-thread-safe implementation of the
//// standard object-counting logic.
////
//HRESULT PathTextRenderer::QueryInterface(
//REFIID riid,
//_Outptr_ void** object
//)
//{
//   *object = nullptr;
//   return E_NOTIMPL;
//}
//
//ULONG PathTextRenderer::AddRef()
//{
//   return InterlockedIncrement(&cRefCount_);
//}
//
//ULONG PathTextRenderer::Release()
//{
//   unsigned long newCount = InterlockedDecrement(&cRefCount_);
//   if(newCount == 0)
//   {
//      delete this;
//      return 0;
//   }
//
//   return newCount;
//}
//
//
//
//
//
//
//namespace draw2d_direct2d
//{
//   void path::CreatePathTextRenderer(FLOAT pixelsPerDip, IDWriteTextRenderer **textRenderer)
//   {
//      *textRenderer = nullptr;
//
//      PathTextRenderer *newRenderer = new PathTextRenderer(pixelsPerDip);
//      newRenderer->AddRef();
//      *textRenderer = newRenderer;
//      newRenderer = nullptr;
//   }
//
//} // namespace draw2d_direct2d
