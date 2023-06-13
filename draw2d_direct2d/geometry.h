// Created by camilo on 2023-06-13 03:14 <3ThomasBorregaardSorensen!!
#pragma once


namespace direct2d
{


   namespace geometry
   {


      CLASS_DECL_DRAW2D_DIRECT2D comptr<ID2D1RectangleGeometry> create_rectangle(const ::rectangle_f32 & rectangle);
      CLASS_DECL_DRAW2D_DIRECT2D comptr<ID2D1EllipseGeometry> create_ellipse(const ::ellipse_f32 & ellipse);
      CLASS_DECL_DRAW2D_DIRECT2D comptr<ID2D1PathGeometry> create_polygon(const ::polygon_f64 & polygon, bool bFilled = true);
      CLASS_DECL_DRAW2D_DIRECT2D comptr<ID2D1PathGeometry> create_poly_polygon(const ::polygon_f64 & polygon, bool bFilled = true);


   } // namespace geometry


} // namespace direct2d



