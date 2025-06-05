// Created by camilo on 2023-06-13 03:14 <3ThomasBorregaardSorensen!!
#pragma once


namespace direct2d_directx11
{


   namespace geometry
   {


      CLASS_DECL_DIRECT2D_DIRECTX11 comptr<ID2D1RectangleGeometry> create_rectangle(const ::float_rectangle & rectangle);
      CLASS_DECL_DIRECT2D_DIRECTX11 comptr<ID2D1EllipseGeometry> create_ellipse(const ::float_ellipse & ellipse);
      CLASS_DECL_DIRECT2D_DIRECTX11 comptr<ID2D1PathGeometry> create_polygon(const ::double_polygon & polygon, bool bFilled = true);
      CLASS_DECL_DIRECT2D_DIRECTX11 comptr<ID2D1PathGeometry> create_poly_polygon(const ::double_poly_polygon & polygon, bool bFilled = true);

      CLASS_DECL_DIRECT2D_DIRECTX11 void add_rectangle(ID2D1GeometrySink * pgeometrysink, const ::double_rectangle & rectangle);
      CLASS_DECL_DIRECT2D_DIRECTX11 void add_ellipse(ID2D1GeometrySink * pgeometrysink, const ::double_ellipse & ellipse, bool bFilled = true);
      CLASS_DECL_DIRECT2D_DIRECTX11 void add_polygon(ID2D1GeometrySink * pgeometrysink, const ::double_polygon & polygon, bool bFilled = true);
      CLASS_DECL_DIRECT2D_DIRECTX11 void add_poly_polygon(ID2D1GeometrySink * pgeometrysink, const ::double_poly_polygon & polygon, bool bFilled = true);


   } // namespace geometry


} // namespace direct2d_directx11



