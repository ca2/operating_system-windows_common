// Created by camilo on 2021-08-10 13:31 BRT <3ThomasBorregaardSorensen!!
#pragma once


#include "acme/operating_system.h"
#include "aura_windows_common/_library.h"
#ifdef UNIVERSAL_WINDOWS
#include "aura_universal_windows/_library.h"
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#endif
#ifdef WINDOWS_DESKTOP
#include "aura_windows/_library.h"
#endif
#include <wincodec.h>


#include "aura/graphics/image/save_image.h"



#include "factory_exchange.h"


comptr < IWICImagingFactory > wic_get_imaging_factory();


#include "context_image.h"

