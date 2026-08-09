#pragma once


#include "draw2d_direct2d/draw2d.h"


namespace draw2d_direct2d_for_directx11
{


   class CLASS_DECL_DRAW2D_DIRECT2D_FOR_DIRECTX11 draw2d :
      virtual public ::draw2d_direct2d::draw2d
   {
   public:


      draw2d();
      ~draw2d() override;

      virtual void initialize(::particle * pparticle) override;

      virtual string write_text_get_default_implementation_name() override;

      bool lock_device() override;
      void unlock_device() override;

      //void on_create_window(::windowing::window* pwindow) override;
      
      void adjust_composited_window_styles(::u32& nExStyle, ::u32& nStyle) override;
      virtual ::draw2d::graphics_pointer do_allocation_strategy(::acme::user::interaction * pacmeuserinteractionAffinity, ::image::image * pimage,
                                                                const ::i32_size & size);


   };


} // namespace draw2d_direct2d_for_directx11



