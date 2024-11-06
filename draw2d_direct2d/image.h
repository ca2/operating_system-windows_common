#pragma once


#include "aura/graphics/image/image.h"


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D image :
      virtual public ::image::image
   {
   public:


      HRESULT                             m_hrEndDraw;
      comptr<ID2D1Bitmap1>                m_pbitmap1Map;


      image();
      ~image() override;


      virtual void _map(bool bApplyAlphaTransform = true) override;
      virtual void _unmap() override;


      void copy_from(::image::image * pimage, enum_flag eflagCreate = e_flag_success) override;


      virtual ::draw2d::graphics * _get_graphics() const override;
      virtual ::draw2d::bitmap_pointer get_bitmap() const;
      virtual ::draw2d::bitmap_pointer detach_bitmap();


      void dc_select(bool bSelect = true);


      using ::image::image::create;
      void create(const ::size_i32 & size, ::enum_flag eflagCreate = DEFAULT_CREATE_IMAGE_FLAG, int iStride = -1, bool bPreserve = false) override;
      using ::image::image::initialize;
      void initialize(const ::size_i32 & size, ::image32_t * pimage32, int iScan, ::enum_flag eflagCreate = DEFAULT_CREATE_IMAGE_FLAG) override;
      bool _create(::draw2d::graphics * pgraphics);
      void destroy() override;
      void destroy_os_data() override;


      virtual void create_ex(const ::size_i32 & size, ::image32_t * pimage32, int iScan, ::enum_flag eflagCreate = DEFAULT_CREATE_IMAGE_FLAG, int iStride = -1, bool bPreserve = false);

      //virtual bool _draw_raw(const ::rectangle_i32 & rectangleTarget, ::image::image * pimage, const ::point_i32 & pointSrc) override;


      virtual void SetIconMask(::image::icon * picon, int cx, int cy) override;


      virtual void defer_realize(::draw2d::graphics * pgraphics) const;
      //virtual bool realize(::draw2d::graphics * pgraphics) const;
      virtual void unrealize() const;
      virtual bool is_realized() const;

      using ::image::image::blend;
      virtual void blend(const ::point_i32 & pointDst, ::image::image * pimageSrc, const ::point_i32 & pointSrc, const ::size_i32 & size, unsigned char bA) override;

      using ::image::image::tint;
      virtual void tint(::image::image * pimage, ::color::color color32) override;

      


   };


} // namespace draw2d_direct2d



