#pragma once


#include "aura/_.h"
#include "gpu_directx/_.h"
#include "acme/_win32_gdi.h"
#include "direct2d/_.h"
#include "acme_windows_common/_.h"
#include "acme_windows_common/hresult_exception.h"


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
//   //inline D2D1_RECT_F float_rectangle(const ::int_rectangle & rectangle)
//   //{
//
//   //   return D2D1::Rect<FLOAT>(
//   //      (FLOAT)(rectangle.left()),
//   //      (FLOAT)(rectangle.top()),
//   //      (FLOAT)(rectangle.right()),
//   //      (FLOAT)(rectangle.bottom())
//   //      );
//
//   //}
//
//
//   //inline D2D1_POINT_2F Point2F(double x = 0.0, double y = 0.0)
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



} // namespace draw2d_direct2d


#define __graphics(pgraphics) ((::draw2d_direct2d::graphics *) pgraphics->m_pthis)
#define __pen(ppen) ((::draw2d_direct2d::pen *) ppen->m_pthis)
#define __brush(pbrush) ((::draw2d_direct2d::brush *) pbrush->m_pthis)
#define __font(pfont) ((::draw2d_direct2d::font *) pfont->m_pthis)
#define __graphics_path(ppath) ((::draw2d_direct2d::path *) ppath->m_pthis)






//#define ___new ACME_NEW



