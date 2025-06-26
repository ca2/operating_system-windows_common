#pragma once


#include "bred/gpu/draw2d.h"
#include "directx11/object.h"


namespace draw2d_directx11
{


   class CLASS_DECL_DRAW2D_DIRECTX11 draw2d :
      virtual public ::draw2d_gpu::draw2d,
      virtual public ::directx11::object
   {
   public:


      draw2d();
      ~draw2d() override;

      virtual void initialize(::particle * pparticle) override;

      virtual string write_text_get_default_implementation_name() override;

      bool lock_device() override;
      void unlock_device() override;

      void on_create_window(::windowing::window* pwindow) override;
      
      void adjust_composited_window_styles(unsigned int& nExStyle, unsigned int& nStyle) override;
      

   };


} // namespace draw2d_directx11



