#include "framework.h"
#include "context.h"
#include "acme/exception/exception.h"
#include "acme/parallelization/task.h"
#include "acme/platform/node.h"
#include "acme/platform/system.h"
#include "apex/parallelization/handler_manager.h"
#include "apex_windows_common/single_threaded_handler_manager.h"
#include "aura/graphics/image/save_options.h"
#include "acme_windows_common/comptr.h"


#include "acme/_operating_system.h"
#include <wincodec.h>


#ifdef UNIVERSAL_WINDOWS
#include "acme/operating_system/universal_windows/_winrt_foundation.h"
#include "acme_universal_windows/_winrt_stream.h"
#include "_winrt_applicationmodel_datatransfer.h"
#endif


class multi_threaded_handler_manager :
   virtual public handler_manager
{
public:


   multi_threaded_handler_manager() {}
   ~multi_threaded_handler_manager() override {}




   void on_start_loop()
   {

      node()->defer_co_initialize_ex(true, true);

   }



};





namespace imaging_wic
{


   bool windows_image_from_bitmap_source(::image::image * pimageFrame, IWICBitmapSource * pbitmapsource, IWICImagingFactory * pimagingfactory);


   image_context::image_context()
   {

   }


   image_context::~image_context()
   {

   }


   void image_context::initialize(::particle * pparticle)
   {

      //auto estatus = 
      
      ::image::image_context::initialize(pparticle);


      //__øconstruct(m_pparticleImageSynchronization);

      //if (!estatus)
      //{

      //   return estatus;

      //}


//#ifdef UNIVERSAL_WINDOWS
//
//      m_pmanagerImageLoad = __allocate multi_threaded_handler_manager();
//
//#else
//
      m_pmanagerImageLoadSlowQueue = __allocate single_threaded_handler_manager();

      m_pmanagerImageLoadFastQueue = __allocate single_threaded_handler_manager();

//#endif

      if (!m_pmanagerImageLoadSlowQueue || !m_pmanagerImageLoadFastQueue)
      {

         throw ::exception(error_null_pointer);

      }

      //estatus = 
      
      m_pmanagerImageLoadSlowQueue->initialize_handler_manager(this, "imaging_load_image", true);
      m_pmanagerImageLoadFastQueue->initialize_handler_manager(this, "imaging_load_image", true);

      //if (!estatus)
      //{

      //   return estatus;

      //}

#ifdef WINDOWS

      m_pmanagerImageLoadSlowQueue->m_bUseDedicatedThread = true;
      m_pmanagerImageLoadFastQueue->m_bUseDedicatedThread = true;

#endif

      //return estatus;

   }


   void image_context::destroy()
   {

      m_pmanagerImageLoadSlowQueue.defer_destroy();

      m_pmanagerImageLoadFastQueue.defer_destroy();

      ::image::image_context::destroy();

   }


   comptr < IWICImagingFactory > get_imaging_factory()
   {

      comptr < IWICImagingFactory > pimagingfactory;

      //defer_co_initialize_ex(false);

      HRESULT hrCreate = pimagingfactory.CoCreateInstance(CLSID_WICImagingFactory1, nullptr, CLSCTX_INPROC_SERVER);

      if (FAILED(hrCreate))
      {

         ::get_task()->information() << "Failed creating WICImagingFactory1 %x" << hrCreate;

      }

      return pimagingfactory;

   }


#ifdef UNIVERSAL_WINDOWS

   // platform implementation may use this image_context-"routing" to manipulate the image/clipboard
   bool image_context::_desk_to_image(::image::image *  pimage)
   {


      bool bOk = true;

      user_send({ e_timeout, 15_s, [&]()
         {

            auto dataPackage = ::winrt::Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();

            if (dataPackage == nullptr)
            {

               bOk = false;

               return;

            }

            if (!dataPackage.Contains(::winrt::Windows::ApplicationModel::DataTransfer::StandardDataFormats::Bitmap()))
            {

               bOk = false;

               return;

            }

            auto p = dataPackage.GetDataAsync(L"DeviceIndependentBitmap").get();

            //::winrt::Windows::Storage::Streams::IRandomAccessStream^ stream = (::winrt::Windows::Storage::Streams::IRandomAccessStream^)

            //auto ref = ::wait(dataPackage.GetDataAsync(L"DeviceIndependentBitmap"));

            if (p == nullptr)
            {

               p = dataPackage.GetDataAsync(L"DeviceIndependentBitmapV5").get();

               if (p == nullptr)
               {

                  p = dataPackage.GetDataAsync(L"DeviceInterchangeFormat").get();

                  if (p == nullptr)
                  {

                     bOk = false;

                     return;

                  }

               }

            }

            auto stream = p.as< ::winrt::Windows::Storage::Streams::IRandomAccessStream>();

            //::winrt::Windows::Storage::Streams::IRandomAccessStreamWithContentType ^ stream = ::wait(ref->OpenReadAsync());

            //if (stream == nullptr)
            //{


            //   bOk = false;

            //   return;

            //}

            memsize s = (memsize)stream.Size();

            ::winrt::Windows::Storage::Streams::Buffer buffer((unsigned int)s);

            if (buffer == nullptr)
               return;

            stream.ReadAsync(buffer, (unsigned int)s, ::winrt::Windows::Storage::Streams::InputStreamOptions::ReadAhead).get();

            memory m;

            m.set_size(s);

            windows_runtime_read_buffer(m.data(), s, buffer);

            BITMAPINFO* _ = (BITMAPINFO*)m.data();

            pimage->create({ _->bmiHeader.biWidth,  _->bmiHeader.biHeight });

            pimage->map();

            pimage->image32()->vertical_swap_copy(
               pimage->width(),
               pimage->height(),
               pimage->scan_size(),
               (::image32_t*)&m.data()[_->bmiHeader.biSize],
               _->bmiHeader.biSizeImage / _->bmiHeader.biHeight);


            //memory m;

            //m.

            //if (!windows_load_image_from_file(pimage, stream, get_application()))
            //{


            //   bOk = false;

            //   return;

            //}

         } });

      return bOk;

   }
   // platform implementation may use this image_context-"routing" to manipulate the image/clipboard
   bool image_context::_desk_has_image()
   {

      bool bOk = false;

      user_send({ e_timeout, 15_s, [&bOk]()
         {


         } });

      return bOk;

   }


   // platform implementation may use this image_context-"routing" to manipulate the image/clipboard
   bool image_context::_image_to_desk(::image::image *  pimage)
   {

      bool bOk = true;

      auto package = ::winrt::Windows::ApplicationModel::DataTransfer::DataPackage();

      if (package == nullptr)
      {

         return false;

      }

      ::winrt::Windows::Storage::Streams::InMemoryRandomAccessStream randomAccessStream;

      ::image::save_options saveoptions;

      saveoptions.m_eformat = ::image::e_format_png;

      saveoptions.m_iQuality = 100;

      if (!node_save_image(randomAccessStream, pimage, saveoptions))
      {

         return false;

      }

      package.RequestedOperation(::winrt::Windows::ApplicationModel::DataTransfer::DataPackageOperation::Copy);

      auto object = ::winrt::Windows::Storage::Streams::RandomAccessStreamReference::CreateFromStream(randomAccessStream);

      package.SetBitmap(object);

      user_send({ e_timeout, 15_s, [&package, this]()
         {

            ::winrt::Windows::ApplicationModel::DataTransfer::Clipboard::SetContent(package);

         } });

      return true;


   }

#endif


   //void image_context::_load_icon(::image::icon * picon, const ::payload & payloadFile)




} // namespace imaging_wic




