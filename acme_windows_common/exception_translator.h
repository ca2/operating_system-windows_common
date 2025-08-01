// Created on 2021-04-29 12:33 BRT <3TBS_!! Second celebration of Mummis Birthday 70!!
#pragma once


#include "acme/exception/translator.h"
#include "acme/_operating_system.h"


namespace acme_windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON exception_translator :
      virtual public ::exception_translator
   {
   public:


      _se_translator_function   m_pfn;


      exception_translator();
      ~exception_translator() override;


      static void __cdecl filter2(unsigned int uiCode, EXCEPTION_POINTERS* ppointers);


      virtual bool attach() override;


      virtual bool detach() override;

      virtual void filter(unsigned int uiCode, EXCEPTION_POINTERS* ppointers);


      //virtual string _get_standard_exception_name(unsigned int uiCode) override;
      //virtual string _get_standard_exception_description(unsigned int uiCode) override;

      static const_char_pointer _get_standard_exception_name(unsigned int uiCode);
      static const_char_pointer _get_standard_exception_description(unsigned int uiCode);

   };


} // namespace acme_windows_common



