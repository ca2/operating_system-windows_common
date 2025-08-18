// Created by camilo 2021-06-05 22:57 BRT <3ThomasBS_!!
#include "framework.h"
#include "font_enumeration.h"
#include "directx11/directx11.h"
#include "aura/graphics/write_text/font_enumeration_item.h"
#include "bred/gpu/bred_approach.h"
#include <dwrite.h>


namespace write_text_directx11
{


   font_enumeration::font_enumeration()
   {


   }


   font_enumeration::~font_enumeration()
   {

   }


   void font_enumeration::on_initialize_particle()
   {

      ::write_text::font_enumeration::on_initialize_particle();

      ::directx11::object::on_initialize_particle();

      auto papproach = m_papplication->get_gpu_approach();

      auto pdevice = papproach->get_gpu_device();

      auto pdirectx11 = ::directx11::from_gpu_device(pdevice);

      initialize_directx11_object(pdirectx11);

   }


   void font_enumeration::on_enumerate_fonts()
   {

      ødefer_construct_new(m_pfontenumerationitema);

      m_pfontenumerationitema->erase_all();

      //::comptr<IDWriteFontCollection> pFontCollection;

      //HRESULT hr = m_pdirectx11->dwrite_factory()->GetSystemFontCollection(&pFontCollection);

      //unsigned int familyCount = 0;

      //if (SUCCEEDED(hr))
      //{

      //   familyCount = pFontCollection->GetFontFamilyCount();

      //}

      //unsigned int index = 0;

      //BOOL exists = false;

      //wchar_t localeName[LOCALE_NAME_MAX_LENGTH];

      //int defaultLocaleSuccess = GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);

      //for (unsigned int i = 0; i < familyCount; ++i)
      //{

      //   ::comptr<IDWriteFontFamily> pFontFamily;

      //   if (SUCCEEDED(hr))
      //   {

      //      hr = pFontCollection->GetFontFamily(i, &pFontFamily);

      //   }

      //   ::comptr<IDWriteLocalizedStrings> pFamilyNames;

      //   if (SUCCEEDED(hr))
      //   {

      //      hr = pFontFamily->GetFamilyNames(&pFamilyNames);

      //   }

      //   if (SUCCEEDED(hr))
      //   {

      //      if (defaultLocaleSuccess)
      //      {

      //         hr = pFamilyNames->FindLocaleName(localeName, &index, &exists);

      //      }

      //      if (SUCCEEDED(hr) && !exists) // if the above find did not find a match, retry with US English
      //      {

      //         hr = pFamilyNames->FindLocaleName(L"en-us", &index, &exists);

      //      }

      //   }

      //   // If the specified locale doesn't exist, select the first on the list_base.
      //   if (!exists)
      //   {
      //      index = 0;
      //   }

      //   unsigned int length = 0;

      //   // Get the string length.
      //   if (SUCCEEDED(hr))
      //   {

      //      hr = pFamilyNames->GetStringLength(index, &length);

      //   }

      //   // Allocate a string big enough to hold the name.
      //   wstring wstr;

      //   auto name = wstr.get_buffer(length + 1);

      //   if (name == nullptr)
      //   {

      //      hr = E_OUTOFMEMORY;

      //   }

      //   // Get the family name.
      //   if (SUCCEEDED(hr))
      //   {

      //      hr = pFamilyNames->GetString(index, name, length + 1);

      //   }

      //   wstr.release_buffer();

      //   // Add the family name to the String Array.
      //   if (SUCCEEDED(hr))
      //   {

      //      string strName = string((const ::wide_character*)(name));

      //      m_pfontenumerationitema->add(øallocate ::write_text::font_enumeration_item(strName, strName));

      //   }

      //}

      //return ::success;

   }


} // namespace write_text_directx11



