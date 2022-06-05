#include "framework.h"
#include "apex/parallelization/handler_manager.h"
#include "apex_windows_common/single_threaded_handler_manager.h"
#include <wincodec.h>

class multi_threaded_handler_manager :
   virtual public handler_manager
{
public:


   multi_threaded_handler_manager() {}
   ~multi_threaded_handler_manager() override {}




   void on_start_loop()
   {

      defer_co_initialize_ex(true, true);

   }



};





namespace imaging_wic
{


   bool windows_image_from_bitmap_source(::image * pimageFrame, IWICBitmapSource * pbitmapsource, IWICImagingFactory * pimagingfactory);


   context_image::context_image()
   {

   }


   context_image::~context_image()
   {

   }


   void context_image::initialize(::object * pobject)
   {

      //auto estatus = 
      
      ::context_image::initialize(pobject);

      //if (!estatus)
      //{

      //   return estatus;

      //}


#ifdef _UWP

      __own(this, m_pmanagerImageLoad, __new(multi_threaded_handler_manager));

#else

      __own(this, m_pmanagerImageLoad, __new(single_threaded_handler_manager));

#endif

      if (!m_pmanagerImageLoad)
      {

         throw ::exception(error_null_pointer);

      }

      //estatus = 
      
      m_pmanagerImageLoad->initialize_handler_manager(this, "imaging_load_image", true);

      //if (!estatus)
      //{

      //   return estatus;

      //}

#ifdef WINDOWS

      m_pmanagerImageLoad->m_bUseDedicatedThread = true;

#endif

      //return estatus;

   }


   comptr < IWICImagingFactory > get_imaging_factory()
   {

      comptr < IWICImagingFactory > pimagingfactory;

      //defer_co_initialize_ex(false);

      HRESULT hrCreate = pimagingfactory.CoCreateInstance(CLSID_WICImagingFactory1, nullptr, CLSCTX_INPROC_SERVER);

      if (FAILED(hrCreate))
      {

         _INFORMATION(::get_task(), "Failed creating WICImagingFactory1 %x" << hrCreate);

      }

      return pimagingfactory;

   }


#ifdef _UWP

   // platform implementation may use this context_image-"routing" to manipulate the image/clipboard
   bool context_image::_desk_to_image(::image* pimage)
   {


      bool bOk = true;

      m_psystem->m_paurasystem->m_paurasession->m_puser->m_pwindowing->windowing_send({ e_timeout, 15_s, [&]()
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

            ::winrt::Windows::Storage::Streams::Buffer buffer((::u32)s);

            if (buffer == nullptr)
               return;

            stream.ReadAsync(buffer, (::u32)s, ::winrt::Windows::Storage::Streams::InputStreamOptions::ReadAhead).get();

            memory m;

            m.set_size(s);

            windows_runtime_read_buffer(m.get_data(), s, buffer);

            BITMAPINFO* _ = (BITMAPINFO*)m.get_data();

            pimage->create({ _->bmiHeader.biWidth,  _->bmiHeader.biHeight });

            pimage->map();

            vertical_swap_copy_colorref(
               pimage->get_data(),
               pimage->width(),
               pimage->height(),
               pimage->scan_size(),
               (::color32_t*)&m.get_data()[_->bmiHeader.biSize],
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
   // platform implementation may use this context_image-"routing" to manipulate the image/clipboard
   bool context_image::_desk_has_image()
   {

      bool bOk = false;

      m_psystem->m_paurasession->m_puser->m_pwindowing->windowing_send({ e_timeout, 15_s, [&bOk]()
         {


         } });

      return bOk;

   }


   // platform implementation may use this context_image-"routing" to manipulate the image/clipboard
   bool context_image::_image_to_desk(const ::image* pimage)
   {

      bool bOk = true;

      auto package = ::winrt::Windows::ApplicationModel::DataTransfer::DataPackage();

      if (package == nullptr)
      {

         return false;

      }

      ::winrt::Windows::Storage::Streams::InMemoryRandomAccessStream randomAccessStream;

      ::save_image saveimage;

      saveimage.m_eformat = ::draw2d::e_format_png;

      saveimage.m_iQuality = 100;

      if (!node_save_image(randomAccessStream, pimage, &saveimage))
      {

         return false;

      }

      package.RequestedOperation(::winrt::Windows::ApplicationModel::DataTransfer::DataPackageOperation::Copy);

      auto object = ::winrt::Windows::Storage::Streams::RandomAccessStreamReference::CreateFromStream(randomAccessStream);

      package.SetBitmap(object);

      m_psystem->m_paurasystem->m_paurasession->m_puser->m_pwindowing->windowing_send({ e_timeout, 15_s, [&package, this]()
         {

            ::winrt::Windows::ApplicationModel::DataTransfer::Clipboard::SetContent(package);

         } });

      return true;


   }

#endif


   //void context_image::_load_icon(::draw2d::icon * picon, const ::payload & payloadFile)




} // namespace imaging_wic




