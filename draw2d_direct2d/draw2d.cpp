#include "framework.h"
#include "draw2d.h"
#include "direct2d/direct2d.h"
#include "aura/windowing/window.h"


namespace draw2d_direct2d
{


   draw2d::draw2d()
   {

      defer_create_synchronization();

   }


   draw2d::~draw2d()
   {

      ::direct2d::finalize();

   }


   void draw2d::initialize(::particle * pparticle)
   {

      //auto estatus = 
      
      ::draw2d::draw2d::initialize(pparticle);

      //if (!estatus)
      //{

      //   return estatus;

      //}
   
      /*::direct2d::defer_initialize(this);*/

      //estatus = initialize_gdiplus();

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //return estatus;
   
   }



   string draw2d::write_text_get_default_implementation_name()
   {

      return system()->implementation_name("write_text", "direct2d");

   }


   bool draw2d::lock_device()
   {

      auto pdirect2d = ::direct2d::get();

      if (!pdirect2d)
      {

         return false;

      }

      auto pmultithread = pdirect2d->m_pd2dMultithread;

      if (!pmultithread)
      {

         return false;

      }

      pmultithread->Enter();

      return true;

   }


   void draw2d::unlock_device()
   {
      
      auto pdirect2d = ::direct2d::get();

      if (!pdirect2d)
      {

         return;

      }

      auto pmultithread = pdirect2d->m_pd2dMultithread;

      if (!pmultithread)
      {

         return;

      }

      pmultithread->Leave();

   }


   void draw2d::on_create_window(::windowing::window* pwindow)
   {

      auto rectangleWindow = pwindow->get_window_rectangle();
      
      ::direct2d::defer_initialize(pwindow, rectangleWindow);

   }


} // namespace draw2d_direct2d



