#pragma once



using hfile = void *;

#define hfile_null ((::hfile)(::iptr) -1)

inline int_bool is_nok(hfile hfile)
{

   return hfile == nullptr || hfile == hfile_null;

}


inline int_bool is_ok(hfile hfile)
{

   return !is_nok(hfile);

}

using HANDLE = void *;

//CLASS_DECL_ACME hfile hfile_create(
//const char *            pFileName,
//unsigned int                   dwDesiredAccess,
//unsigned int                   dwShareMode,
//void *                  pSecurityAttributes,
//unsigned int                   dwCreationDisposition,
//unsigned int                   dwFlagsAndAttributes,
//HANDLE                  hTemplateFile
//);
//
//
//CLASS_DECL_ACME_WINDOWS_COMMON void hfile_close(hfile hfile);
//CLASS_DECL_ACME_WINDOWS_COMMON memsize hfile_read(hfile hfile, void * p, memsize s);
//CLASS_DECL_ACME_WINDOWS_COMMON void hfile_write(hfile hfile, const void* p, memsize s);
