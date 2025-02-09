// Create by camilo on 2024-12-30 21:16 <3ThomasBorregaardSorensen!!
// chatgpt-2024-12-30-21-06://How to load a font file into memory and load a font from this font using DirectWrite Windows Component in C++?
#include "framework.h"
#include "font.h"
#include "internal_font.h"
#include "direct2d/direct2d.h"
//#include <wrl.h>
//#include <fstream>
//#include <vector>
//#include <string>

//using Microsoft::WRL::ComPtr;

struct custom_data_size_key
{

   void *            data;
   unsigned long long    size;

};

template < typename TYPE >
class com_object :
   virtual public TYPE
{
public:
   
   
   ULONG             m_uReferenceCount;

   com_object() :
      m_uReferenceCount(1)
   {

   }

   
   HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject) override
   {

      if (riid == __uuidof(IUnknown)) 
      {
      
         *ppvObject = (IUnknown *)this;
         
         AddRef();

         return S_OK;

      }

      *ppvObject = nullptr;

      return E_NOINTERFACE;

   }
        

   ULONG STDMETHODCALLTYPE AddRef() override
   {

      return InterlockedIncrement(&m_uReferenceCount);

   }

   ULONG STDMETHODCALLTYPE Release() override 
   {

      ULONG newCount = InterlockedDecrement(&m_uReferenceCount);

      if (newCount == 0) 
      {

         delete this;

      }

      return newCount;

   }

};


// Custom Font File Stream
class CustomFontFileStream : 
   public com_object < IDWriteFontFileStream >
{
public:


   custom_data_size_key  m_datasizekey;

   CustomFontFileStream(custom_data_size_key datasizekey):
      m_datasizekey(datasizekey)
   {
   }

   HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject) override
   {

      if (riid == __uuidof(IDWriteFontFileStream)) 
      {
         
         *ppvObject = static_cast<IDWriteFontFileStream *>(this);
         
         AddRef();

         return S_OK;

      }

      return com_object::QueryInterface(riid, ppvObject);

   }


   HRESULT STDMETHODCALLTYPE ReadFileFragment(
       const void ** fragmentStart,
       UINT64 fileOffset,
       UINT64 fragmentSize,
       void ** fragmentContext) override 
   {

      if (fileOffset + fragmentSize > m_datasizekey.size) 
      {
         
         return E_FAIL;

      }

      *fragmentStart = static_cast<const BYTE *>(m_datasizekey.data) + fileOffset;

      *fragmentContext = nullptr;

      return S_OK;

   }


   void STDMETHODCALLTYPE ReleaseFileFragment(void * /*fragmentContext*/) override
   {
   
   }

   HRESULT STDMETHODCALLTYPE GetFileSize(UINT64 * fileSize) override 
   {

      *fileSize = m_datasizekey.size;

      return S_OK;

   }

   HRESULT STDMETHODCALLTYPE GetLastWriteTime(UINT64 * lastWriteTime) override 
   {

      *lastWriteTime = 0;

      return S_OK;

   }

};


// Custom Font File Loader
class CustomFontFileLoader :
   public com_object < IDWriteFontFileLoader >
{
public:


   CustomFontFileLoader() {}

   HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject) override 
   {
      
      if (riid == __uuidof(IDWriteFontFileLoader)) 
      {
         
         *ppvObject = static_cast<IDWriteFontFileLoader *>(this);
         
         AddRef();
         
         return S_OK;

      }

      return com_object::QueryInterface(riid, ppvObject);

   }


   HRESULT STDMETHODCALLTYPE CreateStreamFromKey(
       const void * fontFileReferenceKey,
       UINT32 fontFileReferenceKeySize,
       IDWriteFontFileStream ** fontFileStream) override
   {

      const auto * pkey = (custom_data_size_key *)(fontFileReferenceKey);

      *fontFileStream = new CustomFontFileStream(*pkey);

      return S_OK;

   }

};


// Custom Font File Enumerator
class CustomFontFileEnumerator : 
   public com_object < IDWriteFontFileEnumerator  >
{
public:

   ::comptr<IDWriteFactory>         factory_;
   custom_data_size_key             m_datasizekey;
   bool                             hasNext_;
   ::comptr<IDWriteFontFileLoader>   fontFileLoader_;
   
   CustomFontFileEnumerator(
      IDWriteFactory * factory, 
      custom_data_size_key datasizekey, 
      IDWriteFontFileLoader * ploader) :
      factory_(factory),
      m_datasizekey(datasizekey), 
      hasNext_(true),
      fontFileLoader_(ploader)
   {
      
   }

   ~CustomFontFileEnumerator() {
   }

   HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject) override
   {
      if (riid == __uuidof(IDWriteFontFileEnumerator)) {
         *ppvObject = static_cast<IDWriteFontFileEnumerator *>(this);
         AddRef();
         return S_OK;
      }
      return com_object::QueryInterface(riid, ppvObject);
   }


   HRESULT STDMETHODCALLTYPE MoveNext(BOOL * hasCurrentFile) override {
      *hasCurrentFile = hasNext_;
      hasNext_ = FALSE; // Only one font file in this case
      return S_OK;
   }

   
   HRESULT STDMETHODCALLTYPE GetCurrentFontFile(IDWriteFontFile ** fontFile) override 
   {

      return factory_->CreateCustomFontFileReference(
         &m_datasizekey,
         sizeof(m_datasizekey), 
         fontFileLoader_,
         fontFile);

   }

};

//int main() {
//   // Initialize COM
//   HRESULT hr = CoInitialize(nullptr);
//   if (FAILED(hr)) {
//      return -1;
//   }
//
//   // Load font data into memory
//   std::ifstream fontFile("path/to/font.ttf", std::ios::binary);
//   if (!fontFile) {
//      return -1;
//   }

   //std::vector<char> fontData((std::istreambuf_iterator<char>(fontFile)), std::istreambuf_iterator<char>());
   //fontFile.close();

   //// Create DirectWrite factory
   //ComPtr<IDWriteFactory> factory;
   //hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &factory);
   //if (FAILED(hr)) {
   //   return -1;
   //}

//   // Register custom loader
//   auto * loader = new CustomFontFileLoader();
//   hr = factory->RegisterFontFileLoader(loader);
//   if (FAILED(hr)) {
//      return -1;
//   }
//
//   // Create font file reference
//   ComPtr<IDWriteFontFile> fontFileReference;
//   std::pair<const void *, UINT32> key(fontData.data(), static_cast<UINT32>(fontData.size()));
//   hr = factory->CreateCustomFontFileReference(&key, sizeof(key), loader, &fontFileReference);
//   if (FAILED(hr)) {
//      return -1;
//   }
//
//   // Create font face
//   ComPtr<IDWriteFontFace> fontFace;
//   hr = factory->CreateFontFaceFromFile(fontFileReference.Get(), 0, DWRITE_FONT_SIMULATIONS_NONE, &fontFace);
//   if (FAILED(hr)) {
//      return -1;
//   }
//
//   // Cleanup
//   factory->UnregisterFontFileLoader(loader);
//   loader->Release();
//   CoUninitialize();
//
//   return 0;
//}
//



// Custom Font Collection Loader
class CustomFontCollectionLoader : 
   public com_object < IDWriteFontCollectionLoader  >
{
public:

   ::comptr< IDWriteFontFileLoader >m_pfontfileloader;


   CustomFontCollectionLoader(IDWriteFontFileLoader * pfontfileloader) :
      m_pfontfileloader(pfontfileloader) 
   {
   
   }

   HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject) override {
      if (riid == __uuidof(IDWriteFontCollectionLoader)) {
         *ppvObject = static_cast<IDWriteFontCollectionLoader *>(this);
         AddRef();
         return S_OK;

      }
      return com_object::QueryInterface(riid, ppvObject);

   }



   HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(
       IDWriteFactory * factory,
       const void * collectionKey,
       UINT32 collectionKeySize,
       IDWriteFontFileEnumerator ** fontFileEnumerator) override 
   {
   
      auto * pdatasizekey = (custom_data_size_key *) (collectionKey);

      *fontFileEnumerator = new CustomFontFileEnumerator(factory,
         *pdatasizekey,
         m_pfontfileloader);

      return S_OK;

   }


};



namespace draw2d_direct2d
{


   internal_font::internal_font()
   {


   }


   internal_font::~internal_font()
   {
      IDWriteFactory * pfactory = ::direct2d::direct2d()->dwrite_factory();

      pfactory->UnregisterFontFileLoader(m_pfontfileloader);
      pfactory->UnregisterFontCollectionLoader(m_pfontcollectionloader);

   }


   void internal_font::load_from_memory(::memory_base * pmemory)
   {

      if (pmemory->is_empty())
      {

         throw ::exception(error_wrong_state);

      }

      IDWriteFactory * pfactory = ::direct2d::direct2d()->dwrite_factory();

      //// Register custom loader
      m_pfontfileloader.m_p = new CustomFontFileLoader();
      HRESULT hr = pfactory->RegisterFontFileLoader(m_pfontfileloader);
      if (FAILED(hr)) {
         throw ::exception(error_failed);
      }

      // Register custom loader
      m_pfontcollectionloader.m_p = new CustomFontCollectionLoader(m_pfontfileloader);

      hr = pfactory->RegisterFontCollectionLoader(m_pfontcollectionloader);
      if (FAILED(hr)) {
         throw ::exception(error_failed);
      }


      //// Create font file reference
      //::comptr<IDWriteFontFile> pfontFileReference;
      //hr = pfactory->CreateCustomFontFileReference(pmemory, sizeof(pmemory), ploader, &pfontFileReference);
      //if (FAILED(hr)) {
      //   throw ::exception(error_failed);
      //}

      custom_data_size_key datasizekey;

      datasizekey.data = pmemory->data();
      datasizekey.size = pmemory->size();

      // Create font file reference
      //::comptr<IDWriteFontCollection> pfontCollection;
      hr = pfactory->CreateCustomFontCollection(
         m_pfontcollectionloader, 
         &datasizekey,
         sizeof(datasizekey),
         &m_pcollection);

      if (FAILED(hr)) 
      {

         throw ::exception(error_failed);

      }

      //// Create font face
      //::comptr<IDWriteFontFace> pfontFace;
      //hr = pfactory->CreateFontFaceFromFile(pfontFileReference, 0, DWRITE_FONT_SIMULATIONS_NONE, &pfontFace);
      //if (FAILED(hr)) {
      //   throw ::exception(error_failed);
      //}

      // Cleanup
      //loader->Release();
      //CoUninitialize();

      //return 0;
   }



   //   m_pcollection = __raw_new Gdiplus::PrivateFontCollection();

   //   m_pcollection->AddMemoryFont(pmemory->data(), (INT)pmemory->size());

   //   auto & fontCollection = *m_pcollection;

   //   auto iFamilyCount = fontCollection.GetFamilyCount();

   //   m_familya.set_size(iFamilyCount);

   //   fontCollection.GetFamilies(iFamilyCount, m_familya.data(), &m_iFamilyCount);

   //   m_familya.set_size(iFamilyCount);

   //   for (int iFamily = 0; iFamily < iFamilyCount; iFamily++)
   //   {

   //      if (m_familya[iFamily].GetLastStatus() != Gdiplus::Ok)
   //      {

   //         warningf("font family nok");

   //      }

   //   }

   //}


   void internal_font::on_create_font(::draw2d::graphics * pgraphics, ::write_text::font * pfont)
   {

      ::cast < ::draw2d_direct2d::font> pdraw2ddirect2dfont = pfont;

      IDWriteFactory * pfactory = ::direct2d::direct2d()->dwrite_factory();

      ::wstring wstrFamilyName;

      pdraw2ddirect2dfont->m_pcollection = m_pcollection;

      if (pdraw2ddirect2dfont->::write_text::font::m_pfontfamily->m_strFamilyName.has_character())
      {

         wstrFamilyName = pdraw2ddirect2dfont->::write_text::font::m_pfontfamily->m_strFamilyName;

      }
      else
      {

         auto uFamilyCount = m_pcollection->GetFontFamilyCount();

         if (uFamilyCount <= 0)
         {

            throw exception(error_resource);

         }

         auto & pdwritefontfamily = pdraw2ddirect2dfont->m_pfamily;

         HRESULT hrItem = m_pcollection->GetFontFamily(0, &pdwritefontfamily);

         ::comptr<IDWriteLocalizedStrings> pFamilyNames;

         // Get a list of localized strings for the family name.
         if (SUCCEEDED(hrItem))
         {

            hrItem = pdwritefontfamily->GetFamilyNames(&pFamilyNames);

         }

         UINT32 index = 0;

         BOOL exists = false;

         if (SUCCEEDED(hrItem))
         {

            hrItem = pFamilyNames->FindLocaleName(L"en-us", &index, &exists);

         }

         if (!exists)
         {

            index = 0;

         }

         UINT32 length = 0;

         if (SUCCEEDED(hrItem))
         {

            hrItem = pFamilyNames->GetStringLength(index, &length);

         }

         // Allocate a string big enough to hold the name.
         wchar_t * name = new (std::nothrow) wchar_t[length + 1];

         if (name == NULL)
         {

            hrItem = E_OUTOFMEMORY;

         }

         // Get the family name.
         if (SUCCEEDED(hrItem))
         {

            hrItem = pFamilyNames->GetString(index, name, length + 1);

         }

         wstrFamilyName = name;

         delete name;

      }

      auto weight = pdraw2ddirect2dfont->_dwrite_font_weight();
      auto style = pdraw2ddirect2dfont->_dwrite_font_style();
      auto stretch = pdraw2ddirect2dfont->_dwrite_font_stretch();
      auto size = pdraw2ddirect2dfont->_dwrite_font_size(pgraphics);

      HRESULT hr = pfactory->CreateTextFormat(
         wstrFamilyName.c_str(),
         m_pcollection,
         weight,
         style,
         stretch,
         size,
         L"",
         &pdraw2ddirect2dfont->m_pformat);

      if (FAILED(hr))
      {

         throw exception(error_resource);

      }


      //auto uFamilyCount = m_pcollection->GetFontFamilyCount();

      //if (uFamilyCount <= 0)
      //{

      //   throw exception(error_resource);

      //}

      //::cast < ::draw2d_direct2d::font> pdraw2ddirect2dfont = pfont;

      //int iFoundFamily = -1;

      ////WCHAR wszGetFamilyName[LF_FACESIZE];

      //auto & pfamily = pdraw2ddirect2dfont->m_pfamily;

      //if (pdraw2ddirect2dfont->::write_text::font::m_pfontfamily->m_strFamilyName.has_character())
      //{

      //   for (decltype(uFamilyCount) iFamily = 0; iFamily < uFamilyCount; iFamily++)
      //   {

      //      ::comptr < IDWriteFontFamily > pdwritefontfamily;

      //      HRESULT hrItem = m_pcollection->GetFontFamily(0, &pdwritefontfamily);

      //      m_familya.add(pdwritefontfamily);

      //      IDWriteLocalizedStrings * pFamilyNames = NULL;

      //      // Get a list of localized strings for the family name.
      //      if (SUCCEEDED(hrItem))
      //      {
      //         hrItem = pdwritefontfamily->GetFamilyNames(&pFamilyNames);
      //      }

      //      UINT32 index = 0;
      //      BOOL exists = false;

      //      wchar_t localeName[LOCALE_NAME_MAX_LENGTH];

      //      if (SUCCEEDED(hrItem))
      //      {
      //         // Get the default locale for this user.
      //         int defaultLocaleSuccess = GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);

      //         // If the default locale is returned, find that locale name, otherwise use "en-us".
      //         if (defaultLocaleSuccess)
      //         {
      //            hrItem = pFamilyNames->FindLocaleName(localeName, &index, &exists);
      //         }
      //         if (SUCCEEDED(hrItem) && !exists) // if the above find did not find a match, retry with US English
      //         {
      //            hrItem = pFamilyNames->FindLocaleName(L"en-us", &index, &exists);
      //         }
      //      }

      //      // If the specified locale doesn't exist, select the first on the list.
      //      if (!exists)
      //         index = 0;

      //      UINT32 length = 0;

      //      // Get the string length.
      //      if (SUCCEEDED(hrItem))
      //      {
      //         hrItem = pFamilyNames->GetStringLength(index, &length);
      //      }

      //      // Allocate a string big enough to hold the name.
      //      wchar_t * name = new (std::nothrow) wchar_t[length + 1];
      //      if (name == NULL)
      //      {
      //         hrItem = E_OUTOFMEMORY;
      //      }

      //      // Get the family name.
      //      if (SUCCEEDED(hrItem))
      //      {
      //         hrItem = pFamilyNames->GetString(index, name, length + 1);
      //      }

      //      ::string strName;

      //      strName = name;

      //      delete name;

      //      if (strName == pdraw2ddirect2dfont->m_pfontfamily->m_strFamilyName)
      //      {

      //         pfamily = pdwritefontfamily;

      //         break;

      //      }

      //   }

      //}

      //if (::is_null(pfamily))
      //{

      //   pfamily = m_familya.first();

      //}

      //if (::is_null(pfamily))
      //{

      //   throw exception(error_resource);

      //}


      //::comptr < IDWriteFont > pdwritefont;

      //HRESULT hrFirstMatchingFont = pfamily->GetFirstMatchingFont(
      //         pdraw2ddirect2dfont->_dwrite_font_weight(),
      //         pdraw2ddirect2dfont->_dwrite_font_stretch(),
      //         pdraw2ddirect2dfont->_dwrite_font_style(),
      //         &pdwritefont);

      //if (FAILED(hrFirstMatchingFont) || !pfont)
      //{

      //   throw exception(error_resource);

      //}

      ////pfontfamily = &pprivatefont->m_familya[iFoundFamily];

      //////if (pfontfamily->GetFamilyName(wszGetFamilyName) != Gdiplus::Ok)
      //////{

      //////   throw exception(error_resource);

      //////}

      //////auto pfont = ___new Gdiplus::Font(
      //////   wszGetFamilyName,
      //////   (Gdiplus::REAL)m_dFontSize,
      //////   iStyle,
      //////   unit,
      //////   pprivatefont->m_pcollection);

      ////pfontfamily

      ////set_gdiplus_font(pfont);

      ////bFont = true;

      ////auto pgdiplusfont = __raw_new Gdiplus::Font(
      ////   pgdiplusfontfamily,
      ////   gdiplus_font_size(pdraw2dgdiplusfont->m_fontsize),
      ////   pdraw2dgdiplusfont->m_iStyle,
      ////   gdiplus_font_unit(pdraw2dgdiplusfont->m_fontsize));

      ////pdraw2dgdiplusfont->set_gdiplus_font(pgdiplusfont);

      ////}
      ////else
      ////{

      ////   auto pfont = ___new Gdiplus::Font(
      ////      &pprivatefont->m_familya.first(),
      ////      (Gdiplus::REAL)m_dFontSize,
      ////      iStyle,
      ////      unit);

      ////   set_gdiplus_font(pfont);

      ////   bFont = true;

      ////}

   }



} // namespace draw2d_direct2d



