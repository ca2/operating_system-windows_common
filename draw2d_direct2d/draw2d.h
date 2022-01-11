#pragma once


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D draw2d :
      virtual public ::draw2d::draw2d
   {
   public:


      


      draw2d();
      ~draw2d() override;


      virtual void initialize(::object * pobject) override;

      virtual string write_text_get_default_library_name() override;

      void lock_device() override;
      void unlock_device() override;


   };


} // namespace draw2d_direct2d



