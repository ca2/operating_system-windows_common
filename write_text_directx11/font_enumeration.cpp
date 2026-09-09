// Created by camilo 2021-06-05 22:57 BRT <3ThomasBS_!!
#include "platform.h"
#include "font_enumeration.h"
#include "directx11/directx11.h"
#include "aura/graphics/write_text/font_enumeration_item.h"
#include "bred/gpu/bred_approach.h"
#include "acme/operating_system/windows_common/com/hresult_exception.h"
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

      auto pdevice = papproach->get_gpu_device(m_papplication->main_acme_user_interaction()->m_pacmewindowingwindow);

      auto pdirectx11 = ::directx11::from_gpu_device(pdevice);

      initialize_directx11_object(pdirectx11);

   }


   void font_enumeration::on_enumerate_fonts()
   {

      defer_construct_newø(m_pfontenumerationitema);

      m_pfontenumerationitema->erase_all();


      //
      // Font enumeration is device-independent.
      //
      // There is no need to obtain this factory from the D3D11
      // device or gpu_directx11::device.
      //

      ::comptr<IDWriteFactory> pwritefactory;

      HRESULT hr =
         ::DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown **>(&pwritefactory));

      ::defer_throw_hresult(hr);


      ::comptr<IDWriteFontCollection> pfontcollection;

      hr =
         pwritefactory->GetSystemFontCollection(
            &pfontcollection,
            FALSE);

      ::defer_throw_hresult(hr);


      wchar_t wszLocaleName[LOCALE_NAME_MAX_LENGTH]{};

      bool bHasUserLocale =
         ::GetUserDefaultLocaleName(
            wszLocaleName,
            LOCALE_NAME_MAX_LENGTH) != 0;


      const ::u32 uFamilyCount =
         pfontcollection->GetFontFamilyCount();


      for (::u32 iFamily = 0;
           iFamily < uFamilyCount;
           ++iFamily)
      {

         //
         // IMPORTANT:
         //
         // Do not carry hr, index or exists from the previous family.
         //

         ::comptr<IDWriteFontFamily> pfontfamily;

         hr =
            pfontcollection->GetFontFamily(
               iFamily,
               &pfontfamily);

         if (FAILED(hr))
         {

            continue;

         }


         ::comptr<IDWriteLocalizedStrings> pfamilynames;

         hr =
            pfontfamily->GetFamilyNames(
               &pfamilynames);

         if (FAILED(hr))
         {

            continue;

         }


         ::u32 uNameIndex = 0;

         BOOL bExists = FALSE;


         //
         // First try the user's Windows locale.
         //

         if (bHasUserLocale)
         {

            hr =
               pfamilynames->FindLocaleName(
                  wszLocaleName,
                  &uNameIndex,
                  &bExists);

            if (FAILED(hr))
            {

               bExists = FALSE;

            }

         }


         //
         // Then fall back to en-us.
         //

         if (!bExists)
         {

            uNameIndex = 0;

            bExists = FALSE;

            hr =
               pfamilynames->FindLocaleName(
                  L"en-us",
                  &uNameIndex,
                  &bExists);

            if (FAILED(hr))
            {

               bExists = FALSE;

            }

         }


         //
         // If neither locale exists, DirectWrite guarantees
         // that the localized-string collection has at least
         // the family names it returned, so use entry zero.
         //

         if (!bExists)
         {

            uNameIndex = 0;

         }


         ::u32 uLength = 0;

         hr =
            pfamilynames->GetStringLength(
               uNameIndex,
               &uLength);

         if (FAILED(hr))
         {

            continue;

         }


         wstring wstrName;

         auto pwszName =
            wstrName.get_buffer(
               uLength + 1);

         if (!pwszName)
         {

            throw ::exception(error_no_memory);

         }


         hr =
            pfamilynames->GetString(
               uNameIndex,
               pwszName,
               uLength + 1);

         if (FAILED(hr))
         {

            wstrName.release_buffer();

            continue;

         }


         //
         // Convert while the buffer is definitely valid and
         // null-terminated.
         //

         string strName(
            (const ::wide_character *)pwszName);

         wstrName.release_buffer();


         if (strName.is_empty())
         {

            continue;

         }


         //
         // This constructor is preferable here because we have
         // a font FAMILY NAME, not a font file path.
         //

         m_pfontenumerationitema->add(
            allocateø::write_text::font_enumeration_item(
               strName));

      }

   }

} // namespace write_text_directx11



