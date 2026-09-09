// Run from main. WARP/debug-layer stress for D2D batches versus D3D submissions.
#include <d2d1_1.h>
#include <d3d11_4.h>
#include <d3d11sdklayers.h>
#include <wrl/client.h>
#include <atomic>
#include <thread>
#include <vector>
#include <fstream>
#include <iterator>
#include <string>
#include <cstdio>
#include <cstdlib>
using Microsoft::WRL::ComPtr;
static void require(bool value, const char * message)
{
   if (!value) { std::fprintf(stderr, "%s\n", message); std::exit(1); }
}
static void check(HRESULT hr) { require(SUCCEEDED(hr), "Native graphics call failed"); }
static std::string read(const char * path)
{
   std::ifstream file(path);
   require(file.good(), path);
   return {std::istreambuf_iterator<char>(file), {}};
}
struct interop_scope
{
   ID2D1Multithread * d2d;
   ID3D11Multithread * d3d;
   interop_scope(ID2D1Multithread * a, ID3D11Multithread * b) : d2d(a), d3d(b)
   {
      d2d->Enter(); d3d->Enter();
   }
   ~interop_scope() { d3d->Leave(); d2d->Leave(); }
};
int main()
{
   auto source = read("operating_system/operating_system-windows_common/gpu_directx11/context.cpp");
   auto start = source.find("void context::_context_lock()");
   auto end = source.find("void context::_context_unlock()", start);
   require(start != std::string::npos && end != std::string::npos, "Missing context lock methods");
   auto lock = source.substr(start, end - start);
   require(lock.find("make_unique<draw2d_interop_lock>") < lock.find("synchronization()->lock()")
      && lock.find("synchronization()->lock()") < lock.find("m_pmultithread->Enter()"),
      "Lock order must be D2D factory, ca2 context, native D3D");
   auto unlock = source.substr(end, source.find("void context::draw()", end) - end);
   require(unlock.find("m_pmultithread->Leave()") < unlock.find("synchronization()->unlock()")
      && unlock.find("synchronization()->unlock()") < unlock.find("t_draw2dInteropLocks.pop_back()"),
      "Unlock order must reverse lock order");
   require(source.find("pdraw2d && pdraw2d->lock_device()") != std::string::npos
      && source.find("m_pdraw2d->unlock_device()") != std::string::npos,
      "Use the active backend's existing native device lock");

   ComPtr<ID3D11Device> d3d;
   ComPtr<ID3D11DeviceContext> immediate, deferred;
   check(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP, nullptr,
      D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
      nullptr, 0, D3D11_SDK_VERSION, &d3d, nullptr, &immediate));
   ComPtr<ID3D11Multithread> d3dLock;
   check(immediate.As(&d3dLock));
   d3dLock->SetMultithreadProtected(TRUE);
   ComPtr<IDXGIDevice> dxgi;
   check(d3d.As(&dxgi));
   ComPtr<ID2D1Factory1> factory;
   D2D1_FACTORY_OPTIONS options{D2D1_DEBUG_LEVEL_INFORMATION};
   check(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,
      __uuidof(ID2D1Factory1), &options, reinterpret_cast<void **>(factory.GetAddressOf())));
   ComPtr<ID2D1Multithread> d2dLock;
   check(factory.As(&d2dLock));
   require(d2dLock->GetMultithreadProtected(), "D2D factory must be multithreaded");
   ComPtr<ID2D1Device> device;
   check(factory->CreateDevice(dxgi.Get(), &device));
   ComPtr<ID2D1DeviceContext> icons;
   check(device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &icons));
   auto props = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET,
      D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));
   ComPtr<ID2D1Bitmap1> bitmap;
   check(icons->CreateBitmap(D2D1::SizeU(64, 64), nullptr, 0, &props, &bitmap));
   ComPtr<ID2D1SolidColorBrush> brush;
   check(icons->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brush));
   check(d3d->CreateDeferredContext(0, &deferred));
   D3D11_VIEWPORT viewport{0, 0, 16, 16, 0, 1};
   deferred->RSSetViewports(1, &viewport);
   ComPtr<ID3D11CommandList> commands;
   check(deferred->FinishCommandList(FALSE, &commands));
   ComPtr<ID3D11InfoQueue> info;
   check(d3d.As(&info));
   info->ClearStoredMessages();
   std::atomic<bool> go{false};
   std::thread render([&]
   {
      while (!go.load()) std::this_thread::yield();
      for (int frame = 0; frame < 1000; ++frame)
      {
         // Like the shell worker: only automatic D2D locking on this side.
         icons->SetTarget(bitmap.Get()); icons->BeginDraw();
         icons->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));
         for (int i = 0; i < 64; ++i)
            icons->FillRectangle(D2D1::RectF(0, (float)i, 64, (float)i + 1), brush.Get());
         check(icons->EndDraw()); icons->SetTarget(nullptr);
      }
   });
   std::thread submit([&]
   {
      go.store(true);
      for (int frame = 0; frame < 1000; ++frame)
      {
         interop_scope scope(d2dLock.Get(), d3dLock.Get());
         immediate->ExecuteCommandList(commands.Get(), FALSE);
         immediate->ClearState();
         // Recursion matches D2D callbacks/context nesting under a GPU lock.
         { interop_scope nested(d2dLock.Get(), d3dLock.Get()); }
      }
   });
   render.join(); submit.join();

   // Check that an exception releases both native locks for another thread.
   try { interop_scope scope(d2dLock.Get(), d3dLock.Get()); throw 1; }
   catch (int) {}
   std::thread afterException([&] { interop_scope scope(d2dLock.Get(), d3dLock.Get()); });
   afterException.join();
   props.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
   ComPtr<ID2D1Bitmap1> readback;
   check(icons->CreateBitmap(D2D1::SizeU(64, 64), nullptr, 0, &props, &readback));
   check(readback->CopyFromBitmap(nullptr, bitmap.Get(), nullptr));
   D2D1_MAPPED_RECT mapped{};
   check(readback->Map(D2D1_MAP_OPTIONS_READ, &mapped));
   for (unsigned y = 0; y < 64; ++y)
      for (unsigned x = 0; x < 64; ++x)
      {
         auto p = mapped.bits + y * mapped.pitch + x * 4;
         require(p[0] == 0 && p[1] == 0 && p[2] == 255 && p[3] == 255,
            "Offscreen icon pixels corrupted by concurrent submissions");
      }
   check(readback->Unmap());
   for (UINT64 i = 0; i < info->GetNumStoredMessages(); ++i)
   {
      SIZE_T length = 0;
      check(info->GetMessage(i, nullptr, &length));
      std::vector<unsigned char> bytes(length);
      auto message = reinterpret_cast<D3D11_MESSAGE *>(bytes.data());
      check(info->GetMessage(i, message, &length));
      require(message->Severity != D3D11_MESSAGE_SEVERITY_ERROR
         && message->Severity != D3D11_MESSAGE_SEVERITY_CORRUPTION, message->pDescription);
   }
   std::puts("PASS: 1000 D2D icon batches vs 1000 D3D submissions; debug layers; pixel readback; recursive/exception lock release; source lock ordering");
}
