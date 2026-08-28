#pragma once


#include "aura/_.h"
#include "acme/_win32_gdi.h"
#include "direct2d/_.h"
#include "operating_system-windows/aura_windows/_.h"
#include "acme/operating_system/windows_common/com/hresult_exception.h"


#if defined(_draw2d_direct2d_project)
#define CLASS_DECL_DRAW2D_DIRECT2D  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_DRAW2D_DIRECT2D  CLASS_DECL_IMPORT
#endif


#include "_constant.h"

//
//namespace d2d1
//{
//
//
//   //inline D2D1_RECT_F ::f32_rectangle(const ::i32_rectangle & rectangle)
//   //{
//
//   //   return D2D1::Rect<FLOAT>(
//   //      (FLOAT)(rectangle.left),
//   //      (FLOAT)(rectangle.top),
//   //      (FLOAT)(rectangle.right),
//   //      (FLOAT)(rectangle.bottom)
//   //      );
//
//   //}
//
//
//   //inline D2D1_POINT_2F Point2F(::f64 x = 0.0, ::f64 y = 0.0)
//   //{
//
//   //   return Point2F(
//   //      (FLOAT)(x),
//   //      (FLOAT)(y)
//   //   );
//
//   //}
//
//
//} // namespace d2d1
//

namespace draw2d_direct2d
{

   CLASS_DECL_AURA void initialize();

   CLASS_DECL_AURA void destroy();

   class graphics;


   class swap_chain_interop;



} // namespace draw2d_direct2d


//#define __graphics(pdraw2dgraphics) ((::draw2d_direct2d::graphics *) pdraw2dgraphics->m_pthis)
//#define __pen(pdraw2dpen) ((::draw2d_direct2d::pen *) pdraw2dpen->m_pthis)
//#define __brush(pdraw2dbrush) ((::draw2d_direct2d::brush *) pdraw2dbrush->m_pthis)
//#define __font(pwritetextfont) ((::draw2d_direct2d::font *) pwritetextfont->m_pthis)
//#define __graphics_path(pdraw2dpath) ((::draw2d_direct2d::path *) pdraw2dpath->m_pthis)
//
//




//#define ___new ACME_NEW



