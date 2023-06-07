// Created by camilo on 2022-11-11 16:08 <3ThomasBorregaardSorensen!!
#pragma once



#include "acme/_operating_system.h"


#include <wincodec.h>




namespace imaging_wic
{


   comptr < IWICImagingFactory > get_imaging_factory();


   CLASS_DECL_IMAGING_WIC bool node_save_image(IStream* pstream, ::image* pimage, const ::save_image* psaveimage);


} // namespace imaging_wic



