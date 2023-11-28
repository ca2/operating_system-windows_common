// Created by camilo 2021-06-05 22:57 BRT <3ThomasBS_!!
#include "framework.h"
#include "font_enumeration.h"
#include "direct2d/direct2d.h"
#include "aura/graphics/write_text/font_enumeration_item.h"
#include <dwrite.h>


namespace write_text_direct2d
{


   font_enumeration::font_enumeration()
   {


   }


   font_enumeration::~font_enumeration()
   {

   }


   void font_enumeration::on_enumerate_fonts()
   {

      __defer_construct_new(m_pfontenumerationitema);

      m_pfontenumerationitema->erase_all();

      ::comptr<IDWriteFontCollection> pFontCollection;

      HRESULT hr = ::direct2d::direct2d()->dwrite_factory()->GetSystemFontCollection(&pFontCollection);

      ::u32 familyCount = 0;

      if (SUCCEEDED(hr))
      {

         familyCount = pFontCollection->GetFontFamilyCount();

      }

      u32 index = 0;

      BOOL exists = false;

      wchar_t localeName[LOCALE_NAME_MAX_LENGTH];

      int defaultLocaleSuccess = GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);

      for (::u32 i = 0; i < familyCount; ++i)
      {

         ::comptr<IDWriteFontFamily> pFontFamily;

         if (SUCCEEDED(hr))
         {

            hr = pFontCollection->GetFontFamily(i, &pFontFamily);

         }

         ::comptr<IDWriteLocalizedStrings> pFamilyNames;

         if (SUCCEEDED(hr))
         {

            hr = pFontFamily->GetFamilyNames(&pFamilyNames);

         }

         if (SUCCEEDED(hr))
         {

            if (defaultLocaleSuccess)
            {

               hr = pFamilyNames->FindLocaleName(localeName, &index, &exists);

            }

            if (SUCCEEDED(hr) && !exists) // if the above find did not find a match, retry with US English
            {

               hr = pFamilyNames->FindLocaleName(L"en-us", &index, &exists);

            }

         }

         // If the specified locale doesn't exist, select the first on the list.
         if (!exists)
         {
            index = 0;
         }

         ::u32 length = 0;

         // Get the string length.
         if (SUCCEEDED(hr))
         {

            hr = pFamilyNames->GetStringLength(index, &length);

         }

         // Allocate a string big enough to hold the name.
         wstring wstr;

         auto name = wstr.get_buffer(length + 1);

         if (name == nullptr)
         {

            hr = E_OUTOFMEMORY;

         }

         // Get the family name.
         if (SUCCEEDED(hr))
         {

            hr = pFamilyNames->GetString(index, name, length + 1);

         }

         wstr.release_buffer();

         // Add the family name to the String Array.
         if (SUCCEEDED(hr))
         {

            string strName = string((const ::wide_character*)(name));

            m_pfontenumerationitema->add(__allocate< ::write_text::font_enumeration_item >(strName, strName));

         }

      }

      //return ::success;

   }


} // namespace write_text_direct2d



