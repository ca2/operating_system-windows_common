// Created by camilo on 2025-06-21 16:58 <3ThomasBorregaardSørensen!!
#include "platform.h"
#include "dxgi_surface_bindable.h"
#include "acme/exception/interface_only.h"


dxgi_surface_bindable::dxgi_surface_bindable()
{


}


dxgi_surface_bindable::~dxgi_surface_bindable()
{


}


void dxgi_surface_bindable::_bind(::i32 iIndex, ::i32 iLayerIndex, IDXGISurface* psurface)
{

   throw ::interface_only();

}



