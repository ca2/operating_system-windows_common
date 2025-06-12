#include "framework.h"
#include "directx11.h"
#include "swap_chain.h"
#include "acme/graphics/write_text/font_weight.h"
#include "acme/prototype/geometry2d/rectangle.h"
#include "acme_windows_common/hresult_exception.h"
#include "aura/graphics/gpu/approach.h"
#include "aura/graphics/gpu/device.h"
#include "aura/windowing/window.h"


namespace directx11
{


   ::pointer < class directx11 > directx11::s_pdirectx11;


   directx11::directx11()
   {

      if (s_pdirectx11)
      {

         throw ::exception(error_wrong_state);

      }

      s_pdirectx11 = this;

   }


   directx11::~directx11()
   {


   }


   void directx11::initialize(::particle* pparticle)
   {

      ::app_consumer<::aura::application>::initialize(pparticle);

   }


   CLASS_DECL_DIRECTX11 void defer_initialize(::windowing::window* pwindow, const ::int_rectangle& rectanglePlacement)
   {

      if (::is_set(directx11::s_pdirectx11))
      {

         return;

      }

      pwindow->__defer_construct(directx11::s_pdirectx11);

   }


   CLASS_DECL_DIRECTX11 void finalize()
   {

      directx11::s_pdirectx11.release();

   }


} // namespace draw2d_directx11



