// Native WARP smoke test plus production integration checks. Run from main.
#include <d3d11_4.h>
#include <d3d11sdklayers.h>
#include <wrl/client.h>
#include <thread>
#include <vector>
#include <fstream>
#include <iterator>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using Microsoft::WRL::ComPtr;
static void require(bool condition, const char * message)
{
   if (!condition) { std::fprintf(stderr, "%s\n", message); std::exit(1); }
}
static void check(HRESULT result) { require(SUCCEEDED(result), "D3D11 call failed"); }
static std::string read(const char * path)
{
   std::ifstream source(path);
   require(source.good(), path);
   return {std::istreambuf_iterator<char>(source), {}};
}

int main()
{
   ComPtr<ID3D11Device> device;
   ComPtr<ID3D11DeviceContext> immediate, deferred;
   check(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, D3D11_CREATE_DEVICE_DEBUG,
      nullptr, 0, D3D11_SDK_VERSION, &device, nullptr, &immediate));
   check(device->CreateDeferredContext(0, &deferred));
   ComPtr<ID3D11Multithread> multithread;
   check(immediate.As(&multithread));
   multithread->SetMultithreadProtected(TRUE);
   require(multithread->GetMultithreadProtected() == TRUE, "Native protection must be enabled");

   D3D11_RASTERIZER_DESC desc{};
   desc.FillMode = D3D11_FILL_SOLID;
   desc.CullMode = D3D11_CULL_NONE;
   desc.DepthClipEnable = TRUE;
   desc.ScissorEnable = TRUE;
   ComPtr<ID3D11RasterizerState> rasterizer;
   check(device->CreateRasterizerState(&desc, &rasterizer));

   for (auto context : {immediate.Get(), deferred.Get()})
   {
      for (UINT count : {0u, 3u, (UINT)D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE})
      {
         D3D11_VIEWPORT expected[16]{};
         D3D11_RECT scissors[16]{};
         for (UINT i = 0; i < count; ++i)
         {
            expected[i] = {(float)i, (float)i + 1, 30.f + i, 40.f + i, 0.2f, 0.8f};
            scissors[i] = {(LONG)i, (LONG)i + 1, (LONG)i + 20, (LONG)i + 21};
         }
         context->RSSetViewports(count, expected);
         context->RSSetScissorRects(count, scissors);
         context->RSSetState(count ? rasterizer.Get() : nullptr);
         D3D11_VIEWPORT savedViewports[16]{};
         D3D11_RECT savedScissors[16]{};
         UINT views = 16, rects = 16;
         ComPtr<ID3D11RasterizerState> savedState;
         context->RSGetViewports(&views, savedViewports);
         context->RSGetScissorRects(&rects, savedScissors);
         context->RSGetState(&savedState);
         const D3D11_VIEWPORT temporary{0, 0, 1, 1, 0, 1};
         context->RSSetViewports(1, &temporary);
         context->RSSetScissorRects(0, nullptr);
         context->RSSetState(nullptr);
         context->RSSetViewports(views, savedViewports);
         context->RSSetScissorRects(rects, savedScissors);
         context->RSSetState(savedState.Get());
         D3D11_VIEWPORT actual[16]{};
         D3D11_RECT actualScissors[16]{};
         UINT actualViews = 16, actualRects = 16;
         ComPtr<ID3D11RasterizerState> actualState;
         context->RSGetViewports(&actualViews, actual);
         context->RSGetScissorRects(&actualRects, actualScissors);
         context->RSGetState(&actualState);
         require(actualViews == count && actualRects == count, "Rasterizer slot counts were not restored");
         require(!std::memcmp(expected, actual, count * sizeof(D3D11_VIEWPORT)), "Viewport/depth mismatch");
         require(!std::memcmp(scissors, actualScissors, count * sizeof(D3D11_RECT)), "Scissor mismatch");
         require(actualState.Get() == savedState.Get(), "Rasterizer/scissor enable state mismatch");
      }
   }

   ComPtr<ID3D11CommandList> commands;
   check(deferred->FinishCommandList(FALSE, &commands));
   D3D11_QUERY_DESC queryDesc{D3D11_QUERY_EVENT, 0};
   ComPtr<ID3D11Query> query;
   check(device->CreateQuery(&queryDesc, &query));
   ComPtr<ID3D11InfoQueue> info;
   check(device.As(&info));
   info->ClearStoredMessages();
   std::thread submit([&]()
   {
      for (int i = 0; i < 1000; ++i)
      {
         multithread->Enter();
         immediate->ExecuteCommandList(commands.Get(), FALSE);
         multithread->Leave();
      }
   });
   std::thread poll([&]()
   {
      for (int i = 0; i < 1000; ++i)
      {
         immediate->End(query.Get());
         HRESULT result;
         while ((result = immediate->GetData(query.Get(), nullptr, 0, 0)) == S_FALSE)
            std::this_thread::yield();
         check(result);
      }
   });
   submit.join();
   poll.join();
   for (UINT64 i = 0; i < info->GetNumStoredMessages(); ++i)
   {
      SIZE_T length = 0;
      check(info->GetMessage(i, nullptr, &length));
      std::vector<unsigned char> data(length);
      auto message = reinterpret_cast<D3D11_MESSAGE *>(data.data());
      check(info->GetMessage(i, message, &length));
      require(message->Severity != D3D11_MESSAGE_SEVERITY_CORRUPTION &&
         message->Severity != D3D11_MESSAGE_SEVERITY_ERROR, message->pDescription);
   }

   const auto factory = read("operating_system/operating_system-windows_common/gpu_directx11/_factory.cpp");
   require(factory.find("::gpu_directx11::viewport_scissor_restore, ::gpu::viewport_scissor_restore") != std::string::npos,
      "Restore factory registration missing");
   const auto implementation = read("operating_system/operating_system-windows_common/gpu_directx11/viewport_scissor_restore.cpp");
   for (auto call : {"RSGetViewports(", "RSGetScissorRects(", "RSGetState(", "RSSetViewports(", "RSSetScissorRects(", "RSSetState("})
      require(implementation.find(call) != std::string::npos, call);
   require(implementation.find("e_state_recording") != std::string::npos, "Restore must not record after submission");
   const auto initialization = read("operating_system/operating_system-windows_common/gpu_directx11/device.cpp");
   require(initialization.find("multithread->SetMultithreadProtected(TRUE);") != std::string::npos,
      "Immediate context protection missing");
   std::puts("WARP passed: 0/3/16 rasterizer slots on immediate/deferred contexts; 1000 concurrent submissions/query polls without corruption/errors; factory integration checks.");
}
