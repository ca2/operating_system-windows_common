// From direct2d/object by camilo on 2025-06-25 21:16 <3ThomasBorregaardSørensen!!
// From draw2d_direct2d/pen.cpp by camilo
// on 2014-12-30 01:48 <3ThomasBorregaardSorensen!!
#pragma once


namespace directx11
{


   class CLASS_DECL_DIRECTX11 object :
      virtual public ::particle
   {
   public:


      ::pointer < ::directx11::directx11 > m_pdirectx11;

      object();
      ~object() override;


      void initialize_directx11_object(::directx11::directx11* pdirectx11);


      //virtual comptr < ID2D1StrokeStyle1 > _create_stroke_style(
      //   ::draw2d_gpu::graphics* pgraphics,
      //   ::draw2d::enum_line_cap elinecapBeg,
      //   ::draw2d::enum_line_cap elinecapEnd
      //);


   };


} // namespace directx11



