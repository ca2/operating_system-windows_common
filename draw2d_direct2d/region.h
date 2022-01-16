#pragma once


namespace draw2d_direct2d
{


   class region :
      virtual public ::draw2d::region
   {
   public:


      comptr<ID2D1Geometry>            m_pgeometry;


      region();
      ~region() override;

      //void defer_update();

      void destroy() override;
      void create(::draw2d::graphics * pgraphics, i8 iCreate) override;


      void SetRectRgn(int x1, int y1, int x2, int y2);
      void SetRectRgn(const ::rectangle_i32 & rectangle);
      int CombineRgn(const ::draw2d::region* pRgn1, const ::draw2d::region* pRgn2, int nCombineMode);
      int CopyRgn(const ::draw2d::region* pRgnSrc);
      bool EqualRgn(const ::draw2d::region* pRgn) const;
      int OffsetRgn(int x, int y);
      int OffsetRgn(const ::point_i32 & point);
      int GetRgnBox(RECTANGLE_I32 * prectangle) const;
      bool contains(::draw2d::graphics * pgraphics, int x, int y) const;
      bool contains(::draw2d::graphics * pgraphics, const ::point_i32 & point) const;
      bool RectInRegion(const ::rectangle_i32 & rectangle) const;
      int GetRegionData(LPRGNDATA lpRgnData, int nCount) const;


      ID2D1Geometry * get(::draw2d::graphics * pgraphics);
      ID2D1Geometry * get_rectangle(::draw2d::graphics * pgraphics);
      ID2D1Geometry * get_ellipse(::draw2d::graphics * pgraphics);
      ID2D1Geometry * get_polygon(::draw2d::graphics * pgraphics);
      ID2D1Geometry * get_poly_polygon(::draw2d::graphics * pgraphics);
      ID2D1Geometry * get_combine(::draw2d::graphics * pgraphics);

   };


} // namespace draw2d_direct2d



