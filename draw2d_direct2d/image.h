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


      virtual void _map(bool bApplyAlphaTransform = true) override;
      virtual void _unmap() override;


      void copy_from(::image * pimage, enum_flag eflagCreate = e_flag_success) override;


      virtual ::draw2d::graphics * _get_graphics() const override;
      virtual ::draw2d::bitmap_pointer get_bitmap() const;
      virtual ::draw2d::bitmap_pointer detach_bitmap();


      void dc_select(bool bSelect = true);


      using ::image::create;
      void create(const ::size_i32 & size, ::enum_flag eflagCreate = DEFAULT_CREATE_IMAGE_FLAG, int iStride = -1, bool bPreserve = false) override;
      using ::image::initialize;
      void initialize(const ::size_i32 & size, ::color32_t * pcolorref, int iScan, ::enum_flag eflagCreate = DEFAULT_CREATE_IMAGE_FLAG) override;
      bool _create(::draw2d::graphics * pgraphics);
      void destroy() override;


      virtual void create_ex(const ::size_i32 & size, ::color32_t * pcolorref, int iScan, ::enum_flag eflagCreate = DEFAULT_CREATE_IMAGE_FLAG, int iStride = -1, bool bPreserve = false);

      //virtual bool _draw_raw(const ::rectangle_i32 & rectangleTarget, ::image * pimage, const ::point_i32 & pointSrc) override;


      virtual void SetIconMask(::draw2d::icon * picon, i32 cx, i32 cy) override;


      virtual void defer_realize(::draw2d::graphics * pgraphics) const;
      //virtual bool realize(::draw2d::graphics * pgraphics) const;
      virtual void unrealize() const;
      virtual bool is_realized() const;

      using ::image::blend;
      virtual void blend(const ::point_i32 & pointDst, ::image * pimageSrc, const ::point_i32 & pointSrc, const ::size_i32 & size, byte bA) override;

      using ::image::tint;
      virtual void tint(::image * pimage, const ::color::color & color32) override;

      


   };


} // namespace draw2d_direct2d



