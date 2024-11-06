//
//
//	   Main header for the file_watcher class. Declares all implementation
//	   classes to reduce compilation overhead.
//
//	   @author James Wynn
//	   @date 4/15/2009
//
//	   Copyright (c) 2009 James Wynn (james@jameswynn.com)
//
//	   Permission is hereby granted, free of charge, to any person obtaining a copy
//	   of this software and associated documentation files (the "Software"), to deal
//	   in the Software without restriction, including without limitation the rights
//	   to uxse, cxopy, mxodify, mxerge, pxublish, dxistribute, sxublicense, and/or sxell
//	   copies of the Software, and to permit persons to whom the Software is
//	   furnished to do so, topic to the following conditions:
//
//
#pragma once


namespace file
{


   class CLASS_DECL_APEX_WINDOWS_COMMON  os_watch :
      virtual public watch
   {
   public:


      OVERLAPPED           m_overlapped;
      HANDLE               m_hDirectory;
      unsigned char                 m_buffer[32 * 1024];
      LPARAM               m_lparam;
      DWORD                m_dwNotify;
      bool                 m_bRefresh;


      os_watch();
      virtual ~os_watch();

      virtual bool open(const ::file::path & pathFolder, bool bRecursive) override;

      static void CALLBACK callback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED pOverlapped);


      virtual void step() override;

   };


   class CLASS_DECL_APEX_WINDOWS_COMMON os_watcher :
      virtual public watcher
   {
   public:


      os_watcher();
      virtual ~os_watcher();


      //virtual void     run() override;

      virtual void step();


   };


} // namespace file



