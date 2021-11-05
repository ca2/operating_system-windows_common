// Created on 2021-04-29 12:33 BRT <3TBS_!! Second celebration of Mummis Birthday 70!!
#pragma once


namespace windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON exception_translator :
      virtual public ::exception_translator
   {
   public:


      _se_translator_function   m_pfn;


      exception_translator();


      virtual ~exception_translator();

      static void __cdecl filter2(u32 uiCode, EXCEPTION_POINTERS* ppointers);

      //virtual void filter_sigsegv(i32 signal, siginfo_t* psiginfo, void* pc);
      //virtual void filter_sigfpe(i32 signal, siginfo_t* psiginfo, void* pc);
      //virtual void filter_sigpipe(i32 signal, siginfo_t* psiginfo, void* pc);


      virtual bool attach() override;


      virtual bool detach() override;

      virtual void filter(u32 uiCode, EXCEPTION_POINTERS* ppointers);


      virtual string _get_standard_exception_name(u32 uiCode) override;
      virtual string _get_standard_exception_description(u32 uiCode) override;

      static const char * _s_get_standard_exception_name(u32 uiCode);
      static const char * _s_get_standard_exception_description(u32 uiCode);

   };


} // namespace windows_common



