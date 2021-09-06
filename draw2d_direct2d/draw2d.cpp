#include "framework.h"


namespace draw2d_direct2d
{


   draw2d::draw2d()
   {
      
   }


   draw2d::~draw2d()
   {

      ::direct2d::finalize();

   }


   ::e_status draw2d::initialize(::object * pobject)
   {

      auto estatus = ::draw2d::draw2d::initialize(pobject);

      if (!estatus)
      {

         return estatus;

      }
   
      ::direct2d::defer_initialize(this);

      //estatus = initialize_gdiplus();

      //if (!estatus)
      //{

      //   return estatus;

      //}

      return estatus;
   
   }


   string draw2d::write_text_get_default_library_name()
   {

      return "direct2d";

   }

} // namespace draw2d_direct2d



