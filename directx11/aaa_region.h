#pragma once


namespace draw2d_directx11
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
      virtual bool create(::draw2d::graphics * pgraphics, ::i8 iCreate) override;


      void SetRectRgn(::i32 x1, ::i32 y1, ::i32 x2, ::i32 y2);
      void SetRectRgn(const ::i32_rectangle & rectangle);
      ::i32 CombineRgn(const ::draw2d::region* pRgn1, const ::draw2d::region* pRgn2, ::i32 nCombineMode);
      ::i32 CopyRgn(const ::draw2d::region* pRgnSrc);
      bool EqualRgn(const ::draw2d::region* pRgn) const;
      ::i32 OffsetRgn(::i32 x, ::i32 y);
      ::i32 OffsetRgn(const ::i32_point & point);
      ::i32 GetRgnBox(::i32_rectangle * prectangle) const;
      bool contains(::draw2d::graphics * pgraphics, ::i32 x, ::i32 y) const;
      bool contains(::draw2d::graphics * pgraphics, const ::i32_point & point) const;
      bool RectInRegion(const ::i32_rectangle & rectangle) const;
      ::i32 GetRegionData(LPRGNDATA lpRgnData, ::i32 nCount) const;


      ID2D1Geometry * get(::draw2d::graphics * pgraphics);
      ID2D1Geometry * get_rect(::draw2d::graphics * pgraphics);
      ID2D1Geometry * get_oval(::draw2d::graphics * pgraphics);
      ID2D1Geometry * get_polygon(::draw2d::graphics * pgraphics);
      ID2D1Geometry * get_poly_polygon(::draw2d::graphics * pgraphics);
      ID2D1Geometry * get_combine(::draw2d::graphics * pgraphics);

   };


} // namespace draw2d_directx11



