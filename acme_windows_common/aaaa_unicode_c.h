#pragma once





CLASS_DECL_ACME
int
MultiByteToWideChar2(
unsigned int     CodePage,
unsigned int    dwFlags,
const char *   pMultiByteStr,

int      cbMultiByte,
::wide_character *  pWideCharStr,

int      cchWideChar);

CLASS_DECL_ACME
int
WideCharToMultiByte2(
unsigned int     CodePage,
unsigned int    dwFlags,
const ::wide_character *  pWideCharStr,

int      cchWideChar,
char *   pMultiByteStr,

int      cbMultiByte,
const char *   pDefaultChar,

int_bool *  pUsedDefaultChar);






