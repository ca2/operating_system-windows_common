// Created on 2021-04-29 12:33 BRT <3TBS_!! Second celebration of Mummis Birthday 70!!
#include "framework.h"
#include <eh.h>


bool g_bExiting;


#ifdef WINDOWS_DESKTOP


#define EXCEPTION_NO_MEMORY STATUS_NO_MEMORY


#endif


namespace windows_common
{


   namespace exception
   {


      translator::translator()
      {

//#ifdef EXCEPTION_TRANSLATOR_USE_SIGNAL
//
//         m_psig = new sig_companion;
//
//#endif

      }


      translator::~translator()
      {
//
//#ifdef EXCEPTION_TRANSLATOR_USE_SIGNAL
//
//         if (m_psig != nullptr)
//         {
//
//            delete (sig_companion*)m_psig;
//
//         }
//
//#endif

      }


      void __cdecl translator::filter2(u32 uiCode, EXCEPTION_POINTERS* ppointers)
      {

         if (g_bExiting)
         {

            return;

         }

         UNREFERENCED_PARAMETER(uiCode);

         //::acme::application * papp = ::get_application();

         switch (ppointers->ExceptionRecord->ExceptionCode)
         {
#ifdef WINDOWS_DESKTOP
         case EXCEPTION_NO_MEMORY:                 __throw(::windows::standard_no_memory(ppointers));                  break;
#endif
         case EXCEPTION_ACCESS_VIOLATION:          __throw(::windows::standard_access_violation(ppointers));           break;
         case EXCEPTION_DATATYPE_MISALIGNMENT:     __throw(::windows::standard_datatype_misalignment(ppointers));      break;
         case EXCEPTION_BREAKPOINT:                __throw(::windows::standard_breakpoint(ppointers));                 break;
         case EXCEPTION_SINGLE_STEP:               __throw(::windows::standard_single_step(ppointers));                break;
         case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:     __throw(::windows::standard_array_bounds_exceeded(ppointers));      break;
         case EXCEPTION_FLT_DENORMAL_OPERAND:      __throw(::windows::standard_flt_denormal_operand(ppointers));       break;
         case EXCEPTION_FLT_DIVIDE_BY_ZERO:        __throw(::windows::standard_flt_divide_by_zero(ppointers));         break;
         case EXCEPTION_FLT_INEXACT_RESULT:        __throw(::windows::standard_flt_inexact_result(ppointers));         break;
         case EXCEPTION_FLT_INVALID_OPERATION:     __throw(::windows::standard_flt_invalid_operation(ppointers));      break;
         case EXCEPTION_FLT_OVERFLOW:              __throw(::windows::standard_flt_overflow(ppointers));               break;
         case EXCEPTION_FLT_STACK_CHECK:           __throw(::windows::standard_flt_stack_check(ppointers));            break;
         case EXCEPTION_FLT_UNDERFLOW:             __throw(::windows::standard_flt_underflow(ppointers));              break;
         case EXCEPTION_INT_DIVIDE_BY_ZERO:        __throw(::windows::standard_int_divide_by_zero(ppointers));         break;
         case EXCEPTION_INT_OVERFLOW:              __throw(::windows::standard_int_overflow(ppointers));               break;
         case EXCEPTION_PRIV_INSTRUCTION:          __throw(::windows::standard_priv_instruction(ppointers));           break;
         case EXCEPTION_IN_PAGE_ERROR:             __throw(::windows::standard_in_page_error(ppointers));              break;
         case EXCEPTION_ILLEGAL_INSTRUCTION:       __throw(::windows::standard_illegal_instruction(ppointers));        break;
         case EXCEPTION_NONCONTINUABLE_EXCEPTION:  __throw(::windows::standard_noncontinuable_exception(ppointers));   break;
         case EXCEPTION_STACK_OVERFLOW:            __throw(::windows::standard_stack_overflow(ppointers));             break;
         case EXCEPTION_INVALID_DISPOSITION:       __throw(::windows::standard_invalid_disposition(ppointers));        break;
         case EXCEPTION_GUARD_PAGE:                __throw(::windows::standard_guard_page(ppointers));                 break;
         case EXCEPTION_INVALID_HANDLE:            __throw(::windows::standard_invalid_handle(ppointers));             break;
         case 0xE06D7363:                          __throw(::windows::standard_microsoft_cpp(ppointers));              break;
         case 0x40080201:                          __throw(::windows::standard_winrt_originate_error(ppointers));      break;
         default:                                  __throw(::windows::standard_exception(ppointers));                  break;
         };

      }

//#ifndef WINDOWS
//
//      void filter_sigsegv(i32 signal, siginfo_t* psiginfo, void* pc);
//      void filter_sigfpe(i32 signal, siginfo_t* psiginfo, void* pc);
//      void filter_sigpipe(i32 signal, siginfo_t* psiginfo, void* pc);
//
//#endif


      bool translator::attach()
      {

         if (!m_bSet)
         {

            m_pfn = _set_se_translator(&translator::filter2);

            //m_pfn = SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER) GetThunk());

            //PreventSetUnhandledExceptionFilter();

            //EnforceFilter(true);
//
//#if defined(__APPLE__)
//
//            ::InstallUncaughtExceptionHandler();
//
//#endif

            output_debug_string("exception standard translator");

//#if defined(__SANITIZE_ADDRESS__) || defined(__FOR_PERF__)
//
//            INFO("sanitize address compilation, not going to install standard exception translator");
//
//#else
//
//            __zero(m_sig.m_saSeg);
//
//            m_sig.m_saSeg.sa_flags = SA_SIGINFO;
//
//            m_sig.m_saSeg.sa_sigaction = &filter_sigsegv;
//
//            if (sigaction(SIGSEGV, &m_sig.m_saSeg, &m_sig.m_saSegOld) < 0)
//            {
//
//               output_debug_string("failed to install segmentation fault signal handler");
//
//            }
//
//            __zero(m_sig.m_saFpe);
//
//            m_sig.m_saSeg.sa_flags = SA_SIGINFO;
//
//            m_sig.m_saFpe.sa_sigaction = &filter_sigfpe;
//
//            if (sigaction(SIGFPE, &m_sig.m_saFpe, &m_sig.m_saFpeOld) < 0)
//            {
//
//               output_debug_string("failed to install floating point_i32 exception signal handler");
//
//            }
//
//            __zero(m_sig.m_saPipe);
//
//            m_sig.m_saSeg.sa_flags = SA_SIGINFO;
//
//            m_sig.m_saPipe.sa_sigaction = &filter_sigpipe;
//
//            if (sigaction(SIGPIPE, &m_sig.m_saPipe, &m_sig.m_saPipeOld) < 0)
//            {
//
//               output_debug_string("failed to install pipe signal handler");
//
//            }
//
//#endif
//
//#endif

            m_bSet = true;

            return true;

         }
         else
         {

            return false;

         }

      }


      bool translator::detach()
      {

         if (m_bSet)
         {

            _set_se_translator(m_pfn);

//#else
//
//            sigaction(SIGSEGV, &m_sig.m_saSegOld, nullptr);
//
//            sigaction(SIGFPE, &m_sig.m_saFpeOld, nullptr);
//
//#endif

            m_bSet = false;

            return true;

         }
         else
         {

            return false;

         }

      }


      void translator::filter(u32 uiCode, EXCEPTION_POINTERS* ppointers)
      {

         UNREFERENCED_PARAMETER(uiCode);

         switch (ppointers->ExceptionRecord->ExceptionCode)
         {
#ifdef WINDOWS_DESKTOP
         case EXCEPTION_NO_MEMORY:                 __throw(::windows::standard_no_memory(ppointers));                  break;
#endif
         case EXCEPTION_ACCESS_VIOLATION:          __throw(::windows::standard_access_violation(ppointers));           break;
         case EXCEPTION_DATATYPE_MISALIGNMENT:     __throw(::windows::standard_datatype_misalignment(ppointers));      break;
         case EXCEPTION_BREAKPOINT:                __throw(::windows::standard_breakpoint(ppointers));                 break;
         case EXCEPTION_SINGLE_STEP:               __throw(::windows::standard_single_step(ppointers));                break;
         case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:     __throw(::windows::standard_array_bounds_exceeded(ppointers));      break;
         case EXCEPTION_FLT_DENORMAL_OPERAND:      __throw(::windows::standard_flt_denormal_operand(ppointers));       break;
         case EXCEPTION_FLT_DIVIDE_BY_ZERO:        __throw(::windows::standard_flt_divide_by_zero(ppointers));         break;
         case EXCEPTION_FLT_INEXACT_RESULT:        __throw(::windows::standard_flt_inexact_result(ppointers));         break;
         case EXCEPTION_FLT_INVALID_OPERATION:     __throw(::windows::standard_flt_invalid_operation(ppointers));      break;
         case EXCEPTION_FLT_OVERFLOW:              __throw(::windows::standard_flt_overflow(ppointers));               break;
         case EXCEPTION_FLT_STACK_CHECK:           __throw(::windows::standard_flt_stack_check(ppointers));            break;
         case EXCEPTION_FLT_UNDERFLOW:             __throw(::windows::standard_flt_underflow(ppointers));              break;
         case EXCEPTION_INT_DIVIDE_BY_ZERO:        __throw(::windows::standard_int_divide_by_zero(ppointers));         break;
         case EXCEPTION_INT_OVERFLOW:              __throw(::windows::standard_int_overflow(ppointers));               break;
         case EXCEPTION_PRIV_INSTRUCTION:          __throw(::windows::standard_priv_instruction(ppointers));           break;
         case EXCEPTION_IN_PAGE_ERROR:             __throw(::windows::standard_in_page_error(ppointers));              break;
         case EXCEPTION_ILLEGAL_INSTRUCTION:       __throw(::windows::standard_illegal_instruction(ppointers));        break;
         case EXCEPTION_NONCONTINUABLE_EXCEPTION:  __throw(::windows::standard_noncontinuable_exception(ppointers));   break;
         case EXCEPTION_STACK_OVERFLOW:            __throw(::windows::standard_stack_overflow(ppointers));             break;
         case EXCEPTION_INVALID_DISPOSITION:       __throw(::windows::standard_invalid_disposition(ppointers));        break;
         };

      }


      string translator::_get_standard_exception_name(u32 uiCode)
      {

         auto pszName = _s_get_standard_exception_name(uiCode);

         return pszName;

      }


      string translator::_get_standard_exception_description(u32 uiCode)
      {

         auto pszDescription= _s_get_standard_exception_description(uiCode);

         return pszDescription;

      }


      const char * translator::_s_get_standard_exception_name(u32 uiCode)
      {

         const char * pszName = nullptr;

         switch (uiCode)
         {
#ifdef WINDOWS_DESKTOP
         case EXCEPTION_NO_MEMORY:                 pszName = "No Memory";                  break;
#endif
         case EXCEPTION_ACCESS_VIOLATION:          pszName = "Access Violation";           break;
         case EXCEPTION_DATATYPE_MISALIGNMENT:     pszName = "Datatype Misalignment";      break;
         case EXCEPTION_BREAKPOINT:                pszName = "Breakpoint";                 break;
         case EXCEPTION_SINGLE_STEP:               pszName = "Single Step";                break;
         case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:     pszName = "Array Bounds Exceeded";      break;
         case EXCEPTION_FLT_DENORMAL_OPERAND:      pszName = "Float Denormal Operand";     break;
         case EXCEPTION_FLT_DIVIDE_BY_ZERO:        pszName = "Float Divide by Zero";       break;
         case EXCEPTION_FLT_INEXACT_RESULT:        pszName = "Float Inexact Result";       break;
         case EXCEPTION_FLT_INVALID_OPERATION:     pszName = "Float Invalid Operation";    break;
         case EXCEPTION_FLT_OVERFLOW:              pszName = "Float Overflow";             break;
         case EXCEPTION_FLT_STACK_CHECK:           pszName = "Float Stack Check";          break;
         case EXCEPTION_FLT_UNDERFLOW:             pszName = "Float Underflow";            break;
         case EXCEPTION_INT_DIVIDE_BY_ZERO:        pszName = "Integer Divide by Zero";     break;
         case EXCEPTION_INT_OVERFLOW:              pszName = "Integer Overflow";           break;
         case EXCEPTION_PRIV_INSTRUCTION:          pszName = "Privileged Instruction";     break;
         case EXCEPTION_IN_PAGE_ERROR:             pszName = "In Page Error";              break;
         case EXCEPTION_ILLEGAL_INSTRUCTION:       pszName = "Illegal Instruction";        break;
         case EXCEPTION_NONCONTINUABLE_EXCEPTION:  pszName = "Noncontinuable Exception";   break;
         case EXCEPTION_STACK_OVERFLOW:            pszName = "Stack Overflow";             break;
         case EXCEPTION_INVALID_DISPOSITION:       pszName = "Invalid Disposition";        break;
         case EXCEPTION_GUARD_PAGE:                pszName = "Guard Page";                 break;
         case EXCEPTION_INVALID_HANDLE:            pszName = "Invalid Handle";             break;
         case 0xE06D7363:                          pszName = "Microsoft C++ Exception";    break;
         default:
            //pszName = "0x";
            //str += hex::lower_from(uiCode);
            break;
         };

         return pszName;

      }


      const char * translator::_s_get_standard_exception_description(u32 uiCode)
      {

         const char* pszDescription = nullptr;

         switch (uiCode)
         {
#ifdef WINDOWS_DESKTOP
         case EXCEPTION_NO_MEMORY:
            pszDescription = "The allocation attempt failed because of a lack of available memory or heap corruption.";
            break;
#endif
         case EXCEPTION_ACCESS_VIOLATION:
            pszDescription = "The thread attempted to read from or write to a virtual address for which it does not have the appropriate access";
            break;
         case EXCEPTION_DATATYPE_MISALIGNMENT:
            pszDescription = "The thread attempted to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries, 32-bit values on 4-byte boundaries, and so on";
            break;
         case EXCEPTION_BREAKPOINT:
            pszDescription = "A breakpoint was encountered";
            break;
         case EXCEPTION_SINGLE_STEP:
            pszDescription = "A trace trap or other single-instruction mechanism signaled that one instruction has been executed";
            break;
         case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            pszDescription = "The thread attempted to access an bastandard_array matter that is out of bounds, and the underlying hardware supports bounds checking";
            break;
         case EXCEPTION_FLT_DENORMAL_OPERAND:
            pszDescription = "One of the operands in a floating-point_i32 operation is denormal. A denormal value is one that is too small to represent as a standard floating-point_i32 value";
            break;
         case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            pszDescription = "The thread attempted to divide a floating-point_i32 value by a floating-point_i32 divisor of zero";
            break;
         case EXCEPTION_FLT_INEXACT_RESULT:
            pszDescription = "The result of a floating-point_i32 operation cannot be represented exactly as a decimal fraction";
            break;
         case EXCEPTION_FLT_INVALID_OPERATION:
            pszDescription = "This exception represents any floating-point_i32 exception not included in this list";
            break;
         case EXCEPTION_FLT_OVERFLOW:
            pszDescription = "The exponent of a floating-point_i32 operation is greater than the magnitude allowed by the corresponding type";
            break;
         case EXCEPTION_FLT_STACK_CHECK:
            pszDescription = "The stack overflowed or underflowed as the result of a floating-point_i32 operation";
            break;
         case EXCEPTION_FLT_UNDERFLOW:
            pszDescription = "The exponent of a floating-point_i32 operation is less than the magnitude allowed by the corresponding type";
            break;
         case EXCEPTION_INT_DIVIDE_BY_ZERO:
            pszDescription = "The thread attempted to divide an integer value by an integer divisor of zero";
            break;
         case EXCEPTION_INT_OVERFLOW:
            pszDescription = "The result of an integer operation caused a carry out of the most significant bit of the result";
            break;
         case EXCEPTION_PRIV_INSTRUCTION:
            pszDescription = "The thread attempted to execute an instruction whose operation is not allowed in the current machine mode";
            break;
         case EXCEPTION_IN_PAGE_ERROR:
            pszDescription = "The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network";
            break;
         case EXCEPTION_ILLEGAL_INSTRUCTION:
            pszDescription = "The thread tried to execute an invalid instruction";
            break;
         case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            pszDescription = "The thread attempted to continue execution after a noncontinuable exception occurred";
            break;
         case EXCEPTION_STACK_OVERFLOW:
            pszDescription = "The thread used up its stack";
            break;
         case EXCEPTION_INVALID_DISPOSITION:
            pszDescription = "An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception";
            break;
         case EXCEPTION_GUARD_PAGE:
            pszDescription = "Guard Page Exception";
            break;
         case EXCEPTION_INVALID_HANDLE:
            pszDescription = "Invalid Handle Exception";
            break;
         case 0xE06D7363:
            pszDescription = "Microsoft C++ Exception";
            break;
         default:
            pszDescription = "Unknown Exception (Not currently known by acme platform)";
            break;
         };

         return pszDescription;

      }


   } // namespace translator


} // namespace windows_common



