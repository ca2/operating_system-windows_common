#pragma once


namespace draw2d_directx
{


   class region :
      virtual public ::draw2d::region
   {
   public:


      comptr<ID2D1Geometry>            m_pgeometry;


      region();
      virtual ~region();

      //void defer_update();

      virtual void destroy() override;
      virtual bool create(::draw2d::graphics * pgraphics, char iCreate) override;


      void SetRectRgn(int x1, int y1, int x2, int y2);
      void SetRectRgn(const ::int_rectangle & rectangle);
      int CombineRgn(const ::draw2d::region* pRgn1, const ::draw2d::region* pRgn2, int nCombineMode);
      int CopyRgn(const ::draw2d::region* pRgnSrc);
      bool EqualRgn(const ::draw2d::region* pRgn) const;
      int OffsetRgn(int x, int y);
      int OffsetRgn(const ::int_point & point);
      int GetRgnBox(::int_rectangle * prectangle) const;
      bool contains(::draw2d::graphics * pgraphics, int x, int y) const;
      bool contains(::draw2d::graphics * pgraphics, const ::int_point & point) const;
      bool RectInRegion(const ::int_rectangle & rectangle) const;
      int GetRegionData(LPRGNDATA lpRgnData, int nCount) const;


      ID2D1Geometry * get(::draw2d::graphics * pgraphics);
      ID2D1Geometry * get_rect(::draw2d::graphics * pgraphics);
      ID2D1Geometry * get_oval(::draw2d::graphics * pgraphics);
      ID2D1Geometry * get_polygon(::draw2d::graphics * pgraphics);
      ID2D1Geometry * get_poly_polygon(::draw2d::graphics * pgraphics);
      ID2D1Geometry * get_combine(::draw2d::graphics * pgraphics);

   };


} // namespace draw2d_directx



