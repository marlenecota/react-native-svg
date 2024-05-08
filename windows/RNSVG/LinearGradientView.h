#pragma once
#include "LinearGradientProps.g.h"
#include "LinearGradientView.g.h"
#include "BrushView.h"

namespace winrt::RNSVG::implementation {

REACT_STRUCT(LinearGradientProps)
struct LinearGradientProps : LinearGradientPropsT<LinearGradientProps, SvgGroupCommonProps> {
  LinearGradientProps(const winrt::Microsoft::ReactNative::ViewProps &props);

  void SetProp(uint32_t hash, winrt::hstring propName, winrt::Microsoft::ReactNative::IJSValueReader value) noexcept
      override;

  REACT_SVG_NODE_COMMON_PROPS;
  REACT_SVG_RENDERABLE_COMMON_PROPS;
  REACT_SVG_GROUP_COMMON_PROPS;

  REACT_FIELD(x1)
  RNSVG::SVGLength x1{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(y1)
  RNSVG::SVGLength y1{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(x2)
  RNSVG::SVGLength x2{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(y2)
  RNSVG::SVGLength y2{0, winrt::RNSVG::LengthType::Unknown};
  REACT_FIELD(gradient)
  std::optional<std::vector<float>> gradient{};
  REACT_FIELD(gradientUnits)
  std::optional<int32_t> gradientUnits;
  REACT_FIELD(gradientTransform)
  std::optional<std::vector<float>> gradientTransform;
};

struct LinearGradientView : LinearGradientViewT<LinearGradientView, RNSVG::implementation::BrushView> {
 public:
  LinearGradientView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args);

  // RenderableView
  void UpdateProperties(
      const winrt::Microsoft::ReactNative::IComponentProps &props,
      const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
      bool forceUpdate = true,
      bool invalidate = true) noexcept override;
  void Unload();

  static void RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept;

 private:
  std::vector<D2D1_GRADIENT_STOP> m_stops{};
  std::string m_gradientUnits{"objectBoundingBox"};
  com_ptr<LinearGradientProps> m_props;

  void CreateBrush();
  void UpdateBounds();
  void SetPoints(ID2D1LinearGradientBrush *brush, D2D1_RECT_F bounds);
};
} // namespace winrt::RNSVG::implementation

namespace winrt::RNSVG::factory_implementation {
struct LinearGradientView : LinearGradientViewT<LinearGradientView, implementation::LinearGradientView> {};
} // namespace winrt::RNSVG::factory_implementation
