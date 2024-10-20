// Created by camilo on 2022-09-23 00:18 <3ThomasBorregaardSorensen!!
#pragma once


#undef ___new
#include <winrt/Windows.ApplicationModel.DataTransfer.h>


namespace imaging_wic
{


#ifdef UNIVERSAL_WINDOWS


   CLASS_DECL_IMAGING_WIC bool node_save_image(::winrt::Windows::Storage::Streams::IRandomAccessStream const & stream, ::image::image * pimage, const ::image::save_image * psaveimage);


#endif


} // namespace imaging_wic



