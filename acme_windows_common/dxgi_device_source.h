// Created by camilo on 2025-06-20 02:09 <3ThomasBorregaardSørensen!!
#pragma once


#include <dxgi.h>


class CLASS_DECL_ACME_WINDOWS_COMMON dxgi_device_source :
   virtual public ::particle
{
public:


   dxgi_device_source();
   ~dxgi_device_source() override;


   virtual IDXGIDevice* _get_dxgi_device();


};
