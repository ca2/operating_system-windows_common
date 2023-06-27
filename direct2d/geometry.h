// Created by camilo on 2023-06-13 03:14 <3ThomasBorregaardSorensen!!
#pragma once


namespace direct2d
{


   namespace geometry
   {


      CLASS_DECL_DIRECT2D comptr<ID2D1RectangleGeometry> create_rectangle(const ::rectangle_f32 & rectangle);
      CLASS_DECL_DIRECT2D comptr<ID2D1EllipseGeometry> create_ellipse(const ::ellipse_f32 & ellipse);
      CLASS_DECL_DIRECT2D comptr<ID2D1PathGeometry> create_polygon(const ::polygon_f64 & polygon, bool bFilled = true);
      CLASS_DECL_DIRECT2D comptr<ID2D1PathGeometry> create_poly_polygon(const ::poly_polygon_f64 & polygon, bool bFilled = true);

      CLASS_DECL_DIRECT2D void add_rectangle(ID2D1GeometrySink * pgeometrysink, const ::rectangle_f64 & rectangle);
      CLASS_DECL_DIRECT2D void add_ellipse(ID2D1GeometrySink * pgeometrysink, const ::ellipse_f64 & ellipse, bool bFilled = true);
      CLASS_DECL_DIRECT2D void add_polygon(ID2D1GeometrySink * pgeometrysink, const ::polygon_f64 & polygon, bool bFilled = true);
      CLASS_DECL_DIRECT2D void add_poly_polygon(ID2D1GeometrySink * pgeometrysink, const ::poly_polygon_f64 & polygon, bool bFilled = true);


   } // namespace geometry


} // namespace direct2d



