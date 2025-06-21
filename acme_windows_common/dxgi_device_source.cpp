// Created by camilo on 2025-06-20 02:09 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "dxgi_device_source.h"
#include "acme/exception/interface_only.h"


dxgi_device_source::dxgi_device_source()
{


}


dxgi_device_source::~dxgi_device_source()
{


}


IDXGIDevice* dxgi_device_source::_get_dxgi_device()
{

   throw ::interface_only();

   return nullptr;

}



