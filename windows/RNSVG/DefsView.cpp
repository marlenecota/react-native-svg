#include "pch.h"
#include "DefsView.h"
#if __has_include("DefsView.g.cpp")
#include "DefsView.g.cpp"
#endif

using namespace winrt;

namespace winrt::RNSVG::implementation {
DefsView::DefsView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) : base_type(args) {}

void DefsView::Draw(RNSVG::D2DDeviceContext const& /*deviceContext*/, Size const & /*size*/) {}

void DefsView::RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept {
  builder.AddViewComponent(
      L"RNSVGDefs", [](winrt::Microsoft::ReactNative::IReactViewComponentBuilder const &builder) noexcept {
        //builder.SetCreateProps([](winrt::Microsoft::ReactNative::ViewProps props) noexcept {
        //  return winrt::make<winrt::RNSVG::implementation::DefsProps>(props);
        //});
        builder.SetCreateComponentView([](const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) noexcept {
          return winrt::make<winrt::RNSVG::implementation::DefsView>(args);
        });
      });
}
} // namespace winrt::RNSVG::implementation
