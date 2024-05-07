#include "pch.h"
#include "LineView.h"
#if __has_include("LineView.g.cpp")
#include "LineView.g.cpp"
#endif

#include "JSValueXaml.h"
#include "Utils.h"

using namespace winrt;
using namespace Microsoft::ReactNative;

namespace winrt::RNSVG::implementation {
LineProps::LineProps(const winrt::Microsoft::ReactNative::ViewProps &props) : base_type(props) {}

void LineProps::SetProp(
    uint32_t hash,
    winrt::hstring propName,
    winrt::Microsoft::ReactNative::IJSValueReader value) noexcept {
  winrt::Microsoft::ReactNative::ReadProp(hash, propName, value, *this);
}

LineView::LineView(const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) : base_type(args) {}

void LineView::UpdateProperties(
    const winrt::Microsoft::ReactNative::IComponentProps &props,
    const winrt::Microsoft::ReactNative::IComponentProps &oldProps,
    bool forceUpdate,
    bool invalidate) noexcept {
  auto lineProps = props.try_as<LineProps>();
  if (lineProps) {
    m_props = lineProps;
  }

  base_type::UpdateProperties(props, oldProps, forceUpdate, invalidate);
}

void LineView::CreateGeometry(RNSVG::D2DDeviceContext const &context) {
  auto const &root{SvgRoot()};

  float x1{Utils::GetAbsoluteLength(m_props->x1, root.ActualSize().Width)};
  float y1{Utils::GetAbsoluteLength(m_props->y1, root.ActualSize().Height)};
  float x2{Utils::GetAbsoluteLength(m_props->x2, root.ActualSize().Width)};
  float y2{Utils::GetAbsoluteLength(m_props->y2, root.ActualSize().Height)};

  com_ptr<ID2D1DeviceContext> deviceContext{get_self<D2DDeviceContext>(context)->Get()};

  com_ptr<ID2D1Factory> factory;
  deviceContext->GetFactory(factory.put());

  com_ptr<ID2D1PathGeometry> geometry;
  check_hresult(factory->CreatePathGeometry(geometry.put()));

  com_ptr<ID2D1GeometrySink> sink;
  check_hresult(geometry->Open(sink.put()));

  sink->BeginFigure({x1, y1}, D2D1_FIGURE_BEGIN_FILLED);
  sink->AddLine({x2, y2});
  sink->EndFigure(D2D1_FIGURE_END_OPEN);

  Geometry(make<RNSVG::implementation::D2DGeometry>(geometry.as<ID2D1Geometry>()));
}

void LineView::RegisterComponent(const winrt::Microsoft::ReactNative::IReactPackageBuilderFabric &builder) noexcept {
  builder.AddViewComponent(
      L"RNSVGLine", [](winrt::Microsoft::ReactNative::IReactViewComponentBuilder const &builder) noexcept {
        builder.SetCreateProps([](winrt::Microsoft::ReactNative::ViewProps props) noexcept {
          return winrt::make<winrt::RNSVG::implementation::LineProps>(props);
        });
        builder.SetCreateComponentView([](const winrt::Microsoft::ReactNative::CreateComponentViewArgs &args) noexcept {
          return winrt::make<winrt::RNSVG::implementation::LineView>(args);
        });
      });
}

} // namespace winrt::RNSVG::implementation
