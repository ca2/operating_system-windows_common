// Standalone D3D11 WARP test: deferred copies must execute before CPU readback.
#include <d3d11.h>
#include <wrl/client.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <memory>
#include "../../../../source/app/bred/gpu/renderer_layer_cache.h"

using Microsoft::WRL::ComPtr;

static void require(bool condition, const char * message)
{
   if (!condition) { std::fprintf(stderr, "%s\n", message); std::exit(1); }
}

static void check(HRESULT result)
{
   if (FAILED(result))
   {
      std::fprintf(stderr, "D3D11 failed: 0x%08lx\n", (unsigned long)result);
      std::exit(1);
   }
}

int main()
{
   ComPtr<ID3D11Device> device;
   ComPtr<ID3D11DeviceContext> immediate, deferred;
   check(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0, nullptr, 0,
      D3D11_SDK_VERSION, &device, nullptr, &immediate));
   check(device->CreateDeferredContext(0, &deferred));

   D3D11_TEXTURE2D_DESC description{};
   description.Width = 8;
   description.Height = 6;
   description.MipLevels = description.ArraySize = description.SampleDesc.Count = 1;
   description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
   description.Usage = D3D11_USAGE_DEFAULT;
   ComPtr<ID3D11Texture2D> source, staging;
   check(device->CreateTexture2D(&description, nullptr, &source));
   description.Usage = D3D11_USAGE_STAGING;
   description.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
   check(device->CreateTexture2D(&description, nullptr, &staging));

   std::vector<std::uint32_t> expected(8 * 6, 0xff001122u);
   immediate->UpdateSubresource(source.Get(), 0, nullptr, expected.data(), 8 * 4, 0);
   immediate->CopyResource(staging.Get(), source.Get());

   auto verifyRegion = [&](const std::vector<std::uint32_t> & pixels)
   {
      D3D11_MAPPED_SUBRESOURCE mapped{};
      check(immediate->Map(staging.Get(), 0, D3D11_MAP_READ, 0, &mapped));
      // Nonzero source origin, cropped destination, and native padded row pitch.
      const auto begin = static_cast<const unsigned char *>(mapped.pData)
         + 2 * mapped.RowPitch + 3 * sizeof(std::uint32_t);
      for (unsigned y = 0; y < 3; ++y)
      {
         const auto row = reinterpret_cast<const std::uint32_t *>(begin + y * mapped.RowPitch);
         for (unsigned x = 0; x < 4; ++x)
            require(row[x] == pixels[(y + 2) * 8 + x + 3], "Stale or misplaced readback pixels");
      }
      immediate->Unmap(staging.Get(), 0);
   };
   verifyRegion(expected);

   for (unsigned scene = 1; scene <= 3; ++scene)
   {
      const auto previous = expected;
      for (unsigned i = 0; i < expected.size(); ++i)
         expected[i] = 0xff000000u | (scene << 16) | (i << 8) | scene;

      deferred->UpdateSubresource(source.Get(), 0, nullptr, expected.data(), 8 * 4, 0);
      deferred->CopyResource(staging.Get(), source.Get());

      // Reproduce the original bug: Map cannot execute pending deferred work.
      verifyRegion(previous);

      ComPtr<ID3D11CommandList> commands;
      check(deferred->FinishCommandList(FALSE, &commands));
      immediate->ExecuteCommandList(commands.Get(), FALSE);

      // The fixed order needs no extra Flush: blocking Map waits for the copy.
      verifyRegion(expected);
   }

   std::puts("D3D11 WARP: reproduced stale deferred readback; submit-before-map and cropped regions passed for three scenes.");

   // A layer used in the same global slot by another tab must not retain the
   // preceding tab's deferred context. Exercise the production cache policy.
   struct renderer
   {
      ComPtr<ID3D11DeviceContext> context;
   } renderers[3];
   struct layer
   {
      ComPtr<ID3D11DeviceContext> commandContext;
   };
   for (auto & renderer : renderers)
      check(device->CreateDeferredContext(0, &renderer.context));

   auto execute = [&](ID3D11DeviceContext * context)
   {
      ComPtr<ID3D11CommandList> commands;
      check(context->FinishCommandList(FALSE, &commands));
      immediate->ExecuteCommandList(commands.Get(), FALSE);
   };
   auto record = [&](renderer & owner, unsigned serial)
   {
      for (unsigned i = 0; i < expected.size(); ++i)
         expected[i] = 0xff000000u | (serial << 16) | (i << 8) | serial;
      owner.context->UpdateSubresource(source.Get(), 0, nullptr, expected.data(), 8 * 4, 0);
      owner.context->CopyResource(staging.Get(), source.Get());
   };

   // Reproduce slot reuse: tab B records, but tab A's cached command buffer submits.
   const auto previous = expected;
   record(renderers[1], 4);
   execute(renderers[0].context.Get());
   verifyRegion(previous);
   execute(renderers[1].context.Get());
   verifyRegion(expected);

   gpu::renderer_layer_cache<renderer *, std::shared_ptr<layer>> cache;
   for (unsigned selection = 0; selection < 60; ++selection)
   {
      auto & owner = renderers[(selection * 2) % 3];
      const auto selectedLayer = cache.acquire(&owner, 0, [&]()
      {
         return std::make_shared<layer>(layer{owner.context});
      });
      record(owner, selection + 5);
      execute(selectedLayer->commandContext.Get());
      verifyRegion(expected);
   }
   require(cache.size() == 3, "Renderer-owned layer cache grew during repeated tab switches");
   std::puts("D3D11 WARP: reproduced wrong-owner layer submission; renderer-owned cache passed 60 scene switches.");
}
