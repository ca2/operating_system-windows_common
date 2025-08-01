#pragma once


#include <eh.h>
#include "acme/exception/exception.h"
#include "exception_translator.h"

#if OSBIT == 64

#define DEFAULT_SE_EXCEPTION_CALLSTACK_SKIP 3000

#else

#define DEFAULT_SE_EXCEPTION_CALLSTACK_SKIP 0

#endif


#define DECLARE_SE_EXCEPTION_CLASS(name) \
   class CLASS_DECL_ACME_WINDOWS_COMMON name : \
      public standard_exception \
   { \
   \
      friend class translator; \
   \
   public: \
   \
         name (EXCEPTION_POINTERS * ppointers); \
      \
      \
   }; 

#define DEFINE_SE_EXCEPTION_CLASS(name) \
         name::name (EXCEPTION_POINTERS * ppointers) : \
            standard_exception(DECLARE_SE_EXCEPTION_ERROR(name), nullptr, nullptr, ppointers) \
      { \
        \
      } 



namespace acme_windows_common
{


   class CLASS_DECL_ACME_WINDOWS_COMMON standard_exception :
      public ::exception
   {
   public:


      friend class ::exception_translator;


#ifdef WINDOWS
      EXCEPTION_POINTERS * m_ppointers;
#else
      int            m_iSignal;
      void * m_psiginfo;
#ifndef ANDROID
      ucontext_t     m_ucontext;
#endif
#endif




      unsigned int         code() const { return m_ppointers->ExceptionRecord->ExceptionCode; }
      void * address() const { return m_ppointers->ExceptionRecord->ExceptionAddress; }
      EXCEPTION_POINTERS * info() const { return m_ppointers; }
      const_char_pointer name() const { return exception_translator::_get_standard_exception_name(code()); }
      const_char_pointer description() const { return exception_translator::_get_standard_exception_description(code()); }
      bool is_read_op() const { return !info()->ExceptionRecord->ExceptionInformation[0]; }
      uptr inaccessible_address() const { return info()->ExceptionRecord->ExceptionInformation[1]; }



      standard_exception(const ::e_status & estatus, const ::scoped_string & scopedstrMessage, const ::scoped_string & scopedstrDetails, EXCEPTION_POINTERS * ppointers);
      ~standard_exception() override;


   };


   class standard_no_memory : public standard_exception
   {
   public:


      standard_no_memory(EXCEPTION_POINTERS * ppointers) :
         standard_exception(error_no_memory, nullptr, nullptr, ppointers)
      {

      }

   public:
      size_t mem_size() const { return info()->ExceptionRecord->ExceptionInformation[0]; }
   };

   class standard_access_violation : public standard_exception
   {
   public:

      standard_access_violation(EXCEPTION_POINTERS * ppointers) :
         standard_exception(error_segmentation_fault, nullptr, nullptr, ppointers)
      {

      }

   };


   DECLARE_SE_EXCEPTION_CLASS(standard_datatype_misalignment)
   DECLARE_SE_EXCEPTION_CLASS(standard_breakpoint)
   DECLARE_SE_EXCEPTION_CLASS(standard_single_step)
   DECLARE_SE_EXCEPTION_CLASS(standard_array_bounds_exceeded)
   DECLARE_SE_EXCEPTION_CLASS(standard_flt_denormal_operand)
   DECLARE_SE_EXCEPTION_CLASS(standard_flt_divide_by_zero)
   DECLARE_SE_EXCEPTION_CLASS(standard_flt_inexact_result)
   DECLARE_SE_EXCEPTION_CLASS(standard_flt_invalid_operation)
   DECLARE_SE_EXCEPTION_CLASS(standard_flt_overflow)
   DECLARE_SE_EXCEPTION_CLASS(standard_flt_stack_check)
   DECLARE_SE_EXCEPTION_CLASS(standard_flt_underflow)
   DECLARE_SE_EXCEPTION_CLASS(standard_int_divide_by_zero)
   DECLARE_SE_EXCEPTION_CLASS(standard_int_overflow)
   DECLARE_SE_EXCEPTION_CLASS(standard_priv_instruction)
   DECLARE_SE_EXCEPTION_CLASS(standard_in_page_error)
   DECLARE_SE_EXCEPTION_CLASS(standard_illegal_instruction)
   DECLARE_SE_EXCEPTION_CLASS(standard_noncontinuable_exception)
   DECLARE_SE_EXCEPTION_CLASS(standard_stack_overflow)
   DECLARE_SE_EXCEPTION_CLASS(standard_invalid_disposition)
   DECLARE_SE_EXCEPTION_CLASS(standard_guard_page)
   DECLARE_SE_EXCEPTION_CLASS(standard_invalid_handle)
   DECLARE_SE_EXCEPTION_CLASS(standard_microsoft_cpp)
   DECLARE_SE_EXCEPTION_CLASS(standard_winrt_originate_error)


} // namespace acme_windows_common




