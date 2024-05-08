#pragma once
#include "ClipPathView.g.h"
#include "GroupView.h"

namespace winrt::RNSVG::implementation {
struct ClipPathView : ClipPathViewT<ClipPathView, RNSVG::implementation::GroupView> {
 public:
  ClipPathView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args);

  // RenderableView
  void Draw(RNSVG::D2DDeviceContext const & /*deviceContext*/, Windows::Foundation::Size const & /*size*/){};

  static void RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept;
};
} // namespace winrt::RNSVG::implementation

namespace winrt::RNSVG::factory_implementation {
struct ClipPathView : ClipPathViewT<ClipPathView, implementation::ClipPathView> {};
} // namespace winrt::RNSVG::factory_implementation
