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


      comptr<ID2D1Bitmap>                    m_pbitmap;
      comptr<ID2D1Bitmap1>                   m_pbitmap1;
      memory                                 m_memory;




      bitmap();
      ~bitmap() override;


      //void dump(dump_context& dumpcontext) const override;


      void map();
      void unmap();


      bool LoadBitmap(const ::string & lpszResourceName);
      bool LoadBitmap(::u32 nIDResource);
      bool LoadOEMBitmap(::u32 nIDBitmap); // for OBM_/OCR_/OIC_


      virtual void CreateBitmap(::draw2d::graphics * pgraphics, const ::size_i32& size, ::u32 nPlanes, ::u32 nBitcount, const void * lpBits, int stride) override;
      virtual bool CreateBitmapIndirect(::draw2d::graphics * pgraphics, LPBITMAP lpBitmap);
      virtual void CreateCompatibleBitmap(::draw2d::graphics * pgraphics, int nWidth, int nHeight);
      virtual void CreateDiscardableBitmap(::draw2d::graphics * pgraphics, int nWidth, int nHeight);
      virtual void create_bitmap(::draw2d::graphics * pgraphics, const ::size_i32 & size, void **ppvBits, int * stride) override;
      virtual void CreateDIBitmap(::draw2d::graphics * pgraphics, int cx, int cy, u32 flInit, const void *pjBits, ::u32 iUsage) override;


      virtual void attach(void * posdata);
      virtual void * detach();

      virtual void defer_update(::draw2d::graphics * pgraphics) const;

      u32 SetBitmapBits(u32 dwCount, const void * lpBits);
      u32 GetBitmapBits(u32 dwCount, LPVOID lpBits) const;
      //::size_i32 SetBitmapDimension(int nWidth, int nHeight);
      ::size_i32 GetBitmapDimension() const;

      

      void destroy() override;
      void destroy_os_data() override;


   };


} // namespace draw2d_direct2d



