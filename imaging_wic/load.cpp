#include "platform.h"
#include "context.h"
#include "acme/filesystem/file/file.h"
#include "apex/parallelization/handler_manager.h"
#include "aura/graphics/image/load_image.h"
#include "aura/graphics/image/encoding_options.h"
#include "acme/operating_system/windows_common/com/comptr.h"
#include "acme/operating_system/windows_common/com/bstring.h"


#include "acme/_operating_system.h"
#include <wincodec.h>

#include <wincodec.h>
#ifdef UNIVERSAL_WINDOWS
#include <ShCore.h>
#endif



#ifdef UNIVERSAL_WINDOWS
#include <shcore.h>
#include "acme_universal_windows/_winrt_stream.h"
#include "_winrt_applicationmodel_datatransfer.h"
#endif

#include <gdiplus.h>
//#include <wincodec.h>
//#include <windows.h>
//#include <wrl/client.h>

//#include <cstdint>
//#include <limits>
//#include <memory>
//#include <vector>

#include <gdiplus.h>

#include <cstddef>
#include <cstdlib>



namespace imaging_wic
{

   comptr < IWICImagingFactory > get_imaging_factory();

   bool windows_load_image_from_bitmap_source(::image::load_image * ploadimage, IWICBitmapSource * pbitmapsource, IWICImagingFactory * pimagingfactory);


   void image_context::_load_image(::image::load_image * ploadimage, const ::payload & payloadFile, const ::image::load_options & loadoptions)
   {

      //auto ploadimage = allocateø::image::load_image(this);

      //auto estatus = 

      //ploadimage->initialize(this);

      /*if (!estatus)
      {

         return estatus;

      }*/

      //ploadimage->m_pimage = pimageParam;

      //if (ploadimage->m_iRedLower < 0)
      {

         ploadimage->m_iRedLower = 0;

      }

      ploadimage->m_estatus = error_failed;

      ploadimage->set_nok();

      ploadimage->m_payload = payloadFile;

      if (loadoptions.functionLoaded)
      {

         if (ploadimage->m_loadoptions.functionLoaded)
         {

            auto function1 = ploadimage->m_loadoptions.functionLoaded;

            auto function2 = loadoptions.functionLoaded;

            ploadimage->m_loadoptions.functionLoaded = [function1, function2](::image::load_image * ploadimage)
            {

               function1(ploadimage);

               function2(ploadimage);

            };

         }
         else
         {

            ploadimage->m_loadoptions.functionLoaded = loadoptions.functionLoaded;

         }

      }

      ploadimage->m_bCreateHelperMaps = loadoptions.helper_maps;

      auto filepath = payloadFile.as_file_path();

      //if (!filepath.case_insensitive_ends(".webp"))
      //{

      //   return;

      //}

      if (filepath.case_insensitive_begins("http:/") || filepath.case_insensitive_begins("https:/"))
      {

         m_pmanagerImageLoadSlowQueue->handle(loadoptions.sync, { e_timeout, 1_minute, ploadimage});

      }
      else
      {

         m_pmanagerImageLoadFastQueue->handle(loadoptions.sync, { e_timeout, 15_s, ploadimage });

      }

      //return ploadimage->m_estatus;

   }


   //   void image_context::_load_pixmap(::pixmap *ppixmapParam, const ::payload &payloadFile,
   //                                const ::image::load_options &loadoptions)
   //{

   //   auto ploadimage = allocateø::image::load_image(this);

   //   // auto estatus =

   //   ploadimage->initialize(this);

   //   /*if (!estatus)
   //   {

   //      return estatus;

   //   }*/

   //   ploadimage->m_ppixmap = ppixmapParam;

   //   ploadimage->m_ppixmap->m_estatus = error_failed;

   //   ploadimage->m_ppixmap->set_nok();

   //   ploadimage->m_payload = payloadFile;

   //   ploadimage->m_functionLoaded = loadoptions.functionLoaded;

   //   //ppixmapParam->m_bCreateHelperMaps = loadoptions.helper_maps;

   //   auto filepath = payloadFile.as_file_path();

   //   // if (!filepath.case_insensitive_ends(".webp"))
   //   //{

   //   //   return;

   //   //}

   //   if (filepath.case_insensitive_begins("http:/") || filepath.case_insensitive_begins("https:/"))
   //   {

   //      m_pmanagerImageLoadSlowQueue->handle(loadoptions.sync, {e_timeout, 1_minute, ploadimage});
   //   }
   //   else
   //   {

   //      m_pmanagerImageLoadFastQueue->handle(loadoptions.sync, {e_timeout, 15_s, ploadimage});
   //   }

   //   // return ploadimage->m_estatus;
   //}

   void image_context::_os_load_image(::image::load_image * ploadimage, memory & memory)
   {

      ploadimage->m_estatus = ::error_failed;

      ploadimage->set_nok();

      comptr < IWICImagingFactory > pimagingfactory;

      pimagingfactory = get_imaging_factory();

      comptr < IWICStream > piStream;

      HRESULT hr = pimagingfactory->CreateStream(&piStream);

      if (FAILED(hr))
      {

         return;

      }

      const_char_pointer pszData = (const_char_pointer )memory.data();

      auto size = memory.size();

      if(::is_null(pszData) || size <= 0)
      {

         return;

      }

      auto dwSize = (DWORD) size;

      hr = piStream->InitializeFromMemory((WICInProcPointer) pszData, dwSize);

      if (FAILED(hr))
      {

         return;

      }

      comptr < IWICBitmapDecoder > piDecoder;

      // jpeg,png:OK, bmp:88982f50
      // "bmp:88982f50 results in error, icon also errors"(TranslatedFromJapanese)
      hr = pimagingfactory->CreateDecoderFromStream(piStream, 0, WICDecodeMetadataCacheOnLoad, &piDecoder);

      if (FAILED(hr))
      {

         return;

      }

      comptr < IWICBitmapFrameDecode > pframedecode;

      hr = piDecoder->GetFrame(0, &pframedecode);

      if (FAILED(hr))
      {

         return;

      }

      ::i32 iOrientation = -1;

      {

         HRESULT hrExif = hr;

         comptr<IWICMetadataQueryReader> pqueryreader;

         if (SUCCEEDED(hrExif))
         {

            hrExif = pframedecode->GetMetadataQueryReader(&pqueryreader);

         }

         PROPVARIANT value;

         if (SUCCEEDED(hrExif))
         {

            PropVariantInit(&value);

            bstring bstr("/app1/ifd/{ushort=274}");

            hrExif = pqueryreader->GetMetadataByName(bstr.c_str(), &value);

            if (FAILED(hrExif))
            {

               PropVariantClear(&value);

               PropVariantInit(&value);

               bstring bstr("/ifd/{ushort=274}");

               hrExif = pqueryreader->GetMetadataByName(bstr.c_str(), &value);

            }

            string strOrientation;

            if (SUCCEEDED(hrExif))
            {

               iOrientation = value.uiVal;

            }

            PropVariantClear(&value);

         }

      }

      // Convert the image format to 32bppPBGRA
      // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
      // http://d.hatena.ne.jp/sugarontop/20141015
      comptr < IWICFormatConverter > pbitmapsource;

      hr = pimagingfactory->CreateFormatConverter(&pbitmapsource);

      if (FAILED(hr))
      {

         return;

      }

      hr = pbitmapsource->Initialize(pframedecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);

      if (FAILED(hr))
      {

         return;

      }

      if (!windows_load_image_from_bitmap_source(ploadimage, pbitmapsource, pimagingfactory))
      {

         return;

      }

   }


   bool node_save_image(IStream * pstream, ::image::image * pimage, const ::image::encoding_options & encodingoptions);


   //#ifdef UNIVERSAL_WINDOWS
   //
   //
   //   CLASS_DECL_IMAGING_WIC bool node_save_image(::winrt::Windows::Storage::Streams::InMemoryRandomAccessStream const & randomAccessStream, const ::image::image * pimage, ::save_image * psaveimage);
   //
   //
   //#endif
   //



   std::unique_ptr<Gdiplus::Bitmap> create_stretched_gdiplus_bitmap(IWICBitmap *pWicBitmap, UINT targetWidth,
                                                                    UINT targetHeight)
   {

      if (!pWicBitmap || targetWidth == 0 || targetHeight == 0)
      {

         return nullptr;
      }

      if (targetWidth > static_cast<UINT>((std::numeric_limits<INT>::max)()) ||
          targetHeight > static_cast<UINT>((std::numeric_limits<INT>::max)()))
      {

         return nullptr;
      }

      auto pimagingfactory = get_imaging_factory();

      /*
       * Convert the WIC bitmap to premultiplied BGRA.
       *
       * GUID_WICPixelFormat32bppPBGRA corresponds to the memory layout expected
       * by GDI+ PixelFormat32bppPARGB on little-endian Windows:
       *
       * memory bytes: B, G, R, A
       */
      ::comptr<IWICFormatConverter> pConverter;

      HRESULT hr = pimagingfactory->CreateFormatConverter(&pConverter);

      if (FAILED(hr))
      {

         return nullptr;

      }

      hr = pConverter->Initialize(pWicBitmap, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0,
                                  WICBitmapPaletteTypeCustom);

      if (FAILED(hr))
      {

         return nullptr;
      }

      UINT sourceWidth = 0;
      UINT sourceHeight = 0;

      hr = pConverter->GetSize(&sourceWidth, &sourceHeight);

      if (FAILED(hr) || sourceWidth == 0 || sourceHeight == 0)
      {

         return nullptr;
      }

      if (sourceWidth > static_cast<UINT>((std::numeric_limits<INT>::max)()) ||
          sourceHeight > static_cast<UINT>((std::numeric_limits<INT>::max)()))
      {

         return nullptr;
      }

      constexpr UINT bytesPerPixel = 4;

      if (sourceWidth > (std::numeric_limits<UINT>::max)() / bytesPerPixel)
      {

         return nullptr;
      }

      const UINT sourceStride = sourceWidth * bytesPerPixel;

      if (sourceStride > static_cast<UINT>((std::numeric_limits<INT>::max)()) ||
          sourceHeight > (std::numeric_limits<UINT>::max)() / sourceStride)
      {

         return nullptr;
      }

      const UINT sourceBufferSize = sourceStride * sourceHeight;

      memory sourcePixels;

      try
      {

         sourcePixels.set_size(sourceBufferSize);
      }
      catch (...)
      {

         return nullptr;
      }

      hr = pConverter->CopyPixels(nullptr, sourceStride, sourceBufferSize, sourcePixels.data());

      if (FAILED(hr))
      {

         return nullptr;
      }

      /*
       * This bitmap references sourcePixels; it does not need to outlive this
       * function because it is only used while rendering the destination.
       */
      Gdiplus::Bitmap sourceBitmap(static_cast<INT>(sourceWidth), static_cast<INT>(sourceHeight),
                                   static_cast<INT>(sourceStride), PixelFormat32bppPARGB, sourcePixels.data());

      if (sourceBitmap.GetLastStatus() != Gdiplus::Ok)
      {

         return nullptr;
      }

      auto pDestination = std::make_unique<Gdiplus::Bitmap>(static_cast<INT>(targetWidth),
                                                            static_cast<INT>(targetHeight), PixelFormat32bppPARGB);

      if (pDestination->GetLastStatus() != Gdiplus::Ok)
      {

         return nullptr;
      }

      Gdiplus::Graphics graphics(pDestination.get());

      if (graphics.GetLastStatus() != Gdiplus::Ok)
      {

         return nullptr;
      }

      /*
       * SourceCopy is important when the source contains transparency. It avoids
       * blending the resized pixels with the initially transparent destination.
       */
      if (graphics.SetCompositingMode(Gdiplus::CompositingModeSourceCopy) != Gdiplus::Ok)
      {

         return nullptr;
      }

      if (graphics.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality) != Gdiplus::Ok)
      {

         return nullptr;
      }

      if (graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic) != Gdiplus::Ok)
      {

         return nullptr;
      }

      graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
      graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

      /*
       * TileFlipXY prevents the high-quality interpolation kernel from sampling
       * transparent black beyond the source edges, which can otherwise produce
       * dark or faded borders.
       */
      Gdiplus::ImageAttributes imageAttributes;

      if (imageAttributes.SetWrapMode(Gdiplus::WrapModeTileFlipXY) != Gdiplus::Ok)
      {

         return nullptr;
      }

      const Gdiplus::Rect destinationRectangle(0, 0, static_cast<INT>(targetWidth), static_cast<INT>(targetHeight));

      const Gdiplus::Status status =
         graphics.DrawImage(&sourceBitmap, destinationRectangle, 0, 0, static_cast<INT>(sourceWidth),
                            static_cast<INT>(sourceHeight), Gdiplus::UnitPixel, &imageAttributes);

      if (status != Gdiplus::Ok)
      {

         return nullptr;
      }

      return pDestination;
   }



   class gdiplus_bitmap_lock
   {
   public:

      gdiplus_bitmap_lock(Gdiplus::Bitmap *pBitmap, UINT lockMode = Gdiplus::ImageLockModeRead,
                          Gdiplus::PixelFormat pixelFormat = PixelFormat32bppPARGB) : m_pBitmap(pBitmap)
      {

         if (!m_pBitmap)
         {

            return;
         }

         const UINT width = m_pBitmap->GetWidth();
         const UINT height = m_pBitmap->GetHeight();

         if (width == 0 || height == 0)
         {

            return;
         }

         Gdiplus::Rect rectangle(0, 0, static_cast<INT>(width), static_cast<INT>(height));

         m_status = m_pBitmap->LockBits(&rectangle, lockMode, pixelFormat, &m_bitmapData);

         m_bLocked = m_status == Gdiplus::Ok;
      }


      ~gdiplus_bitmap_lock() { unlock(); }


      gdiplus_bitmap_lock(const gdiplus_bitmap_lock &) = delete;
      gdiplus_bitmap_lock &operator=(const gdiplus_bitmap_lock &) = delete;


      bool is_locked() const { return m_bLocked; }


      Gdiplus::Status status() const { return m_status; }


      void *data() { return m_bLocked ? m_bitmapData.Scan0 : nullptr; }


      const void *data() const { return m_bLocked ? m_bitmapData.Scan0 : nullptr; }


      UINT width() const { return m_bLocked ? m_bitmapData.Width : 0; }


      UINT height() const { return m_bLocked ? m_bitmapData.Height : 0; }


      INT stride() const { return m_bLocked ? m_bitmapData.Stride : 0; }


      size_t absolute_stride() const
      {

         if (!m_bLocked)
         {

            return 0;
         }

         return static_cast<size_t>(std::abs(static_cast<long long>(m_bitmapData.Stride)));
      }


      size_t storage_size() const { return absolute_stride() * height(); }


      BYTE *scanline(UINT y)
      {

         if (!m_bLocked || y >= m_bitmapData.Height)
         {

            return nullptr;
         }

         return static_cast<BYTE *>(m_bitmapData.Scan0) + static_cast<ptrdiff_t>(y) * m_bitmapData.Stride;
      }


      const BYTE *scanline(UINT y) const
      {

         if (!m_bLocked || y >= m_bitmapData.Height)
         {

            return nullptr;
         }

         return static_cast<const BYTE *>(m_bitmapData.Scan0) + static_cast<ptrdiff_t>(y) * m_bitmapData.Stride;
      }


      void unlock()
      {

         if (m_bLocked && m_pBitmap)
         {

            m_pBitmap->UnlockBits(&m_bitmapData);
            m_bLocked = false;
         }
      }


   private:

      Gdiplus::Bitmap *m_pBitmap = nullptr;

      Gdiplus::BitmapData m_bitmapData{};

      Gdiplus::Status m_status = Gdiplus::GenericError;

      bool m_bLocked = false;
   };


   bool windows_pixmap_from_bitmap_source(::pixmap * ppixmap, IWICBitmapSource * pbitmapsource, IWICImagingFactory * pimagingfactory)
   {

      comptr < IWICBitmap > piBmp;

      HRESULT hr = pimagingfactory->CreateBitmapFromSource(pbitmapsource, WICBitmapCacheOnLoad, &piBmp);

      if (hr != S_OK)
      {

         return false;

      }

      ::u32 uWidth;

      ::u32 uHeight;

      hr = piBmp->GetSize(&uWidth, &uHeight);

      if (hr != S_OK)
      {

         return false;

      }

      WICRect rc;

      rc.X = 0;
      rc.Y = 0;
      rc.Width = uWidth;
      rc.Height = uHeight;

      comptr < IWICBitmapLock > piLock;

      hr = piBmp->Lock(&rc, WICBitmapLockRead, &piLock);

      if (hr != S_OK)
      {

         return false;

      }

      ::u32 cbStride;

      piLock->GetStride(&cbStride);

      if (hr != S_OK)
      {

         return false;

      }

      ::u32 uArea;

      ::u8 * pData;

      hr = piLock->GetDataPointer(&uArea, &pData);

      if (hr != S_OK)
      {

         return false;

      }

      ppixmap->create_from_data({(::i32)uWidth, (::i32)uHeight}, (::image32_t *)pData, cbStride);

      if (!ppixmap->is_ok())
      {

         return false;

      }

      return true;

   }


   bool windows_load_image_from_bitmap_source(::image::load_image * ploadimage, IWICBitmapSource * pbitmapsource, IWICImagingFactory * pimagingfactory)
   {

      comptr < IWICBitmap > piBmp;

      HRESULT hr = pimagingfactory->CreateBitmapFromSource(pbitmapsource, WICBitmapCacheOnLoad, &piBmp);

      if (hr != S_OK)
      {

         return false;

      }

      ::u32 uWidth;

      ::u32 uHeight;

      hr = piBmp->GetSize(&uWidth, &uHeight);

      if (hr != S_OK)
      {

         return false;

      }

      if (ploadimage->m_sizePreferred.area() > 0)
      {

         if (ploadimage->m_sizePreferred.cx != uWidth || ploadimage->m_sizePreferred.cy != uHeight)
         {

            std::unique_ptr<Gdiplus::Bitmap> pResized = create_stretched_gdiplus_bitmap(piBmp, 
               ploadimage->m_sizePreferred.cx, ploadimage->m_sizePreferred.cy);

            if (pResized)
            {
               gdiplus_bitmap_lock lock(pResized.get());

               if (lock.is_locked())
               {

                  void *pStorage = lock.data();
                  UINT width = lock.width();
                  UINT height = lock.height();
                  INT scanSize = lock.stride();
                  size_t storageSize = lock.storage_size();

                  ploadimage->on_load_image({(::i32)width, (::i32)height}, (::image32_t *)pStorage, scanSize);

                  if (ploadimage->is_ok())
                  {

                     return true;

                  }

               }

            }

         }

      }
         

      WICRect rc;

      rc.X = 0;
      rc.Y = 0;
      rc.Width = uWidth;
      rc.Height = uHeight;

      comptr < IWICBitmapLock > piLock;

      hr = piBmp->Lock(&rc, WICBitmapLockRead, &piLock);

      if (hr != S_OK)
      {

         return false;

      }

      ::u32 cbStride;

      piLock->GetStride(&cbStride);

      if (hr != S_OK)
      {

         return false;

      }

      ::u32 uArea;

      ::u8 * pData;

      hr = piLock->GetDataPointer(&uArea, &pData);

      if (hr != S_OK)
      {

         return false;

      }

      ploadimage->on_load_image({(::i32)uWidth, (::i32)uHeight}, (::image32_t *)pData, cbStride);

      if (!ploadimage->is_ok())
      {

         return false;

      }

      return true;

   }



   //bool is_jpeg(IWICBitmapDecoder * pBitmapDecoder)
   //{
   //   
   //   GUID guidFormat = { };
   //
   //   HRESULT hr = pBitmapDecoder->GetContainerFormat(&guidFormat);
   //
   //   if (FAILED(hr))
   //   {
   //
   //      return false;
   //
   //   }
   //
   //   return IsEqualGUID(guidFormat, GUID_ContainerFormatJpeg);
   //
   //}
   //
   //HRESULT get_query_reader(const ::wide_character * wszPath, IWICMetadataQueryReader * pqueryreaderParent, comptr<IWICMetadataQueryReader> & pqueryreader)
   //{
   //
   //   PROPVARIANT value;
   //   
   //   ::PropVariantInit(&value);
   //
   //   HRESULT hr = pqueryreaderRoot->GetMetadataByName(wszIFDPath, &value);
   //   
   //   if (FAILED(hr))
   //   {
   //
   //      return hr;
   //
   //   }
   //   else if (value.vt != VT_UNKNOWN)
   //   {
   //
   //      return E_FAIL;
   //
   //   }
   //
   //   return value.punkVal->QueryInterface(IID_IWICMetadataQueryReader, &pqueryreader);
   //
   //}




#ifdef UNIVERSAL_WINDOWS


   bool node_save_image(::winrt::Windows::Storage::Streams::InMemoryRandomAccessStream const & randomAccessStream, ::image::image * pimage, const ::image::save_options & saveoptions)
   {

      comptr < IStream > pstream;

      ::CreateStreamOverRandomAccessStream(winrt::get_unknown(randomAccessStream), IID_PPV_ARGS(&pstream));

      if (!node_save_image(pstream, pimage, saveoptions))
      {

         return false;

      }

      return true;

   }


#endif


   bool image_context::_save_image(::file::file * pfile, ::image::image * pimage, const ::image::encoding_options & encodingoptions)
   {

#ifdef UNIVERSAL_WINDOWS

      ::winrt::Windows::Storage::Streams::InMemoryRandomAccessStream randomAccessStream;

      //::wait(randomAccessStream->WriteAsync(get_os_buffer()));

      comptr < IStream > pstream;

      ::CreateStreamOverRandomAccessStream(winrt::get_unknown(randomAccessStream), IID_PPV_ARGS(&pstream));

#else

      comptr < IStream > pstream = SHCreateMemStream(nullptr, NULL);

#endif

      if (!node_save_image(pstream, pimage, encodingoptions))
      {

         return false;

      }

      STATSTG stg;
      zero(stg);
      pstream->Stat(&stg, STATFLAG_NONAME);
      LARGE_INTEGER l;
      l.QuadPart = 0;
      pstream->Seek(l, STREAM_SEEK_SET, nullptr);


      memory mem;

      mem.set_size(1024 * 1024);

      ULONG ulPos = 0;
      ULONG ulRead;
      ULONG ul;
      do
      {

         ulRead = 0;

         ul = (ULONG)stg.cbSize.QuadPart - ulPos;

         pstream->Read(mem.data(), (ULONG)minimum(ul, mem.size()), &ulRead);

         if (ulRead > 0)
         {

            pfile->write(mem(0, ulRead));

            ulPos += ulRead;

         }

      } while (ulRead > 0 && stg.cbSize.QuadPart - ulPos > 0);

      return true;

   }


   //bool node_save_image(IStream * pstream, const ::image::image * pimage, const ::image::save_options & saveoptions)
   //{

   //   comptr < IWICImagingFactory > pimagingfactory = nullptr;

   //   comptr < IWICBitmapEncoder > piEncoder = nullptr;

   //   comptr < IWICBitmapFrameEncode > piBitmapFrame = nullptr;

   //   comptr < IPropertyBag2 > pPropertybag = nullptr;

   //   comptr < IWICStream > piStream = nullptr;

   //   ::u32 uWidth = pimage->width();

   //   ::u32 uHeight = pimage->height();

   //   HRESULT hr = CoCreateInstance(
   //      CLSID_WICImagingFactory,
   //      nullptr,
   //      CLSCTX_INPROC_SERVER,
   //      IID_IWICImagingFactory,
   //      (LPVOID *)&pimagingfactory);

   //   if (SUCCEEDED(hr))
   //   {

   //      hr = pimagingfactory->CreateStream(&piStream);

   //   }

   //   if (SUCCEEDED(hr))
   //   {

   //      hr = piStream->InitializeFromIStream(pstream);

   //   }

   //   if (SUCCEEDED(hr))
   //   {

   //      switch (saveoptions.m_eformat)
   //      {
   //      case ::image::e_format_bmp:
   //         hr = pimagingfactory->CreateEncoder(GUID_ContainerFormatBmp, nullptr, &piEncoder);
   //         break;
   //      case ::image::e_format_gif:
   //         hr = pimagingfactory->CreateEncoder(GUID_ContainerFormatGif, nullptr, &piEncoder);
   //         break;
   //      case ::image::e_format_jpeg:
   //         hr = pimagingfactory->CreateEncoder(GUID_ContainerFormatJpeg, nullptr, &piEncoder);
   //         break;
   //      case ::image::e_format_png:
   //         hr = pimagingfactory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &piEncoder);
   //         break;
   //      default:
   //         break;
   //      }

   //   }

   //   if (SUCCEEDED(hr))
   //   {

   //      hr = piEncoder->Initialize(piStream, WICBitmapEncoderNoCache);

   //   }

   //   if (SUCCEEDED(hr))
   //   {

   //      hr = piEncoder->CreateNewFrame(&piBitmapFrame, &pPropertybag);

   //   }

   //   if (SUCCEEDED(hr))
   //   {

   //      //if(m_bJxr)
   //      //{
   //      //   //PROPBAG2 option ={0};
   //      //   //option.pstrName = L"ImageQuality";
   //      //   //VARIANT varValue;
   //      //   //VariantInit(&varValue);
   //      //   //varValue.vt = VT_R4;
   //      //   //varValue.fltVal = 0.49f;
   //      //   PROPBAG2 option ={0};
   //      //   option.pstrName = L"UseCodecOptions";
   //      //   VARIANT varValue;
   //      //   VariantInit(&varValue);
   //      //   varValue.vt = VT_BOOL;
   //      //   varValue.boolVal = -1;
   //      //   if(SUCCEEDED(hr))
   //      //   {
   //      //      hr = pPropertybag->Write(1,&option,&varValue);
   //      //   }
   //      //   option.pstrName = L"Quality";
   //      //   VariantInit(&varValue);
   //      //   varValue.vt = VT_UI1;
   //      //   varValue.bVal = 200;
   //      //   if(SUCCEEDED(hr))
   //      //   {
   //      //      hr = pPropertybag->Write(1,&option,&varValue);
   //      //   }
   //      //   option.pstrName = L"Subsampling";
   //      //   VariantInit(&varValue);
   //      //   varValue.vt = VT_UI1;
   //      //   varValue.bVal = 1;
   //      //   if(SUCCEEDED(hr))
   //      //   {
   //      //      hr = pPropertybag->Write(1,&option,&varValue);
   //      //   }
   //      //   option.pstrName = L"Overlap";
   //      //   VariantInit(&varValue);
   //      //   varValue.vt = VT_UI1;
   //      //   varValue.bVal = 2;
   //      //   if(SUCCEEDED(hr))
   //      //   {
   //      //      hr = pPropertybag->Write(1,&option,&varValue);
   //      //   }
   //      //   option.pstrName = L"StreamOnly";
   //      //   VariantInit(&varValue);
   //      //   varValue.vt = VT_BOOL;
   //      //   varValue.boolVal = -1;
   //      //   if(SUCCEEDED(hr))
   //      //   {
   //      //      hr = pPropertybag->Write(1,&option,&varValue);
   //      //   }
   //      //}
   //      if (saveoptions.m_eformat == ::image::e_format_jpeg)
   //      {

   //         PROPBAG2 option = { 0 };
   //         option.pstrName = (LPOLESTR)L"ImageQuality";
   //         VARIANT varValue;
   //         VariantInit(&varValue);
   //         varValue.vt = VT_R4;
   //         varValue.fltVal = maximum(0.f, minimum(1.f, saveoptions.m_iQuality / 100.0f));

   //         if (SUCCEEDED(hr))
   //         {

   //            hr = pPropertybag->Write(1, &option, &varValue);

   //         }

   //      }

   //      if (SUCCEEDED(hr))
   //      {

   //         hr = piBitmapFrame->Initialize(pPropertybag);

   //      }

   //   }

   //   if (SUCCEEDED(hr))
   //   {

   //      hr = piBitmapFrame->SetSize(uWidth, uHeight);

   //   }

   //   WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppBGRA;

   //   if (SUCCEEDED(hr))
   //   {

   //      hr = piBitmapFrame->SetPixelFormat(&formatGUID);

   //   }

   //   pimage->map();

   //   auto * pcr = pimage->data();

   //   //   memory m;
   //   //
   //   //#ifdef UNIVERSAL_WINDOWS
   //   //
   //   //   m.set_size(uiHeight*pimage->scan_size());
   //   //
   //   //   pcr = (::color::color *)m.data();
   //   //
   //   //   ::draw2d::vertical_swap_copy_image32(pimage->width(), pimage->height(), pcr,
   //   //                                         pimage->scan_size(), pimage->data(), pimage->scan_size());
   //   //
   //   //#endif

   //   if (SUCCEEDED(hr))
   //   {

   //      if (IsEqualGUID(formatGUID, GUID_WICPixelFormat32bppBGRA))
   //      {

   //         if (SUCCEEDED(hr))
   //         {

   //            hr = piBitmapFrame->WritePixels(uHeight, pimage->scan_size(), uHeight * pimage->scan_size(), (::u8 *)pcr);

   //         }

   //      }
   //      else
   //      {

   //         comptr <IWICBitmap> pdraw2dbitmap;

   //         if (SUCCEEDED(hr))
   //         {

   //            hr = pimagingfactory->CreateBitmapFromMemory(
   //               pimage->width(),
   //               pimage->height(),
   //               GUID_WICPixelFormat32bppBGRA,
   //               pimage->scan_size(),
   //               pimage->scan_size() * pimage->height(),
   //               (::u8 *)pcr,
   //               &pdraw2dbitmap
   //            );

   //         }

   //         comptr<IWICFormatConverter> pconverter;

   //         if (SUCCEEDED(hr))
   //         {

   //            hr = pimagingfactory->CreateFormatConverter(&pconverter);

   //         }

   //         if (SUCCEEDED(hr))
   //         {

   //            hr = pconverter->Initialize(pdraw2dbitmap, formatGUID, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);

   //         }

   //         if (SUCCEEDED(hr))
   //         {

   //            hr = piBitmapFrame->WriteSource(pconverter, nullptr);

   //         }

   //      }

   //   }

   //   if (SUCCEEDED(hr))
   //   {

   //      hr = piBitmapFrame->Commit();

   //   }

   //   if (SUCCEEDED(hr))
   //   {

   //      hr = piEncoder->Commit();

   //   }

   //   return true;

   //}


} // namespace imaging_wic



