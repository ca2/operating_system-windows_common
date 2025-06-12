// Created by camilo on 2025-06-11 19:42 <3ThomasBorregaardSørensen!!
#include "framework.h"


namespace directx11
{

   
   bool errorModeSilent = false;

   ::string resourcePath = "";

   ::string errorString(HRESULT hr)
   {
      ::string str;

      if (hr == S_OK)
      {
         str = "S_OK";
      }
      else if (hr == E_FAIL)
      {
         str = "E_FAILED";
      }
      else if (hr == E_NOINTERFACE)
      {
         str = "E_NOINTERFACE";
      }
      else if (SUCCEEDED(hr))
      {
         str.formatf("Succeeded HRESULT = 0x%08X", hr);
      }
      else
      {
         str.formatf("Failed HRESULT = 0x%08X", hr);
      }
      return str;

   }


} // namespace directx11



