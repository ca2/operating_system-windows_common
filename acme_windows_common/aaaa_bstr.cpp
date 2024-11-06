#include "framework.h"

//
//BSTR AllocSysString(const ::ansi_character * pchData, strsize nDataLength) noexcept
//{
//
//   BSTR bstr = nullptr;
//
//   strsize nLen = utf_to_utf_length(bstr, pchData, nDataLength);
//
//   bstr = ::SysAllocStringLen(nullptr, (unsigned int)nLen);
//
//   if (bstr != nullptr)
//   {
//
//      ::utf_to_utf(bstr, pchData, nDataLength);
//
//   }
//
//   return bstr;
//
//}
//
//
//// pbstr is [in,out] BSTR string
//bool SetSysString(BSTR * pbstr, const ::ansi_character * pchData, strsize nDataLength) noexcept
//{
//
//   strsize nLen = utf_to_utf_length(pbstr, pchData, nDataLength);
//
//   bool bSuccess = ::SysReAllocStringLen(pbstr, nullptr, (unsigned int)nLen) != 0;
//
//   if (bSuccess)
//   {
//
//      ::utf_to_utf(*pbstr, pchData, nDataLength);
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
