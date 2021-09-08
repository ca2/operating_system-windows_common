// Created by camilo on 2021-08-10 13:31 BRT <3ThomasBorregaardS�rensen!!
#pragma once


#include "aura/operating_system.h"
#include "aura_windows_common/_aura_windows_common.h"
#ifdef _UWP
#include "aura_universal_windows/_aura_universal_windows.h"
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#endif
#ifdef WINDOWS_DESKTOP
#include "aura_windows/_aura_windows.h"
#endif
#include <wincodec.h>


namespace imaging_wic
{


   comptr < IWICImagingFactory > get_imaging_factory();


   CLASS_DECL_IMAGING_WIC bool node_save_image(comptr < IStream > pstream, const ::image * pimage, const ::save_image * psaveimage);


#ifdef _UWP


   CLASS_DECL_IMAGING_WIC bool node_save_image(::winrt::Windows::Storage::Streams::IRandomAccessStream const & stream, const ::image * pimage, const ::save_image * psaveimage);


#endif


} // namespace imaging_wic


#include "factory_exchange.h"


comptr < IWICImagingFactory > wic_get_imaging_factory();


#include "context_image.h"

