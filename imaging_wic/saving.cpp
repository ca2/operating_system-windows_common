#include "framework.h"
#include "context_image.h"
#include "acme/exception/exception.h"
#include "acme/platform/node.h"
#include "aura/graphics/image/save_image.h"
#include "acme_windows_common/comptr.h"


#include "acme/_operating_system.h"
#include <wincodec.h>


#ifdef _UWP
#include "acme_universal_windows/_winrt_stream.h"
#include <ShCore.h>
#endif


namespace imaging_wic
{


   CLASS_DECL_IMAGING_WIC bool node_save_image(IStream* pstream, ::image* pimage, const ::save_image* psaveimage);


   void context_image::save_image(memory & mem, ::image * pimage, const ::save_image * psaveimage)
   {

      if (::is_null(pimage))
      {

         throw ::exception(error_bad_argument);

      }

      acmenode()->defer_co_initialize_ex(false);

#ifdef _UWP

      ::winrt::Windows::Storage::Streams::InMemoryRandomAccessStream randomAccessStream;

      //::wait(randomAccessStream->WriteAsync(get_os_buffer()));

      comptr <IUnknown> punknown = randomAccessStream.as<IUnknown>().get();

      comptr <IStream> pstream;
      CreateStreamOverRandomAccessStream(punknown,__interface_of(pstream));
#else

      comptr < IStream > pstream = SHCreateMemStream(nullptr, NULL);

#endif

      node_save_image(pstream, pimage, psaveimage);

      STATSTG stg;
      zero(stg);
      pstream->Stat(&stg, STATFLAG_NONAME);
      LARGE_INTEGER l;
      l.QuadPart = 0;
      pstream->Seek(l, STREAM_SEEK_SET, nullptr);

      memory memBuffer;

      memBuffer.set_size(1024 * 1024);

      ULONG ulPos = 0;
      ULONG ulRead;
      ULONG ul;
      do
      {

         ulRead = 0;

         ul = (ULONG)stg.cbSize.QuadPart - ulPos;

         pstream->Read(memBuffer.get_data(), (ULONG)minimum(ul, memBuffer.get_size()), &ulRead);

         if (ulRead > 0)
         {

            mem.append(memBuffer.get_data(), ulRead);

            ulPos += ulRead;

         }

      } while (ulRead > 0 && stg.cbSize.QuadPart - ulPos > 0);

      //pstream->Release();

      //return true;

   }



   bool node_save_image(IStream * pstream, ::image * pimage, const ::save_image * psaveimage)
   {

      comptr < IWICImagingFactory > pimagingfactory;

      comptr < IWICBitmapEncoder > pbitmapencoder;

      comptr < IWICBitmapFrameEncode > pbitmapframeencode;

      comptr < IPropertyBag2 > ppropertybag;

      comptr < IWICStream > pwicstream;

      ::u32 uWidth = pimage->width();

      ::u32 uHeight = pimage->height();

      HRESULT hr = CoCreateInstance(
         CLSID_WICImagingFactory,
         nullptr,
         CLSCTX_INPROC_SERVER,
         IID_IWICImagingFactory,
         (LPVOID *)&pimagingfactory);

      if (SUCCEEDED(hr))
      {

         hr = pimagingfactory->CreateStream(&pwicstream);

      }

      if (SUCCEEDED(hr))
      {

         hr = pwicstream->InitializeFromIStream(pstream);

      }

      if (SUCCEEDED(hr))
      {

         switch (psaveimage == nullptr ? ::draw2d::e_format_png : psaveimage->m_eformat)
         {
         case ::draw2d::e_format_bmp:
            hr = pimagingfactory->CreateEncoder(GUID_ContainerFormatBmp, nullptr, &pbitmapencoder);
            break;
         case ::draw2d::e_format_gif:
            hr = pimagingfactory->CreateEncoder(GUID_ContainerFormatGif, nullptr, &pbitmapencoder);
            break;
         case ::draw2d::e_format_jpeg:
            hr = pimagingfactory->CreateEncoder(GUID_ContainerFormatJpeg, nullptr, &pbitmapencoder);
            break;
         default:
            hr = pimagingfactory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &pbitmapencoder);
            break;
         }
      }

      if (SUCCEEDED(hr))
      {

         hr = pbitmapencoder->Initialize(pwicstream, WICBitmapEncoderNoCache);

      }

      if (SUCCEEDED(hr))
      {

         hr = pbitmapencoder->CreateNewFrame(&pbitmapframeencode, &ppropertybag);

      }

      if (SUCCEEDED(hr))
      {
         //if(m_bJxr)
         //{
         //   //PROPBAG2 option ={0};
         //   //option.pstrName = L"ImageQuality";
         //   //VARIANT varValue;
         //   //VariantInit(&varValue);
         //   //varValue.vt = VT_R4;
         //   //varValue.fltVal = 0.49f;
         //   PROPBAG2 option ={0};
         //   option.pstrName = L"UseCodecOptions";
         //   VARIANT varValue;
         //   VariantInit(&varValue);
         //   varValue.vt = VT_BOOL;
         //   varValue.boolVal = -1;
         //   if(SUCCEEDED(hr))
         //   {
         //      hr = ppropertybag->Write(1,&option,&varValue);
         //   }
         //   option.pstrName = L"Quality";
         //   VariantInit(&varValue);
         //   varValue.vt = VT_UI1;
         //   varValue.bVal = 200;
         //   if(SUCCEEDED(hr))
         //   {
         //      hr = ppropertybag->Write(1,&option,&varValue);
         //   }
         //   option.pstrName = L"Subsampling";
         //   VariantInit(&varValue);
         //   varValue.vt = VT_UI1;
         //   varValue.bVal = 1;
         //   if(SUCCEEDED(hr))
         //   {
         //      hr = ppropertybag->Write(1,&option,&varValue);
         //   }
         //   option.pstrName = L"Overlap";
         //   VariantInit(&varValue);
         //   varValue.vt = VT_UI1;
         //   varValue.bVal = 2;
         //   if(SUCCEEDED(hr))
         //   {
         //      hr = ppropertybag->Write(1,&option,&varValue);
         //   }
         //   option.pstrName = L"StreamOnly";
         //   VariantInit(&varValue);
         //   varValue.vt = VT_BOOL;
         //   varValue.boolVal = -1;
         //   if(SUCCEEDED(hr))
         //   {
         //      hr = ppropertybag->Write(1,&option,&varValue);
         //   }
         //}

         if (psaveimage != nullptr && psaveimage->m_eformat == ::draw2d::e_format_jpeg)
         {

            PROPBAG2 option = { 0 };
            option.pstrName = (LPOLESTR) L"ImageQuality";
            VARIANT varValue;
            VariantInit(&varValue);
            varValue.vt = VT_R4;
            varValue.fltVal = maximum(0.f, minimum(1.f, psaveimage->m_iQuality / 100.0f));
            if (SUCCEEDED(hr))
            {
               hr = ppropertybag->Write(1, &option, &varValue);
            }

         }

         if (SUCCEEDED(hr))
         {

            hr = pbitmapframeencode->Initialize(ppropertybag);

         }

      }

      if (psaveimage != nullptr && psaveimage->m_iDpi > 0)
      {

         pbitmapframeencode->SetResolution(psaveimage->m_iDpi, psaveimage->m_iDpi);

      }

      if (SUCCEEDED(hr))
      {
         hr = pbitmapframeencode->SetSize(uWidth, uHeight);
      }

      WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppBGRA;

      if (SUCCEEDED(hr))
      {

         hr = pbitmapframeencode->SetPixelFormat(&formatGUID);

      }

      pimage->map();

      auto pcolorref = pimage->get_data();

      auto iScan = pimage->scan_size();

      if (SUCCEEDED(hr))
      {

         if (IsEqualGUID(formatGUID, GUID_WICPixelFormat32bppBGRA))
         {

            if (SUCCEEDED(hr))
            {

               hr = pbitmapframeencode->WritePixels(uHeight, iScan, uHeight * iScan, (byte *)pcolorref);

            }

         }
         else
         {

            comptr <IWICBitmap> pbitmap;

            if (SUCCEEDED(hr))
            {

               hr = pimagingfactory->CreateBitmapFromMemory(
                  pimage->width(),
                  pimage->height(),
                  GUID_WICPixelFormat32bppBGRA,
                  pimage->scan_size(),
                  pimage->scan_size() * pimage->height(),
                  (byte *)pimage->get_data(),
                  &pbitmap
               );

            }

            comptr<IWICFormatConverter> pconverter;

            if (SUCCEEDED(hr))
            {

               hr = pimagingfactory->CreateFormatConverter(&pconverter);

            }

            if (SUCCEEDED(hr))
            {

               hr = pconverter->Initialize(pbitmap, formatGUID, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);

            }

            //Step 4: Create render target and D2D bitmap from IWICBitmapSource
            //::u32 width=0;
            //::u32 height=0;
            //if(SUCCEEDED(hr))
            //{
            //   hr = pbitmap->GetSize(&width,&height);
            //}

            //pimage = create_image({width, height});

            if (SUCCEEDED(hr))
            {
               hr = pbitmapframeencode->WriteSource(pconverter, nullptr);
            }


            //for(int k = 0; k < height; k++)
            //{
            //   ::memcpy_dup(&pb[k * iStride],&mem.get_data()[(height - 1 - k) * iStride],iStride);
            //}

         }

      }

      if (SUCCEEDED(hr))
      {

         hr = pbitmapframeencode->Commit();

      }

      if (SUCCEEDED(hr))
      {

         hr = pbitmapencoder->Commit();

      }

      return true;

   }


#ifdef _UWP


   bool node_save_image(::winrt::Windows::Storage::Streams::IRandomAccessStream const & stream, const ::image * pimage, const ::save_image * psaveimage)
   {

      ::winrt::Windows::Storage::Streams::InMemoryRandomAccessStream randomAccessStream;

      comptr < IStream > pstream;

      ::CreateStreamOverRandomAccessStream(winrt::get_unknown(randomAccessStream), IID_PPV_ARGS(&pstream));

      return node_save_image(pstream, pimage, psaveimage);

   }


#endif


} // namespace imaging_wic



