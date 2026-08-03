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
      HDC m_hdcMemory;
      HBITMAP m_hbitmap;
      HBITMAP m_hbitmapOld;
      bool m_bMappedD2;
      //= CreateCompatibleDC(nullptr);
      //g_dib = CreateDIBSection(g_memoryDC, &info, DIB_RGB_COLORS, &g_pixels, nullptr, 0);

      //if (!g_memoryDC || !g_dib || !g_pixels)
      //{
      //   return false;
      //}

      //g_oldBitmap = SelectObject(g_memoryDC, g_dib);
      image();
      ~image() override;


      void create_from_graphics(::draw2d::graphics *pgraphics) override;

      void copy_from(::image::image * pimage, enum_flag eflagCreate = e_flag_success) override;


      //virtual ::draw2d::graphics * _get_graphics() const override;
      virtual ::draw2d::bitmap_pointer get_bitmap() const;
      virtual ::draw2d::bitmap_pointer detach_bitmap();


      void dc_select(bool bSelect = true);

      virtual void _create_dib_hbitmap(const ::i32_size &sizeRaw);
      virtual void _delete_dib_hbitmap();
      //using ::image::image::create;
      void create_as_render_target(const ::i32_size & size, ::user::interaction * puserinteraction, ::enum_flag eflagCreate = DEFAULT_CREATE_IMAGE_FLAG, ::i32 iStride = -1, bool bPreserve = false) override;
      //void create_for_image(::image::image *pimage) override;
      using ::image::image::initialize;
      void initialize(const ::i32_size & size, ::image32_t * pimage32, ::i32 iScan, ::enum_flag eflagCreate = DEFAULT_CREATE_IMAGE_FLAG) override;
      bool _create(::draw2d::graphics * pgraphics);
      void destroy() override;
      void destroy_os_data() override;


      bool host(::windowing::window_buffer *pwindowbuffer, ::windowing::window *pwindow, const ::i32_size & sizeRaw) override;


      //virtual void create_ex(const ::i32_size & size, ::image32_t * pimage32, ::i32 iScan, ::enum_flag eflagCreate = DEFAULT_CREATE_IMAGE_FLAG, ::i32 iStride = -1, bool bPreserve = false);

      //virtual bool _draw_raw(const ::i32_rectangle & rectangleTarget, ::image::image * pimage, const ::i32_point & pointSrc) override;


      virtual void SetIconMask(::image::icon * picon, ::i32 cx, ::i32 cy) override;


      virtual void defer_realize(::draw2d::graphics * pgraphics) const;
      //virtual bool realize(::draw2d::graphics * pgraphics) const;
      virtual void unrealize() const;
      virtual bool is_realized() const;

      using ::image::image::blend;
      virtual void blend(const ::i32_point & pointDst, ::image::image * pimageSrc, const ::i32_point & pointSrc, const ::i32_size & size, ::u8 bA) override;

      using ::image::image::tint;
      virtual void tint(::image::image * pimage, ::color::color color32) override;

      
protected:
      void _map(bool bApplyAlphaTransform = true) override;
      void _unmap(bool bDoUnmap = false) override;

   };


} // namespace draw2d_direct2d



