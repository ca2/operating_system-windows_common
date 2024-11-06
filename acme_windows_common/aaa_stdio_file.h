#pragma once


#define iFileNull ((int)-1)

namespace windows
{


   class CLASS_DECL_ACME stdio_file :
      virtual public ::file::text_file
   {
   public:


      FILE *            m_pStream;     // stdio FILE
      int               m_iFile;       // _fileno(m_pStream)



      stdio_file();
      virtual ~stdio_file();

      
      void dump(dump_context & dumpcontext) const;


      virtual void write_string(const ::string & psz);

      virtual char * read_string(char * psz, unsigned int nMax);

      virtual bool read_string(string & rString);



      virtual filesize get_position() const;
      virtual ::extended::status open(const ::file::path & pszFileName, ::file::e_open eopen) override;

      virtual memsize read(void * pdata, memsize nCount);

      virtual void write(const void * pdata, memsize nCount);

      virtual filesize seek(filesize lOff, ::enum_seek nFrom);
      virtual void Abort();
      virtual void Flush();
      virtual void close();
      virtual filesize get_length() const;

      // Unsupported APIs
      virtual file_pointer  Duplicate() const;
      virtual void LockRange(filesize dwPos, filesize dwCount);
      virtual void UnlockRange(filesize dwPos, filesize dwCount);


   };



} // namespace windows



