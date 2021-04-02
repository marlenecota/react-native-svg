#pragma once
#include "LinearGradientView.g.h"
#include "GroupView.h"

namespace winrt::RNSVG::implementation {
struct LinearGradientView : LinearGradientViewT<LinearGradientView, RNSVG::implementation::GroupView> {
 public:
  LinearGradientView() = default;

  // IDefinitionView
  void SaveDefinition();
  void RemoveDefinition();

  // RenderableView
  void UpdateProperties(Microsoft::ReactNative::IJSValueReader const &reader, bool forceUpdate, bool invalidate);
  void Render(
      Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const &canvas,
      Microsoft::Graphics::Canvas::CanvasDrawingSession const &session);

 private:
  RNSVG::SVGLength m_x1{};
  RNSVG::SVGLength m_y1{};
  RNSVG::SVGLength m_x2{};
  RNSVG::SVGLength m_y2{};
};
} // namespace winrt::RNSVG::implementation

namespace winrt::RNSVG::factory_implementation {
struct LinearGradientView : LinearGradientViewT<LinearGradientView, implementation::LinearGradientView> {};
} // namespace winrt::RNSVG::factory_implementation
