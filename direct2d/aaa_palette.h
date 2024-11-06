#pragma once


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D palette :
      virtual public ::draw2d_direct2d::object,
      virtual public ::draw2d::palette
   {
   public:


      palette();
      virtual ~palette();


      bool CreatePalette(LPLOGPALETTE lpLogPalette);
      bool CreateHalftonePalette(::image::image * pimage);

      operator HPALETTE() const;
      int GetEntryCount();
      unsigned int GetPaletteEntries(unsigned int nStartIndex, unsigned int nNumEntries, LPPALETTEENTRY lpPaletteColors) const;
      unsigned int SetPaletteEntries(unsigned int nStartIndex, unsigned int nNumEntries, LPPALETTEENTRY lpPaletteColors);

      void AnimatePalette(unsigned int nStartIndex, unsigned int nNumEntries, LPPALETTEENTRY lpPaletteColors);
      unsigned int GetNearestPaletteIndex(color32_t crColor) const;
      bool ResizePalette(unsigned int nNumEntries);


   };


} // namespace draw2d_direct2d




