#pragma once

#include "UseProps.g.h"
#include "UseView.g.h"
#include "RenderableView.h"

namespace winrt::RNSVG::implementation {

REACT_STRUCT(UseProps)
struct UseProps : UsePropsT<UseProps, SvgRenderableCommonProps> {
  UseProps(const winrt::Microsoft::ReactNative::ViewProps &props);

  void SetProp(uint32_t hash, winrt::hstring propName, winrt::Microsoft::ReactNative::IJSValueReader value) noexcept
      override;

  REACT_SVG_NODE_COMMON_PROPS;
  REACT_SVG_RENDERABLE_COMMON_PROPS;

  REACT_FIELD(href)
  std::string href;
  REACT_FIELD(x)
  RNSVG::SVGLength x{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(y)
  RNSVG::SVGLength y{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(width)
  RNSVG::SVGLength width{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(height)
  RNSVG::SVGLength height{0, winrt::RNSVG::LengthType::Unknown};
};

struct UseView : UseViewT<UseView, RNSVG::implementation::RenderableView> {
 public:
  UseView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args);

  void UpdateProperties(
      const winrt::Microsoft::ReactNative::IComponentProps &props,
      const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
      bool forceUpdate = true,
      bool invalidate = true) noexcept override;
  void Draw(RNSVG::D2DDeviceContext const &deviceContext, Windows::Foundation::Size const &size);

  static void RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept;
 
private:
  com_ptr<UseProps> m_props;
  RNSVG::IRenderable GetRenderableTemplate();
};
} // namespace winrt::RNSVG::implementation

namespace winrt::RNSVG::factory_implementation {
struct UseView : UseViewT<UseView, implementation::UseView> {};
} // namespace winrt::RNSVG::factory_implementation
