#pragma once


#include "aura/graphics/draw2d/palette.h"
#include "object.h"


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D palette :
      virtual public ::draw2d_direct2d::object,
      virtual public ::draw2d::palette
   {
   public:


      palette();
      ~palette() override;


   };


} // namespace draw2d_direct2d




