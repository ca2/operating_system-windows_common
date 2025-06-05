// From draw2d_direct2d/pen.cpp by camilo
// on 2014-12-30 01:48 <3ThomasBorregaardSorensen!!
#include "framework.h"


namespace direct2d_directx11
{


   class CLASS_DECL_DIRECT2D_DIRECTX11 object :
      virtual public ::particle
   {
   public:


      object();
      ~object() override;


      virtual comptr < ID2D1StrokeStyle1 > _create_stroke_style(
         ::direct2d_directx11::graphics * pgraphics,
         ::draw2d::enum_line_cap elinecapBeg,
         ::draw2d::enum_line_cap elinecapEnd
         );


   };


} // namespace direct2d_directx11



