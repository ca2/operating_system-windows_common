// Created by camilo on 2024-12-30 21:23 <3ThomasBorregaardSorensen!!
#pragma once


#include "acme/platform/auto_pointer.h"
#include "aura/graphics/write_text/internal_font.h"
#include "acme/_operating_system.h"
#include "acme_windows_common/comptr.h"
#include <dwrite.h>


namespace draw2d_direct2d
{


   class internal_font :
      virtual public ::write_text::internal_font,
      virtual public ::direct2d::object
   {
   public:

      ::comptr < IDWriteFontFileLoader > m_pfontfileloader;
      ::comptr < IDWriteFontCollectionLoader > m_pfontcollectionloader;

      ::comptr<IDWriteFontCollection>                       m_pcollection;
      ::array < ::comptr < IDWriteFontFamily > >            m_familya;
      //auto_pointer < Gdiplus::PrivateFontCollection >    m_pcollection;
      //::raw_array < Gdiplus::FontFamily >                m_familya;
      //int                                                m_iFamilyCount;


      internal_font();
      ~internal_font() override;


      void load_from_memory(::memory_base * pmemory) override;
      void on_create_font(::draw2d::graphics * pgraphics, ::write_text::font * pfont) override;


   };


} // namespace draw2d_direct2d



