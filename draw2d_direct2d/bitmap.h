#pragma once


#include "acme/prototype/prototype/memory.h"
#include "aura/graphics/draw2d/bitmap.h"
#include "object.h"


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D bitmap :
      virtual public ::draw2d_direct2d::object,
      virtual public ::draw2d::bitmap

   {
   public:


      enum e_data
      {

         data_bitmap,
         data_bitmap1,

      };


      comptr<ID2D1Bitmap>                    m_pd2d1bitmap;
      comptr<ID2D1Bitmap1>                   m_pd2d1bitmap1;
      memory                                 m_memory;

      comptr<ID2D1Bitmap1>                   m_pd2d1bitmap1Map;
      //comptr<ID2D1BitmapRenderTarget>        m_pd2d1bitmaprendertarget;
      //comptr<ID2D1DeviceContext>             m_pd2d1devicecontext;


      bitmap();
      bitmap(bitmap && bitmap);
      ~bitmap() override;


      //void dump(dump_context& dumpcontext) const override;




      bool LoadBitmap(const ::scoped_string & scopedstrResourceName);
      bool LoadBitmap(::u32 nIDResource);
      bool LoadOEMBitmap(::u32 nIDBitmap); // for OBM_/OCR_/OIC_


      void update_bitmap_as_image_render_target(
         ::image::image * pimage,
         ::acme::user::interaction * pacmeuserinteractionAffinity = nullptr,
         ::draw2d::graphics * pdraw2dgraphics = nullptr) override;
      void _create_d2d1_bitmap(
         ::draw2d::graphics * pdraw2dgraphics,
         const ::i32_size & size,
         const void * pbits,
         const ::i32_point & pointBits,
         const ::i32_size & sizeBits,
         ::i32 stride,
         ::acme::user::interaction * pacmeuserinteractionAffinity);

      void preserve_image(const ::i32_size & size, ::image::image * pimage) override;
      virtual void CreateBitmap(::draw2d::graphics * pdraw2dgraphics, const ::i32_size& size, ::u32 nPlanes, ::u32 nBitcount, const void * lpBits, ::i32 stride) override;
      virtual bool CreateBitmapIndirect(::draw2d::graphics * pdraw2dgraphics, LPBITMAP lpBitmap);
      virtual void CreateCompatibleBitmap(::draw2d::graphics * pdraw2dgraphics, ::i32 nWidth, ::i32 nHeight);
      virtual void CreateDiscardableBitmap(::draw2d::graphics * pdraw2dgraphics, ::i32 nWidth, ::i32 nHeight);
      virtual void create_bitmap(::draw2d::graphics * pdraw2dgraphics, const ::i32_size & size, ::pixmap * ppixmap) override;
      virtual void CreateDIBitmap(::draw2d::graphics * pdraw2dgraphics, ::i32 cx, ::i32 cy, ::u32 flInit, const void *pjBits, ::u32 iUsage) override;


      void read_pixels(const ::i32_size & size, const ::i32_point & point, ::image32_t * pimage32, ::i32 iScan) override;
      void write_pixels(const ::i32_size & size, const ::i32_point & point, const ::image32_t * pimage32, ::i32 iScan, bool bTopDown) override;


      void map();
      void unmap();


      //virtual void attach(void * posdata);
      //virtual void * detach();

      virtual void defer_update(::draw2d::graphics * pdraw2dgraphics) const;

      ::u32 SetBitmapBits(::u32 dwCount, const void * lpBits);
      ::u32 GetBitmapBits(::u32 dwCount, LPVOID lpBits) const;
      //::i32_size SetBitmapDimension(::i32 nWidth, ::i32 nHeight);
      //::i32_size GetBitmapDimension() const;

      ::i32_size size() const override;
      void set_size(const ::i32_size & size, bool bPreserve) override;

      virtual ID2D1Bitmap1 * _map_bitmap1();
      

      void clear_node_data() override;
      // void destroy_os_data() override;


   };


} // namespace draw2d_direct2d



