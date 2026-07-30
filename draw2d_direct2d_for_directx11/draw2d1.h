#pragma once


namespace draw2d_gdiplus
{


   class CLASS_DECL_DRAW2D_GDIPLUS draw2d :
      virtual public ::draw2d::draw2d
   {
   public:


      draw2d();
      virtual ~draw2d();


      virtual void initialize(::particle * pparticle) override;

      virtual string write_text_get_default_implementation_name() override;

   };


} // namespace draw2d_gdiplus



