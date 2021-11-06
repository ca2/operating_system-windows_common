#pragma once


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D image :
      virtual public ::image
   {
   public:


      HRESULT                             m_hrEndDraw;
//      BITMAPINFO                          m_info;
      //bool                                m_bTrans;


      //D2D1_MAPPED_RECT                    m_map;
      comptr<ID2D1Bitmap1>                m_pbitmap1Map;
      //memory                              m_memory;


      image();
      ~image() override;


      virtual bool _map(bool bApplyAlphaTransform = true) override;
      virtual bool _unmap() override;


      bool copy_from(::image * pimage, enum_flag eflagCreate = e_flag_success) override;


      virtual ::draw2d::graphics * _get_graphics() const override;
      virtual ::draw2d::bitmap_pointer get_bitmap() const;
      virtual ::draw2d::bitmap_pointer detach_bitmap();


      bool dc_select(bool bSelect = true);


      using ::image::create;
      ::e_status create(const ::size_i32 & size, ::enum_flag eflagCreate = DEFAULT_CREATE_IMAGE_FLAG, int iStride = -1, bool bPreserve = false) override;
      using ::image::initialize;
      ::e_status initialize(const ::size_i32 & size, ::color32_t * pcolorref, int iScan, ::enum_flag eflagCreate = DEFAULT_CREATE_IMAGE_FLAG) override;
      bool _create(::draw2d::graphics * pgraphics);
      ::e_status destroy() override;


      virtual ::e_status create_ex(const ::size_i32 & size, ::color32_t * pcolorref, int iScan, ::enum_flag eflagCreate = DEFAULT_CREATE_IMAGE_FLAG, int iStride = -1, bool bPreserve = false);

      //virtual bool _draw_raw(const ::rectangle_i32 & rectangleTarget, ::image * pimage, const ::point_i32 & pointSrc) override;


      virtual ::e_status SetIconMask(::draw2d::icon * picon, i32 cx, i32 cy) override;


      virtual bool defer_realize(::draw2d::graphics * pgraphics) const;
      //virtual bool realize(::draw2d::graphics * pgraphics) const;
      virtual bool unrealize() const;
      virtual bool is_realized() const;

      using ::image::blend;
      virtual bool blend(const ::point_i32 & pointDst, ::image * pimageSrc, const ::point_i32 & pointSrc, const ::size_i32 & size, byte bA) override;

      using ::image::tint;
      virtual bool tint(::image * pimage, const ::color::color & color32) override;

      


   };


} // namespace draw2d_direct2d



