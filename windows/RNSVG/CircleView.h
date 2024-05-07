#pragma once

#include "CircleProps.g.h"
#include "CircleView.g.h"
#include "RenderableView.h"

namespace winrt::RNSVG::implementation {

REACT_STRUCT(CircleProps)
struct CircleProps : CirclePropsT<CircleProps, SvgRenderableCommonProps> {
  CircleProps(const winrt::Microsoft::ReactNative::ViewProps &props);

  void SetProp(uint32_t hash, winrt::hstring propName, winrt::Microsoft::ReactNative::IJSValueReader value) noexcept
      override;

  REACT_SVG_NODE_COMMON_PROPS;
  REACT_SVG_RENDERABLE_COMMON_PROPS;

  REACT_FIELD(r)
  RNSVG::SVGLength r{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(cx)
  RNSVG::SVGLength cx{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(cy)
  RNSVG::SVGLength cy{0, winrt::RNSVG::LengthType::Unknown};
};

struct CircleView : CircleViewT<CircleView, RNSVG::implementation::RenderableView> {
 public:
  CircleView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args);

  void UpdateProperties(
      const winrt::Microsoft::ReactNative::IComponentProps &props,
      const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
      bool forceUpdate = true,
      bool invalidate = true) noexcept override;
  void CreateGeometry(RNSVG::D2DDeviceContext const &context);

  static void RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept;

 private:
  com_ptr<CircleProps> m_props;
};
} // namespace winrt::RNSVG::implementation
namespace winrt::RNSVG::factory_implementation {
struct CircleView : CircleViewT<CircleView, implementation::CircleView> {};
} // namespace winrt::RNSVG::factory_implementation
