// From draw2d_direct2d/pen.cpp by camilo
// on 2014-12-30 01:48 <3ThomasBorregaardSorensen!!
#include "framework.h"


namespace direct2d
{


   class CLASS_DECL_DIRECT2D object :
      virtual public ::particle
   {
   public:


      object();
      ~object() override;


      virtual comptr < ID2D1StrokeStyle1 > _create_stroke_style(
         ::draw2d_gpu::graphics * pgraphics,
         ::draw2d::enum_line_cap elinecapBeg,
         ::draw2d::enum_line_cap elinecapEnd
         );


   };


} // namespace direct2d



