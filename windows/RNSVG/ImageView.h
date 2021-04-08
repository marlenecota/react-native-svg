#pragma once
#include "ImageView.g.h"
#include "RenderableView.h"

namespace winrt::RNSVG::implementation {
struct ImageView : ImageViewT<ImageView, RNSVG::implementation::RenderableView> {
 public:
  ImageView() = default;

  void UpdateProperties(Microsoft::ReactNative::IJSValueReader const &reader, bool forceUpdate, bool invalidate);
  void Render(
      Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const &canvas,
      Microsoft::Graphics::Canvas::CanvasDrawingSession const &session);

 private:
  RNSVG::SVGLength m_x{};
  RNSVG::SVGLength m_y{};
  RNSVG::SVGLength m_width{};
  RNSVG::SVGLength m_height{};
  std::string m_src{""};
  std::string m_align{""};
  RNSVG::MeetOrSlice m_meetOrSlice{RNSVG::MeetOrSlice::Meet};
};
} // namespace winrt::RNSVG::implementation

namespace winrt::RNSVG::factory_implementation {
struct ImageView : ImageViewT<ImageView, implementation::ImageView> {};
} // namespace winrt::RNSVG::factory_implementation
