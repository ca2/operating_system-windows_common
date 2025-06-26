// Created by camilo 2021-02-10 <3TBS_!!
#pragma once


#include "aura/graphics/write_text/fonts.h"


namespace write_text_directx11
{


   class CLASS_DECL_WRITE_TEXT_DIRECTX11 fonts :
      virtual public ::write_text::fonts
   {
   public:


      fonts();
      ~fonts() override;


      virtual void initialize(::particle * pparticle);


   };


} // namespace write_text_directx11



