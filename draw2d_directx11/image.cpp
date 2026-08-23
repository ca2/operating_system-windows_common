#include "platform.h"
#include "bitmap.h"
#include "image.h"
#include "graphics.h"
#include "aura/graphics/draw2d/lock.h"
#include "aura/graphics/draw2d/device_lock.h"
#include "aura/graphics/image/context.h"
#include "aura/graphics/image/icon.h"
#include "aura/graphics/image/drawing.h"


namespace draw2d_directx11
{


   image::image()
   {

      defer_create_synchronization();

      //g_pimagea->add(this);

   }


   image::~image()
   {

      destroy();

   }


   ::draw2d::bitmap_pointer image::get_bitmap() const
   {

      return m_pbitmap;

   }


   ::draw2d::bitmap_pointer image::detach_bitmap()
   {
   
      return m_pbitmap.detach();

   }


   //void image::create_ex(const ::i32_size & size, ::image32_t * pimage32, ::i32 iScan, ::enum_flag eflagCreate, ::i32 iGoodStride, bool bPreserve)
   //{

   //   //::draw2d::lock draw2dlock;

   //   //::draw2d::device_lock devicelock(this);

   //   auto sizeCurrent = this->size();

   //   if (m_pbitmap.is_set() && size == sizeCurrent)
   //   {

   //      //return true;

   //      return;

   //   }

   //   destroy();

   //   if (size.is_empty())
   //   {

   //      throw ::exception(error_failed);

   //   }

   //   ::i32 iStride = size.cx * 4;

   //   if (iGoodStride > iStride)
   //   {

   //      iStride = iGoodStride;

   //   }

   //   //m_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   //   //m_info.bmiHeader.biWidth = size.cx;
   //   //m_info.bmiHeader.biHeight = -size.cy;
   //   //m_info.bmiHeader.biPlanes = 1;
   //   //m_info.bmiHeader.biBitCount = 32;
   //   //m_info.bmiHeader.biCompression = BI_RGB;
   //   //m_info.bmiHeader.biSizeImage = iStride * size.cy;

   //   ::draw2d::bitmap_pointer         pbitmap;
   //   ::draw2d::graphics_pointer       pgraphics;

   //   constructø(pbitmap);
   //   constructø(pgraphics);

   //   if (::is_null(pbitmap) || ::is_null(pgraphics))
   //   {

   //      throw ::exception(error_failed);

   //   }

   //   pgraphics->create_memory_graphics(size);

   //   pbitmap->create_bitmap(pgraphics, size, &pimage32,nullptr,  &iScan);

   //   //if (!pbitmap->create_bitmap(pgraphics, size, (void **)&pimage32, &iScan))
   //   //{

   //   //   return false;

   //   //}

   //   //if (pbitmap->m_osdata[0] == nullptr)
   //   //{

   //   //   return;

   //   //}

   //   //auto estatus = 
   //   
   //   pgraphics->set(pbitmap);

   //   //if (!estatus)
   //   //{

   //   //   return false;

   //   //}

   //   //m_pgraphics = pgraphics;

   //   m_pbitmap = pbitmap;

   //   m_sizeRaw = size;

   //   m_size = m_sizeRaw;

   //   m_bMapped = false;

   //   //((ID2D1DeviceContext *)m_pgraphics->get_os_data())->BeginDraw();

   //   m_hrEndDraw = S_OK;

   //   //m_pgraphics->m_pimage = this;

   //   set_flag(eflagCreate);

   //   //return true;

   //}


   //void image::create(const ::i32_size & size, ::enum_flag eflagCreate, ::i32 iGoodStride, bool bPreserve)
   //{

   //   //auto estatus = 
   //   
   //   create_ex(size, nullptr, 0, eflagCreate, iGoodStride, bPreserve);

   //   //if (!estatus)
   //   //{

   //   //   return estatus;

   //   //}

   //   //return estatus;

   //}


   //void image::initialize(const ::i32_size & size, ::image32_t * pimage32, ::i32 iScan, ::enum_flag eflagCreate)
   //{

   //   //auto estatus =
   //   
   //   //create_ex(size, pimage32, iScan, eflagCreate);

   //   //if (!estatus)
   //   //{

   //   //   return estatus;

   //   //}

   //   //return estatus;

   //}


   //void image::dc_select(bool bSelect)
   //{

   //   //return true;

   //}


   bool image::_create(::draw2d::graphics* pgraphicsParam)
   {

      ::draw2d::bitmap * pbitmap = dynamic_cast<::draw2d_directx11::graphics *>(pgraphicsParam)->get_current_bitmap();

      if (pbitmap == nullptr)
      {

         return false;

      }

      ::i32_size size = pbitmap->size();

      create_as_descriptor(size);

      //if (!create(size))
      //{

      //   return false;

      //}

      copy_from(pgraphicsParam->m_pimage);

      return true;

   }


   //void image::copy_from(::image::image * pimage, ::enum_flag eflagCreate)
   //{

   //   ::i32_size s(pimage->width(), pimage->height());

   //   if (size() != s)
   //   {

   //      pimage->map();
   //      
   //      if (::is_set(pimage->m_pimage32Raw) && pimage->m_iScan > 0)
   //      {

   //         create_from_data(s, pimage->m_pimage32Raw, pimage->m_iScan, eflagCreate);

   //         //if (initialize(s, pimage->m_pimage32Raw, pimage->m_iScan, eflagCreate))
   //         //{

   //         //   return true;

   //         //}

   //      }

   //   }

   //   //auto bOk = 
   //   ::image::image::copy_from(pimage, 0, eflagCreate);

   //   //if (!bOk)
   //   //{

   //   //   return bOk;

   //   //}

   //   //return true;

   //}


   void image::destroy()
   {

      destroy_os_data();

   }


   void image::destroy_os_data()
   {

      //::draw2d::device_lock devicelock(this);

      //if (m_bMapped)
      //{

      //   unmap();

      //}

      m_pbitmap.release();

      //m_pgraphics.release();

   }




   //bool image::draw(::draw2d::graphics* pgraphics, const ::i32_point & point, const ::i32_size & size, const ::i32_point & pointSrc)
   //{

   //   return pgraphics->BitBlt(point.x, point.y, size.cx, size.cy, get_graphics(), pointSrc.x, pointSrc.y) != false;

   //}



   void image::set_image_icon(::image::icon * picon, ::i32 cx, ::i32 cy)
   {

      if (cx <= 0 || cy <= 0)
      {

         throw ::exception(error_bad_argument);

      }

      //auto estatus = 
      
      create_as_descriptor({ cx, cy });

      //if (!estatus)
      //{


      //   return estatus;

      //}

      // White blend image
      auto pimage1 = ::particle::image()->create_image({cx,  cy});

      {

         auto pgraphicsImage1 = pimage1->acquire_graphics();

         pgraphicsImage1->clear(rgba(0, 255, 255, 255));

         ::image::image_source imagesource(picon);

         ::image::image_drawing_options imagedrawingoptions(::f64_size(cx, cy));

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pgraphicsImage1->draw(imagedrawing);

      }

      // Black blend image
      auto pimage2 = ::particle::image()->create_image({cx,  cy});

      {

         ::image::image_source imagesource(picon);

         ::image::image_drawing_options imagedrawingoptions(::f64_size(cx, cy));

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         auto pgraphicsImage2 = pimage2->acquire_graphics();

         pgraphicsImage2->clear(::color::transparent);

         pgraphicsImage2->draw(imagedrawing);

      }

//#else
//      pgraphicsImage2->DrawIcon(
//      0, 0,
//      picon,
//      cx, cy,
//      0,
//      nullptr,
//      0);
//#endif

      // Mask image
      auto pimageM = ::particle::image()->create_image({cx,  cy});

      {

         auto pgraphicsImageM = pimageM->acquire_graphics();

         ::image::image_source imagesource(picon);

         ::image::image_drawing_options imagedrawingoptions(::f64_size(cx, cy));

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pgraphicsImageM->draw(imagedrawing);

      }

      auto ppixmapImage1 = pimage1->map();
      auto ppixmapImage2= pimage2->map();
      auto ppixmapImageM = pimageM->map();
      auto ppixmapImageThis = this->map();

      ::u8 * r1 = (::u8*) ppixmapImage1->image32();
      ::u8 * r2 = (::u8*) ppixmapImage2->image32();
      ::u8 * srcM = (::u8*) ppixmapImageM->image32();
      ::u8 * dest = (::u8*) ppixmapImageThis->image32();
      ::i32 iSize = cx * cy;

      ::u8 b;
      ::u8 bMax;
      while (iSize-- > 0)
      {
         if (srcM[0] == 255)
         {
            bMax = 0;
         }
         else
         {
            bMax = 0;
            b = (::u8)(r1[0] - r2[0]);
            bMax = maximum(b, bMax);
            b = (::u8)(r1[1] - r2[1]);
            bMax = maximum(b, bMax);
            b = (::u8)(r1[2] - r2[2]);
            bMax = maximum(b, bMax);
            bMax = 255 - bMax;
         }
         dest[0] = bMax;
         dest[1] = bMax;
         dest[2] = bMax;
         dest += 4;
         srcM += 4;
         r1 += 4;
         r2 += 4;
      }

      //return true;

   }

   //void image::rotate(image image, ::f64 dAngle, ::f64 dScale)
   //{
   //   // image image(this);
   //   //   pimage->Paste(this);

   //   /*      ::i32 cx = cx;
   //   ::i32 cy = cy; */

   //   ::i32 l = maximum(cx, cy);


   //   ::i32 jmax = minimum(l, cy / 2);
   //   ::i32 jmin = - jmax;
   //   ::i32 imax = minimum(l, cx / 2);
   //   ::i32 imin = - imax;


   //   ::i32 joff = cy / 2;
   //   ::i32 ioff = cx / 2;

   //   //::i32 iAngle = iStep % 360;
   //   //::i32 iAngle = iStep;
   //   //::i32 iAngle = 1;
   //   //::i32 k = 0;

   //   /*     for ( ::i32 j=jmin; j<jmax; j++ )
   //   {
   //   for ( ::i32 i=imin; i<imax; i++ )
   //   {
   //   ::i32 x, y;

   //   // A Combination of a 2d Translation/rotation/Scale Matrix
   //   x=::i32(cos10(i, iAngle) - sin10(j, iAngle)) + ioff;
   //   y=::i32(sin10(i, iAngle) + cos10(j, iAngle)) + joff;
   //   m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //   pimage->m_pcolorref[abs(y%cy)*cx+abs(x%cx)];
   //   //k++;
   //   }
   //   (j+joff)*cx+(i+ioff)
   //   }*/

   //   ::i32 k = 0;
   //   ::f64 dCos = ::cos(dAngle * dPi / 180.0) * dScale;
   //   ::f64 dSin = ::sin(dAngle * dPi / 180.0) * dScale;
   //   ::i32 cx1 = cx - 1;
   //   ::i32 cy1 = cy - 1;
   //   for ( ::i32 j=jmin; j<jmax; j++ )
   //   {
   //      for ( ::i32 i=imin; i<imax; i++ )
   //      {
   //         ::i32 x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         //x=abs((::i32(dCos * i - dSin * j) + ioff) % cx);
   //         //y=abs((::i32(dSin * i + dCos * j) + joff) % cy);

   //         x = (::i32) abs((dCos * i - dSin * j) + ioff);
   //         y = (::i32) abs((dSin * i + dCos * j) + joff);

   //         if((x / cx) % 2 == 0)
   //         {
   //            x %= cx;
   //         }
   //         else
   //         {
   //            x = cx1 - (x % cx);
   //         }

   //         if((y / cy) % 2 == 0)
   //         {
   //            y %= cy;
   //         }
   //         else
   //         {
   //            y = cy1 - (y % cy);
   //         }



   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            pimage->get_data()[y * cx + x];
   //         k++;
   //      }
   //   }
   //}


   //void image::Rotate034(image image, ::f64 dAngle, ::f64 dScale)
   //{

   //   /*      ::i32 cx = cx;
   //   ::i32 cy = cy;*/

   //   ::i32 l = maximum(cx, cy);


   //   ::i32 jmax = minimum(l, cy / 2);
   //   ::i32 jmin = - jmax;
   //   ::i32 imax = minimum(l, cx / 2);
   //   ::i32 imin = - imax;


   //   if((cy % 2) == 1)
   //      jmax++;

   //   if((cx % 2) == 1)
   //      imax++;

   //   ::i32 joff = cy / 2;
   //   ::i32 ioff = cx / 2;


   //   ::i32 k = 0;
   //   ::f64 dCos = ::cos(dAngle * dPi / 180.0) * dScale;
   //   ::f64 dSin = ::sin(dAngle * dPi / 180.0) * dScale;
   //   ::i32 cx1 = cx - 1;
   //   ::i32 cy1 = cy - 1;
   //   for ( ::i32 j=jmin; j<jmax; j++ )
   //   {
   //      for ( ::i32 i=imin; i<imax; i++ )
   //      {
   //         ::i32 x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         //x=abs((::i32(dCos * i - dSin * j) + ioff) % cx);
   //         //y=abs((::i32(dSin * i + dCos * j) + joff) % cy);

   //         x = (::i32) abs((dCos * i - dSin * j) + ioff);
   //         y = (::i32) abs((dSin * i + dCos * j) + joff);

   //         if((x / cx) % 2 == 0)
   //         {
   //            x %= cx;
   //         }
   //         else
   //         {
   //            x = cx1 - (x % cx);
   //         }

   //         if((y / cy) % 2 == 0)
   //         {
   //            y %= cy;
   //         }
   //         else
   //         {
   //            y = cy1 - (y % cy);
   //         }



   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            pimage->get_data()[y * cx + x];
   //         k++;
   //      }
   //   }
   //}

   //void image::rotate(image image, const ::i32_rectangle & rectangle, ::f64 dAngle, ::f64 dScale)
   //{
   //   // image image(this);
   //   //   pimage->Paste(this);



   //   ::i32_rectangle rectangle(rectangle);

   //   ::i32 cx = rectangle.width();
   //   ::i32 cy = rectangle.height();

   //   ::i32 l = maximum(cx, cy);

   //   ::i32 jmax = minimum(l, cy / 2);
   //   ::i32 jmin = - jmax;
   //   ::i32 imax = minimum(l, cx / 2);
   //   ::i32 imin = - imax;


   //   ::i32 joff = cy / 2 + rectangle.left;
   //   ::i32 ioff = cx / 2 + rectangle.top;

   //   //::i32 iAngle = iStep % 360;
   //   //::i32 iAngle = iStep;
   //   //::i32 iAngle = 1;
   //   //::i32 k = 0;

   //   /*     for ( ::i32 j=jmin; j<jmax; j++ )
   //   {
   //   for ( ::i32 i=imin; i<imax; i++ )
   //   {
   //   ::i32 x, y;

   //   // A Combination of a 2d Translation/rotation/Scale Matrix
   //   x=::i32(cos10(i, iAngle) - sin10(j, iAngle)) + ioff;
   //   y=::i32(sin10(i, iAngle) + cos10(j, iAngle)) + joff;
   //   m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //   pimage->m_pcolorref[abs(y%cy)*cx+abs(x%cx)];
   //   //k++;
   //   }
   //   (j+joff)*cx+(i+ioff)
   //   }*/

   //   ::i32 k = 0;
   //   ::f64 dCos = ::cos(dAngle * dPi / 180.0) * dScale;
   //   ::f64 dSin = ::sin(dAngle * dPi / 180.0) * dScale;
   //   ::i32 cx1 = this->cx - 1;
   //   ::i32 cy1 = this->cy - 1;
   //   for ( ::i32 j=jmin; j<jmax; j++ )
   //   {
   //      for ( ::i32 i=imin; i<imax; i++ )
   //      {
   //         ::i32 x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         //x=abs((::i32(dCos * i - dSin * j) + ioff) % cx);
   //         //y=abs((::i32(dSin * i + dCos * j) + joff) % cy);

   //         x = (::i32) abs((dCos * i - dSin * j) + ioff);
   //         y = (::i32) abs((dSin * i + dCos * j) + joff);

   //         if((x / cx) % 2 == 0)
   //         {
   //            x %= cx;
   //         }
   //         else
   //         {
   //            x = cx1 - (x % cx);
   //         }

   //         if((y / cy) % 2 == 0)
   //         {
   //            y %= cy;
   //         }
   //         else
   //         {
   //            y = cy1 - (y % cy);
   //         }



   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            pimage->get_data()[y * cx + x];
   //         k++;
   //      }
   //   }
   //}

   /*::i32 image::cos(::i32 i, ::i32 iAngle)
   {
   return (::i32) (((_int64) i * CosN[iAngle]) >> 32);
   }

   ::i32 image::sin(::i32 i, ::i32 iAngle)
   {
   return (::i32) (((_int64) i * SinN[iAngle]) >> 32);
   }*/




   /*void image::Fill (::i32 A, ::i32 R, ::i32 G, ::i32 B )
   {
   ::color::color color = rgb ( B, G, R ) | (A << 24);
   ::i32 i32_size=stride*cy;

   ::color::color * pcr;

   map();

   ::i32 iSize32 = size / 32;
   ::i32 i;
   for (i=0; i < iSize32; i+=32 )
   {
   pcr = &m_pcolorref[i];
   pcr[0] = color;
   pcr[1] = color;
   pcr[2] = color;
   pcr[3] = color;
   pcr[4] = color;
   pcr[5] = color;
   pcr[6] = color;
   pcr[7] = color;
   pcr[8] = color;
   pcr[9] = color;
   pcr[10] = color;
   pcr[11] = color;
   pcr[12] = color;
   pcr[13] = color;
   pcr[14] = color;
   pcr[15] = color;
   pcr[16] = color;
   pcr[17] = color;
   pcr[18] = color;
   pcr[19] = color;
   pcr[20] = color;
   pcr[21] = color;
   pcr[22] = color;
   pcr[23] = color;
   pcr[24] = color;
   pcr[25] = color;
   pcr[26] = color;
   pcr[27] = color;
   pcr[28] = color;
   pcr[29] = color;
   pcr[30] = color;
   pcr[31] = color;
   }

   for (i=0; i<i32_size; i++ )
   {
   m_pcolorref[i]=color;
   }

   }*/


   //::color::color image::GetAverageColor()
   //{
   //   ::f64 dR = 0.0;
   //   ::f64 dG = 0.0;
   //   ::f64 dB = 0.0;
   //   ::i32 iRLine;
   //   ::i32 iGLine;
   //   ::i32 iBLine;
   //   ::f64 dDiv = cx * cy;
   //   if(dDiv > 0)
   //   {
   //      ::u8 * lpb = (::u8 *) m_pcolorref;
   //      for (::i32 y = 0; y < cy; y++)
   //      {
   //         iRLine = 0;
   //         iGLine = 0;
   //         iBLine = 0;
   //         for (::i32 x = 0; x < cx; x++)
   //         {
   //            iRLine += lpb[2];
   //            iGLine += lpb[1];
   //            iBLine += lpb[0];
   //            lpb += 4;
   //         }
   //         dR += iRLine / dDiv;
   //         dG += iGLine / dDiv;
   //         dB += iBLine / dDiv;
   //      }
   //      ::i32 iR = (::i32) dR;
   //      ::i32 iG = (::i32) dG;
   //      ::i32 iB = (::i32) dB;
   //      return rgb(iR, iG, iB);
   //   }
   //   else
   //   {
   //      return 0;
   //   }

   //}


   //void image::xor(image image)
   //{
   //   if(cx != pimage->cx
   //      || cy != pimage->cy)
   //   {
   //      return;
   //   }
   //   ::i32 iCount = cx * cy;
   //   LPDWORD lpd1 = (LPDWORD) m_pcolorref;
   //   LPDWORD lpd2 = (LPDWORD) pimage->get_data();
   //   for(::i32 i = 0; i < iCount; i++)
   //   {
   //      *lpd1 = *lpd1 ^ *lpd2;
   //      lpd1++;
   //      lpd2++;
   //   }
   //}

   //void image::create_frame(::i32_size size, ::i32 iFrameCount)
   //{
   //   ::i32 iSliceCount = (::i32) sqrt((::f64) iFrameCount);
   //   ::i32 iFrameWidth = size.cx / iSliceCount;
   //   ::i32 iFrameHeight = size.cy / iSliceCount;
   //   create(iFrameWidth, iFrameHeight);
   //}

   //void image::set_frame1(void * lpdata, ::i32 iFrame, ::i32 iFrameCount)
   //{
   //   ::i32 iSliceCount = (::i32) sqrt((::f64) iFrameCount);
   //   if(iSliceCount == 0)
   //      iSliceCount = 1;
   //   ::i32 iFrameWidth = cx / iSliceCount;
   //   ::i32 iFrameHeight = cy / iSliceCount;
   //   ::i32 iX = iFrame % iSliceCount;
   //   ::i32 iY = iFrame / iSliceCount;
   //   ::color::color * lpDest = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight * cx];
   //   ::color::color * lpSrc = (::color::color *) lpdata;
   //   ::color::color * lpDestLine;
   //   for(::i32 y = 0; y < iFrameHeight; y++)
   //   {
   //      lpDestLine = &lpDest[y * cx];
   //      for(::i32 x = 0; x < iFrameWidth; x++)
   //      {
   //         *lpDestLine = *lpSrc;
   //         lpDestLine++;
   //         lpSrc++;
   //      }
   //   }
   //}

   //void image::set_frame2(void * lpdata, ::i32 iFrame, ::i32 iFrameCount)
   //{
   //   if(lpdata == nullptr)
   //      return;
   //   ::i32 iSliceCount = (::i32) sqrt((::f64) iFrameCount);
   //   if(iSliceCount == 0)
   //      iSliceCount = 1;
   //   ::i32 iFrameWidth = cx / iSliceCount;
   //   ::i32 iFrameHeight = cy / iSliceCount;
   //   ::i32 iX = iFrame % iSliceCount;
   //   ::i32 iY = iFrame / iSliceCount;
   //   ::color::color * lpDest = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight * cx];
   //   ::color::color * lpSrc = (::color::color *) lpdata;
   //   ::color::color * lpDestLine;
   //   for(::i32 y = iFrameHeight - 1; y >= 0; y--)
   //   {
   //      lpDestLine = &lpDest[y * cx];
   //      for(::i32 x = 0; x < iFrameWidth; x++)
   //      {
   //         *lpDestLine = *lpSrc;
   //         lpDestLine++;
   //         lpSrc++;
   //      }
   //   }
   //}

   //void image::xor_image_frame2(void * lpdata, ::i32 iFrame, ::i32 iFrameCount)
   //{
   //   if(lpdata == nullptr)
   //      return;
   //   ::i32 iSliceCount = (::i32) sqrt((::f64) iFrameCount);
   //   if(iSliceCount == 0)
   //      iSliceCount = 1;
   //   ::i32 iFrameWidth = cx / iSliceCount;
   //   ::i32 iFrameHeight = cy / iSliceCount;
   //   ::i32 iX = iFrame % iSliceCount;
   //   ::i32 iY = iFrame / iSliceCount;
   //   ::color::color * lpDest = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight * cx];
   //   ::color::color * lpSrc = (::color::color *) lpdata;
   //   ::color::color * lpDestLine;
   //   for(::i32 y = iFrameHeight - 1; y >= 0; y--)
   //   {
   //      lpDestLine = &lpDest[y * cx];
   //      for(::i32 x = 0; x < iFrameWidth; x++)
   //      {
   //         *lpDestLine ^= *lpSrc;
   //         lpDestLine++;
   //         lpSrc++;
   //      }
   //   }
   //}

   //void image::get_frame(void * lpdata, ::i32 iFrame, ::i32 iFrameCount)
   //{
   //   ::i32 iSliceCount = (::i32) sqrt((::f64) iFrameCount);
   //   ::i32 iFrameWidth = cx / iSliceCount;
   //   ::i32 iFrameHeight = cy / iSliceCount;
   //   ::i32 iX = iFrame % iSliceCount;
   //   ::i32 iY = iFrame / iSliceCount;
   //   ::color::color * lpSrc = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight *  cx];
   //   ::color::color * lpDest = (::color::color *) lpdata;
   //   ::color::color * lpSrcLine;
   //   for(::i32 y = 0; y < iFrameHeight; y++)
   //   {
   //      lpSrcLine = &lpSrc[y * cx];
   //      for(::i32 x = 0; x < iFrameWidth; x++)
   //      {
   //         *lpDest = *lpSrcLine;
   //         lpDest++;
   //         lpSrcLine++;
   //      }
   //   }
   //}

   //bool image::is_rgb_black()
   //{
   //   ::i32 iSize = cx * cy;
   //   ::color::color * lp = m_pcolorref;
   //   for(::i32 i = 0; i < iSize; i++)
   //   {
   //      if((*lp & 0x00FFFFFF) != 0)
   //         return false;
   //      lp++;
   //   }
   //   return true;
   //}

   //void image::DivideRGB(::i32 iDivide)
   //{
   //   if(iDivide == 0)
   //   {
   //      return;
   //   }
   //   ::i32 iCount = cx * cy;
   //   ::u8 * lp = ((::u8 *) m_pcolorref);
   //   ::i32 i = 0;
   //   ::i32 iCount1 = iCount - iCount % 8;
   //   for(; i < iCount1; i++)
   //   {
   //      lp[0] /= (::u8) iDivide;
   //      lp[1] /= (::u8) iDivide;
   //      lp[2] /= (::u8) iDivide;

   //      lp[4] /= (::u8) iDivide;
   //      lp[5] /= (::u8) iDivide;
   //      lp[6] /= (::u8) iDivide;

   //      lp[8] /= (::u8) iDivide;
   //      lp[9] /= (::u8) iDivide;
   //      lp[10] /= (::u8) iDivide;

   //      lp[12] /= (::u8) iDivide;
   //      lp[13] /= (::u8) iDivide;
   //      lp[14] /= (::u8) iDivide;

   //      lp[16] /= (::u8) iDivide;
   //      lp[17] /= (::u8) iDivide;
   //      lp[28] /= (::u8) iDivide;

   //      lp[20] /= (::u8) iDivide;
   //      lp[21] /= (::u8) iDivide;
   //      lp[22] /= (::u8) iDivide;

   //      lp[24] /= (::u8) iDivide;
   //      lp[25] /= (::u8) iDivide;
   //      lp[26] /= (::u8) iDivide;

   //      lp[28] /= (::u8) iDivide;
   //      lp[29] /= (::u8) iDivide;
   //      lp[30] /= (::u8) iDivide;

   //      lp += 4 * 8;
   //   }
   //   for(; i < iCount; i++)
   //   {
   //      lp[0] /= (::u8) iDivide;
   //      lp[1] /= (::u8) iDivide;
   //      lp[2] /= (::u8) iDivide;
   //      lp +=4;
   //   }
   //}

   //void image::DivideARGB(::i32 iDivide)
   //{
   //   if(iDivide == 0)
   //   {
   //      return;
   //   }
   //   ::i32 iCount = cx * cy;
   //   ::u8 * lp = ((::u8 *) m_pcolorref);
   //   for(::i32 i = 0; i < iCount; i++)
   //   {
   //      lp[0] /= (::u8) iDivide;
   //      lp[1] /= (::u8) iDivide;
   //      lp[2] /= (::u8) iDivide;
   //      lp[3] /= (::u8) iDivide;
   //      lp +=4;
   //   }
   //}

   //void image::DivideA(::i32 iDivide)
   //{
   //   if(iDivide == 0)
   //   {
   //      return;
   //   }
   //   ::i32 iCount = cx * cy;
   //   ::u8 * lp = ((::u8 *) m_pcolorref);
   //   for(::i32 i = 0; i < iCount; i++)
   //   {
   //      lp[3] /= (::u8) iDivide;
   //      lp +=4;
   //   }
   //}

   //bool image::_draw_raw(const ::i32_rectangle & rectangleTarget, ::image::image * pimage, const ::i32_point & pointSrc)
   //{

   //   return ::image::draw(rectangleTarget, pimage, pointSrc);

   //}

   //bool image::stretch(const ::image::image * pimage)
   //{

   //   pimage->unmap();

   //   unmap();

   //   if (pimage->get_bitmap() == nullptr)
   //   {

   //      return false;

   //   }

   //   if (pimage->get_bitmap()->m_osdata[0] == nullptr)
   //   {

   //      return false;

   //   }

   //   D2D1_RECT_F rectangleDest = D2D1::RectF(0, 0, (FLOAT)this->width(), (FLOAT)this->height());

   //   D2D1_RECT_F rectangleSource = D2D1::RectF(0, 0, (FLOAT) pimage->width(), (FLOAT) pimage->height());

   //   ((ID2D1RenderTarget *)m_pgraphics->get_os_data())->DrawBitmap(((ID2D1Bitmap1 *) pimage->get_bitmap()->m_osdata[0]), rectangleDest, 1.0, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rectangleSource);

   //   /*
   //   ::StretchDIBits(
   //   SP_HDC(m_pgraphics),
   //   0, 0,
   //   cx, cy,
   //   0, 0,
   //   pimage->cx, pimage->cy,
   //   pimage->get_data(),
   //   &pimage->m_info,
   //   DIB_RGB_COLORS,
   //   SRCCOPY);*/

   //   return true;

   //}

   //::draw2d::graphics * image::_get_graphics() const
   //{

   //   return m_pgraphics;

   //}



   ::image_pixmap_lease image::_map(const ::i32_rectangle & rectangle)
   {

      //::draw2d::lock draw2dlock;

      //::draw2d::device_lock devicelock(this);

      //if (m_bMapped)
      //{

      //   throw ::exception(error_invalid_empty_argument);

      //}

      if (m_pbitmap.is_null())
      {

         throw ::exception(error_invalid_empty_argument);

      }

      //auto pgraphics2d = m_pgraphics.cast < ::draw2d_directx11::graphics>();

      //D2D1_SIZE_U size;

      //size.width = m_size.cx;
      //size.height = m_size.cy;

      //HRESULT hrFlush = pgraphics2d->m_pdevicecontext->Flush();

      //if (FAILED(hrFlush))
      //{

      //   throw ::exception(error_failed);

      //}

      //m_hrEndDraw = pgraphics2d->m_pdevicecontext->EndDraw();

      //if (FAILED(m_hrEndDraw))
      //{

      //   throw ::exception(error_failed);

      //}

      ::cast < ::draw2d_directx11::bitmap> pbitmap = m_pbitmap;

      //D2D1_BITMAP_OPTIONS options = 
      //   D2D1_BITMAP_OPTIONS_CPU_READ |
      //   D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

      //auto props = D2D1::BitmapProperties1(options, pbitmap->GetPixelFormat());

      //HRESULT hr = pgraphics2d->m_pdevicecontext->CreateBitmap(size, nullptr, 0, props, &m_pbitmap1Map);

      //if (FAILED(hr))
      //{

      //   throw ::exception(error_failed);

      //}

      //D2D1_POINT_2U pointDst = {};

      //D2D1_RECT_U srcRect = {};

      //srcRect.right = width();

      //srcRect.bottom = height();

      //hr = m_pbitmap1Map->CopyFromBitmap(&pointDst, pbitmap, &srcRect);

      //D2D1_MAPPED_RECT map_base = {};

      //hr = m_pbitmap1Map->Map(D2D1_MAP_OPTIONS_READ, &map_base);

      //if (FAILED(hr) || map_base.bits == nullptr)
      //{

      //   throw ::exception(error_failed);

      //}

      auto pimage32 = (::image32_t *)pbitmap->m_memory.data();

      //auto p = pimage32;

      auto iScan = pbitmap->m_size.width() *4;

      //auto area = (iScan / sizeof(*pimage32)) * m_size.cy;

      //initialize_pixmap(m_size, pimage32, iScan);

      //m_bMapped = true;

      //return true;

   }


   void image::_unmap(::image_pixmap_lease * pimagepixmaplease)
   {

      //::draw2d::lock draw2dlock;

      //if (!m_bMapped)
      //{

      //   throw ::exception(error_wrong_state);

      //}

      if (m_pbitmap.is_null())
      {

         throw ::exception(error_wrong_state);

      }

      try
      {

         //D2D1_POINT_2U pointDst = {};

         //D2D1_RECT_U srcRect={};

         //srcRect.right = this->width();

         //srcRect.bottom = this->height();

         //auto pbitmap = m_pbitmap->get_os_data < ID2D1Bitmap * >(data_bitmap);

         //auto hr = pbitmap->CopyFromMemory(&srcRect, m_pimage32Raw, m_iScan);

         //m_pbitmap1Map->Unmap();

         //m_pbitmap1Map = nullptr;

         //m_pimage32Raw = nullptr;

         //m_pimage32 = nullptr;

         //if (FAILED(hr))
         //{

         //   m_bMapped = false;

         //   defer_throw_hresult(hr);

         //}

      }
      catch (...)
      {

      }

      //if (SUCCEEDED(m_hrEndDraw))
      //{

      //   ((ID2D1DeviceContext *)m_pgraphics->get_os_data())->BeginDraw();

      //}

      //m_bMapped = false;

      //m_bTrans = false;

      //return true;

   }


   /*   ::i32 image::cx
   {
   return cx;
   }

   ::i32 image::cy
   {
   return cy;
   }*/


   void image::defer_realize(::draw2d::graphics* pgraphics) const
   {

      if (is_realized())
      {
         
         //return true;

         return;

      }

      return realize(pgraphics);

   }


   ////bool image::realize(::draw2d::graphics *) const
   ////{

   ////   ::draw2d::lock draw2dlock;

   ////   if (is_realized())
   ////   {

   ////      unrealize();

   ////   }

   ////   if (is_realized())
   ////   {

   ////      return false;

   ////   }

   ////   if (m_pbitmap.is_null()
   ////         || m_pbitmapMap.is_null()
   ////         || m_pgraphics.is_null()
   ////         || m_pgraphicsMap.is_null())
   ////   {

   ////      return false;

   ////   }

   ////   //::pointer<::draw2d_directx11::graphics>pgraphicsMap = m_pgraphicsMap;

   ////   ::pointer<::draw2d_directx11::graphics>pgraphics = m_pgraphics;

   ////   ::pointer<::draw2d_directx11::bitmap>pbitmap = m_pbitmap;

   ////   pgraphics->m_pbitmaprendertarget = nullptr;

   ////   pgraphics->m_iType = 11;

   ////   ::i32_size size = m_pbitmapMap->GetBitmapDimension();

   ////   D2D1_SIZE_U sizeu = D2D1::SizeU(size.cx, size.cy);

   ////   D2D1_PIXEL_FORMAT pixelformat;

   ////   pixelformat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

   ////   pixelformat.format = DXGI_FORMAT_B8G8R8A8_UNORM;

   ////   auto prendertarget = pgraphicsMap->m_prendertarget;

   ////   HRESULT hr = prendertarget->CreateCompatibleRenderTarget(nullptr, &sizeu, &pixelformat, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE, &pgraphics->m_pbitmaprendertarget);

   ////   if (pgraphics->m_pbitmaprendertarget == nullptr)
   ////   {

   ////      return false;

   ////   }

   ////   if (FAILED(pgraphics->m_pbitmaprendertarget.as(pgraphics->m_prendertarget)))
   ////   {

   ////      pgraphics->m_pbitmaprendertarget = nullptr;

   ////      return false;

   ////   }

   ////   if (FAILED(pgraphics->m_pbitmaprendertarget.as(pgraphics->m_pdevicecontext)))
   ////   {

   ////      pgraphics->m_pbitmaprendertarget = nullptr;

   ////      pgraphics->m_prendertarget = nullptr;

   ////      return false;

   ////   }

   ////   pgraphics->m_pbitmaprendertarget->GetBitmap(&pbitmap->m_pbitmap);

   ////   if (pbitmap->m_pbitmap == nullptr)
   ////   {

   ////      ((image *) this)->m_pgraphics.release();

   ////      return false;

   ////   }

   ////   //pgraphics->m_pplugin = pgraphicsMap->m_pplugin;

   ////   pgraphics->m_pbitmap = pbitmap;

   ////   pbitmap->m_pbitmap.As(&pbitmap->m_pbitmap1);

   ////   pbitmap->m_osdata[0] = pbitmap->m_pbitmap.Get();

   ////   pbitmap->m_osdata[1] = pbitmap->m_pbitmap1.Get();

   ////   pgraphics->m_osdata[0] = pgraphics->m_pdevicecontext.Get();

   ////   pgraphics->m_osdata[1] = pgraphics->m_prendertarget.Get();

   ////   //D2D1_POINT_2U p;

   ////   //p.x = 0;
   ////   //p.y = 0;

   ////   //D2D1_RECT_U srcRect;

   ////   //srcRect.left = 0;
   ////   //srcRect.right = this->width();
   ////   //srcRect.top = 0;
   ////   //srcRect.bottom = this->height();

   ////   return true;

   ////}


   void image::unrealize() const
   {

      //if (!is_realized())
      //   return false;

      ////((ID2D1DeviceContext *)((image *) this)->m_pgraphics->get_os_data())->EndDraw();

      //D2D1_POINT_2U p;

      //p.x = 0;
      //p.y = 0;

      //D2D1_RECT_U srcRect;

      //srcRect.left = 0;
      //srcRect.right = this->width();
      //srcRect.top = 0;
      //srcRect.bottom = this->height();

      //HRESULT hr = ((ID2D1Bitmap *)((image *) this)->m_pbitmap->m_osdata[0])->CopyFromBitmap(&p, ((ID2D1Bitmap *)((image *) this)->m_pbitmap->m_osdata[0]), &srcRect);

      //((image *) this)->m_pgraphics.release();

      //return true;

   }


   bool image::is_realized() const
   {

      //if (((image *) this)->get_graphics2() == nullptr || ((image *) this)->get_graphics2()->get_os_data() == nullptr)
       //  return false;

      return true;

   }

   /*   ::i32 image::scan()
   {

   return scan;

   }*/

   //#if defined(WINDOWS_DESKTOP)


   //bool image::update_window(::aura::draw_interface * puserinteraction, ::message::message * pmessage, bool bTransferBuffer)
   //{

   //   i64_rectangle rectangleWindow;

   //   puserinteraction->window_rectangle(rectangleWindow);

   //   m_pgraphics->SetViewportOrg(0, 0);

   //   map_base(false);

   //   ::i32_rectangle rectangle(rectangleWindow);

   //   //      papplication->window_graphics_update_window(puserinteraction->get_window_graphics(),puserinteraction->get_handle(),m_pcolorref,rectangle,this->width(), this->height(), m_iScan, bTransferBuffer);

   //   return true;

   //}


   //void image::blend(const ::i32_point & pointDst, ::image::image * pimageSrc, const ::i32_point & pointSrc, const ::i32_size & sizeParam, ::u8 bA)
   //{

   //   //return 
   //   
   //   //::image::blend(pointDst, pimageSrc, pointSrc, sizeParam, bA);

   //   //::i32_size size(sizeParam);

   //   //::draw2d::lock draw2dlock;

   //   //try
   //   //{

   //   //   ::draw2d::graphics * pgraphicsMap = pgraphicsImageSrc;

   //   //   if (pgraphicsMap == nullptr)
   //   //   {

   //   //      return false;

   //   //   }

   //   //   if (pgraphicsMap->get_current_bitmap() == nullptr)
   //   //   {

   //   //      return false;

   //   //   }

   //   //   if (pgraphicsMap->get_current_bitmap()->get_os_data() == nullptr)
   //   //   {

   //   //      return false;

   //   //   }

   //   //   if (get_graphics() != nullptr &&
   //   //         get_graphics()->get_current_bitmap() != nullptr &&
   //   //         get_graphics()->get_current_bitmap()->get_os_data() != nullptr)
   //   //   {

   //   //      D2D1_SIZE_U sz = ((ID2D1Bitmap *)get_graphics()->get_current_bitmap()->get_os_data())->GetPixelSize();

   //   //      if (natural(size.cx + pointDst.x) > sz.width)
   //   //         size.cx = sz.width - pointDst.x;

   //   //      if (natural(size.cy + pointDst.y) > sz.height)
   //   //         size.cy = sz.height - pointDst.y;

   //   //   }

   //   //   bool bSmallerSourceRegion = false;

   //   //   ::i32_size sizeSource;

   //   //   {

   //   //      D2D1_SIZE_U sz = ((ID2D1Bitmap *)pgraphicsMap->get_current_bitmap()->get_os_data())->GetPixelSize();

   //   //      sizeSource.cx = sz.width;

   //   //      sizeSource.cy = sz.height;

   //   //      if (natural(size.cx + pointSrc.x) > sz.width)
   //   //      {

   //   //         size.cx = sz.width - pointSrc.x;

   //   //         bSmallerSourceRegion = true;

   //   //      }

   //   //      if (natural(size.cy + pointSrc.y) < sz.height)
   //   //      {

   //   //         size.cy = sz.height - pointSrc.y;

   //   //         bSmallerSourceRegion = true;

   //   //      }

   //   //   }

   //   //   if (pointDst.x > 0 || pointDst.y > 0)
   //   //   {

   //   //      bSmallerSourceRegion = true;

   //   //   }

   //   //   D2D1_RECT_F rectangleTarget = D2D1::RectF((::f32)pointDst.x, (::f32)pointDst.y, (::f32)(pointDst.x + size.cx), (::f32)(pointDst.y + size.cy));

   //   //   D2D1_RECT_F rectangleSource = D2D1::RectF((::f32)pointSrc.x, (::f32)pointSrc.y, (::f32)(pointSrc.x + size.cx), (::f32)(pointSrc.y + size.cy));

   //   //   HRESULT hr = ((ID2D1DeviceContext *)pgraphicsMap->get_os_data())->EndDraw();

   //   //   ::draw2d_directx11::graphics * pgraphics = dynamic_cast <::draw2d_directx11::graphics *> (get_graphics());

   //   //   pgraphics->set_alpha_mode(::draw2d::e_alpha_mode_set);

   //   //   {

   //   //      D2D1_POINT_2F p;
   //   //      p.x = rectangleTarget.left;
   //   //      p.y = rectangleTarget.top;

   //   //      pgraphics->m_pdevicecontext->DrawImage((ID2D1Bitmap *)pgraphicsMap->get_current_bitmap()->get_os_data(), p, rectangleSource, pgraphics->m_interpolationmode, D2D1_COMPOSITE_MODE_DESTINATION_IN);

   //   //   }

   //   //   if(bSmallerSourceRegion)
   //   //   {

   //   //      ::draw2d::savedc k1(pgraphics);

   //   //      ::i32_rectangle rDst;
   //   //      rDst.left = pointDst.x;
   //   //      rDst.top = pointDst.y;
   //   //      rDst.right = pointDst.x + size.cx;
   //   //      rDst.bottom = pointDst.y + size.cy;

   //   //      pgraphics->ExcludeClipRect(rDst);

   //   //      D2D1_RECT_F r1;
   //   //      r1.left = (FLOAT) (pointDst.x);
   //   //      r1.top = (FLOAT)(pointDst.y);
   //   //      r1.right = (FLOAT)(pointDst.x + sizeParam.cx);
   //   //      r1.bottom = (FLOAT)(pointDst.y + sizeParam.cy);

   //   //      D2D1_RECT_F r2;
   //   //      r2.left = (FLOAT)(pointSrc.x);
   //   //      r2.top = (FLOAT)(pointSrc.y);
   //   //      r2.right = (FLOAT)(pointSrc.x + sizeParam.cx);
   //   //      r2.bottom = (FLOAT)(pointSrc.y + sizeParam.cy);

   //   //      pgraphics->m_pdevicecontext->DrawBitmap((ID2D1Bitmap *)pgraphicsMap->get_current_bitmap()->get_os_data(), r1,bA / 255.0f, pgraphics->m_interpolationmode, r2);

   //   //   }
   //   //   else
   //   //   {

   //   //      output_debug_string("opt.out.exc.draw");

   //   //   }

   //   //   //hr = m_prendertarget->Flush();
   //   //   pgraphics->flush();

   //   //   if (SUCCEEDED(hr))
   //   //   {

   //   //      ((ID2D1DeviceContext *)pgraphicsMap->get_os_data())->BeginDraw();

   //   //      //dynamic_cast <::draw2d_directx11::graphics *> (pgraphicsMap)->RestoreClip();

   //   //   }

   //   //   return true;

   //   //}
   //   //catch (...)
   //   //{
   //   //   return false;
   //   //}

   //}


   //void image::tint(::image::image * pimage, ::color::color color)
   //{

   //   //return 
   //   
   //   ::image::image::tint(pimage, color);

   //   //if (pimage.nok()) // || pimage->get_bitmap() || !pimage->get_bitmap()->m_osdata[0])
   //   //{

   //   //   return false;

   //   //}

   //   //if (!create(pimage->get_size()))
   //   //{

   //   //   return false;

   //   //}

   //   //::i32_rectangle rectangleDib1(::i32_point(), pimage->get_size());

   //   //fill(a_rgb(255, rgb));

   //   //auto pgraphicsDib1 = __graphics(get_graphics());

   //   //auto pgraphicsDib2 = __graphics(pgraphicsImage);

   //   //pgraphicsDib2->m_pdevicecontext->EndDraw();

   //   //pgraphicsDib1->m_pdevicecontext->DrawImage(
   //   //pimage->get_bitmap()->get_os_data< ID2D1Bitmap>(),
   //   //D2D1::Point2F(0.f, 0.f),
   //   //d2d1::f32_rectangle(rectangleDib1),
   //   //D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
   //   //D2D1_COMPOSITE_MODE_SOU_IN);

   //   //set_alpha_mode(::draw2d::e_alpha_mode_blend);

   //   //pgraphicsDib2->m_pdevicecontext->BeginDraw();

   //   //return true;

   //}


} // namespace draw2d_directx11



