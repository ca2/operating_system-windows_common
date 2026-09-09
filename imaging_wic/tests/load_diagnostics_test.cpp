// Standalone WIC tests plus production diagnostic wiring checks. Run from main.
#include <windows.h>
#include <wincodec.h>
#include <d2d1_1.h>
#include <wrl/client.h>
#include <fstream>
#include <iterator>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <future>
#include <chrono>
using Microsoft::WRL::ComPtr;
static void require(bool value, const char * message)
{
   if (!value) { std::fprintf(stderr, "%s\n", message); std::exit(1); }
}
static std::string read(const char * path)
{
   std::ifstream file(path, std::ios::binary);
   require(file.good(), path);
   return {std::istreambuf_iterator<char>(file), {}};
}
struct result { HRESULT hr; const char * stage; UINT width = 0, height = 0; };
static result decode(IWICImagingFactory * factory, std::string & bytes)
{
   if (bytes.empty()) return {E_INVALIDARG, "InputBufferEmpty"};
   ComPtr<IWICStream> stream;
   HRESULT hr = factory->CreateStream(&stream);
   if (FAILED(hr)) return {hr, "CreateStream"};
   hr = stream->InitializeFromMemory(reinterpret_cast<BYTE *>(bytes.data()), static_cast<DWORD>(bytes.size()));
   if (FAILED(hr)) return {hr, "InitializeFromMemory"};
   ComPtr<IWICBitmapDecoder> decoder;
   hr = factory->CreateDecoderFromStream(stream.Get(), nullptr, WICDecodeMetadataCacheOnLoad, &decoder);
   if (FAILED(hr)) return {hr, "CreateDecoderFromStream"};
   ComPtr<IWICBitmapFrameDecode> frame;
   hr = decoder->GetFrame(0, &frame);
   if (FAILED(hr)) return {hr, "GetFrame(0)"};
   ComPtr<IWICFormatConverter> converter;
   hr = factory->CreateFormatConverter(&converter);
   if (FAILED(hr)) return {hr, "CreateFormatConverter"};
   hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA,
      WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom);
   if (FAILED(hr)) return {hr, "Convert32bppPBGRA"};
   ComPtr<IWICBitmap> bitmap;
   hr = factory->CreateBitmapFromSource(converter.Get(), WICBitmapCacheOnLoad, &bitmap);
   if (FAILED(hr)) return {hr, "CreateBitmapFromSource"};
   UINT width, height;
   hr = bitmap->GetSize(&width, &height);
   if (FAILED(hr)) return {hr, "GetSize"};
   WICRect rect{0, 0, static_cast<INT>(width), static_cast<INT>(height)};
   ComPtr<IWICBitmapLock> lock;
   hr = bitmap->Lock(&rect, WICBitmapLockRead, &lock);
   if (FAILED(hr)) return {hr, "LockBitmapRead"};
   UINT stride, area;
   BYTE * data;
   hr = lock->GetStride(&stride);
   if (FAILED(hr)) return {hr, "GetStride"};
   hr = lock->GetDataPointer(&area, &data);
   if (FAILED(hr)) return {hr, "GetDataPointer"};
   require(data && stride >= width * 4 && area >= stride * (height - 1) + width * 4,
      "Invalid native pixel buffer");
   return {S_OK, "decoded", width, height};
}
int main()
{
   auto wic = read("operating_system/operating_system-windows_common/imaging_wic/load.cpp");
   for (auto stage : {"CreateStream", "InputBufferEmpty", "InputBufferExceedsWICDWORD",
      "InitializeFromMemory", "CreateDecoderFromStream", "GetFrame(0)", "CreateFormatConverter",
      "Convert32bppPBGRA", "CreateBitmapFromSource", "GetSize", "ValidateDimensions",
      "LockBitmapRead", "GetStride", "GetDataPointer", "ValidatePixelBuffer", "DeliverDecodedPixels"})
      require(wic.find(std::string("\"") + stage + "\"") != std::string::npos, stage);
   require(wic.find("hr = piLock->GetStride(&cbStride);") != std::string::npos,
      "GetStride must check its own HRESULT");
   require(wic.find("stage=%s hr=0x%08X path=%s size=%ux%u stride=%u") != std::string::npos,
      "Failure details must include stage/HRESULT/path/geometry");
   require(wic.find("#ifndef IMAGING_WIC_LOAD_TRACE_LEVEL") != std::string::npos
      && wic.find("#if IMAGING_WIC_LOAD_TRACE_LEVEL >= 1") != std::string::npos
      && wic.find("#if IMAGING_WIC_LOAD_TRACE_LEVEL >= 2") != std::string::npos,
      "Successful decoding must be opt-in at two verbosity levels");
   auto loader = read("source/app/aura/graphics/image/load_image.cpp");
   for (auto text : {"exception.get_message()", "exception.what()", "attempt < 2",
      "completion callback failed", "retry-without-cache", "give-up"})
      require(loader.find(text) != std::string::npos, text);
   auto file = read("source/app/aura/graphics/image/context.cpp");
   require(file.find("#define AURA_IMAGE_LOAD_TRACE_LEVEL 0") != std::string::npos
      && file.find("file read returned no bytes") != std::string::npos,
      "File reads must be quiet on success and descriptive on failure");
   require(file.find("synchronous load returned invalid destination") != std::string::npos
      && file.find("invalid path-cache return") != std::string::npos
      && file.find("cache_hit=%d") != std::string::npos,
      "Pixmap-return diagnostics must distinguish loader completion and cache state");
   auto taskStart = file.find("void image_context::_task_load_image(");
   auto taskEnd = file.find("void image_context::_os_load_image(", taskStart);
   require(taskStart != std::string::npos && taskEnd != std::string::npos,
      "Missing CPU loader implementation");
   require(file.substr(taskStart, taskEnd - taskStart).find("::draw2d::lock") == std::string::npos,
      "CPU image worker must not wait for the scene caller's device lock");

   require(SUCCEEDED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)), "COM initialization failed");
   {
      ComPtr<IWICImagingFactory> factory;
      require(SUCCEEDED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
         IID_PPV_ARGS(&factory))), "WIC initialization failed");
      auto png = read("source/app-graphics3d/_matter/continuum/_std/_std/textures/rosendal/right.png");
      auto good = decode(factory.Get(), png);
      require(SUCCEEDED(good.hr) && good.width == 4096 && good.height == 4096,
         "Rosendal PNG must fully decode");

      ComPtr<ID2D1Factory> d2dFactory;
      require(SUCCEEDED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, d2dFactory.GetAddressOf())),
         "D2D factory creation failed");
      ComPtr<ID2D1Multithread> d2dLock;
      require(SUCCEEDED(d2dFactory.As(&d2dLock)), "D2D factory lock missing");

      // Reproduce the old wait cycle without waiting for the application's
      // 15-second timeout: the caller owns D2D, the worker tries to own it too.
      d2dLock->Enter();
      std::promise<void> enteredWorker;
      auto oldWorker = std::async(std::launch::async, [&]
      {
         enteredWorker.set_value();
         d2dLock->Enter();
         d2dLock->Leave();
      });
      enteredWorker.get_future().wait();
      auto blocked = oldWorker.wait_for(std::chrono::milliseconds(50));
      d2dLock->Leave();
      oldWorker.get();
      require(blocked == std::future_status::timeout, "Expected old device-lock wait cycle");

      // The corrected CPU path must complete on a worker while the caller
      // still owns D2D, as skybox scene loading does before GPU upload.
      d2dLock->Enter();
      auto cpuWorker = std::async(std::launch::async, [&]
      {
         require(SUCCEEDED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)), "Worker COM initialization failed");
         result decoded;
         {
            ComPtr<IWICImagingFactory> workerFactory;
            require(SUCCEEDED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
               IID_PPV_ARGS(&workerFactory))), "Worker WIC initialization failed");
            decoded = decode(workerFactory.Get(), png);
         }
         CoUninitialize();
         return decoded;
      });
      auto ready = cpuWorker.wait_for(std::chrono::seconds(10));
      d2dLock->Leave();
      require(ready == std::future_status::ready, "CPU decode waited on caller's D2D lock");
      auto decoded = cpuWorker.get();
      require(SUCCEEDED(decoded.hr) && decoded.width == 4096 && decoded.height == 4096,
         "CPU worker must decode the full PNG while caller owns D2D");
      for (auto bytes : {std::string(), std::string("not an image"), png.substr(0, 64)})
      {
         auto bad = decode(factory.Get(), bytes);
         require(FAILED(bad.hr), "Malformed input must fail at a known stage");
         require(wic.find(std::string("\"") + bad.stage + "\"") != std::string::npos,
            "Native failure stage must be covered by production diagnostics");
         std::printf("Expected failure: stage=%s hr=0x%08X\n", bad.stage, static_cast<unsigned>(bad.hr));
      }
   }
   CoUninitialize();
   std::puts("PASS: old lock cycle reproduced; CPU PNG decode completed while caller held D2D; malformed inputs; diagnostics/lock-free worker source wiring");
}
