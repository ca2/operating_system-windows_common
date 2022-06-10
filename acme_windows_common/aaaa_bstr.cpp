#include "framework.h"

//
//BSTR AllocSysString(const ansichar * pchData, strsize nDataLength) noexcept
//{
//
//   BSTR bstr = nullptr;
//
//   strsize nLen = ::str().utf_to_utf_length(bstr, pchData, nDataLength);
//
//   bstr = ::SysAllocStringLen(nullptr, (::u32)nLen);
//
//   if (bstr != nullptr)
//   {
//
//      ::str().utf_to_utf(bstr, pchData, nDataLength);
//
//   }
//
//   return bstr;
//
//}
//
//
//// pbstr is [in,out] BSTR string
//bool SetSysString(BSTR * pbstr, const ansichar * pchData, strsize nDataLength) noexcept
//{
//
//   strsize nLen = ::str().utf_to_utf_length(pbstr, pchData, nDataLength);
//
//   bool bSuccess = ::SysReAllocStringLen(pbstr, nullptr, (::u32)nLen) != 0;
//
//   if (bSuccess)
//   {
//
//      ::str().utf_to_utf(*pbstr, pchData, nDataLength);
//
//   }
//
//   return bSuccess;
//
//}
//
//
//wstring bstr_to_wstr(BSTR bstr)
//{
//
//   int len = ::SysStringLen(bstr);
//
//   wstring wstr((wchar_t *)bstr, len);
//
//   return wstr;
//
//}
//
//
//string bstr_to_str(BSTR bstr)
//{
//
//   wstring wstr = bstr_to_wstr(bstr);
//
//   return wstr;
//
//}
//
