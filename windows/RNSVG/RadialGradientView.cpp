#include "pch.h"
#include "RadialGradientView.h"
#if __has_include("RadialGradientView.g.cpp")
#include "RadialGradientView.g.cpp"
#endif

#include "Utils.h"

using namespace winrt;
using namespace Microsoft::ReactNative;

namespace winrt::RNSVG::implementation {
RadialGradientProps::RadialGradientProps(const winrt::Microsoft::ReactNative::ViewProps &props) : base_type(props) {}

void RadialGradientProps::SetProp(
    uint32_t hash,
    winrt::hstring propName,
    winrt::Microsoft::ReactNative::IJSValueReader value) noexcept {
  winrt::Microsoft::ReactNative::ReadProp(hash, propName, value, *this);
}

RadialGradientView::RadialGradientView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args)
    : base_type(args) {}

void RadialGradientView::UpdateProperties(
    const winrt::Microsoft::ReactNative::IComponentProps &props,
    const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
    bool forceUpdate,
    bool invalidate) noexcept {
  auto radialGradientProps = props.try_as<RadialGradientProps>();
  if (radialGradientProps) {
    m_props = radialGradientProps;

    m_stops = Utils::JSValueAsGradientStops(m_props->gradient);

    m_gradientUnits = Utils::JSValueAsBrushUnits(m_props->gradientUnits);

    m_transform = Utils::JSValueAsD2DTransform(m_props->gradientTransform);
  }

  base_type::UpdateProperties(props, oldProps, forceUpdate, invalidate);

  SaveDefinition();
}

void RadialGradientView::Unload() {
  m_stops.clear();
  __super::Unload();
}

void RadialGradientView::CreateBrush() {
  auto const root{SvgRoot()};

  com_ptr<ID2D1DeviceContext> deviceContext{get_self<D2DDeviceContext>(root.DeviceContext())->Get()};

  winrt::com_ptr<ID2D1GradientStopCollection> stopCollection;
  winrt::check_hresult(deviceContext->CreateGradientStopCollection(&m_stops[0], static_cast<uint32_t>(m_stops.size()), stopCollection.put()));

  D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES brushProperties{};
  winrt::com_ptr<ID2D1RadialGradientBrush> radialBrush;
  winrt::check_hresult(deviceContext->CreateRadialGradientBrush(brushProperties, stopCollection.get(), radialBrush.put()));

  SetPoints(radialBrush.get(), {0, 0, root.ActualSize().Width, root.ActualSize().Height});

  if (!m_transform.IsIdentity()) {
    radialBrush->SetTransform(m_transform);
  }

  m_brush = make<RNSVG::implementation::D2DBrush>(radialBrush.as<ID2D1Brush>());
}

void RadialGradientView::UpdateBounds() {
  if (m_gradientUnits == "objectBoundingBox") {
    com_ptr<ID2D1RadialGradientBrush> brush{get_self<D2DBrush>(m_brush)->Get().as<ID2D1RadialGradientBrush>()};
    SetPoints(brush.get(), m_bounds);
  }
}

void RadialGradientView::SetPoints(ID2D1RadialGradientBrush *brush, D2D1_RECT_F bounds) {
  float width{D2DHelpers::WidthFromD2DRect(bounds)};
  float height{D2DHelpers::HeightFromD2DRect(bounds)};

  float rx{Utils::GetAbsoluteLength(m_props->rx, width)};
  float ry{Utils::GetAbsoluteLength(m_props->ry, height)};

  float fx{Utils::GetAbsoluteLength(m_props->fx, width) + bounds.left};
  float fy{Utils::GetAbsoluteLength(m_props->fy, height) + bounds.top};

  float cx{Utils::GetAbsoluteLength(m_props->cx, width) + bounds.left};
  float cy{Utils::GetAbsoluteLength(m_props->cy, height) + bounds.top};

  brush->SetRadiusX(rx);
  brush->SetRadiusY(ry);

  brush->SetCenter({cx, cy});
  brush->SetGradientOriginOffset({(fx - cx), (fy - cy)});
}

void RadialGradientView::RegisterComponent(
    const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept {
  builder.AddViewComponent(
      L"RNSVGRadialGradient", [](winrt::Microsoft::ReactNative::IReactViewComponentBuilder const &builder) noexcept {
        builder.SetCreateProps([](winrt::Microsoft::ReactNative::ViewProps props) noexcept {
          return winrt::make<winrt::RNSVG::implementation::RadialGradientProps>(props);
        });
        builder.SetCreateComponentView([](const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) noexcept {
          return winrt::make<winrt::RNSVG::implementation::RadialGradientView>(args);
        });
      });
}

} // namespace winrt::RNSVG::implementation
