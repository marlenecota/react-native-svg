#pragma once

#include "EllipseProps.g.h"
#include "EllipseView.g.h"
#include "RenderableView.h"

namespace winrt::RNSVG::implementation {

REACT_STRUCT(EllipseProps)
struct EllipseProps : EllipsePropsT<EllipseProps, SvgRenderableCommonProps> {
  EllipseProps(const winrt::Microsoft::ReactNative::ViewProps &props);

  void SetProp(uint32_t hash, winrt::hstring propName, winrt::Microsoft::ReactNative::IJSValueReader value) noexcept
      override;

  REACT_SVG_NODE_COMMON_PROPS;
  REACT_SVG_RENDERABLE_COMMON_PROPS;

  REACT_FIELD(cx)
  RNSVG::SVGLength cx{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(cy)
  RNSVG::SVGLength cy{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(rx)
  RNSVG::SVGLength rx{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(ry)
  RNSVG::SVGLength ry{0, winrt::RNSVG::LengthType::Unknown};
};

struct EllipseView : EllipseViewT<EllipseView, RNSVG::implementation::RenderableView> {
 public:
  EllipseView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args);

  void UpdateProperties(
      const winrt::Microsoft::ReactNative::IComponentProps &props,
      const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
      bool forceUpdate = true,
      bool invalidate = true) noexcept override;
  void CreateGeometry(RNSVG::D2DDeviceContext const &context);

  static void RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept;

 private:
  com_ptr<EllipseProps> m_props;
};
} // namespace winrt::RNSVG::implementation
namespace winrt::RNSVG::factory_implementation {
struct EllipseView : EllipseViewT<EllipseView, implementation::EllipseView> {};
} // namespace winrt::RNSVG::factory_implementation
