#pragma once

#include "DefsProps.g.h"
#include "DefsView.g.h"
#include "GroupView.h"

namespace winrt::RNSVG::implementation {
REACT_STRUCT(DefsProps)
struct DefsProps : DefsPropsT<DefsProps, SvgGroupCommonProps> {
  DefsProps(const winrt::Microsoft::ReactNative::ViewProps &props);

  void SetProp(uint32_t hash, winrt::hstring propName, winrt::Microsoft::ReactNative::IJSValueReader value) noexcept
      override;

  REACT_SVG_NODE_COMMON_PROPS;
  REACT_SVG_RENDERABLE_COMMON_PROPS;
};

struct DefsView : DefsViewT<DefsView, RNSVG::implementation::GroupView> {
 public:
  DefsView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args);

  // RenderableView
  void UpdateProperties(
      const winrt::Microsoft::ReactNative::IComponentProps &props,
      const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
      bool forceUpdate = true,
      bool invalidate = true) noexcept override;
  void Draw(RNSVG::D2DDeviceContext const &deviceContext, Windows::Foundation::Size const &size);

  static void RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept;
};
} // namespace winrt::RNSVG::implementation

namespace winrt::RNSVG::factory_implementation {
struct DefsView : DefsViewT<DefsView, implementation::DefsView> {};
} // namespace winrt::RNSVG::factory_implementation
