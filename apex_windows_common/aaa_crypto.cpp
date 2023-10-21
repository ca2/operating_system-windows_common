#include "framework.h"
#include "apex/operating_system.h"
//#include <shlobj.h>
//#include <Security.h>
//#include <shlobj.h>


//#include "windows_dir.h"
//#include "([a-z0-9_]+)_factory.h"
//#include "windows_port_forward.h"
//#include "windows_dir.h"
//#include "windows_file_system.h"
//#include "windows_file_set.h"
#include "crypto.h"
//#include "windows_ip_enum.h"

i32 crypto_encrypt(memory & storageEncrypt, const memory & storageDecrypt, memory & key);
i32 crypto_decrypt(memory & storageDecrypt, const memory & storageEncrypt, memory & key);

namespace windows
{


   crypto::crypto()
   {
   }



   crypto::~crypto()
   {

   }


   ::file::path crypto::get_crypt_key_file_path()
   {

      return m_pcontext->m_papexcontext->dir().appdata() / ".ca2/databin.bin";

   }


   bool crypto::decrypt(memory & storageDecrypt, const memory & storageEncrypt, const ::string & pszSalt)
   {

      return ::crypto::crypto::decrypt(storageDecrypt, storageEncrypt, pszSalt);

#if 0

      DATA_BLOB DataIn;
      DATA_BLOB DataOut;

      if (pszSalt == nullptr)
         pszSalt = "";

      DATA_BLOB DataSalt;
      memory memorySalt;
      memorySalt.from_string(pszSalt);
      DataSalt.pbData = memorySalt.get_data();
      DataSalt.cbData = (::u32)memorySalt.get_size();

      //--------------------------------------------------------------------
      // Initialize the DataIn structure.

      DataIn.pbData = storageEncrypt.get_data();
      DataIn.cbData = (::u32)storageEncrypt.get_size();

      unichar * pwsz = nullptr;


      //--------------------------------------------------------------------
      //  begin protect phase. Note that the encryption key is created
      //  by the function and is not passed.

      if (CryptUnprotectData(
            &DataIn,
            nullptr, // A description string
            // to be included with the
            // encrypted data.
            &DataSalt,                               // Optional entropy not used.
            nullptr,                               // Reserved.
            nullptr,                               // Pass nullptr for the
            // prompt structure.
            0,
            &DataOut))
      {
         informationf("crypto::decrypt The encryption phase worked. \n");
         storageDecrypt.allocate(DataOut.cbData);
         ::memory_copy(storageDecrypt.get_data(), DataOut.pbData, DataOut.cbData);
         LocalFree(pwsz);

         LocalFree(DataOut.pbData);
         return true;
      }
      else
      {
         //         TRACELASTERROR();
         informationf("crypto::decrypt Decryption error! (1)");
         return false;
      }

#else

      memory memOut;

      memory memIn;

      memIn.append(storageEncrypt.get_data(), storageEncrypt.get_size());

      memory memSalt;

      memSalt.append(pszSalt, strlen(pszSalt));

      if (!::crypto_decrypt(memOut, memIn, memSalt))
      {

         return false;

      }

      storageDecrypt = memOut;

      return true;

#endif

   }


   bool crypto::encrypt(memory & storageEncrypt, const memory & storageDecrypt, const ::string & pszSalt)
   {

      return ::crypto::crypto::encrypt(storageEncrypt, storageDecrypt, pszSalt);
#if 0
      DATA_BLOB DataIn;
      DATA_BLOB DataOut;

      if (pszSalt == nullptr)
         pszSalt = "";

      DATA_BLOB DataSalt;
      memory memorySalt;
      memorySalt.from_string(pszSalt);
      DataSalt.pbData = memorySalt.get_data();
      DataSalt.cbData = (::u32)memorySalt.get_size();


      //--------------------------------------------------------------------
      // Initialize the DataIn structure.

      DataIn.pbData = (::u8 *)storageDecrypt.get_data();
      DataIn.cbData = (::u32)storageDecrypt.get_size();

      //      unichar * pwsz = nullptr;


      //--------------------------------------------------------------------
      //  begin protect phase. Note that the encryption key is created
      //  by the function and is not passed.

      if (CryptProtectData(
            &DataIn,
            nullptr, // A description string
            // to be included with the
            // encrypted data.
            &DataSalt,                               // Optional entropy not used.
            nullptr,                               // Reserved.
            nullptr,                               // Pass nullptr for the
            // prompt structure.
            0,
            &DataOut))
      {
         informationf("crypto::encrypt The encryption phase worked. \n");
         storageEncrypt.allocate(DataOut.cbData);
         ::memory_copy(storageEncrypt.get_data(), DataOut.pbData, DataOut.cbData);
         LocalFree(DataOut.pbData);
         return true;
      }
      else
      {
         informationf("crypto::encrypt Encryption error! (1)");
         return false;
      }
#else

      memory memOut;

      memory memIn;

      memIn.append(storageDecrypt.get_data(), storageDecrypt.get_size());

      memory memSalt;

      memSalt.append(pszSalt, strlen(pszSalt));

      if (!::crypto_encrypt(memOut, memIn, memSalt))
         return false;


      storageEncrypt = memOut;

      return true;
#endif

   }


} // namespace windows



