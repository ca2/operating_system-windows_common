// Native WARP regression: cached-image submission must not reset an active layer.
// Run from main; link d3d11.lib d3dcompiler.lib.
#include <windows.h>
#include <d3d11.h>
#include <d3d11sdklayers.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

using Microsoft::WRL::ComPtr;
static void require(bool value, const char * message)
{
   if (!value) { std::fprintf(stderr, "%s\n", message); std::exit(1); }
}
static void check(HRESULT hr)
{
   if (FAILED(hr)) { std::fprintf(stderr, "HRESULT 0x%08lx\n", hr); std::exit(1); }
}
int main()
{
   std::ifstream file("operating_system/operating_system-windows_common/gpu_directx11/command_buffer.cpp");
   require(file.good(), "Run from main workspace");
   const std::string source{std::istreambuf_iterator<char>(file), {}};
   const auto finish = source.find("->FinishCommandList(");
   const auto execute = source.find("->ExecuteCommandList(");
   require(finish != std::string::npos && execute != std::string::npos, "Submission route missing");
   require(source.substr(finish, source.find(';', finish) - finish).find("TRUE") != std::string::npos,
      "Production FinishCommandList must preserve deferred state");
   require(source.substr(execute, source.find(';', execute) - execute).find("TRUE") != std::string::npos,
      "Production ExecuteCommandList must preserve immediate state");

   ComPtr<ID3D11Device> device;
   ComPtr<ID3D11DeviceContext> immediate, deferred;
   check(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, D3D11_CREATE_DEVICE_DEBUG,
      nullptr, 0, D3D11_SDK_VERSION, &device, nullptr, &immediate));
   check(device->CreateDeferredContext(0, &deferred));
   const char * shader =
      "float4 vs(uint id:SV_VertexID):SV_Position {"
      "float2 p[3]={float2(-1,-1),float2(-1,3),float2(3,-1)};return float4(p[id],0,1);}"
      "float4 ps():SV_Target{return float4(0,1,0,1);}";
   ComPtr<ID3DBlob> vsCode, psCode;
   check(D3DCompile(shader, std::strlen(shader), nullptr, nullptr, nullptr, "vs", "vs_5_0", 0, 0, &vsCode, nullptr));
   check(D3DCompile(shader, std::strlen(shader), nullptr, nullptr, nullptr, "ps", "ps_5_0", 0, 0, &psCode, nullptr));
   ComPtr<ID3D11VertexShader> vs;
   ComPtr<ID3D11PixelShader> ps;
   check(device->CreateVertexShader(vsCode->GetBufferPointer(), vsCode->GetBufferSize(), nullptr, &vs));
   check(device->CreatePixelShader(psCode->GetBufferPointer(), psCode->GetBufferSize(), nullptr, &ps));
   D3D11_TEXTURE2D_DESC desc = {};
   desc.Width = desc.Height = 16;
   desc.MipLevels = desc.ArraySize = desc.SampleDesc.Count = 1;
   desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
   desc.BindFlags = D3D11_BIND_RENDER_TARGET;
   ComPtr<ID3D11Texture2D> target, staging;
   check(device->CreateTexture2D(&desc, nullptr, &target));
   ComPtr<ID3D11RenderTargetView> rtv;
   check(device->CreateRenderTargetView(target.Get(), nullptr, &rtv));
   desc.BindFlags = 0;
   desc.Usage = D3D11_USAGE_STAGING;
   desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
   check(device->CreateTexture2D(&desc, nullptr, &staging));
   D3D11_RASTERIZER_DESC rasterDesc = {};
   rasterDesc.FillMode = D3D11_FILL_SOLID;
   rasterDesc.CullMode = D3D11_CULL_NONE;
   rasterDesc.ScissorEnable = TRUE;
   rasterDesc.DepthClipEnable = TRUE;
   ComPtr<ID3D11RasterizerState> raster;
   check(device->CreateRasterizerState(&rasterDesc, &raster));
   const D3D11_VIEWPORT viewport{0, 0, 16, 16, 0, 1};
   const D3D11_RECT scissor{0, 0, 16, 16};
   auto bind = [&](ID3D11DeviceContext * context)
   {
      context->VSSetShader(vs.Get(), nullptr, 0);
      context->PSSetShader(ps.Get(), nullptr, 0);
      context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      context->RSSetState(raster.Get());
      context->RSSetViewports(1, &viewport);
      context->RSSetScissorRects(1, &scissor);
      ID3D11RenderTargetView * views[] = {rtv.Get()};
      context->OMSetRenderTargets(1, views, nullptr);
   };
   auto intact = [&](ID3D11DeviceContext * context)
   {
      ComPtr<ID3D11VertexShader> actualVs;
      context->VSGetShader(&actualVs, nullptr, nullptr);
      D3D11_PRIMITIVE_TOPOLOGY topology;
      context->IAGetPrimitiveTopology(&topology);
      UINT viewportCount = 1, scissorCount = 1;
      D3D11_VIEWPORT actualViewport = {};
      D3D11_RECT actualScissor = {};
      context->RSGetViewports(&viewportCount, &actualViewport);
      context->RSGetScissorRects(&scissorCount, &actualScissor);
      ComPtr<ID3D11RenderTargetView> actualRtv;
      context->OMGetRenderTargets(1, &actualRtv, nullptr);
      return actualVs.Get() == vs.Get() && topology == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST &&
         viewportCount == 1 && actualViewport.Width == 16 && actualViewport.Height == 16 &&
         scissorCount == 1 && actualScissor.right == 16 && actualScissor.bottom == 16 && actualRtv.Get() == rtv.Get();
   };
   auto verify_pixels = [&]
   {
      immediate->CopyResource(staging.Get(), target.Get());
      D3D11_MAPPED_SUBRESOURCE mapped = {};
      check(immediate->Map(staging.Get(), 0, D3D11_MAP_READ, 0, &mapped));
      for (UINT y = 0; y < 16; ++y)
         for (UINT x = 0; x < 16; ++x)
         {
            const auto p = static_cast<const unsigned char *>(mapped.pData) + y * mapped.RowPitch + x * 4;
            require(p[0] == 0 && p[1] == 255 && p[2] == 0 && p[3] == 255, "Resumed draw did not reach its target");
         }
      immediate->Unmap(staging.Get(), 0);
   };
   const float black[] = {0, 0, 0, 1};
   ComPtr<ID3D11CommandList> commands;
   bind(deferred.Get());
   check(deferred->FinishCommandList(FALSE, &commands));
   require(!intact(deferred.Get()), "Baseline FALSE should reproduce deferred state loss");
   bind(immediate.Get());
   immediate->ExecuteCommandList(commands.Get(), FALSE);
   require(!intact(immediate.Get()), "Baseline FALSE should reproduce immediate state loss");

   bind(immediate.Get());
   immediate->ExecuteCommandList(commands.Get(), TRUE);
   require(intact(immediate.Get()), "Immediate bindings lost after nested submission");
   immediate->ClearRenderTargetView(rtv.Get(), black);
   immediate->Draw(3, 0); // No rebinding after the nested command list.
   verify_pixels();

   bind(deferred.Get());
   commands.Reset();
   check(deferred->FinishCommandList(TRUE, &commands));
   require(intact(deferred.Get()), "Deferred bindings lost at submission boundary");
   immediate->ExecuteCommandList(commands.Get(), TRUE);
   deferred->ClearRenderTargetView(rtv.Get(), black);
   deferred->Draw(3, 0); // Continuation must record restored state into the next list.
   commands.Reset();
   check(deferred->FinishCommandList(TRUE, &commands));
   immediate->ExecuteCommandList(commands.Get(), TRUE);
   verify_pixels();

   ComPtr<ID3D11InfoQueue> info;
   check(device.As(&info));
   for (UINT64 i = 0; i < info->GetNumStoredMessagesAllowedByRetrievalFilter(); ++i)
   {
      SIZE_T size = 0;
      check(info->GetMessage(i, nullptr, &size));
      std::vector<unsigned char> bytes(size);
      auto message = reinterpret_cast<D3D11_MESSAGE *>(bytes.data());
      check(info->GetMessage(i, message, &size));
      require(message->Severity != D3D11_MESSAGE_SEVERITY_ERROR && message->Severity != D3D11_MESSAGE_SEVERITY_CORRUPTION,
         message->pDescription);
   }
   std::puts("PASS: reproduced FALSE state loss; TRUE preserves both contexts and resumed draw pixels; no D3D11 errors.");
}
