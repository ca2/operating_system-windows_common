#pragma once


#include "aura/graphics/draw2d/palette.h"
#include "object.h"


namespace draw2d_directx11
{


   class CLASS_DECL_DRAW2D_DIRECTX11 palette :
      virtual public ::draw2d_directx11::object,
      virtual public ::draw2d::palette
   {
   public:


      palette();
      ~palette() override;


   };


} // namespace draw2d_directx11




