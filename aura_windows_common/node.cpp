#include "framework.h"
#include "node.h"
#include "acme/platform/system.h"


#include "acme/_operating_system.h"


namespace aura_windows_common
{


   node::node()
   {

      //m_pauranode = this;

   }


   node::~node()
   {


   }




   bool node::_os_calc_app_dark_mode()
   {

      //try
      //{

      //   ::acme_windows::registry::key key;

      //   key.open(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize");

      //   unsigned int dw;

      //   auto estatus = key._get("AppsUseLightTheme", dw);

      //   if (::failed(estatus))
      //   {

      //      estatus = key._get("SystemUseLightTheme", dw);

      //      if (::failed(estatus))
      //      {

      //         return false;

      //      }

      //   }

      //   return dw == 0;

      //}
      //catch (...)
      //{

      //   return false;

      //}

      return false;

   }


   bool node::_os_calc_system_dark_mode()
   {

      //try
      //{

      //   ::acme_windows::registry::key key;

      //   key.open(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize");

      //   unsigned int dw;

      //   auto estatus = key._get("SystemUseLightTheme", dw);

      //   if (::failed(estatus))
      //   {

      //      estatus = key._get("AppsUseLightTheme", dw);

      //      if (::failed(estatus))
      //      {

      //         return false;

      //      }

      //   }

      //   return dw == 0;

      //}
      //catch (...)
      {

         return false;

      }

   }


   ::color::color node::get_default_color(unsigned long long u)
   {

      switch (u)
      {
      case COLOR_3DFACE:
         return argb(127, 192, 192, 200);
      case COLOR_WINDOW:
         return argb(127, 255, 255, 255);
      case COLOR_3DLIGHT:
         return argb(127, 218, 218, 210);
      case COLOR_3DHIGHLIGHT:
         return argb(127, 238, 238, 230);
      case COLOR_3DSHADOW:
         return argb(127, 138, 138, 130);
      case COLOR_3DDKSHADOW:
         return argb(127, 90, 90, 80);
      default:
         break;
      }

      return argb(127, 0, 0, 0);

   }


   void node::set_console_colors(unsigned int dwScreenColors, unsigned int dwPopupColors, unsigned int dwWindowAlpha)
   {

      //::acme_windows::registry::key key(HKEY_CURRENT_USER, "Console", true);

      //key._set("ScreenColors", dwScreenColors);
      //key._set("PopupColors", dwPopupColors);
      //key._set("WindowAlpha", dwWindowAlpha);

   }



   //void node::set_system_dark_mode1(bool bSet)
   //{

   //   //::acme_windows::registry::key key(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", true);

   //   //unsigned int dwSystemUseLightTheme;
   //   //if (bSet)
   //   //{
   //   //   dwSystemUseLightTheme = 0;
   //   //}
   //   //else
   //   //{
   //   //   dwSystemUseLightTheme = 1;
   //   //}

   //   //key._set("SystemUsesLightTheme", dwSystemUseLightTheme);
   //   return ::success;

   //}


   //void node::set_app_dark_mode1(bool bSet)
   //{

   //   //::acme_windows::registry::key key(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", true);

   //   //unsigned int dwAppsUseLightTheme;
   //   //if (bSet)
   //   //{
   //   //   dwAppsUseLightTheme = 0;
   //   //}
   //   //else
   //   //{
   //   //   dwAppsUseLightTheme = 1;
   //   //}

   //   //key._set("AppsUseLightTheme", dwAppsUseLightTheme);

   //   return ::success;

   //}


   double node::get_time_zone()
   {

      double dTimeZone = 0.;

#ifdef WINDOWS
      {
         //posix_time t = time(nullptr);

         //struct tm *p = localtime(&t);

         DYNAMIC_TIME_ZONE_INFORMATION i = {};

         unsigned int dw = GetDynamicTimeZoneInformation(&i);

         if (dw == TIME_ZONE_ID_STANDARD)
         {

            dTimeZone = -((double)(i.Bias + i.StandardBias) / 60.0);

         }
         else if (dw == TIME_ZONE_ID_DAYLIGHT)
         {

            dTimeZone = -((double)(i.Bias + i.DaylightBias) / 60.0);

         }
         else
         {

            dTimeZone = -((double)i.Bias / 60.0);

         }

      }
#else
      {

         posix_time t = time(nullptr);

         struct tm lt = { 0 };

         localtime_r(&t, &lt);

         //printf("Offset to GMT is %lds.\n", lt.tm_gmtoff);

         //printf("The time zone is '%s'.\n", lt.tm_zone);

         dTimeZone = +((double)lt.tm_gmtoff / (60.0 * 60.0));

      }
#endif

      return dTimeZone;

   }



   void node::system_main()
   {

      auto psystem = system();

      //auto estatus = 

      psystem->main();

      //if (!estatus)
      //{

      //   return estatus;

      //}

      ///estatus = 

      psystem->inline_term();

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //return estatus;


   }


} // namespace aura_windows_common



