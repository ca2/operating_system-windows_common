#include "framework.h"
#include "draw2d.h"
#include "directx11/directx11.h"
#include "aura/windowing/window.h"


namespace draw2d_directx11
{


   draw2d::draw2d()
   {

      defer_create_synchronization();

   }


   draw2d::~draw2d()
   {

      //::directx11::finalize();

   }


   void draw2d::initialize(::particle * pparticle)
   {

      //auto estatus = 
      
      ::draw2d::draw2d::initialize(pparticle);

      //if (!estatus)
      //{

      //   return estatus;

      //}
   
      /*::directx11::defer_initialize(this);*/

      //estatus = initialize_gdiplus();

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //return estatus;
   
   }



   string draw2d::write_text_get_default_implementation_name()
   {

      return system()->implementation_name("write_text", "directx11");

   }


   bool draw2d::lock_device()
   {

      auto pdirectx11 = m_pdirectx11;

      if (!pdirectx11)
      {

         return false;

      }

      //auto pmultithread = pdirectx11->m_pd2d1multithread.m_p;

      //if (!pmultithread)
      //{

      //   return false;

      //}

      //pmultithread->Enter();

      return true;

   }


   void draw2d::unlock_device()
   {
      
      auto pdirectx11 = m_pdirectx11;

      //auto pmultithread = pdirectx11->m_pd2d1multithread.m_p;

      //pmultithread->Leave();

   }


   //void draw2d::on_create_window(::windowing::window* pwindow)
   //{

   //   ::gpu::draw2d::on_create_window(pwindow);

   //   ///auto rectangleWindow = pwindow->get_window_rectangle();
   //   
   //   //::directx11::defer_initialize(pwindow, rectangleWindow);

   //}


   void draw2d::adjust_composited_window_styles(unsigned int& nExStyle, unsigned int& nStyle)
   {

      nExStyle |= WS_EX_LAYERED;

      if (m_papplication->m_gpu.m_bUseSwapChainWindow)
      {

         nExStyle |= WS_EX_NOREDIRECTIONBITMAP;

      }

   }


} // namespace draw2d_directx11



