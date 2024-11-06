#pragma once


CLASS_DECL_ACME HANDLE create_file(
const char *            pFileName,

unsigned int                   dwDesiredAccess,
unsigned int                   dwShareMode,
LPSECURITY_ATTRIBUTES   pSecurityAttributes,

unsigned int                   dwCreationDisposition,
unsigned int                   dwFlagsAndAttributes,
HANDLE                  hTemplateFile
);



