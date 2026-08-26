#pragma once


#include "bred/gpu/bitmap.h"
#include "draw2d_direct2d/bitmap.h"


namespace draw2d_direct2d_for_directx11
{


   class CLASS_DECL_DRAW2D_DIRECT2D_FOR_DIRECTX11 bitmap :
      virtual public ::draw2d_direct2d::bitmap,
      virtual public ::gpu::bitmap

   {
   public:


      //enum e_data
      //{

      //   data_bitmap,
      //   data_bitmap1,

      //};


      //comptr<ID2D1Bitmap>                    m_pbitmap;
      //comptr<ID2D1Bitmap1>                   m_pbitmap1;
      //memory                                 m_memory;




      bitmap();
      ~bitmap() override;


      //void dump(dump_context& dumpcontext) const override;


      void map();
      void unmap();


      bool LoadBitmap(const ::scoped_string & scopedstrResourceName);
      bool LoadBitmap(::u32 nIDResource);
      bool LoadOEMBitmap(::u32 nIDBitmap); // for OBM_/OCR_/OIC_


      //void _create_from_dxgi_surface(::i32 iIndex, ::i32 iLayerIndex, ::gpu::texture * pgputexture, ::draw2d_direct2d_for_directx11::graphics * pgraphics);
      virtual ::comptr < IDXGISurface> _get_dxgi_surface(::gpu::texture * pgputexture);
      void update_bitmap_as_backed_by_gpu_texture(::gpu::texture * pgputexture, ::draw2d::graphics * pdraw2graphics) override;

      void update_bitmap_as_image_render_target(
         ::image::image * pimage,
         ::acme::user::interaction * pacmeuserinteractionAffinity = nullptr,
         ::draw2d::graphics * pgraphics = nullptr) override;
      virtual void create_gpu_bitmap(::draw2d::graphics * pgraphics, const ::i32_size & size, ::pixmap * ppixmap = nullptr) override;
      virtual void CreateBitmap(::draw2d::graphics * pgraphics, const ::i32_size& size, ::u32 nPlanes, ::u32 nBitcount, const void * lpBits, ::i32 stride) override;
      virtual bool CreateBitmapIndirect(::draw2d::graphics * pgraphics, LPBITMAP lpBitmap);
      virtual void CreateCompatibleBitmap(::draw2d::graphics * pgraphics, ::i32 nWidth, ::i32 nHeight);
      virtual void CreateDiscardableBitmap(::draw2d::graphics * pgraphics, ::i32 nWidth, ::i32 nHeight);
      virtual void create_bitmap(::draw2d::graphics * pgraphics, const ::i32_size & size, ::pixmap * ppixmap) override;
      virtual void CreateDIBitmap(::draw2d::graphics * pgraphics, ::i32 cx, ::i32 cy, ::u32 flInit, const void *pjBits, ::u32 iUsage) override;


      virtual void attach(void * posdata);
      virtual void * detach();

      virtual void defer_update(::draw2d::graphics * pgraphics) const;

      ::u32 SetBitmapBits(::u32 dwCount, const void * lpBits);
      ::u32 GetBitmapBits(::u32 dwCount, LPVOID lpBits) const;
      //::i32_size SetBitmapDimension(::i32 nWidth, ::i32 nHeight);
      //::i32_size GetBitmapDimension() const;

      

      void destroy() override;
      void destroy_os_data() override;


   };


} // namespace draw2d_direct2d_for_directx11



