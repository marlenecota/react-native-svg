#include "pch.h"

#include "SvgView.h"
#if __has_include("SvgView.g.cpp")
#include "SvgView.g.cpp"
#endif

#include <winrt/Windows.Graphics.Display.h>

#include "GroupView.h"
#include "Utils.h"

using namespace winrt;
using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::ReactNative;
using namespace Windows::Graphics::Display;

namespace winrt::RNSVG::implementation {
SvgView::SvgView(IReactContext const &context) : m_reactContext(context) {
  m_scale = static_cast<float>(DisplayInformation::GetForCurrentView().ResolutionScale()) / 100;

  m_canvasDrawRevoker = m_canvas.Draw(winrt::auto_revoke, {get_weak(), &SvgView::Canvas_Draw});
  m_canvaSizeChangedRevoker = m_canvas.SizeChanged(winrt::auto_revoke, {get_weak(), &SvgView::Canvas_SizeChanged});
  m_panelUnloadedRevoker = Unloaded(winrt::auto_revoke, {get_weak(), &SvgView::Panel_Unloaded});

  Children().Append(m_canvas);
}

void SvgView::UpdateProperties(IJSValueReader const &reader, bool /*forceUpdate*/, bool /*invalidate*/) {
  auto const &propertyMap{JSValueObject::ReadFrom(reader)};

  for (auto const &pair : propertyMap) {
    auto const &propertyName{pair.first};
    auto const &propertyValue{pair.second};

    if (propertyName == "width") {
      m_width = SVGLength::From(propertyValue);
    } else if (propertyName == "height") {
      m_height = SVGLength::From(propertyValue);
    } else if (propertyName == "bbWidth") {
      m_bbWidth = SVGLength::From(propertyValue);
      Width(m_bbWidth.Value());
    } else if (propertyName == "bbHeight") {
      m_bbHeight = SVGLength::From(propertyValue);
      Height(m_bbHeight.Value());
    } else if (propertyName == "vbWidth") {
      m_vbWidth = Utils::JSValueAsFloat(propertyValue);
    } else if (propertyName == "vbHeight") {
      m_vbHeight = Utils::JSValueAsFloat(propertyValue);
    } else if (propertyName == "minX") {
      m_minX = Utils::JSValueAsFloat(propertyValue);
    } else if (propertyName == "minY") {
      m_minY = Utils::JSValueAsFloat(propertyValue);
    } else if (propertyName == "align") {
      m_align = Utils::JSValueAsString(propertyValue);
    } else if (propertyName == "meetOrSlice") {
      m_meetOrSlice = Utils::GetMeetOrSlice(propertyValue);
    }
  }

  InvalidateCanvas();
}

void SvgView::SaveDefinition() {}

void SvgView::Unload() {
  m_reactContext = nullptr;
  m_templates.Clear();
  m_brushes.Clear();

  if (m_group) {
    m_group.Unload();
  }

  m_canvas.RemoveFromVisualTree();
  m_canvas = nullptr;
}

void SvgView::MergeProperties(RNSVG::RenderableView const &/*other*/) {}

Size SvgView::MeasureOverride(Size availableSize) {
  for (auto const &child : Children()) {
    child.Measure(availableSize);
  }
  return availableSize;
}

Size SvgView::ArrangeOverride(Size finalSize) {
  for (auto const &child : Children()) {
    child.Arrange({0, 0, finalSize.Width, finalSize.Height});
  }
  return finalSize;
}

void SvgView::Render(
    Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const & /*canvas*/,
    Microsoft::Graphics::Canvas::CanvasDrawingSession const & /*session*/) {

}

void SvgView::Canvas_Draw(UI::Xaml::CanvasControl const &sender, UI::Xaml::CanvasDrawEventArgs const &args) {
  if (!m_hasRendered) {
    m_hasRendered = true;
  }

  if (m_align != "") {
    Rect vbRect{m_minX * m_scale, m_minY * m_scale, m_vbWidth * m_scale, m_vbHeight * m_scale};
    Rect elRect{0, 0, static_cast<float>(sender.ActualWidth()), static_cast<float>(sender.ActualHeight())};

    args.DrawingSession().Transform(Utils::GetViewBoxTransform(vbRect, elRect, m_align, m_meetOrSlice));
  }

  if (m_group) {
    m_group.SaveDefinition();
    m_group.Render(sender, args.DrawingSession());
  }
}

void SvgView::Canvas_SizeChanged(
    IInspectable const & /*sender*/,
    Windows::UI::Xaml::SizeChangedEventArgs const & /*args*/) {
  // sender.Invalidate();
}

void SvgView::InvalidateCanvas() {
  if (m_hasRendered) {
    m_canvas.Invalidate();
  }
}

void SvgView::Panel_Unloaded(IInspectable const &sender, Windows::UI::Xaml::RoutedEventArgs const & /*args*/) {
  if (auto const &svgView{sender.try_as<RNSVG::SvgView>()}) {
    svgView.Unload();
  }
}
} // namespace winrt::RNSVG::implementation
