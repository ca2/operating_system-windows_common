#pragma once


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D image :
      virtual public ::image
   {
   public:


      HRESULT                             m_hrEndDraw;
      BITMAPINFO                          m_info;
      bool                                m_bTrans;

      ::draw2d::bitmap_pointer            m_pbitmapMap;
      ::draw2d::graphics_pointer          m_pgraphicsMap;


      image();
      virtual ~image();


      virtual bool map(bool bApplyAlphaTransform = true) override;
      virtual bool _unmap() override;


      virtual ::draw2d::graphics * _get_graphics() const override;
      virtual ::draw2d::bitmap_pointer get_bitmap() const;
      virtual ::draw2d::bitmap_pointer detach_bitmap();


      bool dc_select(bool bSelect = true);


      using ::image::create;
      virtual void create(const ::int_size & size, ::eobject eobjectCreate = DEFAULT_CREATE_IMAGE_OBJECT_FLAG, int iStride = -1, bool bPreserver = false) override;
      bool _create(::draw2d::graphics * pgraphics);
      bool destroy();


      virtual bool _draw_raw(const ::int_rectangle & rectangleTarget, ::image::image * pimage, const ::int_point & pointSrc) override;


      virtual void SetIconMask(::image::icon * picon, int cx, int cy) override;


      virtual bool defer_realize(::draw2d::graphics * pgraphics) const;
      virtual bool realize(::draw2d::graphics * pgraphics) const;
      virtual bool unrealize() const;
      virtual bool is_realized() const;

      using ::image::blend;
      virtual bool blend(const ::int_point & pointDst, ::image::image * pimageSrc, const ::int_point & pointSrc, const ::int_size & size, unsigned char bA) override;

      using ::image::tint;
      virtual bool tint(::image::image * pimage, const ::color::color & color32) override;

      


   };


} // namespace draw2d_direct2d



