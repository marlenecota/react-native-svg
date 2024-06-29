#include "pch.h"
#include "RectView.h"
#if __has_include("RectView.g.cpp")
#include "RectView.g.cpp"
#endif

#include "JSValueXaml.h"
#include "Utils.h"

using namespace winrt;
using namespace Microsoft::ReactNative;

namespace winrt::RNSVG::implementation {

#ifdef USE_FABRIC
RectProps::RectProps(const winrt::Microsoft::ReactNative::ViewProps &props) : base_type(props) {}

void RectProps::SetProp(
    uint32_t hash,
    winrt::hstring propName,
    winrt::Microsoft::ReactNative::IJSValueReader value) noexcept {
  winrt::Microsoft::ReactNative::ReadProp(hash, propName, value, *this);
}

RectView::RectView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) : base_type(args) {}

void RectView::RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept {
  builder.AddViewComponent(
      L"RNSVGRect", [](winrt::Microsoft::ReactNative::IReactViewComponentBuilder const &builder) noexcept {
        builder.SetCreateProps([](winrt::Microsoft::ReactNative::ViewProps props) noexcept {
          return winrt::make<winrt::RNSVG::implementation::RectProps>(props);
        });
        builder.SetCreateComponentView([](const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) noexcept {
          return winrt::make<winrt::RNSVG::implementation::RectView>(args);
        });
      });
}

void RectView::UpdateProperties(
    const winrt::Microsoft::ReactNative::IComponentProps &props,
    const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
    bool forceUpdate,
    bool invalidate) noexcept {
  auto rectProps = props.try_as<RectProps>();
  if (rectProps) {
    m_props = rectProps;

    m_x = m_props->x;
    m_y = m_props->y;
    m_width = m_props->width;
    m_height = m_props->height;
    m_rx = m_props->rx;
    m_ry = m_props->ry;
  }

  base_type::UpdateProperties(props, oldProps, forceUpdate, invalidate);
}
#else
void RectView::UpdateProperties(IJSValueReader const &reader, bool forceUpdate, bool invalidate) {
  const JSValueObject &propertyMap{JSValue::ReadObjectFrom(reader)};

  for (auto const &pair : propertyMap) {
    auto const &propertyName{pair.first};
    auto const &propertyValue{pair.second};

    if (propertyName == "width") {
      m_width = SVGLength::From(propertyValue);
    } else if (propertyName == "height") {
      m_height = SVGLength::From(propertyValue);
    } else if (propertyName == "x") {
      m_x = SVGLength::From(propertyValue);
    } else if (propertyName == "y") {
      m_y = SVGLength::From(propertyValue);
    } else if (propertyName == "rx") {
      m_rx = SVGLength::From(propertyValue);
    } else if (propertyName == "ry") {
      m_ry = SVGLength::From(propertyValue);
    }
  }

  __super::UpdateProperties(reader, forceUpdate, invalidate);
}
#endif

void RectView::CreateGeometry(RNSVG::D2DDeviceContext const &context) {
  auto const &root{SvgRoot()};

  float canvasWidth{root.CanvasSize().Width};
  float canvasHeight{root.CanvasSize().Height};

  float x{Utils::GetAbsoluteLength(m_x, canvasWidth)};
  float y{Utils::GetAbsoluteLength(m_y, canvasHeight)};
  float width{Utils::GetAbsoluteLength(m_width, canvasWidth)};
  float height{Utils::GetAbsoluteLength(m_height, canvasHeight)};

#ifdef USE_FABRIC
  auto const rxLength{m_rx.Unit == RNSVG::LengthType::Unknown ? m_ry : m_rx};
  auto const ryLength{m_ry.Unit == RNSVG::LengthType::Unknown ? m_rx : m_ry};
#else
  auto const rxLength{m_rx.Unit() == RNSVG::LengthType::Unknown ? m_ry : m_rx};
  auto const ryLength{m_ry.Unit() == RNSVG::LengthType::Unknown ? m_rx : m_ry};
#endif

  float rx{Utils::GetAbsoluteLength(rxLength, canvasWidth)};
  float ry{Utils::GetAbsoluteLength(ryLength, canvasHeight)};

  com_ptr<ID2D1DeviceContext> deviceContext{get_self<D2DDeviceContext>(context)->Get()};

  com_ptr<ID2D1Factory> factory;
  deviceContext->GetFactory(factory.put());

  com_ptr<ID2D1RoundedRectangleGeometry> geometry;
  check_hresult(factory->CreateRoundedRectangleGeometry(
      D2D1::RoundedRect(D2D1::RectF(x, y, width + x, height + y), rx, ry), geometry.put()));

  Geometry(make<RNSVG::implementation::D2DGeometry>(geometry.as<ID2D1Geometry>()));
}
} // namespace winrt::RNSVG::implementation
