// Run from main. Native WARP pixel checks plus production call-path checks.
#include <d3d11.h>
#include <d3d11sdklayers.h>
#include <wrl/client.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
using Microsoft::WRL::ComPtr;
static void require(bool condition, const char * message)
{
   if (!condition) { std::fprintf(stderr, "%s\n", message); std::exit(1); }
}
static void check(HRESULT hr) { require(SUCCEEDED(hr), "D3D11 call failed"); }
static std::string read(const char * path)
{
   std::ifstream file(path);
   require(file.good(), path);
   return {std::istreambuf_iterator<char>(file), {}};
}
int main()
{
   auto source = read("operating_system/operating_system-windows_common/gpu_directx11/texture.cpp");
   auto create = source.substr(source.find("void texture::_create_texture("),
      source.find("void texture::_set_data(") - source.find("void texture::_create_texture("));
   require(create.find("texturedata.is_raw_scoped_pixmap() || texturedata.is_gpu_texture()") != std::string::npos
      && create.find("_set_data(texturedata);") != std::string::npos,
      "Texture creation must dispatch single pixmaps and preserved GPU textures");
   require(source.find("write_pixels(false, pixmap.data(), pixmap.size(), pixmap.m_iScan, 4, {});") != std::string::npos,
      "Single pixmaps must upload with their real row pitch");
   auto upload = source.substr(source.find("void texture::write_pixels("));
   require(upload.find("pmain = pcontext->m_pgpudevice->main_gpu_context()") != std::string::npos
      && upload.find("contextlock(pmain)") != std::string::npos
      && upload.find("pd3d11devicecontext = pmain->m_pd3d11devicecontext") != std::string::npos,
      "Standalone uploads must use the protected immediate context");
   require(source.find("pmain->m_pd3d11devicecontext->CopySubresourceRegion(") != std::string::npos,
      "Atlas growth must copy the existing pixels");

   ComPtr<ID3D11Device> device;
   ComPtr<ID3D11DeviceContext> immediate, deferred;
   check(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, D3D11_CREATE_DEVICE_DEBUG,
      nullptr, 0, D3D11_SDK_VERSION, &device, nullptr, &immediate));
   check(device->CreateDeferredContext(0, &deferred));
   ComPtr<ID3D11InfoQueue> messages;
   check(device.As(&messages));
   auto texture = [&](UINT width, UINT height)
   {
      D3D11_TEXTURE2D_DESC desc{};
      desc.Width = width; desc.Height = height; desc.ArraySize = desc.MipLevels = 1;
      desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; desc.SampleDesc.Count = 1;
      desc.Usage = D3D11_USAGE_DEFAULT;
      desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
      ComPtr<ID3D11Texture2D> result;
      check(device->CreateTexture2D(&desc, nullptr, &result));
      return result;
   };
   auto verify = [&](ID3D11Texture2D * image, UINT width, UINT height,
                     const std::vector<uint32_t> & pixels)
   {
      D3D11_TEXTURE2D_DESC desc{};
      image->GetDesc(&desc);
      desc.Usage = D3D11_USAGE_STAGING; desc.BindFlags = 0;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
      ComPtr<ID3D11Texture2D> staging;
      check(device->CreateTexture2D(&desc, nullptr, &staging));
      immediate->CopyResource(staging.Get(), image);
      D3D11_MAPPED_SUBRESOURCE mapped{};
      check(immediate->Map(staging.Get(), 0, D3D11_MAP_READ, 0, &mapped));
      for (UINT y = 0; y < height; ++y)
         for (UINT x = 0; x < width; ++x)
            require(reinterpret_cast<uint32_t *>(static_cast<char *>(mapped.pData) + y * mapped.RowPitch)[x]
               == pixels[y * width + x], "BGRA/alpha/orientation/pitch or atlas preservation mismatch");
      immediate->Unmap(staging.Get(), 0);
   };

   // Padded icon rows, varying alpha and distinct top/bottom colors.
   std::vector<uint32_t> expected{0xffff0000, 0xff00ff00, 0xff0000ff,
      0x80402010, 0x00000000, 0xffffffff};
   uint32_t padded[]{expected[0], expected[1], expected[2], 0xdeadbeef,
      expected[3], expected[4], expected[5], 0xdeadbeef};
   auto icon = texture(3, 2);
   D3D11_BOX iconBox{0, 0, 0, 3, 2, 1};
   immediate->UpdateSubresource(icon.Get(), 0, &iconBox, padded, 16, 0);
   verify(icon.Get(), 3, 2, expected);

   // Reproduce why recording an upload without submitting cannot publish it.
   uint32_t changed = 0xffabcdef;
   D3D11_BOX pixelBox{0, 0, 0, 1, 1, 1};
   deferred->UpdateSubresource(icon.Get(), 0, &pixelBox, &changed, 4, 0);
   verify(icon.Get(), 3, 2, expected); // The deferred upload is still invisible.
   immediate->UpdateSubresource(icon.Get(), 0, &pixelBox, &changed, 4, 0);
   expected[0] = changed;
   verify(icon.Get(), 3, 2, expected); // No unrelated draw/lease needed.

   auto atlas = texture(30, 4);
   immediate->CopySubresourceRegion(atlas.Get(), 0, 0, 0, 0, icon.Get(), 0, &iconBox);
   verify(atlas.Get(), 3, 2, expected);
   auto largerAtlas = texture(60, 4);
   D3D11_BOX atlasBox{0, 0, 0, 30, 4, 1};
   immediate->CopySubresourceRegion(largerAtlas.Get(), 0, 0, 0, 0, atlas.Get(), 0, &atlasBox);
   verify(largerAtlas.Get(), 3, 2, expected);

   for (UINT64 i = 0; i < messages->GetNumStoredMessages(); ++i)
   {
      SIZE_T size = 0;
      check(messages->GetMessage(i, nullptr, &size));
      std::vector<char> storage(size);
      auto message = reinterpret_cast<D3D11_MESSAGE *>(storage.data());
      check(messages->GetMessage(i, message, &size));
      require(message->Severity > D3D11_MESSAGE_SEVERITY_ERROR, message->pDescription);
   }
   std::puts("PASS: pixmap upload, padded BGRA/alpha, immediate visibility, repeated atlas growth, production wiring");
}
