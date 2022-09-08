#pragma once


#include "aura/_.h"
#include "aura_windows_common/_.h"
#ifdef _UWP
#include "aura_universal_windows/_.h"
#endif
#ifdef WINDOWS_DESKTOP
#include "aura_windows/_.h"
#endif
#ifdef _UWP
#include "aura_universal_windows/_library.h"
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#endif
//#ifdef WINDOWS_DESKTOP
//#include "aura_windows/_library.h"
//#endif
#include <wincodec.h>


//#include "aura/graphics/image/save_image.h"
//
//
//
//#include "factory_exchange.h"



//
//#include "context_image.h"



#ifdef _IMAGING_WIC_STATIC
#define CLASS_DECL_IMAGING_WIC
#elif defined(_IMAGING_WIC_LIBRARY)
#define CLASS_DECL_IMAGING_WIC  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_IMAGING_WIC  CLASS_DECL_IMPORT
#endif


namespace imaging_wic
{


   comptr < IWICImagingFactory > get_imaging_factory();


   CLASS_DECL_IMAGING_WIC bool node_save_image(IStream * pstream, const ::image * pimage, const ::save_image * psaveimage);


#ifdef _UWP


   CLASS_DECL_IMAGING_WIC bool node_save_image(::winrt::Windows::Storage::Streams::IRandomAccessStream const & stream, const ::image * pimage, const ::save_image * psaveimage);


#endif


} // namespace imaging_wic




comptr < IWICImagingFactory > wic_get_imaging_factory();



