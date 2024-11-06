#include "framework.h"


namespace draw2d_directx
{


   palette::palette()
   {

   }


   palette::~palette()
   {

   }


   palette::operator HPALETTE() const
   {

      return nullptr;

   }


   bool palette::CreatePalette(LPLOGPALETTE lpLogPalette)
   {

      //return Attach(::CreatePalette(lpLogPalette));

      return false;

   }


   bool palette::CreateHalftonePalette(::image::image * pimage)
   {
      //ASSERT(pgraphics != nullptr && (dynamic_cast<::draw2d_directx::graphics * >(pgraphics))->get_handle1() != nullptr);
      //return Attach(::CreateHalftonePalette((dynamic_cast<::draw2d_directx::graphics * >(pgraphics))->get_handle1()));
      return false;

   }
   unsigned int palette::GetPaletteEntries(unsigned int nStartIndex, unsigned int nNumEntries,
                                   LPPALETTEENTRY lpPaletteColors) const
   {

//#ifdef WINDOWS_DESKTOP
//      ASSERT(get_os_data() != nullptr);
//      return ::GetPaletteEntries((HPALETTE)get_os_data(), nStartIndex, nNumEntries, lpPaletteColors);
//#else
//      throw ::exception(todo);
//#endif

      return 0;

   }
   unsigned int palette::SetPaletteEntries(unsigned int nStartIndex, unsigned int nNumEntries,
                                   LPPALETTEENTRY lpPaletteColors)
   {
//#ifdef WINDOWS_DESKTOP
//      ASSERT(get_os_data() != nullptr); return ::SetPaletteEntries((HPALETTE)get_os_data(), nStartIndex,
//                                            nNumEntries, lpPaletteColors);
//#else
//      throw ::exception(todo);
//#endif

      return 0;

   }
   void palette::AnimatePalette(unsigned int nStartIndex, unsigned int nNumEntries,
                                LPPALETTEENTRY lpPaletteColors)
   {
      throw ::exception(todo);

      //   ASSERT(get_os_data() != nullptr); ::AnimatePalette((HPALETTE)get_os_data(), nStartIndex, nNumEntries,
      //lpPaletteColors);

   }

   unsigned int palette::GetNearestPaletteIndex(color32_t crColor) const
   {

      throw ::exception(todo);

      /*   ASSERT(get_os_data() != nullptr); return ::GetNearestPaletteIndex((HPALETTE)get_os_data(), crColor);
      */
   }

   bool palette::ResizePalette(unsigned int nNumEntries)
   {
      throw ::exception(todo);

      //ASSERT(get_os_data() != nullptr);
      //return ::ResizePalette((HPALETTE)get_os_data(), nNumEntries) != false;
   }

   int palette::GetEntryCount()
   {
      throw ::exception(todo);

      //   ASSERT(get_os_data() != nullptr); unsigned short nEntries;
      //::GetObject((HANDLE) get_os_data(), sizeof(unsigned short), &nEntries); return (int)nEntries;

   }

} // namespace draw2d_directx
