#include "pch.h"
#include "UseView.h"
#include "UseView.g.cpp"

#include "Utils.h"

using namespace winrt;
using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::ReactNative;

namespace winrt::RNSVG::implementation {
void UseView::UpdateProperties(IJSValueReader const &reader, bool forceUpdate, bool invalidate) {
  const JSValueObject &propertyMap{JSValue::ReadObjectFrom(reader)};

  for (auto const &pair : propertyMap) {
    auto const &propertyName{pair.first};
    auto const &propertyValue{pair.second};

    if (propertyName == "href") {
      m_href = to_hstring(Utils::JSValueAsString(propertyValue));
    } else if (propertyName == "x") {
      m_x = SVGLength::From(propertyValue);
    } else if (propertyName == "y") {
      m_y = SVGLength::From(propertyValue);
    } else if (propertyName == "width") {
      m_width = SVGLength::From(propertyValue);
    } else if (propertyName == "height") {
      m_height = SVGLength::From(propertyValue);
    }
  }

  __super::UpdateProperties(reader, forceUpdate, invalidate);

  if (auto const &view{GetRenderableTemplate()}) {
    view.UpdateProperties(reader, false, false);
  }
}

void UseView::Render(UI::Xaml::CanvasControl const &canvas, CanvasDrawingSession const &session) {
  if (auto const &view{GetRenderableTemplate()}) {
    if (auto const &symbol{view.try_as<RNSVG::SymbolView>()}) {
      auto const &transform{session.Transform()};
      if (symbol.Align() != L"") {
        if (auto const &root{SvgRoot()}) {
          Rect vbRect{
              symbol.MinX() * root.SvgScale(),
              symbol.MinY() * root.SvgScale(),
              (symbol.MinX() + symbol.VbWidth()) * root.SvgScale(),
              (symbol.MinY() + symbol.VbHeight()) * root.SvgScale()};

          float elX{Utils::GetSvgLengthValue(m_x, canvas.Size().Width)};
          float elY{Utils::GetSvgLengthValue(m_y, canvas.Size().Height)};
          float elWidth{Utils::GetSvgLengthValue(m_width, canvas.Size().Width)};
          float elHeight{Utils::GetSvgLengthValue(m_height, canvas.Size().Height)};
          Rect elRect{elX, elY, elWidth, elHeight};

          session.Transform(Utils::GetViewBoxTransform(vbRect, elRect, to_string(symbol.Align()), symbol.MeetOrSlice()));
        }
      }

      symbol.RenderGroup(canvas, session);
      session.Transform(transform);
    } else {
      auto const &originalTransform{session.Transform()};

      float x{Utils::GetSvgLengthValue(m_x, canvas.Size().Width)};
      float y{Utils::GetSvgLengthValue(m_y, canvas.Size().Height)};
      Numerics::float3x2 transform{Numerics::make_float3x2_translation({x, y})};

      if (m_propSetMap[RNSVG::BaseProp::Matrix]) {
        transform = transform * SvgTransform();
      }

      session.Transform(transform);

      view.MergeProperties(*this);

      if (auto const &opacityLayer{session.CreateLayer(m_opacity)}) {
        view.Render(canvas, session);
        opacityLayer.Close();
      }

      if (auto const &parent{view.SvgParent().try_as<RNSVG::RenderableView>()}) {
        view.MergeProperties(parent);
      }

      session.Transform(originalTransform);
    }
  } else {
    throw hresult_not_implemented(L"'Use' element expected a pre-defined svg template as 'href' prop. Template named: " + m_href + L" is not defined");
  }
}

RNSVG::RenderableView UseView::GetRenderableTemplate() {
  if (auto const &root{SvgRoot()}) {
    return root.Templates().TryLookup(m_href);
  }

  return nullptr;
}
} // namespace winrt::RNSVG::implementation
