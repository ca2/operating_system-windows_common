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


      void map_base();
      void unmap();


      bool LoadBitmap(const ::scoped_string & scopedstrResourceName);
      bool LoadBitmap(unsigned int nIDResource);
      bool LoadOEMBitmap(unsigned int nIDBitmap); // for OBM_/OCR_/OIC_


      virtual void CreateBitmap(::draw2d::graphics * pgraphics, const ::int_size& size, unsigned int nPlanes, unsigned int nBitcount, const void * lpBits, int stride) override;
      virtual bool CreateBitmapIndirect(::draw2d::graphics * pgraphics, LPBITMAP lpBitmap);
      virtual void CreateCompatibleBitmap(::draw2d::graphics * pgraphics, int nWidth, int nHeight);
      virtual void CreateDiscardableBitmap(::draw2d::graphics * pgraphics, int nWidth, int nHeight);
      virtual void create_bitmap(::draw2d::graphics * pgraphics, const ::int_size & size, void **ppvBits, int * stride) override;
      virtual void CreateDIBitmap(::draw2d::graphics * pgraphics, int cx, int cy, unsigned int flInit, const void *pjBits, unsigned int iUsage) override;


      virtual void attach(void * posdata);
      virtual void * detach();

      virtual void defer_update(::draw2d::graphics * pgraphics) const;

      unsigned int SetBitmapBits(unsigned int dwCount, const void * lpBits);
      unsigned int GetBitmapBits(unsigned int dwCount, LPVOID lpBits) const;
      //::int_size SetBitmapDimension(int nWidth, int nHeight);
      ::int_size GetBitmapDimension() const;

      

      void destroy() override;
      void destroy_os_data() override;


   };


} // namespace draw2d_direct2d



