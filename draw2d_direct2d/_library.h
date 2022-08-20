// Created by camilo on 2021-06-06 00:32 BRT <3ThomasBS_!!
#pragma once


#include "direct2d/_library.h"


namespace draw2d_direct2d
{


   class draw2d;


   enum enum_data
   {

      data_bitmap = 0,
      data_bitmap1 = 1,
      data_device_context = 0,
      data_render_target = 1,

   };


} // namespace draw2d_direct2d


namespace d2d1
{


   inline D2D1_RECT_F rectangle_f32(const ::rectangle_i32& rectangle)
   {

      return D2D1::Rect<FLOAT>(
         (FLOAT)(rectangle.left),
         (FLOAT)(rectangle.top),
         (FLOAT)(rectangle.right),
         (FLOAT)(rectangle.bottom)
         );

   }


   inline D2D1_POINT_2F Point2F(double x = 0.0, double y = 0.0)
   {

      return Point2F(
         (FLOAT)(x),
         (FLOAT)(y)
      );

   }


} // namespace d2d1

inline void copy(D2D1_POINT_2F* ppointDst, const POINT_F64* ppointSrc)
{
   ppointDst->x = (FLOAT)ppointSrc->x;
   ppointDst->y = (FLOAT)ppointSrc->y;
}


namespace draw2d_direct2d
{

   CLASS_DECL_AURA void initialize();

   CLASS_DECL_AURA void destroy();

   class graphics;

   inline void throw_if_failed(HRESULT hr)
   {

      if (FAILED(hr))
         throw hresult_exception(hr);
      //if(FAILED(hr))
      // throw ::exception(hr);

   }


} // namespace draw2d_direct2d


#define __graphics(pgraphics) ((::draw2d_direct2d::graphics *) pgraphics->m_pthis)
#define __pen(ppen) ((::draw2d_direct2d::pen *) ppen->m_pthis)
#define __brush(pbrush) ((::draw2d_direct2d::brush *) pbrush->m_pthis)
#define __font(pfont) ((::draw2d_direct2d::font *) pfont->m_pthis)
#define __graphics_path(ppath) ((::draw2d_direct2d::path *) ppath->m_pthis)




#include "factory_exchange.h"


#include "object.h"
#include "pen.h"
#include "bitmap.h"
#include "brush.h"
#include "font.h"
#include "palette.h"
#include "region.h"
#include "image.h"
#include "path.h"
//#include "printer.h"


#include "graphics.h"


#include "draw2d.h"


#define new ACME_NEW


inline D2D1_COLOR_F* copy(D2D1_COLOR_F* pcolorTarget, const ::color::color * pcolor)
{

   pcolorTarget->a = pcolor->alpha / 255.0f;
   pcolorTarget->r = pcolor->red / 255.0f;
   pcolorTarget->g = pcolor->green / 255.0f;
   pcolorTarget->b = pcolor->blue / 255.0f;

   return pcolorTarget;

}


inline D2D1_COLOR_F* copy(D2D1_COLOR_F* pcolorTarget, const ::color32_t* pcolor)
{

   pcolorTarget->a = colorref_get_a_value(*pcolor) / 255.0f;
   pcolorTarget->r = colorref_get_r_value(*pcolor) / 255.0f;
   pcolorTarget->g = colorref_get_g_value(*pcolor) / 255.0f;
   pcolorTarget->b = colorref_get_b_value(*pcolor) / 255.0f;

   return pcolorTarget;

}



