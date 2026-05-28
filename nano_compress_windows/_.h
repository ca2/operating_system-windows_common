//
// Created by camilo on 20/08/2023 02:14 <3ThomasBorregaardSorensen!!
//
#pragma once


#include "acme/_.h"
#include "acme/_operating_system.h"


#if defined(_nano_compress_windows_project)
#define CLASS_DECL_NANO_COMPRESS_WINDOWS CLASS_DECL_EXPORT
#else
#define CLASS_DECL_NANO_COMPRESS_WINDOWS CLASS_DECL_IMPORT
#endif




namespace windows
{


   namespace nano
   {


      namespace http
      {


         class connect;
         class request;
         class session;


      } // namespace http


   } // namespace nano


} // namespace windows



