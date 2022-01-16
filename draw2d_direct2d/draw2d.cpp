#include "framework.h"


namespace draw2d_direct2d
{


   draw2d::draw2d()
   {

      defer_create_mutex();
      
   }


   draw2d::~draw2d()
   {

      ::direct2d::finalize();

   }


   void draw2d::initialize(::object * pobject)
   {

      //auto estatus = 
      
      ::draw2d::draw2d::initialize(pobject);

      //if (!estatus)
      //{

      //   return estatus;

      //}
   
      ::direct2d::defer_initialize(this);

      //estatus = initialize_gdiplus();

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //return estatus;
   
   }


   string draw2d::write_text_get_default_library_name()
   {

      return "direct2d";

   }


   void draw2d::lock_device()
   {

      direct2d::direct2d()->m_d2dMultithread->Enter();

      //return success;

   }


   void draw2d::unlock_device()
   {

      direct2d::direct2d()->m_d2dMultithread->Leave();

      //return success;

   }


} // namespace draw2d_direct2d



