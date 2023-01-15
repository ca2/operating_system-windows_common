// Created by camilo on 2022-09-23 00:18 <3ThomasBorregaardSørensen!!
#pragma once


#undef new
#include <winrt/Windows.ApplicationModel.DataTransfer.h>


namespace imaging_wic
{


#ifdef _UWP


   CLASS_DECL_IMAGING_WIC bool node_save_image(::winrt::Windows::Storage::Streams::IRandomAccessStream const & stream, ::image * pimage, const ::save_image * psaveimage);


#endif


} // namespace imaging_wic



