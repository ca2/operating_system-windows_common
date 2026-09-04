// Created by camilo on 2024-12-30 21:23 <3ThomasBorregaardSorensen!!
#pragma once


//#include "acme/platform/auto_pointer.h"
#include "aura/graphics/write_text/internal_font.h"
#include "acme/_operating_system.h"
#include "acme/operating_system/windows_common/com/comptr.h"
#include "operating_system-windows_common/draw2d_direct2d/internal_font.h"
#include <dwrite.h>


namespace draw2d_direct2d_for_directx11
{


   class internal_font :
      virtual public ::draw2d_direct2d::internal_font
   {
   public:

      ::comptr < IDWriteFontFileLoader > m_pdwritefontfileloader;
      ::comptr < IDWriteFontCollectionLoader > m_pdwritefontcollectionloader;

      ::comptr<IDWriteFontCollection>                       m_pcollection;
      ::array < ::comptr < IDWriteFontFamily > >            m_familya;
      //auto_pointer < Gdiplus::PrivateFontCollection >    m_pcollection;
      //::raw_array < Gdiplus::FontFamily >                m_familya;
      //::i32                                                m_iFamilyCount;


      internal_font();
      ~internal_font() override;


      void load_from_memory(::memory_base * pmemory) override;
      void on_create_font(::draw2d::graphics * pdraw2dgraphics, ::write_text::font * pwritetextfont) override;


   };


} // namespace draw2d_direct2d_for_directx11



