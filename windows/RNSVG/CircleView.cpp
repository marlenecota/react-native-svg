#include "pch.h"
#include "CircleView.h"
#if __has_include("CircleView.g.cpp")
#include "CircleView.g.cpp"
#endif

#include "JSValueXaml.h"
#include "Utils.h"

using namespace winrt;
using namespace Microsoft::ReactNative;

namespace winrt::RNSVG::implementation {

CircleProps::CircleProps(const winrt::Microsoft::ReactNative::ViewProps &props) : base_type(props) {}

void CircleProps::SetProp(
    uint32_t hash,
    winrt::hstring propName,
    winrt::Microsoft::ReactNative::IJSValueReader value) noexcept {
  winrt::Microsoft::ReactNative::ReadProp(hash, propName, value, *this);
}

CircleView::CircleView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) : base_type(args) {}

void CircleView::UpdateProperties(
    const winrt::Microsoft::ReactNative::IComponentProps &props,
    const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
    bool forceUpdate,
    bool invalidate) noexcept {
  auto circleProps = props.try_as<CircleProps>();
  if (circleProps) {
    m_props = circleProps;
  }

  base_type::UpdateProperties(props, oldProps, forceUpdate, invalidate);
}

void CircleView::CreateGeometry(RNSVG::D2DDeviceContext const &context) {
  auto const root{SvgRoot()};
  
  float cx{Utils::GetAbsoluteLength(m_props->cx, root.ActualSize().Width)};
  float cy{Utils::GetAbsoluteLength(m_props->cy, root.ActualSize().Height)};
  float r{Utils::GetAbsoluteLength(m_props->r, Utils::GetCanvasDiagonal(root.ActualSize()))};

  com_ptr<ID2D1DeviceContext> deviceContext{get_self<D2DDeviceContext>(context)->Get()};

  com_ptr<ID2D1Factory> factory;
  deviceContext->GetFactory(factory.put());

  com_ptr<ID2D1EllipseGeometry> geometry;
  check_hresult(factory->CreateEllipseGeometry(D2D1::Ellipse({cx, cy}, r, r), geometry.put()));

  Geometry(make<RNSVG::implementation::D2DGeometry>(geometry.as<ID2D1Geometry>()));
}

void CircleView::RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept {
  builder.AddViewComponent(
      L"RNSVGCircle", [](winrt::Microsoft::ReactNative::IReactViewComponentBuilder const &builder) noexcept {
        builder.SetCreateProps([](winrt::Microsoft::ReactNative::ViewProps props) noexcept {
          return winrt::make<winrt::RNSVG::implementation::CircleProps>(props);
        });
        builder.SetCreateComponentView([](const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) noexcept {
          return winrt::make<winrt::RNSVG::implementation::CircleView>(args);
        });
      });
}

} // namespace winrt::RNSVG::implementation
