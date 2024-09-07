#include "framework.h"
#include "context.h"
#include "acme/exception/exception.h"
#include "aura/graphics/image/frame_array.h"
#include "acme_windows_common/comptr.h"


#include "acme/_operating_system.h"
#include <Wincodec.h>

::color::color WICColor_color(WICColor color)
{

   // 0xAARRGGBB WICColor

   return argb(((::u8 *)&color)[3], ((::u8 *)&color)[2], ((::u8 *)&color)[1], ((::u8 *)&color)[0]);

}

#include <math.h>


#ifdef UNIVERSAL_WINDOWS
#include <ShCore.h>
#endif

//namespac
//
//CLASS_DECL_ACME void cra_from_quada(color_array& colora, WINRGBQUAD* prgbquad, int iCount);


namespace imaging_wic
{

   comptr < IWICImagingFactory > get_imaging_factory();

   //bool draw2d_gif_load_frame(::image::image * pimageCanvas, image_frame_array * pframea, ::image::image_frame * pframe, int uFrameIndex, u8 * ba, int iScan, color_array & colora, int transparentIndex);
   bool windows_image_from_bitmap_source(::image::image * pimage, IWICBitmapSource * pbitmapsource, IWICImagingFactory * pimagingfactory);
   ::color::color windows_image_metadata_get_background_color(IWICMetadataQueryReader * pqueryreader, IWICBitmapDecoder * pbitmapdecoder, IWICImagingFactory * pimagingfactory);


   void image_context::_load_image(::image::image * pimageCompose, ::pointer<::image::image_frame_array>& pframea, memory & memory)
   {

      HRESULT hr = E_FAIL;

      {

         auto pimagingfactory = get_imaging_factory();

         comptr < IWICStream > pwicstream;

         hr = pimagingfactory->CreateStream(&pwicstream);

         if (FAILED(hr))
         {
            
            throw ::exception(error_resource);

         }

         hr = pwicstream->InitializeFromMemory(memory.data(), (DWORD) memory.size());

         if (FAILED(hr))
         {

            throw ::exception(error_resource);

         }

         comptr < IWICBitmapDecoder > pbitmapdecoder;

         // jpeg,png:OK, bmp:88982f50
         // "bmp:88982f50 results in error, icon also errors"(TranslatedFromJapanese)
         hr = pimagingfactory->CreateDecoderFromStream(pwicstream, 0, WICDecodeMetadataCacheOnLoad, &pbitmapdecoder); 

         if (FAILED(hr))
         {

            throw ::exception(error_resource);

         }

         ::collection::count cFrame;

         {

            ::u32 uCount = 0;

            hr = pbitmapdecoder->GetFrameCount(&uCount);

            if (FAILED(hr))
            {

               throw ::exception(error_resource);

            }

            cFrame = uCount;

         }

         
         comptr < IWICMetadataQueryReader > pimetadataqueryreader;

         hr = pbitmapdecoder->GetMetadataQueryReader(&pimetadataqueryreader);

         if (FAILED(hr))
         {

            throw ::exception(error_resource);

         }

         PROPVARIANT propValue;

         PropVariantInit(&propValue);

         hr = pimetadataqueryreader->GetMetadataByName(L"/logscrdesc/Width", &propValue);

         if (SUCCEEDED(hr))
         {

            hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

            if (SUCCEEDED(hr))
            {

               pframea->m_size.cx() = propValue.uiVal;

            }

         }

         PropVariantClear(&propValue);

         if (FAILED(hr))
         {

            throw ::exception(error_resource);

         }

         PropVariantInit(&propValue);

         hr = pimetadataqueryreader->GetMetadataByName(L"/logscrdesc/Height", &propValue);

         if (SUCCEEDED(hr))
         {

            hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

            if (SUCCEEDED(hr))
            {

               pframea->m_size.cy() = propValue.uiVal;

            }

         }

         PropVariantClear(&propValue);

         if (FAILED(hr))
         {

            throw ::exception(error_resource);

         }

         pframea->m_colorBack = windows_image_metadata_get_background_color(pimetadataqueryreader, pbitmapdecoder, pimagingfactory);

         pframea->set_size(cFrame);

         ::image::image_pointer pimageFrame;

         for (::collection::index iFrame = 0; iFrame < cFrame; iFrame++)
         {

            auto & pframe = pframea->element_at(iFrame);

            __defer_construct_new(pframe);

            //auto estatus =
            
            __construct(pframe->m_pimage);

            //if (!estatus)
            //{

            //   throw ::exception(error_resource);

            //}

            pframe->m_iFrame = iFrame;

            __defer_construct(pimageFrame);

            //estatus = 
            
            pimageFrame->create(pframea->m_size, e_flag_none);

            //if (!estatus)
            //{

            //   throw ::exception(error_resource);

            //}

            // pframe->m_pimage->fill(0);

            pimageFrame->clear(color::transparent);

            {

               comptr<IWICFormatConverter> pformatconverter;

               comptr<IWICBitmapFrameDecode> pframedecode;

               comptr<IWICMetadataQueryReader > pqueryreader;

               HRESULT hr = pbitmapdecoder->GetFrame((::u32)iFrame, &pframedecode);

               if (SUCCEEDED(hr))
               {

                  hr = pimagingfactory->CreateFormatConverter(&pformatconverter);

               }

               if (SUCCEEDED(hr))
               {

                  hr = pformatconverter->Initialize(
                     pframedecode,
                     GUID_WICPixelFormat32bppPBGRA,
                     WICBitmapDitherTypeNone,
                     nullptr,
                     0.f,
                     WICBitmapPaletteTypeCustom);
               }

               if (SUCCEEDED(hr))
               {

                  hr = windows_image_from_bitmap_source(pimageFrame, pformatconverter, pimagingfactory) ? S_OK : E_FAIL;

               }

               if (SUCCEEDED(hr))
               {

                  hr = pframedecode->GetMetadataQueryReader(&pqueryreader);

               }

               if (SUCCEEDED(hr))
               {

                  PropVariantInit(&propValue);

                  hr = pqueryreader->GetMetadataByName(L"/imgdesc/Left", &propValue);

                  if (SUCCEEDED(hr))
                  {

                     hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

                     if (SUCCEEDED(hr))
                     {

                        pframe->m_rectangle.left() = static_cast<::i32>(propValue.uiVal);

                     }

                  }

                  PropVariantClear(&propValue);

               }

               if (SUCCEEDED(hr))
               {

                  PropVariantInit(&propValue);

                  hr = pqueryreader->GetMetadataByName(L"/imgdesc/Top", &propValue);

                  if (SUCCEEDED(hr))
                  {

                     hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

                     if (SUCCEEDED(hr))
                     {

                        pframe->m_rectangle.top() = static_cast<::i32>(propValue.uiVal);

                     }

                  }

                  PropVariantClear(&propValue);

               }

               if (SUCCEEDED(hr))
               {

                  PropVariantInit(&propValue);

                  hr = pqueryreader->GetMetadataByName(L"/imgdesc/Width", &propValue);

                  if (SUCCEEDED(hr))
                  {

                     hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

                     if (SUCCEEDED(hr))
                     {

                        pframe->m_rectangle.right() = static_cast<::i32>(propValue.uiVal) + pframe->m_rectangle.left();

                     }

                  }

                  PropVariantClear(&propValue);

               }

               if (SUCCEEDED(hr))
               {

                  PropVariantInit(&propValue);

                  hr = pqueryreader->GetMetadataByName(L"/imgdesc/Height", &propValue);

                  if (SUCCEEDED(hr))
                  {

                     hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

                     if (SUCCEEDED(hr))
                     {

                        pframe->m_rectangle.bottom() = static_cast<::i32>(propValue.uiVal) + pframe->m_rectangle.top();

                     }

                  }

                  PropVariantClear(&propValue);

               }

               if (SUCCEEDED(hr))
               {

                  PropVariantInit(&propValue);

                  // Get delay from the optional Graphic Control Extension
                  if (SUCCEEDED(pqueryreader->GetMetadataByName(L"/grctlext/Delay", &propValue)))
                  {

                     hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

                     if (SUCCEEDED(hr))
                     {

                        // Convert the delay retrieved in 10 ms units to a delay in 1 ms units
                        ::u32 uDelayMilliseconds;

                        hr = UIntMult(propValue.uiVal, 10, &uDelayMilliseconds);

                        pframe->m_time = millisecond_time(uDelayMilliseconds);

                     }

                  }
                  else
                  {

                     // Failed to get delay from graphic control extension. Possibly a
                     // single frame image (non-animated gif)
                     pframe->m_time = 0_s;

                  }

                  if (SUCCEEDED(hr))
                  {

                     // Insert an artificial delay to ensure rendering for gif with very small
                     // or 0 delay.  This delay number is picked to match with most browsers'
                     // gif display speed.
                     //
                     // This will defeat the purpose of using zero delay intermediate frames in
                     // order to preserve compatibility. If this is erased, the zero delay
                     // intermediate frames will not be visible.
                     if (pframe->m_time < 20_ms)
                     {

                        pframe->m_time = 90_ms;

                     }

                  }

                  PropVariantClear(&propValue);

               }

               if (SUCCEEDED(hr))
               {

                  PropVariantInit(&propValue);

                  int iDisposal = -1;

                  if (SUCCEEDED(pqueryreader->GetMetadataByName(
                     L"/grctlext/Disposal",
                     &propValue)))
                  {
                     hr = (propValue.vt == VT_UI1) ? S_OK : E_FAIL;
                     if (SUCCEEDED(hr))
                     {
                        iDisposal = propValue.bVal;
                     }
                  }
                  else
                  {
                     // Failed to get the disposal method, use default. Possibly a
                     // non-animated gif.
                     //iDisposal
                  }

                  pframe->gif_set_disposal(iDisposal);

                  PropVariantClear(&propValue);

               }

               if (SUCCEEDED(hr))
               {

                  pimageFrame->set_ok_flag();

               }

            }

            //::draw2d::enum_disposal edisposal = edisposaliFrame <= 0 ? ::draw2d::e_disposal_none : pframea->element_at(iFrame - 1)->m_edisposal;

            //pframe->m_edisposal = edisposal;

            if (pimageFrame.ok())
            {

               pframe->_001Process(pimageCompose, pimageFrame, pframea);

            }

         }

      }

      //return SUCCEEDED(hr);

   }


   //#ifdef WINDOWS

   /******************************************************************
   *                                                                 *
   *  DemoApp::get_background_color()                                  *
   *                                                                 *
   *  Reads and stores the background color for gif.                 *
   *                                                                 *
   ******************************************************************/


   bool windows_load_image_from_frame(
      ::image::image * pimage,
      comptr< IWICBitmapFrameDecode> & pframe,
      IWICImagingFactory * pimagingfactory,
      IWICBitmapDecoder * pbitmapdecoder,
      int iFrame)
   {

      try
      {

         HRESULT hr = pbitmapdecoder->GetFrame(iFrame, &pframe);

         WICPixelFormatGUID px;
         zero(px);
         if (!pframe)
         {
            return false;
         }
         if (FAILED(hr))
         {
            return false;
         }

         hr = pframe->GetPixelFormat(&px);
         if (FAILED(hr))
         {
            return false;
         }


         if (px == GUID_WICPixelFormat32bppRGBA)
         {

            ::u32 width = 0;

            ::u32 height = 0;

            pframe->GetSize(&width, &height);

            pimage->create({(i32) width, (i32)height });

            pimage->map();

            hr = pframe->CopyPixels(nullptr, pimage->scan_size(), pimage->scan_size() * height, (::u8 *)pimage->data());

   #ifdef UNIVERSAL_WINDOWS
            //pimage->mult_alpha();
   #endif

         }
         else if (px == GUID_WICPixelFormat32bppBGRA)
         {

            ::u32 width = 0;

            ::u32 height = 0;

            pframe->GetSize(&width, &height);

            pimage->create(::size_i32(width, height));

            pimage->map();

            hr = pframe->CopyPixels(nullptr, pimage->scan_size(), pimage->scan_size() * height, (::u8 *)pimage->data());
   #ifdef UNIVERSAL_WINDOWS
            //pimage->mult_alpha();
   #endif

         }
         else
         {

            comptr<IWICFormatConverter> pbitmap;

            if (SUCCEEDED(hr))
            {

               hr = pimagingfactory->CreateFormatConverter(&pbitmap);

            }

            px = GUID_WICPixelFormat32bppBGRA;

            if (SUCCEEDED(hr))
            {

               hr = pbitmap->Initialize(pframe, px, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);
            }

            //Step 4: Create render target and D2D bitmap from IWICBitmapSource
            ::u32 width = 0;
            ::u32 height = 0;
            if (SUCCEEDED(hr))
            {
               hr = pbitmap->GetSize(&width, &height);
            }

            pimage->create({ (i32)width, (i32)height });

            pimage->map();

            hr = pbitmap->CopyPixels(nullptr, pimage->scan_size(), pimage->scan_size() * height, (::u8 *)pimage->data());


         }



      }
      catch (...)
      {
         return false;
      }

      return true;

   }



   HRESULT windows_image_get_frame(::image::image * pimageCompose,
      WICColor * rgColors,
      int iUsed,
      ::image::image_frame_array * pframea,
      IWICImagingFactory * pimagingfactory,
      IWICBitmapDecoder * pbitmapdecoder,
      ::u32 uFrameIndex)
   {

      ::pointer<::image::image_frame>pframe = pframea->element_at(uFrameIndex);

      comptr < IWICBitmapFrameDecode >    pbitmapframedecode;

      comptr < IWICMetadataQueryReader >  pqueryreader;

      // Retrieve the current frame
      HRESULT hr = pbitmapdecoder->GetFrame(uFrameIndex, &pbitmapframedecode);

      comptr < IWICFormatConverter > pbitmap;

      if (SUCCEEDED(hr))
      {

         hr = pimagingfactory->CreateFormatConverter(&pbitmap);

      }

      if (SUCCEEDED(hr))
      {

         hr = pbitmap->Initialize(
            pbitmapframedecode,
            GUID_WICPixelFormat8bppIndexed,
            WICBitmapDitherTypeNone,
            nullptr,
            0.f,
            WICBitmapPaletteTypeCustom);

      }

      ::u32 width = 0;

      ::u32 height = 0;

      if (SUCCEEDED(hr))
      {

         hr = pbitmap->GetSize(&width, &height);

      }

      pframe->m_pimage->create({ (i32)width, (i32)height });

      pframe->m_pimage->map();

      byte_array ba;

      ba.set_size((memsize)pframe->m_pimage->area());

      hr = pbitmap->CopyPixels(nullptr, pframe->m_pimage->width(), (::u32)ba.size(), (::u8 *)ba.data());

      if (FAILED(hr))
      {

         return false;

      }

      hr = pbitmapframedecode->GetMetadataQueryReader(&pqueryreader);

      PROPVARIANT propValue;

      if (SUCCEEDED(hr))
      {

         PropVariantInit(&propValue);

         hr = pqueryreader->GetMetadataByName(L"/imgdesc/Left", &propValue);

         if (SUCCEEDED(hr))
         {

            hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

            if (SUCCEEDED(hr))
            {

               pframe->m_rectangle.left() = propValue.uiVal;

            }

         }

         PropVariantClear(&propValue);

      }

      if (SUCCEEDED(hr))
      {

         PropVariantInit(&propValue);

         hr = pqueryreader->GetMetadataByName(L"/imgdesc/Top", &propValue);

         if (SUCCEEDED(hr))
         {

            hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

            if (SUCCEEDED(hr))
            {

               pframe->m_rectangle.top() = propValue.uiVal;

            }

         }

         PropVariantClear(&propValue);

      }

      if (SUCCEEDED(hr))
      {

         PropVariantInit(&propValue);

         hr = pqueryreader->GetMetadataByName(L"/imgdesc/Width", &propValue);

         if (SUCCEEDED(hr))
         {

            hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

            if (SUCCEEDED(hr))
            {

               pframe->m_rectangle.right() = propValue.uiVal + pframe->m_rectangle.left();

            }

         }

         PropVariantClear(&propValue);

      }

      if (SUCCEEDED(hr))
      {

         PropVariantInit(&propValue);

         hr = pqueryreader->GetMetadataByName(L"/imgdesc/Height", &propValue);

         if (SUCCEEDED(hr))
         {

            hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

            if (SUCCEEDED(hr))
            {

               pframe->m_rectangle.bottom() = propValue.uiVal + pframe->m_rectangle.top();

            }

         }

         PropVariantClear(&propValue);

      }

      if (SUCCEEDED(hr))
      {

         PropVariantInit(&propValue);

         // Get delay from the optional Graphic Control Extension
         if (SUCCEEDED(pqueryreader->GetMetadataByName(
            L"/grctlext/Delay",
            &propValue)))
         {

            hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);

            if (SUCCEEDED(hr))
            {

               ::u32 uMilliseconds;

               // Convert the delay retrieved in 10 ms units to a delay in 1 ms units
               hr = UIntMult(propValue.uiVal, 10, &uMilliseconds);

               pframe->m_time = millisecond_time(uMilliseconds);

            }

         }
         else
         {

            // Failed to get delay from graphic control extension. Possibly a
            // single frame image (non-animated gif)
            pframe->m_time = 0_s;

         }

         PropVariantClear(&propValue);

         if (pframe->m_time <= 0_s)
         {

            output_debug_string("0 delay");

         }

         if (SUCCEEDED(hr))
         {
            // Insert an artificial delay to ensure rendering for gif with very small
            // or 0 delay.  This delay number is picked to match with most browsers'
            // gif display speed.
            //
            // This will defeat the purpose of using zero delay intermediate frames in
            // order to preserve compatibility. If this is erased, the zero delay
            // intermediate frames will not be visible.
            //if (pframe->m_tick < 90)
            {
               // pframe->m_tick = 90;
            }

         }

      }

      if (SUCCEEDED(hr))
      {

         PropVariantInit(&propValue);

         if (SUCCEEDED(pqueryreader->GetMetadataByName(
            L"/grctlext/Disposal",
            &propValue)))
         {

            hr = (propValue.vt == VT_UI1) ? S_OK : E_FAIL;

            if (SUCCEEDED(hr))
            {

               pframe->m_edisposal = (::draw2d::enum_disposal) propValue.bVal;

            }

         }
         else
         {

            // Failed to get the disposal method, use default. Possibly a
            // non-animated gif.
            pframe->m_edisposal = ::draw2d::e_disposal_undefined;

         }

         PropVariantClear(&propValue);

      }

      int transparentIndex = -1;

      hr = S_OK;

      if (SUCCEEDED(hr))
      {

         PropVariantInit(&propValue);

         hr = pqueryreader->GetMetadataByName(
            L"/grctlext/TransparencyFlag",
            &propValue);

         if (SUCCEEDED(hr))
         {

            hr = (propValue.vt != VT_BOOL) ? E_FAIL : S_OK;

            if (SUCCEEDED(hr))
            {

               pframe->m_bTransparent = propValue.boolVal != false;

            }

         }

         PropVariantClear(&propValue);

      }

      if (pframe->m_bTransparent)
      {

         if (SUCCEEDED(hr))
         {

            PropVariantInit(&propValue);

            hr = pqueryreader->GetMetadataByName(
               L"/grctlext/TransparentColorIndex",
               &propValue);

            if (SUCCEEDED(hr))
            {

               hr = (propValue.vt != VT_UI1) ? E_FAIL : S_OK;

               if (SUCCEEDED(hr))
               {

                  transparentIndex = propValue.bVal;

               }

            }

            PropVariantClear(&propValue);

         }

      }

      if (SUCCEEDED(hr) && pframe->m_bTransparent)
      {

         hr = (transparentIndex >= pframea->m_colora.get_count()) ? E_FAIL : S_OK;

      }

      if (SUCCEEDED(hr))
      {

         pframe->m_colorTransparent = pframea->m_colora[transparentIndex];

      }
      else
      {

         pframe->m_colorTransparent = argb(255, 255, 255, 255);

      }

      color_array colora;

      ::draw2d::colora_from_quada(colora, (WINRGBQUAD *)rgColors, iUsed);

      if (uFrameIndex == 0)
      {

         if (transparentIndex >= 0)
         {

            pimageCompose->clear(color::transparent);

         }
         else
         {

            pimageCompose->clear(pframea->m_colorBack);


         }

      }

      //if (!draw2d_gif_load_frame(pimageCompose, pframea, pframe, uFrameIndex, (u8 *)ba.data(), width, colora, transparentIndex))
      //{

      //   return false;

      //}

      return true;

   }







   ::color::color windows_image_metadata_get_background_color(IWICMetadataQueryReader * pqueryreader, IWICBitmapDecoder * pbitmapdecoder, IWICImagingFactory * pimagingfactory)
   {

      ::u32 dwBGColor;
      ::u8 backgroundIndex = 0;
      WICColor rgColors[256];
      ::u32 cColorsCopied = 0;
      PROPVARIANT propVariant;
      comptr < IWICPalette > pWicPalette;

      ::color::color crBk = color::transparent;

      PropVariantInit(&propVariant);
      // If we have a global palette, get the palette and background color
      HRESULT hr = pqueryreader->GetMetadataByName(L"/logscrdesc/GlobalColorTableFlag", &propVariant);
      if (SUCCEEDED(hr))
      {
         hr = (propVariant.vt != VT_BOOL || !propVariant.boolVal) ? E_FAIL : S_OK;
      }

      PropVariantClear(&propVariant);


      if (SUCCEEDED(hr))
      {
         PropVariantInit(&propVariant);
         // Background color index
         hr = pqueryreader->GetMetadataByName(L"/logscrdesc/BackgroundColorIndex", &propVariant);
         if (SUCCEEDED(hr))
         {
            hr = (propVariant.vt != VT_UI1) ? E_FAIL : S_OK;
            if (SUCCEEDED(hr))
            {
               backgroundIndex = propVariant.bVal;
            }
         }

         PropVariantClear(&propVariant);

      }


      // Get the color from the palette
      if (SUCCEEDED(hr))
      {
         hr = pimagingfactory->CreatePalette(&pWicPalette);
      }

      if (SUCCEEDED(hr))
      {
         // Get the global palette
         hr = pbitmapdecoder->CopyPalette(pWicPalette);
      }

      if (SUCCEEDED(hr))
      {
         hr = pWicPalette->GetColors(
            ARRAYSIZE(rgColors),
            rgColors,
            &cColorsCopied);
      }

      if (SUCCEEDED(hr))
      {
         // Check whether background color is outside range
         hr = (backgroundIndex >= cColorsCopied) ? E_FAIL : S_OK;
      }

      if (SUCCEEDED(hr))
      {
         // Get the color in argb format
         dwBGColor = rgColors[backgroundIndex];

         // The background color is in argb format, and we want to
         // extract the alpha value and convert it to float
         crBk = WICColor_color(dwBGColor);
      }

      return crBk;

   }


} // namespace imaging_wic



