#pragma once
#include "DefsView.g.h"
#include "GroupView.h"

namespace winrt::RNSVG::implementation {
struct DefsView : DefsViewT<DefsView, RNSVG::implementation::GroupView> {
  DefsView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args);

  void Draw(RNSVG::D2DDeviceContext const &deviceContext, Windows::Foundation::Size const &size);

  static void RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept;
};
} // namespace winrt::RNSVG::implementation

namespace winrt::RNSVG::factory_implementation {
struct DefsView : DefsViewT<DefsView, implementation::DefsView> {};
} // namespace winrt::RNSVG::factory_implementation
