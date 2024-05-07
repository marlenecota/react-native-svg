#pragma once

#include "LineProps.g.h"
#include "LineView.g.h"
#include "RenderableView.h"

namespace winrt::RNSVG::implementation {

REACT_STRUCT(LineProps)
struct LineProps : LinePropsT<LineProps, SvgRenderableCommonProps> {
  LineProps(const winrt::Microsoft::ReactNative::ViewProps &props);

  void SetProp(uint32_t hash, winrt::hstring propName, winrt::Microsoft::ReactNative::IJSValueReader value) noexcept
      override;

  REACT_SVG_NODE_COMMON_PROPS;
  REACT_SVG_RENDERABLE_COMMON_PROPS;

  REACT_FIELD(x1)
  RNSVG::SVGLength x1{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(y1)
  RNSVG::SVGLength y1{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(x2)
  RNSVG::SVGLength x2{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(y2)
  RNSVG::SVGLength y2{0, winrt::RNSVG::LengthType::Unknown};
};

struct LineView : LineViewT<LineView, RNSVG::implementation::RenderableView> {
 public:
  LineView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args);

  void UpdateProperties(
      const winrt::Microsoft::ReactNative::IComponentProps &props,
      const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
      bool forceUpdate = true,
      bool invalidate = true) noexcept override;
  void CreateGeometry(RNSVG::D2DDeviceContext const &context);

  static void RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept;

 private:
  com_ptr<LineProps> m_props;
};
} // namespace winrt::RNSVG::implementation
namespace winrt::RNSVG::factory_implementation {
struct LineView : LineViewT<LineView, implementation::LineView> {};
} // namespace winrt::RNSVG::factory_implementation
