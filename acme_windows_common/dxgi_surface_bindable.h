// Created by camilo on 2025-06-21 16:57 <3ThomasBorregaardSørensen!!
#pragma once


#include <dxgi.h>


class CLASS_DECL_ACME_WINDOWS_COMMON dxgi_surface_bindable :
   virtual public ::particle
{
public:


   dxgi_surface_bindable();
   ~dxgi_surface_bindable() override;


   virtual void _bind(int iIndex, int iLayerIndex, IDXGISurface * psurface);


};
