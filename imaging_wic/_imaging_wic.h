// Created by camilo on 2021-08-10 13:31 BRT <3ThomasBorregaardSørensen!!
#pragma once


#include "aura/operating_system.h"
#include "aura_windows/_aura_windows.h"
#include <wincodec.h>


namespace imaging_wic
{


   comptr < IWICImagingFactory > get_imaging_factory();


   CLASS_DECL_IMAGING_WIC bool node_save_image(comptr < IStream > pstream, const ::image * pimage, const ::save_image * psaveimage);


#ifdef _UWP


   CLASS_DECL_IMAGING_WIC bool node_save_image(Windows::Storage::Streams::IRandomAccessStream ^ stream, const ::image * pimage, const ::save_image * psaveimage);


#endif


} // namespace imaging_wic


#include "factory_exchange.h"


comptr < IWICImagingFactory > wic_get_imaging_factory();


#include "context_image.h"

