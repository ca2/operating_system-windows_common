// Created by camilo on 2021-01-21 05:05 PM <3ThomasBorregaardSorensen
// Renamed to _nano by camilo on 2021-02-01 13:44 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "brush.h"
#include "font.h"
#include "icon.h"
#include "pen.h"
#include "device.h"
#include "graphics.h"
//#include "acme/operating_system/windows/nano/user/user.h"
//#include "acme/operating_system/windows/nano/user/window.h"
#include "acme/nano/nano.h"
#include "acme/platform/factory_function.h"
//#include "acme/windowing/window_base.h"

//#include "acme/prototype/prototype/_factory.h"


IMPLEMENT_FACTORY(nano_graphics_direct2d)
{


   //pfactory->add_factory_item < ::windows::micro::window, ::acme::windowing::window >();
   pfactory->add_factory_item < ::universal_windows::nano::graphics::brush, ::nano::graphics::brush >();
   pfactory->add_factory_item < ::universal_windows::nano::graphics::font, ::nano::graphics::font >();
   pfactory->add_factory_item < ::universal_windows::nano::graphics::pen, ::nano::graphics::pen >();
   pfactory->add_factory_item < ::universal_windows::nano::graphics::icon, ::nano::graphics::icon >();
   pfactory->add_factory_item < ::universal_windows::nano::graphics::device, ::nano::graphics::device >();


   pfactory->add_factory_item < ::universal_windows::nano::graphics::graphics, ::nano::graphics::graphics >();

   //pfactory->add_factory_item < ::windows::micro::user, ::micro::user >();
   //pfactory->add_factory_item < ::windows::micro::user, ::micro::user >();

   //::int_rectangle rectangleMainScreen;

   //rectangleMainScreen.left() = 0;
   //rectangleMainScreen.top() = 0; 
   //rectangleMainScreen.right() = GetSystemMetrics(SM_CXSCREEN);
   //rectangleMainScreen.bottom() = GetSystemMetrics(SM_CYSCREEN); 

   //operating_system_set_main_screen_rectangle(rectangleMainScreen);

}


//_REFERENCE_FACTORY(nano_graphics_win32);



