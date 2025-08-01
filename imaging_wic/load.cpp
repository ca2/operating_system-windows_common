#include "framework.h"
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


namespace imaging_wic
{

   comptr < IWICImagingFactory > get_imaging_factory();

   bool windows_image_from_bitmap_source(::image::image * pimageFrame, IWICBitmapSource * pbitmapsource, IWICImagingFactory * pimagingfactory);


   void image_context::_load_image(::image::image * pimageParam, const ::payload & payloadFile, const ::image::load_options & loadoptions)
   {

      auto ploadimage = __allocate::image::load_image(this);

      //auto estatus = 

      ploadimage->initialize(this);

      /*if (!estatus)
      {

         return estatus;

      }*/

      ploadimage->m_pimage = pimageParam;

      ploadimage->m_pimage->m_estatus = error_failed;

      ploadimage->m_pimage->set_nok();

      ploadimage->m_payload = payloadFile;

      ploadimage->m_functionLoaded = loadoptions.functionLoaded;

      pimageParam->m_bCreateHelperMaps = loadoptions.helper_maps;

      auto filepath = payloadFile.as_file_path();

      //if (!filepath.case_insensitive_ends(".webp"))
      //{

      //   return;

      //}

      if (filepath.case_insensitive_begins("http:/")
         || filepath.case_insensitive_begins("https:/"))
      {

         m_pmanagerImageLoadSlowQueue->handle(
   { e_timeout, 1_minute, ploadimage },
   loadoptions.sync);

      }
      else
      {

         m_pmanagerImageLoadFastQueue->handle(
            { e_timeout, 15_s, ploadimage },
               loadoptions.sync);

      }

      //return ploadimage->m_estatus;

   }


   void image_context::_os_load_image(::image::image * pimage, memory & memory)
   {

      pimage->m_estatus = ::error_failed;

      pimage->set_nok();

      comptr < IWICImagingFactory > pimagingfactory;

      pimagingfactory = get_imaging_factory();

      comptr < IWICStream > piStream;

      HRESULT hr = pimagingfactory->CreateStream(&piStream);

      if (FAILED(hr))
      {

         return;

      }

      const_char_pointer  pszData = (const_char_pointer  ) memory.data();

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

      int iOrientation = -1;

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

      if (!windows_image_from_bitmap_source(pimage, pbitmapsource, pimagingfactory))
      {

         return;

      }

      pimage->m_iExifOrientation = iOrientation;

      pimage->on_load_image();

      pimage->set_ok_flag();

      pimage->m_estatus = ::success;

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
   bool windows_image_from_bitmap_source(::image::image * pimageFrame, IWICBitmapSource * pbitmapsource, IWICImagingFactory * pimagingfactory)
   {

      comptr < IWICBitmap > piBmp;

      HRESULT hr = pimagingfactory->CreateBitmapFromSource(pbitmapsource, WICBitmapCacheOnLoad, &piBmp);

      if (hr != S_OK)
      {

         return false;

      }

      unsigned int uWidth;

      unsigned int uHeight;

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

      unsigned int cbStride;

      piLock->GetStride(&cbStride);

      if (hr != S_OK)
      {

         return false;

      }

      unsigned int uArea;

      unsigned char * pData;

      hr = piLock->GetDataPointer(&uArea, &pData);

      if (hr != S_OK)
      {

         return false;

      }

      pimageFrame->create({ (int)uWidth, (int)uHeight });

      if (pimageFrame->area() <= 0)
      {

         return false;

      }

      pimageFrame->map();

      auto pdataTarget = pimageFrame->data();

      auto scanSizeTarget = pimageFrame->scan_size();

      pdataTarget->copy(uWidth, uHeight, scanSizeTarget, (::image32_t *)pData, cbStride);

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

   //   unsigned int uWidth = pimage->width();

   //   unsigned int uHeight = pimage->height();

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

   //            hr = piBitmapFrame->WritePixels(uHeight, pimage->scan_size(), uHeight * pimage->scan_size(), (unsigned char *)pcr);

   //         }

   //      }
   //      else
   //      {

   //         comptr <IWICBitmap> pbitmap;

   //         if (SUCCEEDED(hr))
   //         {

   //            hr = pimagingfactory->CreateBitmapFromMemory(
   //               pimage->width(),
   //               pimage->height(),
   //               GUID_WICPixelFormat32bppBGRA,
   //               pimage->scan_size(),
   //               pimage->scan_size() * pimage->height(),
   //               (unsigned char *)pcr,
   //               &pbitmap
   //            );

   //         }

   //         comptr<IWICFormatConverter> pconverter;

   //         if (SUCCEEDED(hr))
   //         {

   //            hr = pimagingfactory->CreateFormatConverter(&pconverter);

   //         }

   //         if (SUCCEEDED(hr))
   //         {

   //            hr = pconverter->Initialize(pbitmap, formatGUID, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);

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



