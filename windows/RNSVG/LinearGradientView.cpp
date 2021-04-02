#include "pch.h"
#include "LinearGradientView.h"
#include "LinearGradientView.g.cpp"

#include "SVGLength.h"

using namespace winrt;
using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::ReactNative;

namespace winrt::RNSVG::implementation {
void LinearGradientView::SaveDefinition() {
  throw hresult_not_implemented();
}

void LinearGradientView::RemoveDefinition() {
  throw hresult_not_implemented();
}
void LinearGradientView::UpdateProperties(IJSValueReader const &reader, bool forceUpdate, bool invalidate) {
  const JSValueObject &propertyMap{JSValue::ReadObjectFrom(reader)};

  for (auto const &pair : propertyMap) {
    auto const &propertyName{pair.first};
    auto const &propertyValue{pair.second};

    if (propertyName == "name") {
      RemoveDefinition();
    } else if (propertyName == "x1") {
      m_x1 = SVGLength::From(propertyValue);
    } else if (propertyName == "y1") {
      m_y1 = SVGLength::From(propertyValue);
    } else if (propertyName == "x2") {
      m_x2 = SVGLength::From(propertyValue);
    } else if (propertyName == "y2") {
      m_y2 = SVGLength::From(propertyValue);
    }
  }

  __super::UpdateProperties(reader, forceUpdate, invalidate);
}

void LinearGradientView::Render(UI::Xaml::CanvasControl const &/*canvas*/, CanvasDrawingSession const &/*session*/) {}

} // namespace winrt::RNSVG::implementation
