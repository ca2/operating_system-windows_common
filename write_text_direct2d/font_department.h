// Created by camilo 2021-02-10 <3TBS_!!
#pragma once


namespace write_text_direct2d
{


   class CLASS_DECL_WRITE_TEXT_DIRECT2D font_department :
      virtual public ::write_text::font_department
   {
   public:


      font_department();
      ~font_department() override;


      virtual ::e_status initialize(::object * pobject);


   };


} // namespace write_text_direct2d



