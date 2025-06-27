// Created by camilo on 2025-06-21 16:58 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "dxgi_surface_bindable.h"
#include "acme/exception/interface_only.h"


dxgi_surface_bindable::dxgi_surface_bindable()
{


}


dxgi_surface_bindable::~dxgi_surface_bindable()
{


}


void dxgi_surface_bindable::_bind(int iIndex, int iLayerIndex, IDXGISurface* psurface)
{

   throw ::interface_only();

}



