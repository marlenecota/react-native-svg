#pragma once
#include "RadialGradientView.g.h"
#include "BrushView.h"

namespace winrt::RNSVG::implementation {
struct RadialGradientView : RadialGradientViewT<RadialGradientView, RNSVG::implementation::BrushView> {
 public:
  RadialGradientView() = default;

  // RenderableView
  void UpdateProperties(Microsoft::ReactNative::IJSValueReader const &reader, bool forceUpdate, bool invalidate);
  void Unload();

 private:
  RNSVG::SVGLength m_fx{};
  RNSVG::SVGLength m_fy{};
  RNSVG::SVGLength m_rx{};
  RNSVG::SVGLength m_ry{};
  RNSVG::SVGLength m_cx{};
  RNSVG::SVGLength m_cy{};
  std::vector<D2D1_GRADIENT_STOP> m_stops{};
  std::string m_gradientUnits{"objectBoundingBox"};
  bool m_transformSet{false};
  D2D1_MATRIX_3X2_F m_transform;

  void CreateBrush();
  void UpdateBounds();
  void SetPoints(ID2D1RadialGradientBrush *brush, D2D1_RECT_F bounds);
};
} // namespace winrt::RNSVG::implementation

namespace winrt::RNSVG::factory_implementation {
struct RadialGradientView : RadialGradientViewT<RadialGradientView, implementation::RadialGradientView> {};
} // namespace winrt::RNSVG::factory_implementation
