#include "framework.h"
#include "aura/printer.h"
#include "printer.h"
#include <WinSpool.h>


namespace draw2d_directx
{


   printer::printer()
   {
   }

   printer::~printer()
   {
   }

   bool printer::open(const ::string & pszDeviceName)
   {
      if(is_opened())
         close();
#ifndef _UWP
      return OpenPrinterW((LPWSTR) (const widechar *) wstring(pszDeviceName), &m_hPrinter, nullptr) != false && m_hPrinter != nullptr;
#else
      throw ::exception(todo);
      return false;
#endif
   }

   bool printer::is_opened()
   {
      return m_hPrinter != nullptr;
   }

   ::draw2d::graphics * printer::create_graphics()
   {
      if(!m_documentproperties.initialize(this))
         return nullptr;
      return m_documentproperties.create_graphics();
   }


   bool printer::close()
   {
      bool bOk = true;
      if(m_hPrinter != nullptr)
      {
#ifndef _UWP
         bOk = ::ClosePrinter(m_hPrinter) != false;
#else
         throw ::exception(todo);
         return false;
#endif
         m_hPrinter = nullptr;
      }
      return bOk;
   }


   printer::document_properties::document_properties(::particle * pparticle) :
      ::object(pobject)
   {
      m_hdc = nullptr;
      m_pdevmode = nullptr;
   }

   printer::document_properties::~document_properties()
   {
      close();
   }

   bool printer::document_properties::initialize(::draw2d_directx::printer * pprinter, DEVMODE * pdevmode)
   {
      __UNREFERENCED_PARAMETER(pdevmode);
      if(m_pdevmode != nullptr)
         return false;
      if(m_hdc != nullptr)
         return false;
#ifndef _UWP
      int iSize = DocumentPropertiesW(nullptr, pprinter->m_hPrinter, (LPWSTR)(LPWSTR)wstring(pprinter->m_strName), nullptr, nullptr, 0);
      m_pdevmode = (DEVMODE *) malloc(iSize);
      if(!DocumentPropertiesW(nullptr, pprinter->m_hPrinter, (LPWSTR) (const widechar *) wstring(pprinter->m_strName), m_pdevmode, nullptr, DM_OUT_BUFFER))
      {
         throw ::exception(::exception("failed to get printer DocumentProperties"));
         return false;
      }
#else
      throw ::exception(todo);
      return false;
#endif
      return true;
   }

   bool printer::document_properties::close()
   {
      //throw ::exception(todo);
      return false;
#ifndef _UWP
      if(m_hdc != nullptr)
      {
         ::DeleteDC(m_hdc);
         m_hdc = nullptr;
      }
#else
      throw ::exception(todo);
      return false;
#endif
      if(m_pdevmode != nullptr)
      {
         free(m_pdevmode);
         m_pdevmode = nullptr;
      }
      return true;
   }



   ::draw2d::graphics * printer::document_properties::create_graphics()
   {
      if(m_pdevmode == nullptr)
         return nullptr;
      if(m_hdc != nullptr)
         return nullptr;
#ifndef _UWP
      m_hdc = ::CreateDCW(L"WINSPOOL", (const widechar *) wstring(m_pdevmode->dmDeviceName), nullptr, m_pdevmode);
      ::draw2d::graphics_pointer g(e_create);
      g->attach_hdc(m_hdc);
      return g.detach();
#else
      throw ::exception(todo);
      return nullptr;
#endif
   }


} // namespace win2

