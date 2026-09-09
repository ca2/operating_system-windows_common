// Run from main: native Direct2D/WARP ownership test and source integration checks.
#include <d2d1_1.h>
#include <d3d11.h>
#include <dwrite.h>
#include <wrl/client.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
using Microsoft::WRL::ComPtr;
static void require(bool value, const char * message)
{
   if (!value) { std::fprintf(stderr, "%s\n", message); std::exit(1); }
}
static void check(HRESULT hr) { require(SUCCEEDED(hr), "Native graphics call failed"); }
int main()
{
   std::ifstream file("operating_system/operating_system-windows_common/draw2d_direct2d_for_directx11/graphics.cpp");
   require(file.good(), "Missing production source");
   std::string source{std::istreambuf_iterator<char>(file), {}};
   auto section = [&](const char * first, const char * next)
   {
      auto start = source.find(first);
      auto end = source.find(next, start);
      require(start != std::string::npos && end != std::string::npos, "Missing production method");
      return source.substr(start, end - start);
   };
   auto release = section("void graphics::on_release_memory_graphics()", "bool graphics::_draw_blend(");
   require(release.find("m_bBeginDraw = false") == std::string::npos,
      "Release must not disable end_draw's native EndDraw branch");
   require(release.find("end_draw();") < release.find("::gpu::graphics::on_release_memory_graphics();"),
      "Native EndDraw must precede base image release");
   auto begin = section("void graphics::begin_draw(bool", "void graphics::end_draw()");
   require(begin.find("if (pimageTarget)") < begin.find("else if (::gpu::current_layer())"),
      "Explicit offscreen image must take priority over the ambient layer");
   auto acquire = section("void graphics::on_acquire_memory_graphics(", "//void graphics::text_out(");
   require(acquire.find("rectangleFrame, pimage);") != std::string::npos,
      "Acquisition must pass the actual image to begin_draw");
   require(acquire.find("pimage->update_bitmap_as_render_target(")
      < acquire.find("::draw2d::graphics::on_acquire_memory_graphics("),
      "Native bitmap realization must precede base acquisition");
   require(acquire.find("pimage->update_as_render_target(") == std::string::npos,
      "Descriptor-only initialization does not create a target bitmap");
   std::ifstream bitmapFile("operating_system/operating_system-windows_common/draw2d_direct2d_for_directx11/bitmap.cpp");
   require(bitmapFile.good(), "Missing bitmap source");
   std::string bitmapSource{std::istreambuf_iterator<char>(bitmapFile), {}};
   require(bitmapSource.find("pdevicecontext = pdraw2ddirect2dfordirectx11graphics->m_pd2d1devicecontext") != std::string::npos
      && bitmapSource.find("hr = pdevicecontext->CreateBitmap(") != std::string::npos,
      "Native bitmap must be created in the drawing context's resource domain");

   auto drawImage = section("void graphics::_draw_raw(const ::image::image_drawing &", "void graphics::_draw_raw(const ::f64_rectangle &");
   require(drawImage.find("::image::image_drawer::_draw_raw(imagedrawing);") != std::string::npos,
      "Hybrid image drawing must dispatch to native copy/stretch instead of GPU texture drawing");
   auto stretch = section("void graphics::_stretch_raw(const ::f64_rectangle &", "//void graphics::_stretch_raw(");
   require(stretch.find("pimage->get_bitmap_as_source(this);") < stretch.find("::draw2d_direct2d::graphics::_stretch_raw("),
      "Stretch must realize a CPU source using the destination context");
   std::ifstream imageFile("operating_system/operating_system-windows_common/draw2d_direct2d_for_directx11/image.cpp");
   require(imageFile.good(), "Missing image source");
   std::string imageSource{std::istreambuf_iterator<char>(imageFile), {}};
   require(imageSource.find("return ::image::image::get_bitmap_as_source(pdraw2dgraphics);") != std::string::npos,
      "Native source lookup must realize and upload CPU images");
   require(imageSource.find("return ::transfer(::image::image::_map(emap, rectangle));") != std::string::npos,
      "CPU map must use native bitmap readback and image dirty tracking");
   require(imageSource.find("::image::image::_unmap(pimagepixmaplease);") != std::string::npos
      && imageSource.find("::gpu::image::_unmap(pimagepixmaplease);") == std::string::npos,
      "CPU unmap must release the lease without allocating a GPU texture");

   ComPtr<ID3D11Device> d3d;
   ComPtr<ID3D11DeviceContext> immediate;
   check(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP, nullptr,
      D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0, D3D11_SDK_VERSION, &d3d, nullptr, &immediate));
   ComPtr<IDXGIDevice> dxgi;
   check(d3d.As(&dxgi));
   ComPtr<ID2D1Device> device;
   check(D2D1CreateDevice(dxgi.Get(), nullptr, &device));
   auto context = [&]()
   {
      ComPtr<ID2D1DeviceContext> result;
      check(device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &result));
      return result;
   };
   auto first = context(), second = context();
   auto props = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET,
      D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));
   ComPtr<ID2D1Bitmap1> preview, layer;
   check(first->CreateBitmap(D2D1::SizeU(32, 32), nullptr, 0, &props, &preview));
   check(first->CreateBitmap(D2D1::SizeU(64, 64), nullptr, 0, &props, &layer));

   first->SetTarget(preview.Get()); first->BeginDraw();
   first->Clear(D2D1::ColorF(D2D1::ColorF::Red));
   first->SetTarget(nullptr); // Detaching alone does NOT end ownership.
   second->SetTarget(preview.Get()); second->BeginDraw();
   second->Clear(D2D1::ColorF(D2D1::ColorF::Blue));
   require(FAILED(second->EndDraw()), "Expected rejection of overlapping image ownership");
   check(first->EndDraw());
   second->SetTarget(nullptr);

   first = context(); second = context();
   for (int i = 0; i < 100; ++i)
   {
      first->SetTarget(preview.Get()); first->BeginDraw();
      first->Clear(D2D1::ColorF(D2D1::ColorF::Red));
      check(first->EndDraw()); first->SetTarget(nullptr);
      second->SetTarget(preview.Get()); second->BeginDraw();
      second->Clear(D2D1::ColorF(D2D1::ColorF::Blue));
      check(second->EndDraw()); second->SetTarget(nullptr);
   }

   // Building a cached preview while a layer is active must use a different
   // target; it can then be sampled by that layer after its EndDraw.
   first->SetTarget(layer.Get()); first->BeginDraw();
   first->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));
   second->SetTarget(preview.Get()); second->BeginDraw();
   second->Clear(D2D1::ColorF(D2D1::ColorF::Green));
   check(second->EndDraw()); second->SetTarget(nullptr);
   first->DrawBitmap(preview.Get());
   check(first->EndDraw()); first->SetTarget(nullptr);

   ComPtr<IDWriteFactory> writeFactory;
   check(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
      reinterpret_cast<IUnknown **>(writeFactory.GetAddressOf())));
   ComPtr<IDWriteTextFormat> format;
   check(writeFactory->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
      DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14.f, L"en-US", &format));
   ComPtr<ID2D1SolidColorBrush> brush;
   check(second->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush));
   second->SetTarget(preview.Get()); second->BeginDraw();
   second->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));
   second->DrawText(L"Aa", 2, format.Get(), D2D1::RectF(0, 0, 32, 32), brush.Get());
   check(second->EndDraw()); second->SetTarget(nullptr);

   props.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
   ComPtr<ID2D1Bitmap1> readback;
   check(second->CreateBitmap(D2D1::SizeU(32, 32), nullptr, 0, &props, &readback));
   check(readback->CopyFromBitmap(nullptr, preview.Get(), nullptr));
   D2D1_MAPPED_RECT mapped{};
   check(readback->Map(D2D1_MAP_OPTIONS_READ, &mapped));
   unsigned visible = 0, transparent = 0;
   for (unsigned y = 0; y < 32; ++y)
      for (unsigned x = 0; x < 32; ++x)
         if (mapped.bits[y * mapped.pitch + x * 4 + 3]) ++visible;
         else ++transparent;
   check(readback->Unmap());
   require(visible > 0 && transparent > 0, "Offscreen text preview must contain glyph pixels and transparent background");

   // Verify a cached font bitmap is actually composited, both at its original
   // size and resized, rather than merely checking the cache's own pixels.
   first->SetTarget(layer.Get()); first->BeginDraw();
   first->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));
   first->DrawBitmap(preview.Get(), D2D1::RectF(0, 0, 32, 32), 1.f,
      D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(0, 0, 32, 32));
   first->DrawBitmap(preview.Get(), D2D1::RectF(32, 32, 48, 48), 1.f,
      D2D1_INTERPOLATION_MODE_LINEAR, D2D1::RectF(0, 0, 32, 32));
   check(first->EndDraw()); first->SetTarget(nullptr);
   ComPtr<ID2D1Bitmap1> layerReadback;
   check(first->CreateBitmap(D2D1::SizeU(64, 64), nullptr, 0, &props, &layerReadback));
   check(layerReadback->CopyFromBitmap(nullptr, layer.Get(), nullptr));
   check(layerReadback->Map(D2D1_MAP_OPTIONS_READ, &mapped));
   unsigned copied = 0, resized = 0;
   for (unsigned y = 0; y < 64; ++y)
      for (unsigned x = 0; x < 64; ++x)
      {
         auto alpha = mapped.bits[y * mapped.pitch + x * 4 + 3];
         if (x < 32 && y < 32) { if (alpha) ++copied; }
         else if (x >= 32 && x < 48 && y >= 32 && y < 48) { if (alpha) ++resized; }
         else require(alpha == 0, "Preview pixels must remain inside their destination rectangles");
      }
   check(layerReadback->Unmap());
   require(copied == visible && resized > 0, "Cached font pixels must survive native copy and resize to a layer");

   // Model repeated CPU luminance edits on a native bitmap with no separate
   // gpu::texture. Upload only before sampling; preserve BGRA, alpha and pitch.
   std::vector<unsigned char> pixels(36 * 4 * 32, 0);
   for (unsigned edit = 0; edit < 2; ++edit)
   {
      for (unsigned y = 0; y < 32; ++y)
         for (unsigned x = 0; x < 32; ++x)
         {
            auto p = &pixels[(y * 36 + x) * 4];
            p[0] = static_cast<unsigned char>(edit ? 10 : 70);
            p[1] = static_cast<unsigned char>(20 + y);
            p[2] = static_cast<unsigned char>(edit ? 70 : 10);
            p[3] = 128;
         }
      check(preview->CopyFromMemory(nullptr, pixels.data(), 36 * 4));
      first->SetTarget(layer.Get()); first->BeginDraw();
      first->Clear(D2D1::ColorF(0.f, 0.f, 0.f, 0.f));
      first->DrawBitmap(preview.Get());
      check(first->EndDraw()); first->SetTarget(nullptr);
      check(layerReadback->CopyFromBitmap(nullptr, layer.Get(), nullptr));
      check(layerReadback->Map(D2D1_MAP_OPTIONS_READ, &mapped));
      for (unsigned y = 0; y < 32; ++y)
         for (unsigned x = 0; x < 32; ++x)
            for (unsigned c = 0; c < 4; ++c)
               require(mapped.bits[y * mapped.pitch + x * 4 + c] == pixels[(y * 36 + x) * 4 + c],
                  "Repeated CPU edits must update the sampled native bitmap without stale colors");
      check(layerReadback->Unmap());
   }
   std::puts("PASS: ownership handoffs; font cache/copy/resize; repeated CPU uploads; native map/unmap/dispatch wiring");
}
