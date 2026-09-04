#include "platform.h"
#include "font.h"
#include "path.h"
#include "graphics.h"
#include "CustomRenderer.h"
#include "acme/parallelization/synchronous_lock.h"
#include "acme/prototype/geometry2d/arc.h"
#include "acme/prototype/geometry2d/line.h"
#include "aura/graphics/write_text/font.h"
#include "aura/graphics/write_text/text_out.h"
#include "direct2d/direct2d.h"


#include <math.h>


namespace draw2d_direct2d
{


   path::path()
   {

      m_bUseGeometryRealization = true;

   }


   path::~path()
   {

      destroy();

   }


   bool path::internal_end_figure(bool bClose)
   {

      if (m_pd2d1geometrysinkTime == nullptr)
      {

         return false;

      }

      if(bClose)
      {

         m_pd2d1geometrysinkTime->EndFigure(D2D1_FIGURE_END_CLOSED);

      }
      else
      {

         m_pd2d1geometrysinkTime->EndFigure(D2D1_FIGURE_END_OPEN);

      }

      m_bHasPoint = false;

      return true;

   }


   bool path::internal_add_arc(::draw2d::graphics * pdraw2dgraphics, const ::f64_arc & arc)
   {

      ::f64_point point;

      D2D1_ARC_SEGMENT arcseg;

      internal_get_arc(point, arcseg, arc);

      if (!m_bHasPoint)
      {

         if (!internal_start_figure(pdraw2dgraphics, point.x, point.y))
         {

            return false;

         }

      }
      else
      {

         auto dDistance = point.distance(m_pointEnd);

         if (dDistance > 0.001)
         {

            m_pd2d1geometrysinkTime->AddLine({(FLOAT) point.x, (FLOAT) point.y});

         }

      }

      m_pd2d1geometrysinkTime->AddArc(arcseg);

      m_pointEnd.x = arcseg.point.x;

      m_pointEnd.y = arcseg.point.y;

      m_estatus = ::success;

      return true;

   }


   bool path::internal_add_rectangle(::draw2d::graphics * pdraw2dgraphics, ::f64 x, ::f64 y, ::f64 cx, ::f64 cy)
   {

      internal_start_figure(pdraw2dgraphics, x, y);

      internal_add_line(pdraw2dgraphics, x + cx,y);
      internal_add_line(pdraw2dgraphics, x + cx,y + cy);
      internal_add_line(pdraw2dgraphics, x,y + cy);
      
      internal_end_figure(true);

      return true;

   }


   bool path::internal_add_ellipse(::draw2d::graphics * pdraw2dgraphics, ::f64 x, ::f64 y, ::f64 cx, ::f64 cy)
   {

      const auto xCenter = x + cx / 2.0;
      const auto yCenter = y + cy / 2.0;

      const auto xRight = x + cx;
      const auto xLeft = x;

      // Start at the rightmost point.
      if (!internal_start_figure(
         pdraw2dgraphics,
         xRight,
         yCenter))
      {

         return false;

      }

      // Right -> Left
      {
         ::f64_arc arc{};

         arc.left = x;
         arc.top = y;
         arc.right = x + cx;
         arc.bottom = y + cy;

         arc.m_pointBegin.x = xRight;
         arc.m_pointBegin.y = yCenter;

         arc.m_pointEnd.x = xLeft;
         arc.m_pointEnd.y = yCenter;

         arc.m_angleBeg = 0_degree;
         arc.m_angleEnd2 = 180_degree;
         arc.m_angleExt = 180_degree;

         if (!internal_add_arc(pdraw2dgraphics, arc))
         {

            return false;

         }

      }

      // Left -> Right
      {
         ::f64_arc arc{};

         arc.left = x;
         arc.top = y;
         arc.right = x + cx;
         arc.bottom = y + cy;

         arc.m_pointBegin.x = xLeft;
         arc.m_pointBegin.y = yCenter;

         arc.m_pointEnd.x = xRight;
         arc.m_pointEnd.y = yCenter;

         arc.m_angleBeg = 180_degree;
         arc.m_angleEnd2 = 360_degree;
         arc.m_angleExt = 180_degree;

         if (!internal_add_arc(pdraw2dgraphics, arc))
         {

            return false;

         }

      }

      internal_end_figure(true);

   }


   bool path::internal_add_lines(::draw2d::graphics * pdraw2dgraphics, const ::i32_point_array& pointa, bool bClose)
   {

      if (pointa.get_count() < 1)
      {

         return false;

      }

      internal_start_figure(pdraw2dgraphics, pointa[0].x, pointa[0].y);

      for (::collection::index i = 1; i < pointa.get_count(); i++)
      {

         internal_add_line(pdraw2dgraphics, pointa[i].x, pointa[i].y);

      }

      internal_end_figure(bClose);

      return true;

   }


   bool path::internal_add_lines(::draw2d::graphics * pdraw2dgraphics, const ::f64_point_array& pointa, bool bClose)
   {

      if (pointa.get_count() < 1)
      {

         return false;

      }

      internal_start_figure(pdraw2dgraphics, pointa[0].x, pointa[0].y);

      for (::collection::index i = 1; i < pointa.get_count(); i++)
      {

         internal_add_line(pdraw2dgraphics, pointa[i].x, pointa[i].y);

      }

      internal_end_figure(bClose);

      return true;

   }


   bool path::internal_add_string(::draw2d_direct2d::graphics * pdraw2dgraphics, ::f64 x, ::f64 y, const ::scoped_string & scopedstrText, ::write_text::font * pwritetextfont)
   {

      if(!internal_start_figure(pdraw2dgraphics))
      {

         return false;

      }

      ::cast < ::draw2d_direct2d::font > pdraw2ddirect2dfont = pwritetextfont;

      IDWriteTextFormat * pformat = pdraw2ddirect2dfont->m_pdwritetextformat;

      IDWriteFactory * pfactory = direct2d()->dwrite_factory();

      comptr<IDWriteTextLayout> playout;

      wstring wstr(scopedstrText);

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

      ::direct2d_lock lock(direct2d());

      auto & renderer = direct2d()->m_geometrysinktextrenderer;

      renderer.m_pgeometrysink = m_pd2d1geometrysinkTime;

      FLOAT dpix = 0.f;
      FLOAT dpiy = 0.f;

      pdraw2dgraphics->m_pd2d1devicecontext->GetDpi(&dpix, &dpiy);

      if (dpix <= 0)
      {

         renderer.m_dDpi = 96.0;

      }
      else
      {

         renderer.m_dDpi = dpix;

      }

      renderer.m_figurebeginOverride = pdraw2dgraphics->m_bOutline ? D2D1_FIGURE_BEGIN_HOLLOW : D2D1_FIGURE_BEGIN_FILLED;

      playout->Draw(nullptr, &renderer, (FLOAT)x, (FLOAT)y);

      return true;

   }


   bool path::internal_add_line(::draw2d::graphics * pdraw2dgraphics, ::f64 x, ::f64 y)
   {

      if (::is_null(m_pd2d1geometrysinkTime))
      {

         return false;

      }

      m_pd2d1geometrysinkTime->AddLine({ (FLOAT) x, (FLOAT)y });

      m_pointEnd.x = x;

      m_pointEnd.y = y;

      return true;

   }


   bool path::internal_start_figure(::draw2d::graphics * pdraw2dgraphics)
   {

      if (m_bHasPoint)
      {

         internal_end_figure(false);

      }

      if (m_pd2d1geometrysinkTime == nullptr)
      {

         m_pd2d1pathgeometryTime->Open(&m_pd2d1geometrysinkTime);

         if (m_efillmode == ::draw2d::e_fill_mode_winding)
         {

            m_pd2d1geometrysinkTime->SetFillMode(D2D1_FILL_MODE_WINDING);

         }
         else if (m_efillmode == ::draw2d::e_fill_mode_alternate)
         {

            m_pd2d1geometrysinkTime->SetFillMode(D2D1_FILL_MODE_ALTERNATE);

         }

      }

      return true;

   }


   bool path::internal_start_figure(::draw2d::graphics * pdraw2dgraphics, ::f64 x, ::f64 y)
   {

      if (!internal_start_figure(pdraw2dgraphics))
      {

         return false;

      }

      m_pd2d1geometrysinkTime->BeginFigure({ (FLOAT)x, (FLOAT)y }, pdraw2dgraphics->m_bOutline ? D2D1_FIGURE_BEGIN_HOLLOW : D2D1_FIGURE_BEGIN_FILLED);

      m_pointBegin.x = x;
      m_pointBegin.y = y;
      m_pointEnd.x = x;
      m_pointEnd.y = y;

      m_bHasPoint = true;

      return true;

   }


   bool path::internal_get_arc(::f64_point & pointStart,D2D1_ARC_SEGMENT & arcseg, const ::f64_arc & arc)
   {

      D2D1_POINT_2F pointCenter;

      pointCenter.x = (FLOAT)arc.center().x;
      pointCenter.y = (FLOAT)arc.center().y;

      ::f64 rx = arc.radius().cx;
      ::f64 ry = arc.radius().cy;

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

      if(fabs(arc.m_angleEnd2 - arc.m_angleBeg) <= 2_π)
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


   void path::update(::draw2d::graphics * pdraw2dgraphics)
   {

      //auto pdraw2dgraphics = __graphics(pdraw2dgraphics);
      //m_pd2d1pathgeometryHollow.release();
      //m_pd2d1pathgeometryFilled.release();
      clear_node_data();

   }


   ID2D1PathGeometry * path::_hollow_path_geometry(::draw2d::graphics * pdraw2dgraphics)
   {

      if (m_pd2d1pathgeometryHollow)
      {

         return m_pd2d1pathgeometryHollow;

      }

      auto hrCreatePathGeometry = direct2d()->d2d1_factory1()->CreatePathGeometry(&m_pd2d1pathgeometryHollow);

      ::defer_throw_hresult(hrCreatePathGeometry);

      m_pd2d1pathgeometryTime = m_pd2d1pathgeometryHollow;

      m_pd2d1geometrysinkTime  = nullptr;

      m_bHasPoint = false;

      pdraw2dgraphics->m_bOutline = true;

      if (!_set_create(pdraw2dgraphics))
      {

         m_pd2d1geometrysinkTime = nullptr;

         m_pd2d1pathgeometryTime = nullptr;

         m_pd2d1pathgeometryHollow = nullptr;

         throw ::exception(error_wrong_state);

      }

      if (!m_pd2d1geometrysinkTime)
      {

         throw ::exception(error_wrong_state);

      }

      if (m_bHasPoint)
      {

         internal_end_figure(false);

      }

      auto hrClose = m_pd2d1geometrysinkTime->Close();

      m_pd2d1pathgeometryTime = nullptr;

      m_pd2d1geometrysinkTime = nullptr;

      if (FAILED(hrClose))
      {

         m_pd2d1pathgeometryHollow = nullptr;

         ::defer_throw_hresult(hrClose);

      }

      return m_pd2d1pathgeometryHollow;

   }


   ID2D1PathGeometry * path::_filled_path_geometry(::draw2d::graphics * pdraw2dgraphics)
   {

      if (m_pd2d1pathgeometryFilled)
      {

         return m_pd2d1pathgeometryFilled;

      }

      auto hrCreatePathGeometry = direct2d()->d2d1_factory1()->CreatePathGeometry(&m_pd2d1pathgeometryFilled);

      ::defer_throw_hresult(hrCreatePathGeometry);
         
      m_pd2d1pathgeometryTime = m_pd2d1pathgeometryFilled;

      m_pd2d1geometrysinkTime = nullptr;

      m_bHasPoint = false;

      pdraw2dgraphics->m_bOutline = false;

      if (!_set_create(pdraw2dgraphics))
      {

         m_pd2d1geometrysinkTime = nullptr;

         m_pd2d1pathgeometryTime = nullptr;

         m_pd2d1pathgeometryFilled = nullptr;

         throw ::exception(error_wrong_state);

      }

      if (!m_pd2d1geometrysinkTime)
      {

         throw ::exception(error_wrong_state);

      }

      if (m_bHasPoint)
      {

         internal_end_figure(true);

      }

      auto hrClose = m_pd2d1geometrysinkTime->Close();

      m_pd2d1pathgeometryTime = nullptr;

      m_pd2d1geometrysinkTime = nullptr;

      if (FAILED(hrClose))
      {

         m_pd2d1pathgeometryFilled = nullptr;

         ::defer_throw_hresult(hrClose);

      }

      return m_pd2d1pathgeometryFilled;

   }


   ID2D1GeometryRealization * path::_get_stroked_geometry_realization(::draw2d::graphics * pdraw2dgraphics, ::i32 iWidth)
   {

      auto & prealization = m_mapD2D1HollowGeometryRealization[iWidth];

      if (!prealization)
      {

         ::cast < ::draw2d_direct2d::graphics > pdraw2ddirect2dgraphics = pdraw2dgraphics;

         HRESULT hr = pdraw2ddirect2dgraphics->m_pd2d1devicecontext1->CreateStrokedGeometryRealization(
            m_pd2d1pathgeometryHollow,
            1.0f,
            (FLOAT)iWidth,
            nullptr,
            &prealization);

         defer_throw_hresult(hr);

      }

      return prealization;

   }


   ID2D1GeometryRealization * path::_get_filled_geometry_realization(::draw2d::graphics * pdraw2dgraphics)
   {

      auto & prealization = m_pd2d1geometryrealizationFilled;

      if (!prealization)
      {

         ::cast<::draw2d_direct2d::graphics> pdraw2ddirect2dgraphics = pdraw2dgraphics;

         HRESULT hr = pdraw2ddirect2dgraphics->m_pd2d1devicecontext1->CreateFilledGeometryRealization(
            m_pd2d1pathgeometryFilled,
            1.0f,
            &prealization);

         defer_throw_hresult(hr);

      }

      return prealization;

   }


   void path::clear_node_data()
   {

      m_pd2d1geometrysinkTime = nullptr;

      m_pd2d1pathgeometryTime = nullptr;

      m_pd2d1pathgeometryHollow = nullptr;

      m_pd2d1pathgeometryFilled = nullptr;

      m_mapD2D1HollowGeometryRealization.clear();

      m_pd2d1geometryrealizationFilled.release();

   }


   bool path::_set(::draw2d::graphics * pdraw2dgraphics, const ::f64_arc & arc)
   {

      return internal_add_arc(pdraw2dgraphics, arc);

   }


   bool path::_set(::draw2d::graphics * pdraw2dgraphics, const ::draw2d::enum_item & eitem)
   {

      if (eitem == ::draw2d::e_item_begin_figure)
      {

         if (m_bHasPoint)
         {

            internal_end_figure(false);

         }

         return true;

      }
      else if (eitem == ::draw2d::e_item_close_figure)
      {

         if (m_bHasPoint)
         {

            internal_end_figure(true);

         }

         return true;

      }
      else if (eitem == ::draw2d::e_item_end_figure)
      {

         if (m_bHasPoint)
         {

            internal_end_figure(false);

         }

         return true;

      }
      else
      {

         return ::draw2d::path::_set(pdraw2dgraphics, eitem);

      }

      return true;

   }


   bool path::_set(::draw2d::graphics * pdraw2dgraphics, const ::f64_line & line)
   {

      if (line.m_p1 != m_pointEnd || ::is_null(m_pd2d1geometrysinkTime))
      {

         if (!internal_start_figure(pdraw2dgraphics, line.m_p1.x, line.m_p1.y))
         {

            return false;

         }

      }

      return internal_add_line(pdraw2dgraphics, line.m_p2.x, line.m_p2.y);

   }


   bool path::_set(::draw2d::graphics * pdraw2dgraphics, const ::f64_rectangle & rectangle)
   {

      return internal_add_rectangle(pdraw2dgraphics, rectangle.left, rectangle.top, rectangle.width(), rectangle.height());

   }


   bool path::_set(::draw2d::graphics * pdraw2dgraphics, const ::f64_ellipse & ellipse)
   {

      return internal_add_ellipse(pdraw2dgraphics, ellipse.left, ellipse.top, ellipse.width(), ellipse.height());

   }


   bool path::_set(::draw2d::graphics * pdraw2dgraphics, const ::f64_lines & lines)
   {

      return internal_add_lines(pdraw2dgraphics, (const ::i32_point_array &) lines, false);

   }


   bool path::_set(::draw2d::graphics * pdraw2dgraphics, const ::f64_polygon & polygon)
   {

      return internal_add_lines(pdraw2dgraphics, (const ::f64_point_array&)polygon, true);

   }


   bool path::_set(::draw2d::graphics * pdraw2dgraphics, const ::write_text::text_out & textout)
   {

      ::pointer < ::draw2d_direct2d::graphics > pgraphicsDraw2dDirect2d = pdraw2dgraphics;

      auto bOk = internal_add_string(
         pgraphicsDraw2dDirect2d,
         textout.m_point.x,
         textout.m_point.y,
         textout.m_strText,
         textout.m_pwritetextfont);

      if (!bOk)
      {

         return false;

      }

      return true;

      //IDWriteTextFormat * pformat = textout.m_pwritetextfont->get_os_data < IDWriteTextFormat * >(pdraw2dgraphics);

      //IDWriteFactory * pfactory = direct2d()->dwrite_factory();

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

      //auto pdraw2dgraphics = pdraw2dgraphics->cast < ::draw2d_direct2d::graphics>();

      ////if (m_pd2d1geometrysinkTime == nullptr)
      ////{

      ////   hr = m_pd2d1pathgeometryTime->Open(&m_pd2d1geometrysinkTime);

      ////}

      ////CustomTextRenderer renderer(direct2d()->d2d1_factory1(), m_pd2d1geometrysinkTime);

      ////defer_text_primitive_blend();

      ////defer_text_rendering_hint();

      //synchronous_lock lock(::direct2d::get()->synchronization());

      //auto & renderer = ::direct2d::get()->m_geometrysinktextrenderer;

      //renderer.m_pgeometrysink = m_pd2d1geometrysinkTime;

      //FLOAT dpix, dpiy;

      //if (SUCCEEDED(pdraw2dgraphics->m_prendertarget->GetDpi(dpix, dpiy))
      //{

      //   renderer.m_dDpi = dpix;

      //}
      //else
      //{

      //   renderer.m_dDpi = 96.0;

      //}

      //renderer.m_figurebeginOverride = pdraw2dgraphics->m_bOutline ? D2D1_FIGURE_BEGIN_HOLLOW : D2D1_FIGURE_BEGIN_FILLED;

      //playout->Draw(nullptr, &renderer, (FLOAT)textout.m_point.x, (FLOAT)textout.m_point.y);

      //return true;

   }


   bool path::_set(::draw2d::graphics * pdraw2dgraphics, const ::write_text::draw_text & drawtext)
   {

      return true;

   }


} // namespace draw2d_direct2d





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

//// THIS CODE AND log_information( IS PROVIDED "AS IS" WITHOUT WARRANTY OF
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
//                dc->m_pd2d1geometrysinkTime
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
//namespace draw2d_direct2d
//{
//   void path::CreatePathTextRenderer(FLOAT pixelsPerDip, IDWriteTextRenderer **textRenderer)
//   {
//      *textRenderer = nullptr;
//
//      PathTextRenderer *newRenderer = ___new PathTextRenderer(pixelsPerDip);
//      newRenderer->AddRef();
//      *textRenderer = newRenderer;
//      newRenderer = nullptr;
//   }
//
//} // namespace draw2d_direct2d
