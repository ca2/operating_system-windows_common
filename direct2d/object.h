// From draw2d_direct2d/pen.cpp by camilo
// on 2014-12-30 01:48 <3ThomasBorregaardSorensen!!
#pragma once


namespace direct2d
{


   class CLASS_DECL_DIRECT2D object :
      virtual public ::particle
   {
   public:


      ::pointer < ::direct2d::direct2d > m_pdirect2d;

      object();
      ~object() override;


      void initialize_direct2d_object(::direct2d::direct2d * pdirect2d);


      virtual comptr < ID2D1StrokeStyle1 > _create_stroke_style(
         ::gpu::graphics * pgraphics,
         ::draw2d::enum_line_cap elinecapBeg,
         ::draw2d::enum_line_cap elinecapEnd
         );


   };


} // namespace direct2d



